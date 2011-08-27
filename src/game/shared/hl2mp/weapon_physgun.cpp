//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "beam_shared.h"
#ifndef CLIENT_DLL
#include "player.h"
#endif
#include "gamerules.h"
#ifdef CLIENT_DLL
#include "ClientEffectPrecacheSystem.h"
#endif
#include "weapon_hl2mpbasehlmpcombatweapon.h"
#ifndef CLIENT_DLL
#include "baseviewmodel.h"
#endif
#include "vphysics/constraints.h"
#include "physics.h"
#include "in_buttons.h"
#include "IEffects.h"
#include "engine/IEngineSound.h"
#ifndef CLIENT_DLL
#include "ndebugoverlay.h"
#endif
#include "physics_saverestore.h"
#ifndef CLIENT_DLL
#include "player_pickup.h"
#endif
#include "soundemittersystem/isoundemittersystembase.h"
#ifdef CLIENT_DLL
#include "model_types.h"
#include "view_shared.h"
#include "view.h"
#include "iviewrender.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar phys_gunmass("phys_gunmass", "200", FCVAR_REPLICATED);
ConVar phys_gunvel("phys_gunvel", "400", FCVAR_REPLICATED);
ConVar phys_gunforce("phys_gunforce", "5e5", FCVAR_REPLICATED );
ConVar phys_guntorque("phys_guntorque", "100", FCVAR_REPLICATED );

static int g_physgunBeam;
#define PHYSGUN_BEAM_SPRITE		"sprites/physbeam.vmt"

#define	PHYSGUN_SKIN	1

class CWeaponGravityGun;

#ifdef CLIENT_DLL
CLIENTEFFECT_REGISTER_BEGIN( PrecacheEffectGravityGun )
CLIENTEFFECT_MATERIAL( "sprites/physbeam" )
CLIENTEFFECT_REGISTER_END()

class C_BeamQuadratic : public CDefaultClientRenderable
{
public:
	C_BeamQuadratic();
	void			Update( C_BaseEntity *pOwner );

	const matrix3x4_t &C_BeamQuadratic::RenderableToWorldTransform( void )
	{
		return RenderableToWorldTransform();
	}

	// IClientRenderable
	virtual const Vector&			GetRenderOrigin( void ) { return m_worldPosition; }
	virtual const QAngle&			GetRenderAngles( void ) { return vec3_angle; }
	virtual bool					ShouldDraw( void ) { return true; }
	virtual bool					IsTransparent( void ) { return true; }
	virtual bool					ShouldReceiveProjectedTextures( int flags ) { return false; }
	virtual int						DrawModel( int flags );

	// Returns the bounds relative to the origin (render bounds)
	virtual void	GetRenderBounds( Vector& mins, Vector& maxs )
	{
		ClearBounds( mins, maxs );
		AddPointToBounds( vec3_origin, mins, maxs );
		AddPointToBounds( m_targetPosition, mins, maxs );
		AddPointToBounds( m_worldPosition, mins, maxs );
		mins -= GetRenderOrigin();
		maxs -= GetRenderOrigin();
	}

	C_BaseEntity			*m_pOwner;
	Vector					m_targetPosition;
	Vector					m_worldPosition;
	int						m_active;
	int						m_glueTouching;
	int						m_viewModelIndex;
};


#endif

class CGravControllerPoint : public IMotionEvent
{
	DECLARE_SIMPLE_DATADESC();

public:
	CGravControllerPoint( void );
	~CGravControllerPoint( void );
	void AttachEntity( CBaseEntity *pEntity, IPhysicsObject *pPhys, const Vector &position );
	void DetachEntity( void );
	void SetMaxVelocity( float maxVel )
	{
		m_maxVel = maxVel;
	}
	void SetTargetPosition( const Vector &target )
	{
		m_targetPosition = target;
		if ( m_attachedEntity == NULL )
		{
			m_worldPosition = target;
		}
		m_timeToArrive = gpGlobals->frametime;
	}

	IMotionEvent::simresult_e Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular );
	Vector			m_localPosition;
	Vector			m_targetPosition;
	Vector			m_worldPosition;
	float			m_saveDamping;
	float			m_maxVel;
	float			m_maxAcceleration;
	Vector			m_maxAngularAcceleration;
	EHANDLE			m_attachedEntity;
	QAngle			m_targetRotation;
	float			m_timeToArrive;

	IPhysicsMotionController *m_controller;
};


BEGIN_SIMPLE_DATADESC( CGravControllerPoint )

	DEFINE_FIELD( m_localPosition,		FIELD_VECTOR ),
	DEFINE_FIELD( m_targetPosition,		FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_worldPosition,		FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_saveDamping,			FIELD_FLOAT ),
	DEFINE_FIELD( m_maxVel,				FIELD_FLOAT ),
	DEFINE_FIELD( m_maxAcceleration,		FIELD_FLOAT ),
	DEFINE_FIELD( m_maxAngularAcceleration,	FIELD_VECTOR ),
	DEFINE_FIELD( m_attachedEntity,		FIELD_EHANDLE ),
	DEFINE_FIELD( m_targetRotation,		FIELD_VECTOR ),
	DEFINE_FIELD( m_timeToArrive,			FIELD_FLOAT ),

	// Physptrs can't be saved in embedded classes... this is to silence classcheck
	// DEFINE_PHYSPTR( m_controller ),

END_DATADESC()


CGravControllerPoint::CGravControllerPoint( void )
{
	m_attachedEntity = NULL;
}

CGravControllerPoint::~CGravControllerPoint( void )
{
	DetachEntity();
}


void CGravControllerPoint::AttachEntity( CBaseEntity *pEntity, IPhysicsObject *pPhys, const Vector &position )
{
	m_attachedEntity = pEntity;
	pPhys->WorldToLocal( &m_localPosition, position );
	m_worldPosition = position;
	pPhys->GetDamping( NULL, &m_saveDamping );
	float damping = 2;
	pPhys->SetDamping( NULL, &damping );
	m_controller = physenv->CreateMotionController( this );
	m_controller->AttachObject( pPhys, true );
	m_controller->SetPriority( IPhysicsMotionController::HIGH_PRIORITY );
	SetTargetPosition( position );
	m_maxAcceleration = phys_gunforce.GetFloat() * pPhys->GetInvMass();
	m_targetRotation = pEntity->GetAbsAngles();
	float torque = phys_guntorque.GetFloat();
	m_maxAngularAcceleration = torque * pPhys->GetInvInertia();
}

void CGravControllerPoint::DetachEntity( void )
{
	CBaseEntity *pEntity = m_attachedEntity;
	if ( pEntity )
	{
		IPhysicsObject *pPhys = pEntity->VPhysicsGetObject();
		if ( pPhys )
		{
			// on the odd chance that it's gone to sleep while under anti-gravity
			pPhys->Wake();
			pPhys->SetDamping( NULL, &m_saveDamping );
		}
	}
	m_attachedEntity = NULL;
	if ( physenv )
	{
		physenv->DestroyMotionController( m_controller );
	}
	m_controller = NULL;

	// UNDONE: Does this help the networking?
	m_targetPosition = vec3_origin;
	m_worldPosition = vec3_origin;
}

void AxisAngleQAngle( const Vector &axis, float angle, QAngle &outAngles )
{
	// map back to HL rotation axes
	outAngles.z = axis.x * angle;
	outAngles.x = axis.y * angle;
	outAngles.y = axis.z * angle;
}

IMotionEvent::simresult_e CGravControllerPoint::Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular )
{
	Vector vel;
	AngularImpulse angVel;

	float fracRemainingSimTime = 1.0;
	if ( m_timeToArrive > 0 )
	{
		fracRemainingSimTime *= deltaTime / m_timeToArrive;
		if ( fracRemainingSimTime > 1 )
		{
			fracRemainingSimTime = 1;
		}
	}
	
	m_timeToArrive -= deltaTime;
	if ( m_timeToArrive < 0 )
	{
		m_timeToArrive = 0;
	}

	float invDeltaTime = (1.0f / deltaTime);
	Vector world;
	pObject->LocalToWorld( &world, m_localPosition );
	m_worldPosition = world;
	pObject->GetVelocity( &vel, &angVel );
	//pObject->GetVelocityAtPoint( world, &vel );
	float damping = 1.0;
	world += vel * deltaTime * damping;
	Vector delta = (m_targetPosition - world) * fracRemainingSimTime * invDeltaTime;
	Vector alignDir;
	linear = vec3_origin;
	angular = vec3_origin;

	// clamp future velocity to max speed
	Vector nextVel = delta + vel;
	float nextSpeed = nextVel.Length();
	if ( nextSpeed > m_maxVel )
	{
		nextVel *= (m_maxVel / nextSpeed);
		delta = nextVel - vel;
	}

	delta *= invDeltaTime;

	float linearAccel = delta.Length();
	if ( linearAccel > m_maxAcceleration )
	{
		delta *= m_maxAcceleration / linearAccel;
	}

	Vector accel;
	AngularImpulse angAccel;
	pObject->CalculateForceOffset( delta, world, &accel, &angAccel );
	
	linear += accel;
	angular += angAccel;
	
	return SIM_GLOBAL_ACCELERATION;
}


#ifdef CLIENT_DLL
#define CWeaponGravityGun C_WeaponGravityGun
#endif

class CWeaponGravityGun : public CBaseHL2MPCombatWeapon
{
	DECLARE_DATADESC();

public:
	DECLARE_CLASS( CWeaponGravityGun, CBaseHL2MPCombatWeapon );

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CWeaponGravityGun();

#ifdef CLIENT_DLL
	int KeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding )
	{
		if ( gHUD.m_iKeyBits & IN_ATTACK )
		{
			switch ( keynum )
			{
			case MOUSE_WHEEL_UP:
				gHUD.m_iKeyBits |= IN_WEAPON1;
				return 0;

			case MOUSE_WHEEL_DOWN:
				gHUD.m_iKeyBits |= IN_WEAPON2;
				return 0;
			}
		}

		// Allow engine to process
		return BaseClass::KeyInput( down, keynum, pszCurrentBinding );
	}

	int	 DrawModel( int flags );
	void ViewModelDrawn( C_BaseViewModel *pBaseViewModel );
	bool IsTransparent( void );
	void OnDataChanged( DataUpdateType_t updateType )
	{
		BaseClass::OnDataChanged( updateType );
		// m_beam.Update( this );
	}
#endif

	void Spawn( void );
	void OnRestore( void );
	void Precache( void );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	void WeaponIdle( void );
	void ItemPostFrame( void );
	virtual bool Holster( CBaseCombatWeapon *pSwitchingTo )
	{
		EffectDestroy();
		SoundDestroy();
		return BaseClass::Holster( pSwitchingTo );
	}

	bool Reload( void );
	void Drop(const Vector &vecVelocity)
	{
		EffectDestroy();
		SoundDestroy();
		BaseClass::Drop(vecVelocity);
	}

	bool HasAnyAmmo( void );

	void AttachObject( CBaseEntity *pEdict, const Vector& start, const Vector &end, float distance );
	void DetachObject( void );

	void EffectCreate( void );
	void EffectUpdate( void );
	void EffectDestroy( void );

	void SoundCreate( void );
	void SoundDestroy( void );
	void SoundStop( void );
	void SoundStart( void );
	void SoundUpdate( void );

	int ObjectCaps( void ) 
	{ 
		int caps = BaseClass::ObjectCaps();
		if ( m_active )
		{
			caps |= FCAP_DIRECTIONAL_USE;
		}
		return caps;
	}

	CBaseEntity *GetBeamEntity();

private:
#ifdef CLIENT_DLL
	C_BeamQuadratic	m_beam;
#endif

	CNetworkVar( int, m_active );
	bool		m_useDown;
	EHANDLE		m_hObject;
	float		m_distance;
	float		m_movementLength;
	float		m_lastYaw;
	int			m_soundState;
	Vector		m_originalObjectPosition;

	CGravControllerPoint		m_gravCallback;

	DECLARE_ACTTABLE();
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponGravityGun, DT_WeaponGravityGun )

BEGIN_NETWORK_TABLE( CWeaponGravityGun, DT_WeaponGravityGun )
#ifdef CLIENT_DLL
	RecvPropVector( RECVINFO_NAME(m_beam.m_targetPosition,m_targetPosition) ),
	RecvPropVector( RECVINFO_NAME(m_beam.m_worldPosition, m_worldPosition) ),
	RecvPropInt( RECVINFO_NAME(m_beam.m_active, m_active) ),
	RecvPropVector( RECVINFO_NAME(m_gravCallback.m_targetPosition, m_targetPosition) ),
	RecvPropVector( RECVINFO_NAME(m_gravCallback.m_worldPosition, m_worldPosition) ),
	RecvPropInt( RECVINFO(m_active) ),
#else
	SendPropVector( SENDINFO_NAME(m_gravCallback.m_targetPosition, m_targetPosition), -1, SPROP_COORD ),
	SendPropVector( SENDINFO_NAME(m_gravCallback.m_worldPosition, m_worldPosition), -1, SPROP_COORD ),
	SendPropInt( SENDINFO(m_active), 1, SPROP_UNSIGNED ),
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponGravityGun )
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_physgun, CWeaponGravityGun );
PRECACHE_WEAPON_REGISTER(weapon_physgun);

acttable_t	CWeaponGravityGun::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_PHYSGUN,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_PHYSGUN,			false },

	{ ACT_MP_RUN,						ACT_HL2MP_RUN_PHYSGUN,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_PHYSGUN,			false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PHYSGUN,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PHYSGUN,	false },

	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_PHYSGUN,		false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_PHYSGUN,		false },

	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_PHYSGUN,					false },
};

IMPLEMENT_ACTTABLE(CWeaponGravityGun);


#ifdef CLIENT_DLL
C_BeamQuadratic::C_BeamQuadratic()
{
	m_pOwner = NULL;
}

void C_BeamQuadratic::Update( C_BaseEntity *pOwner )
{
	m_pOwner = pOwner;
	if ( m_active )
	{
		if ( m_hRenderHandle == INVALID_CLIENT_RENDER_HANDLE )
		{
			ClientLeafSystem()->AddRenderable( this, RENDER_GROUP_VIEW_MODEL_TRANSLUCENT );
		}
		else
		{
			ClientLeafSystem()->RenderableChanged( m_hRenderHandle );
		}
	}
	else if ( !m_active && m_hRenderHandle != INVALID_CLIENT_RENDER_HANDLE )
	{
		ClientLeafSystem()->RemoveRenderable( m_hRenderHandle );
	}
}


int	C_BeamQuadratic::DrawModel( int )
{
	C_BasePlayer *pOwner = (C_BasePlayer *)m_pOwner->GetOwnerEntity();

	if ( !pOwner )
		return 0;

	Vector points[3];
	QAngle tmpAngle;

	if ( !m_active )
		return 0;

	C_BaseEntity *pEnt = cl_entitylist->GetEnt( m_viewModelIndex );
	if ( !pEnt )
		return 0;
	pEnt->GetAttachment( 1, points[0], tmpAngle );

	points[1] = 0.5 * (m_targetPosition + points[0]);
	
	// a little noise 11t & 13t should be somewhat non-periodic looking
	//points[1].z += 4*sin( gpGlobals->curtime*11 ) + 5*cos( gpGlobals->curtime*13 );
	points[2] = m_worldPosition;

	IMaterial *pMat = materials->FindMaterial( "sprites/physbeam", TEXTURE_GROUP_CLIENT_EFFECTS );
	Vector color;
	color.Init(1,1,1);

	float flWidthRatio = engine->GetScreenAspectRatio() / ( 4.0f / 3.0f );

	// Now draw it.
	CViewSetup beamView;
	beamView.x = 0;
	beamView.y = 0;
	beamView.width = ScreenWidth();
	beamView.height = ScreenHeight();

	beamView.m_bOrtho = false;

	// scale the FOV for aspect ratios other than 4/3
	beamView.fov = ScaleFOVByWidthRatio( pOwner->GetFOV(), flWidthRatio );

	beamView.origin = pOwner->EyePosition();
	beamView.angles = pOwner->EyeAngles();
	beamView.zNear = view->GetZNear();
	beamView.zFar = view->GetZFar();

	Frustum dummyFrustum;
	render->Push3DView( beamView, 0, NULL, dummyFrustum );

	float scrollOffset = gpGlobals->curtime - (int)gpGlobals->curtime;
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->Bind( pMat );
	DrawBeamQuadratic( points[0], points[1], points[2], 13, color, scrollOffset );

	render->PopView( dummyFrustum );
	return 1;
}

#endif

//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CWeaponGravityGun )

	DEFINE_FIELD( m_active,				FIELD_INTEGER ),
	DEFINE_FIELD( m_useDown,				FIELD_BOOLEAN ),
	DEFINE_FIELD( m_hObject,				FIELD_EHANDLE ),
	DEFINE_FIELD( m_distance,			FIELD_FLOAT ),
	DEFINE_FIELD( m_movementLength,		FIELD_FLOAT ),
	DEFINE_FIELD( m_lastYaw,				FIELD_FLOAT ),
	DEFINE_FIELD( m_soundState,			FIELD_INTEGER ),
	DEFINE_FIELD( m_originalObjectPosition,	FIELD_POSITION_VECTOR ),
	DEFINE_EMBEDDED( m_gravCallback ),
	// Physptrs can't be saved in embedded classes..
	DEFINE_PHYSPTR( m_gravCallback.m_controller ),

END_DATADESC()


enum physgun_soundstate { SS_SCANNING, SS_LOCKEDON };
enum physgun_soundIndex { SI_LOCKEDON = 0, SI_SCANNING = 1, SI_LIGHTOBJECT = 2, SI_HEAVYOBJECT = 3, SI_ON, SI_OFF };


//=========================================================
//=========================================================

CWeaponGravityGun::CWeaponGravityGun()
{
	m_active = false;
	m_bFiresUnderwater = true;
}

//=========================================================
//=========================================================
void CWeaponGravityGun::Spawn( )
{
	BaseClass::Spawn();
//	SetModel( GetWorldModel() );

	// The physgun uses a different skin
	m_nSkin = PHYSGUN_SKIN;

	FallInit();
}

void CWeaponGravityGun::OnRestore( void )
{
	BaseClass::OnRestore();

	if ( m_gravCallback.m_controller )
	{
		m_gravCallback.m_controller->SetEventHandler( &m_gravCallback );
	}
}


//=========================================================
//=========================================================
void CWeaponGravityGun::Precache( void )
{
	BaseClass::Precache();

	g_physgunBeam = PrecacheModel(PHYSGUN_BEAM_SPRITE);

	PrecacheScriptSound( "Weapon_Physgun.Scanning" );
	PrecacheScriptSound( "Weapon_Physgun.LockedOn" );
	PrecacheScriptSound( "Weapon_Physgun.Scanning" );
	PrecacheScriptSound( "Weapon_Physgun.LightObject" );
	PrecacheScriptSound( "Weapon_Physgun.HeavyObject" );
}

void CWeaponGravityGun::EffectCreate( void )
{
	EffectUpdate();
	m_active = true;
}


void CWeaponGravityGun::EffectUpdate( void )
{
	Vector start, angles, forward, right;
	trace_t tr;

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

#ifdef CLIENT_DLL
	m_beam.m_viewModelIndex = pOwner->entindex();
	// Make sure I've got a view model
	CBaseViewModel *vm = pOwner->GetViewModel();
	if ( vm )
	{
		m_beam.m_viewModelIndex = vm->entindex();
	}
#endif

	pOwner->EyeVectors( &forward, &right, NULL );

	start = pOwner->Weapon_ShootPosition();
	Vector end = start + forward * 4096;

	UTIL_TraceLine( start, end, MASK_SHOT, pOwner, COLLISION_GROUP_NONE, &tr );
	end = tr.endpos;
	float distance = tr.fraction * 4096;
	if ( tr.fraction != 1 )
	{
		// too close to the player, drop the object
		if ( distance < 36 )
		{
			DetachObject();
			return;
		}
	}

	if ( m_hObject == NULL && tr.DidHitNonWorldEntity() )
	{
		CBaseEntity *pEntity = tr.m_pEnt;
		AttachObject( pEntity, start, tr.endpos, distance );
		m_lastYaw = pOwner->EyeAngles().y;
	}

	// Add the incremental player yaw to the target transform
	matrix3x4_t curMatrix, incMatrix, nextMatrix;
	AngleMatrix( m_gravCallback.m_targetRotation, curMatrix );
	AngleMatrix( QAngle(0,pOwner->EyeAngles().y - m_lastYaw,0), incMatrix );
	ConcatTransforms( incMatrix, curMatrix, nextMatrix );
	MatrixAngles( nextMatrix, m_gravCallback.m_targetRotation );
	m_lastYaw = pOwner->EyeAngles().y;

	CBaseEntity *pObject = m_hObject;
	if ( pObject )
	{
		if ( m_useDown )
		{
			if ( pOwner->m_afButtonPressed & IN_USE )
			{
				m_useDown = false;
			}
		}
		else 
		{
			if ( pOwner->m_afButtonPressed & IN_USE )
			{
				m_useDown = true;
			}
		}

		if ( m_useDown )
		{
#ifndef CLIENT_DLL
			pOwner->SetPhysicsFlag( PFLAG_DIROVERRIDE, true );
#endif
			if ( pOwner->m_nButtons & IN_FORWARD )
			{
				m_distance = Approach( 1024, m_distance, gpGlobals->frametime * 100 );
			}
			if ( pOwner->m_nButtons & IN_BACK )
			{
				m_distance = Approach( 40, m_distance, gpGlobals->frametime * 100 );
			}
		}

		if ( pOwner->m_nButtons & IN_WEAPON1 )
		{
			m_distance = Approach( 1024, m_distance, m_distance * 0.1 );
		}
		if ( pOwner->m_nButtons & IN_WEAPON2 )
		{
			m_distance = Approach( 40, m_distance, m_distance * 0.1 );
		}

		IPhysicsObject *pPhys = pObject->VPhysicsGetObject();
		if ( pPhys && pPhys->IsAsleep() )
		{
			// on the odd chance that it's gone to sleep while under anti-gravity
			pPhys->Wake();
		}

		Vector newPosition = start + forward * m_distance;
		// 24 is a little larger than 16 * sqrt(2) (extent of player bbox)
		// HACKHACK: We do this so we can "ignore" the player and the object we're manipulating
		// If we had a filter for tracelines, we could simply filter both ents and start from "start"
		Vector awayfromPlayer = start + forward * 24;

		UTIL_TraceLine( start, awayfromPlayer, MASK_SOLID, pOwner, COLLISION_GROUP_NONE, &tr );
		if ( tr.fraction == 1 )
		{
			UTIL_TraceLine( awayfromPlayer, newPosition, MASK_SOLID, pObject, COLLISION_GROUP_NONE, &tr );
			Vector dir = tr.endpos - newPosition;
			float distance = VectorNormalize(dir);
			float maxDist = m_gravCallback.m_maxVel * gpGlobals->frametime;
			if ( distance >  maxDist )
			{
				newPosition += dir * maxDist;
		}
		else
		{
			newPosition = tr.endpos;
		}
		}
		else
		{
			newPosition = tr.endpos;
		}

		m_gravCallback.SetTargetPosition( newPosition );
		Vector dir = (newPosition - pObject->GetLocalOrigin());
		m_movementLength = dir.Length();
	}
	else
	{
		m_gravCallback.SetTargetPosition( end );
	}
}

void CWeaponGravityGun::SoundCreate( void )
{
	m_soundState = SS_SCANNING;
	SoundStart();
}


void CWeaponGravityGun::SoundDestroy( void )
{
	SoundStop();
}


void CWeaponGravityGun::SoundStop( void )
{
	switch( m_soundState )
	{
	case SS_SCANNING:
		GetOwner()->StopSound( "Weapon_Physgun.Scanning" );
		break;
	case SS_LOCKEDON:
		GetOwner()->StopSound( "Weapon_Physgun.Scanning" );
		GetOwner()->StopSound( "Weapon_Physgun.LockedOn" );
		GetOwner()->StopSound( "Weapon_Physgun.LightObject" );
		GetOwner()->StopSound( "Weapon_Physgun.HeavyObject" );
		break;
	}
}



//-----------------------------------------------------------------------------
// Purpose: returns the linear fraction of value between low & high (0.0 - 1.0) * scale
//			e.g. UTIL_LineFraction( 1.5, 1, 2, 1 ); will return 0.5 since 1.5 is
//			halfway between 1 and 2
// Input  : value - a value between low & high (clamped)
//			low - the value that maps to zero
//			high - the value that maps to "scale"
//			scale - the output scale
// Output : parametric fraction between low & high
//-----------------------------------------------------------------------------
static float UTIL_LineFraction( float value, float low, float high, float scale )
{
	if ( value < low )
		value = low;
	if ( value > high )
		value = high;

	float delta = high - low;
	if ( delta == 0 )
		return 0;
	
	return scale * (value-low) / delta;
}

void CWeaponGravityGun::SoundStart( void )
{
	CPASAttenuationFilter filter( GetOwner() );
	filter.MakeReliable();

	switch( m_soundState )
	{
	case SS_SCANNING:
		{
			EmitSound( filter, GetOwner()->entindex(), "Weapon_Physgun.Scanning" );
		}
		break;
	case SS_LOCKEDON:
		{
			// BUGBUG - If you start a sound with a pitch of 100, the pitch shift doesn't work!
			
			EmitSound( filter, GetOwner()->entindex(), "Weapon_Physgun.LockedOn" );
			EmitSound( filter, GetOwner()->entindex(), "Weapon_Physgun.Scanning" );
			EmitSound( filter, GetOwner()->entindex(), "Weapon_Physgun.LightObject" );
			EmitSound( filter, GetOwner()->entindex(), "Weapon_Physgun.HeavyObject" );
		}
		break;
	}
													//   volume, att, flags, pitch
}

void CWeaponGravityGun::SoundUpdate( void )
{
	int newState;
	
	if ( m_hObject )
		newState = SS_LOCKEDON;
	else
		newState = SS_SCANNING;

	if ( newState != m_soundState )
	{
		SoundStop();
		m_soundState = newState;
		SoundStart();
	}

	switch( m_soundState )
	{
	case SS_SCANNING:
		break;
	case SS_LOCKEDON:
		{
			CPASAttenuationFilter filter( GetOwner() );
			filter.MakeReliable();

			float height = m_hObject->GetAbsOrigin().z - m_originalObjectPosition.z;

			// go from pitch 90 to 150 over a height of 500
			int pitch = 90 + (int)UTIL_LineFraction( height, 0, 500, 60 );

			CSoundParameters params;
			if ( GetParametersForSound( "Weapon_Physgun.LockedOn", params, NULL ) )
			{
				EmitSound_t ep( params );
				ep.m_nFlags = SND_CHANGE_VOL | SND_CHANGE_PITCH;
				ep.m_nPitch = pitch;

				EmitSound( filter, GetOwner()->entindex(), ep );
			}

			// attenutate the movement sounds over 200 units of movement
			float distance = UTIL_LineFraction( m_movementLength, 0, 200, 1.0 );

			// blend the "mass" sounds between 50 and 500 kg
			IPhysicsObject *pPhys = m_hObject->VPhysicsGetObject();
			if ( pPhys == NULL )
			{
				// we no longer exist!
				break;
			}
			
			float fade = UTIL_LineFraction( pPhys->GetMass(), 50, 500, 1.0 );

			if ( GetParametersForSound( "Weapon_Physgun.LightObject", params, NULL ) )
			{
				EmitSound_t ep( params );
				ep.m_nFlags = SND_CHANGE_VOL;
				ep.m_flVolume = fade * distance;

				EmitSound( filter, GetOwner()->entindex(), ep );
			}

			if ( GetParametersForSound( "Weapon_Physgun.HeavyObject", params, NULL ) )
			{
				EmitSound_t ep( params );
				ep.m_nFlags = SND_CHANGE_VOL;
				ep.m_flVolume = (1.0 - fade) * distance;

				EmitSound( filter, GetOwner()->entindex(), ep );
			}
		}
		break;
	}
}


CBaseEntity *CWeaponGravityGun::GetBeamEntity()
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return NULL;

	// Make sure I've got a view model
	CBaseViewModel *vm = pOwner->GetViewModel();
	if ( vm )
		return vm;

	return pOwner;
}

void CWeaponGravityGun::EffectDestroy( void )
{
	m_active = false;
	SoundStop();

	DetachObject();
}

void CWeaponGravityGun::DetachObject( void )
{
	if ( m_hObject )
	{
#ifndef CLIENT_DLL
		CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
		Pickup_OnPhysGunDrop( m_hObject, pOwner, DROPPED_BY_CANNON );
#endif

		m_gravCallback.DetachEntity();
		m_hObject = NULL;
	}
}

void CWeaponGravityGun::AttachObject( CBaseEntity *pObject, const Vector& start, const Vector &end, float distance )
{
	m_hObject = pObject;
	m_useDown = false;
	IPhysicsObject *pPhysics = pObject ? (pObject->VPhysicsGetObject()) : NULL;
	if ( pPhysics && pObject->GetMoveType() == MOVETYPE_VPHYSICS )
	{
		m_distance = distance;

		m_gravCallback.AttachEntity( pObject, pPhysics, end );
		float mass = pPhysics->GetMass();
//		Msg( "Object mass: %.2f lbs (%.2f kg)\n", kg2lbs(mass), mass );
		float vel = phys_gunvel.GetFloat();
		if ( mass > phys_gunmass.GetFloat() )
		{
			vel = (vel*phys_gunmass.GetFloat())/mass;
		}
		m_gravCallback.SetMaxVelocity( vel );
//		Msg( "Object mass: %.2f lbs (%.2f kg) %f %f %f\n", kg2lbs(mass), mass, pObject->GetAbsOrigin().x, pObject->GetAbsOrigin().y, pObject->GetAbsOrigin().z );
//		Msg( "ANG: %f %f %f\n", pObject->GetAbsAngles().x, pObject->GetAbsAngles().y, pObject->GetAbsAngles().z );

		m_originalObjectPosition = pObject->GetAbsOrigin();

		pPhysics->Wake();

#ifndef CLIENT_DLL
		CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
		if( pOwner )
		{
			Pickup_OnPhysGunPickup( pObject, pOwner );
		}
#endif
	}
	else
	{
		m_hObject = NULL;
	}
}

//=========================================================
//=========================================================
void CWeaponGravityGun::PrimaryAttack( void )
{
	if ( !m_active )
	{
		SendWeaponAnim( ACT_VM_PRIMARYATTACK );
		EffectCreate();
		SoundCreate();
	}
	else
	{
		EffectUpdate();
		SoundUpdate();
	}
}

void CWeaponGravityGun::SecondaryAttack( void )
{
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.1;
	if ( m_active )
	{
		EffectDestroy();
		SoundDestroy();
		return;
	}

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	Assert( pOwner );

	if ( pOwner->GetAmmoCount(m_iSecondaryAmmoType) <= 0 )
		return;

#ifdef CLIENT_DLL
	m_beam.m_viewModelIndex = pOwner->entindex();
	// Make sure I've got a view model
	CBaseViewModel *vm = pOwner->GetViewModel();
	if ( vm )
	{
		m_beam.m_viewModelIndex = vm->entindex();
	}
#endif

	Vector forward;
	pOwner->EyeVectors( &forward );

	Vector start = pOwner->Weapon_ShootPosition();
	Vector end = start + forward * 4096;

	trace_t tr;
	UTIL_TraceLine( start, end, MASK_SHOT, pOwner, COLLISION_GROUP_NONE, &tr );
	if ( tr.fraction == 1.0 || (tr.surface.flags & SURF_SKY) )
		return;

	CBaseEntity *pHit = tr.m_pEnt;
	
	if ( pHit->entindex() == 0 )
	{
		pHit = NULL;
	}
	else
	{
		// if the object has no physics object, or isn't a physprop or brush entity, then don't glue
		if ( !pHit->VPhysicsGetObject() || pHit->GetMoveType() != MOVETYPE_VPHYSICS )
			return;
	}

	QAngle angles;
	WeaponSound( SINGLE );

	VectorAngles( tr.plane.normal, angles );
	Vector endPoint = tr.endpos + tr.plane.normal;

	// UNDONE: Probably should just do this client side
	CBaseEntity *pEnt = GetBeamEntity();
	CBeam *pBeam = CBeam::BeamCreate( PHYSGUN_BEAM_SPRITE, 1.5 );
	pBeam->PointEntInit( endPoint, pEnt );
	pBeam->SetEndAttachment( 1 );
	pBeam->SetBrightness( 255 );
	pBeam->SetColor( 255, 0, 0 );
	pBeam->RelinkBeam();
	pBeam->LiveForTime( 0.1 );

}

void CWeaponGravityGun::WeaponIdle( void )
{
	if ( HasWeaponIdleTimeElapsed() )
	{
		SendWeaponAnim( ACT_VM_IDLE );
	}
}

#ifdef CLIENT_DLL
//-----------------------------------------------------------------------------
// Purpose: Third-person function call to render world model
//-----------------------------------------------------------------------------
int CWeaponGravityGun::DrawModel( int flags )
{
	// Only render these on the transparent pass
	if ( flags & STUDIO_TRANSPARENCY )
	{
		C_BasePlayer *pOwner = ToBasePlayer( GetOwner() );

		if ( !pOwner )
			return 0;

		Vector points[3];
		QAngle tmpAngle;

		if ( !m_active )
			return 0;

		GetAttachment( 1, points[0], tmpAngle );

		points[1] = 0.5 * (m_gravCallback.m_targetPosition + points[0]);
		
		// a little noise 11t & 13t should be somewhat non-periodic looking
		//points[1].z += 4*sin( gpGlobals->curtime*11 ) + 5*cos( gpGlobals->curtime*13 );
		points[2] = m_gravCallback.m_worldPosition;

		IMaterial *pMat = materials->FindMaterial( "sprites/physbeam", TEXTURE_GROUP_CLIENT_EFFECTS );
		Vector color;
		color.Init(1,1,1);

		float scrollOffset = gpGlobals->curtime - (int)gpGlobals->curtime;
		CMatRenderContextPtr pRenderContext( materials );
		pRenderContext->Bind( pMat );
		DrawBeamQuadratic( points[0], points[1], points[2], 13, color, scrollOffset );
		return 1;
	}

	// Only do this on the opaque pass
	return BaseClass::DrawModel( flags );
}

//-----------------------------------------------------------------------------
// Purpose: First-person function call after viewmodel has been drawn
//-----------------------------------------------------------------------------
void CWeaponGravityGun::ViewModelDrawn( C_BaseViewModel *pBaseViewModel )
{
	// Render our effects
	m_beam.Update( this );

	// Pass this back up
	BaseClass::ViewModelDrawn( pBaseViewModel );
}

//-----------------------------------------------------------------------------
// Purpose: We are always considered transparent
//-----------------------------------------------------------------------------
bool CWeaponGravityGun::IsTransparent( void )
{
	return true;
}

#endif

void CWeaponGravityGun::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if (!pOwner)
		return;

	if ( pOwner->m_afButtonPressed & IN_ATTACK2 )
	{
		SecondaryAttack();
	}
	else if ( pOwner->m_nButtons & IN_ATTACK )
	{
		PrimaryAttack();
	}
	else if ( pOwner->m_afButtonPressed & IN_RELOAD )
	{
		Reload();
	}
	// -----------------------
	//  No buttons down
	// -----------------------
	else 
	{
		if ( m_active )
		{
			EffectDestroy();
			SoundDestroy();
		}
		WeaponIdle( );
		return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponGravityGun::HasAnyAmmo( void )
{
	//Always report that we have ammo
	return true;
}

//=========================================================
//=========================================================
bool CWeaponGravityGun::Reload( void )
{
	return false;
}

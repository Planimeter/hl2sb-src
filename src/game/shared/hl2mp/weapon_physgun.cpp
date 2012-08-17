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
#include "soundenvelope.h"
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
#include "ragdoll.h"
#else
#include "physics_prop_ragdoll.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static int g_physgunBeam1;
static int g_physgunBeam;
static int g_physgunGlow;
#define PHYSGUN_BEAM_SPRITE1	"sprites/physbeam1.vmt"
#define PHYSGUN_BEAM_SPRITE		"sprites/physbeam.vmt"
#define PHYSGUN_BEAM_GLOW		"sprites/physglow.vmt"

#define	PHYSGUN_SKIN	1

class CWeaponGravityGun;

#ifdef CLIENT_DLL
CLIENTEFFECT_REGISTER_BEGIN( PrecacheEffectGravityGun )
CLIENTEFFECT_MATERIAL( "sprites/physbeam1" )
CLIENTEFFECT_MATERIAL( "sprites/physbeam" )
CLIENTEFFECT_MATERIAL( "sprites/physglow" )
CLIENTEFFECT_REGISTER_END()


#endif

IPhysicsObject *GetPhysObjFromPhysicsBone( CBaseEntity *pEntity, short physicsbone )
{
	CBaseAnimating *pModel = static_cast< CBaseAnimating * >( pEntity );
	if ( pModel == NULL )
	{
		return pEntity->VPhysicsGetObject();
	}
	else
	{
		IPhysicsObject	*pPhysicsObject = NULL;
		
		//Find the real object we hit.
		if( physicsbone >= 0 )
		{
#ifdef CLIENT_DLL
			if ( pModel->m_pRagdoll )
			{
				CRagdoll *pCRagdoll = dynamic_cast < CRagdoll * > ( pModel->m_pRagdoll );
#else
				// Affect the object
				CRagdollProp *pCRagdoll = dynamic_cast<CRagdollProp*>( pEntity );
#endif
				if ( pCRagdoll )
				{
					ragdoll_t *pRagdollT = pCRagdoll->GetRagdoll();

					if ( physicsbone < pRagdollT->listCount )
					{
						pPhysicsObject = pRagdollT->list[physicsbone].pObject;
					}
					return pPhysicsObject;
				}
				else
				{
					return pEntity->VPhysicsGetObject();
				}
#ifdef CLIENT_DLL
			}
			else
			{
				return pEntity->VPhysicsGetObject();
			}
#endif
		}
		else
		{
			return pEntity->VPhysicsGetObject();
		}
	}
}

class CGravControllerPoint : public IMotionEvent
{
	DECLARE_SIMPLE_DATADESC();

public:
	CGravControllerPoint( void );
	~CGravControllerPoint( void );
	void AttachEntity( CBasePlayer *pPlayer, CBaseEntity *pEntity, IPhysicsObject *pPhys, short physicsbone, const Vector &position );
	void DetachEntity( void );

	bool UpdateObject( CBasePlayer *pPlayer, CBaseEntity *pEntity );

	void SetTargetPosition( const Vector &target, const QAngle &targetOrientation )
	{
		m_shadow.targetPosition = target;
		m_shadow.targetRotation = targetOrientation;

		m_timeToArrive = gpGlobals->frametime;

		CBaseEntity *pAttached = m_attachedEntity;
		if ( pAttached )
		{
			IPhysicsObject *pObj = GetPhysObjFromPhysicsBone( pAttached, m_attachedPhysicsBone );
			
			if ( pObj != NULL )
			{
				pObj->Wake();
			}
			else
			{
				DetachEntity();
			}
		}
	}
	QAngle TransformAnglesToPlayerSpace( const QAngle &anglesIn, CBasePlayer *pPlayer );
	QAngle TransformAnglesFromPlayerSpace( const QAngle &anglesIn, CBasePlayer *pPlayer );

	IMotionEvent::simresult_e Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular );
	Vector			m_localPosition;
	Vector			m_targetPosition;
	Vector			m_worldPosition;
	float			m_saveDamping;
	float			m_saveMass;
	float			m_maxAcceleration;
	Vector			m_maxAngularAcceleration;
	EHANDLE			m_attachedEntity;
	short			m_attachedPhysicsBone;
	QAngle			m_targetRotation;
	float			m_timeToArrive;

#ifdef ARGG
	// adnan
	// set up the modified pickup angles... allow the player to rotate the object in their grip
	QAngle		m_vecRotatedCarryAngles;
	bool			m_bHasRotatedCarryAngles;
	// end adnan
#endif

	IPhysicsMotionController *m_controller;

private:
	hlshadowcontrol_params_t	m_shadow;
};


BEGIN_SIMPLE_DATADESC( CGravControllerPoint )

	DEFINE_FIELD( m_localPosition,		FIELD_VECTOR ),
	DEFINE_FIELD( m_targetPosition,		FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_worldPosition,		FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_saveDamping,			FIELD_FLOAT ),
	DEFINE_FIELD( m_saveMass,			FIELD_FLOAT ),
	DEFINE_FIELD( m_maxAcceleration,		FIELD_FLOAT ),
	DEFINE_FIELD( m_maxAngularAcceleration,	FIELD_VECTOR ),
	DEFINE_FIELD( m_attachedEntity,		FIELD_EHANDLE ),
	DEFINE_FIELD( m_attachedPhysicsBone,		FIELD_SHORT ),
	DEFINE_FIELD( m_targetRotation,		FIELD_VECTOR ),
	DEFINE_FIELD( m_timeToArrive,			FIELD_FLOAT ),
#ifdef ARGG
	// adnan
	// set up the fields for our added vars
	DEFINE_FIELD( m_vecRotatedCarryAngles, FIELD_VECTOR ),
	DEFINE_FIELD( m_bHasRotatedCarryAngles, FIELD_BOOLEAN ),
	// end adnan
#endif

	// Physptrs can't be saved in embedded classes... this is to silence classcheck
	// DEFINE_PHYSPTR( m_controller ),

END_DATADESC()


CGravControllerPoint::CGravControllerPoint( void )
{
	m_shadow.dampFactor = 0.8;
	m_shadow.teleportDistance = 0;
	// make this controller really stiff!
	m_shadow.maxSpeed = 5000;
	m_shadow.maxAngular = m_shadow.maxSpeed;
	m_shadow.maxDampSpeed = m_shadow.maxSpeed*2;
	m_shadow.maxDampAngular = m_shadow.maxAngular*2;
	m_attachedEntity = NULL;
	m_attachedPhysicsBone = 0;

#ifdef ARGG
	// adnan
	// initialize our added vars
	m_vecRotatedCarryAngles = vec3_angle;
	m_bHasRotatedCarryAngles = false;
	// end adnan
#endif
}

CGravControllerPoint::~CGravControllerPoint( void )
{
	DetachEntity();
}


QAngle CGravControllerPoint::TransformAnglesToPlayerSpace( const QAngle &anglesIn, CBasePlayer *pPlayer )
{
	matrix3x4_t test;
	QAngle angleTest = pPlayer->EyeAngles();
	angleTest.x = 0;
	AngleMatrix( angleTest, test );
	return TransformAnglesToLocalSpace( anglesIn, test );
}

QAngle CGravControllerPoint::TransformAnglesFromPlayerSpace( const QAngle &anglesIn, CBasePlayer *pPlayer )
{
	matrix3x4_t test;
	QAngle angleTest = pPlayer->EyeAngles();
	angleTest.x = 0;
	AngleMatrix( angleTest, test );
	return TransformAnglesToWorldSpace( anglesIn, test );
}


void CGravControllerPoint::AttachEntity( CBasePlayer *pPlayer, CBaseEntity *pEntity, IPhysicsObject *pPhys, short physicsbone, const Vector &vGrabPosition )
{
	m_attachedEntity = pEntity;
	m_attachedPhysicsBone = physicsbone;
	pPhys->WorldToLocal( &m_localPosition, vGrabPosition );
	m_worldPosition = vGrabPosition;
	pPhys->GetDamping( NULL, &m_saveDamping );
	m_saveMass = pPhys->GetMass();
	float damping = 2;
	pPhys->SetDamping( NULL, &damping );
	pPhys->SetMass( 50000 );
	m_controller = physenv->CreateMotionController( this );
	m_controller->AttachObject( pPhys, true );
	Vector position;
	QAngle angles;
	pPhys->GetPosition( &position, &angles );
	SetTargetPosition( vGrabPosition, angles );
	m_targetRotation = TransformAnglesToPlayerSpace( angles, pPlayer );
#ifdef ARGG
	// adnan
	// we need to grab the preferred/non preferred carry angles here for the rotatedcarryangles
	m_vecRotatedCarryAngles = m_targetRotation;
	// end adnan
#endif
}

void CGravControllerPoint::DetachEntity( void )
{
	CBaseEntity *pEntity = m_attachedEntity;
	if ( pEntity )
	{
		IPhysicsObject *pPhys = GetPhysObjFromPhysicsBone( pEntity, m_attachedPhysicsBone );
		if ( pPhys )
		{
			// on the odd chance that it's gone to sleep while under anti-gravity
			pPhys->Wake();
			pPhys->SetDamping( NULL, &m_saveDamping );
			pPhys->SetMass( m_saveMass );
		}
	}
	m_attachedEntity = NULL;
	m_attachedPhysicsBone = 0;
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
	hlshadowcontrol_params_t shadowParams = m_shadow;
#ifndef CLIENT_DLL
	m_timeToArrive = pObject->ComputeShadowControl( shadowParams, m_timeToArrive, deltaTime );
#else
	m_timeToArrive = pObject->ComputeShadowControl( shadowParams, (TICK_INTERVAL*2), deltaTime );
#endif
	
	linear.Init();
	angular.Init();

	return SIM_LOCAL_ACCELERATION;
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
	void GetRenderBounds( Vector& mins, Vector& maxs )
	{
		BaseClass::GetRenderBounds( mins, maxs );

		// add to the bounds, don't clear them.
		// ClearBounds( mins, maxs );
		AddPointToBounds( vec3_origin, mins, maxs );
		AddPointToBounds( m_targetPosition, mins, maxs );
		AddPointToBounds( m_worldPosition, mins, maxs );
		CBaseEntity *pEntity = GetBeamEntity();
		if ( pEntity )
		{
			mins -= pEntity->GetRenderOrigin();
			maxs -= pEntity->GetRenderOrigin();
		}
	}

	void GetRenderBoundsWorldspace( Vector& mins, Vector& maxs )
	{
		BaseClass::GetRenderBoundsWorldspace( mins, maxs );

		// add to the bounds, don't clear them.
		// ClearBounds( mins, maxs );
		AddPointToBounds( vec3_origin, mins, maxs );
		AddPointToBounds( m_targetPosition, mins, maxs );
		AddPointToBounds( m_worldPosition, mins, maxs );
		mins -= GetRenderOrigin();
		maxs -= GetRenderOrigin();
	}

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

	// We need to render opaque and translucent pieces
	RenderGroup_t	GetRenderGroup( void ) {	return RENDER_GROUP_TWOPASS;	}
#endif

	void Spawn( void );
	void OnRestore( void );
	void Precache( void );

#ifdef ARGG
	// adnan
	// for overriding the mouse -> view angles (but still calc view angles)
	bool OverrideViewAngles( void );
	// end adnan
#endif

	virtual void	UpdateOnRemove(void);
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	void ItemPreFrame( void );
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

#ifndef CLIENT_DLL
		UTIL_Remove( this );
#endif
	}

	bool HasAnyAmmo( void );

	void AttachObject( CBaseEntity *pEdict, IPhysicsObject *pPhysics, short physicsbone, const Vector& start, const Vector &end, float distance );
	void UpdateObject( void );
	void DetachObject( void );

	void TraceLine( trace_t *ptr );

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
	CNetworkVar( int, m_active );
	bool		m_useDown;
	CNetworkHandle( CBaseEntity, m_hObject );
	CNetworkVar( int, m_physicsBone );
	float		m_distance;
	float		m_movementLength;
	int			m_soundState;
	Vector		m_originalObjectPosition;
	CNetworkVector	( m_targetPosition );
	CNetworkVector	( m_worldPosition );

#ifdef ARGG
	// adnan
	// this is how we tell if we're rotating what we're holding
	CNetworkVar( bool, m_bIsCurrentlyRotating );
	// end adnan
#endif

	CSoundPatch					*m_sndMotor;		// Whirring sound for the gun
	CSoundPatch					*m_sndLockedOn;
	CSoundPatch					*m_sndLightObject;
	CSoundPatch					*m_sndHeavyObject;

	CGravControllerPoint		m_gravCallback;

	DECLARE_ACTTABLE();
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponGravityGun, DT_WeaponGravityGun )

BEGIN_NETWORK_TABLE( CWeaponGravityGun, DT_WeaponGravityGun )
#ifdef CLIENT_DLL
	RecvPropEHandle( RECVINFO( m_hObject ) ),
	RecvPropInt( RECVINFO( m_physicsBone ) ),
	RecvPropVector( RECVINFO( m_targetPosition ) ),
	RecvPropVector( RECVINFO( m_worldPosition ) ),
	RecvPropInt( RECVINFO(m_active) ),
#ifdef ARGG
	// adnan
	// also receive if we're rotating what we're holding (by pressing use)
	RecvPropBool( RECVINFO( m_bIsCurrentlyRotating ) ),
	// end adnan
#endif
#else
	SendPropEHandle( SENDINFO( m_hObject ) ),
	SendPropInt( SENDINFO( m_physicsBone ) ),
	SendPropVector(SENDINFO( m_targetPosition ), -1, SPROP_COORD),
	SendPropVector(SENDINFO( m_worldPosition ), -1, SPROP_COORD),
	SendPropInt( SENDINFO(m_active), 1, SPROP_UNSIGNED ),
#ifdef ARGG
	// adnan
	// need to seind if we're rotating what we're holding
	SendPropBool( SENDINFO( m_bIsCurrentlyRotating ) ),
	// end adnan
#endif
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


//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CWeaponGravityGun )

	DEFINE_FIELD( m_active,				FIELD_INTEGER ),
	DEFINE_FIELD( m_useDown,				FIELD_BOOLEAN ),
	DEFINE_FIELD( m_hObject,				FIELD_EHANDLE ),
	DEFINE_FIELD( m_distance,			FIELD_FLOAT ),
	DEFINE_FIELD( m_movementLength,		FIELD_FLOAT ),
	DEFINE_FIELD( m_soundState,			FIELD_INTEGER ),
	DEFINE_FIELD( m_originalObjectPosition,	FIELD_POSITION_VECTOR ),
#ifdef ARGG
	// adnan
	DEFINE_FIELD( m_bIsCurrentlyRotating, FIELD_BOOLEAN ),
	// end adnan
#endif
	DEFINE_SOUNDPATCH( m_sndMotor ),
	DEFINE_SOUNDPATCH( m_sndLockedOn ),
	DEFINE_SOUNDPATCH( m_sndLightObject ),
	DEFINE_SOUNDPATCH( m_sndHeavyObject ),
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


//-----------------------------------------------------------------------------
// On Remove
//-----------------------------------------------------------------------------
void CWeaponGravityGun::UpdateOnRemove(void)
{
	EffectDestroy();
	SoundDestroy();
	BaseClass::UpdateOnRemove();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// adnan
// want to add an angles modifier key
bool CGravControllerPoint::UpdateObject( CBasePlayer *pPlayer, CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->VPhysicsGetObject() )
	{
		return false;
	}

#ifdef ARGG
	// adnan
	// if we've been rotating it, set it to its proper new angles (change m_attachedAnglesPlayerSpace while modifier)
	//Pickup_GetRotatedCarryAngles( pEntity, pPlayer, pPlayer->EntityToWorldTransform(), angles );
	// added the ... && (mousedx | mousedy) so we dont have to calculate if no mouse movement
	// UPDATE: m_vecRotatedCarryAngles has become a temp variable... can be cleaned up by using actual temp vars
#ifdef CLIENT_DLL
	if( m_bHasRotatedCarryAngles && (pPlayer->m_pCurrentCommand->mousedx || pPlayer->m_pCurrentCommand->mousedy) )
#else
	if( m_bHasRotatedCarryAngles && (pPlayer->GetCurrentCommand()->mousedx || pPlayer->GetCurrentCommand()->mousedy) )
#endif
	{
		// method II: relative orientation
		VMatrix vDeltaRotation, vCurrentRotation, vNewRotation;
		
		MatrixFromAngles( m_targetRotation, vCurrentRotation );

#ifdef CLIENT_DLL
		m_vecRotatedCarryAngles[YAW] = pPlayer->m_pCurrentCommand->mousedx*0.05;
		m_vecRotatedCarryAngles[PITCH] = pPlayer->m_pCurrentCommand->mousedy*-0.05;
#else
		m_vecRotatedCarryAngles[YAW] = pPlayer->GetCurrentCommand()->mousedx*0.05;
		m_vecRotatedCarryAngles[PITCH] = pPlayer->GetCurrentCommand()->mousedy*-0.05;
#endif
		m_vecRotatedCarryAngles[ROLL] = 0;
		MatrixFromAngles( m_vecRotatedCarryAngles, vDeltaRotation );

		MatrixMultiply(vDeltaRotation, vCurrentRotation, vNewRotation);
		MatrixToAngles( vNewRotation, m_targetRotation );
	}
	// end adnan
#endif

	SetTargetPosition( m_targetPosition, m_targetRotation );

	return true;
}

#ifdef ARGG
// adnan
// this is where we say that we dont want ot apply the current calculated view angles
//-----------------------------------------------------------------------------
// Purpose: Allow weapons to override mouse input to viewangles (for orbiting)
//-----------------------------------------------------------------------------
bool CWeaponGravityGun::OverrideViewAngles( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if(!pPlayer)
		return false;

	if (m_bIsCurrentlyRotating) {
		return true;
	}

	return false;
}
// end adnan
#endif

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

	g_physgunBeam1 = PrecacheModel(PHYSGUN_BEAM_SPRITE1);
	g_physgunBeam = PrecacheModel(PHYSGUN_BEAM_SPRITE);
	g_physgunGlow = PrecacheModel(PHYSGUN_BEAM_GLOW);

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


// Andrew; added so we can trace both in EffectUpdate and DrawModel with the same results
void CWeaponGravityGun::TraceLine( trace_t *ptr )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

	Vector start, forward, right;
	pOwner->EyeVectors( &forward, &right, NULL );

	start = pOwner->Weapon_ShootPosition();
	Vector end = start + forward * 4096;

	UTIL_TraceLine( start, end, MASK_SHOT, pOwner, COLLISION_GROUP_NONE, ptr );
}


void CWeaponGravityGun::EffectUpdate( void )
{
	Vector start, forward, right;
	trace_t tr;

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

	pOwner->EyeVectors( &forward, &right, NULL );

	start = pOwner->Weapon_ShootPosition();

	TraceLine( &tr );
	Vector end = tr.endpos;
	float distance = tr.fraction * 4096;

	IPhysicsObject *pPhys = NULL;

	if ( m_hObject == NULL && tr.DidHitNonWorldEntity() )
	{
		CBaseEntity *pEntity = tr.m_pEnt;
		pPhys = GetPhysObjFromPhysicsBone( pEntity, tr.physicsbone );
		AttachObject( pEntity, pPhys, tr.physicsbone, start, tr.endpos, distance );
	}

	// Add the incremental player yaw to the target transform
	QAngle angles = m_gravCallback.TransformAnglesFromPlayerSpace( m_gravCallback.m_targetRotation, pOwner );

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

		if ( pPhys && pPhys->IsAsleep() )
		{
			// on the odd chance that it's gone to sleep while under anti-gravity
			pPhys->Wake();
		}

		Vector newPosition = start + forward * m_distance;
		Vector offset;
		pObject->EntityToWorldSpace( m_worldPosition, &offset );
		m_gravCallback.SetTargetPosition( newPosition + (pObject->GetAbsOrigin() - offset), angles );
		Vector dir = (newPosition - pObject->GetLocalOrigin());
		m_movementLength = dir.Length();
	}
	else
	{
		m_targetPosition = end;
		//m_gravCallback.SetTargetPosition( end, m_gravCallback.m_targetRotation );
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
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

	switch( m_soundState )
	{
	case SS_SCANNING:
		(CSoundEnvelopeController::GetController()).SoundDestroy( m_sndMotor );
		m_sndMotor = NULL;
		break;
	case SS_LOCKEDON:
		(CSoundEnvelopeController::GetController()).SoundDestroy( m_sndMotor );
		m_sndMotor = NULL;
		(CSoundEnvelopeController::GetController()).SoundDestroy( m_sndLockedOn );
		m_sndLockedOn = NULL;
		(CSoundEnvelopeController::GetController()).SoundDestroy( m_sndLightObject );
		m_sndLightObject = NULL;
		(CSoundEnvelopeController::GetController()).SoundDestroy( m_sndHeavyObject );
		m_sndHeavyObject = NULL;
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
	CPASAttenuationFilter filter( this );

	switch( m_soundState )
	{
	case SS_SCANNING:
		{
			m_sndMotor = (CSoundEnvelopeController::GetController()).SoundCreate( filter, entindex(), CHAN_STATIC, "Weapon_Physgun.Scanning", ATTN_NORM );
			(CSoundEnvelopeController::GetController()).Play( m_sndMotor, 1.0f, 100 );
		}
		break;
	case SS_LOCKEDON:
		{
			m_sndLockedOn = (CSoundEnvelopeController::GetController()).SoundCreate( filter, entindex(), CHAN_STATIC, "Weapon_Physgun.LockedOn", ATTN_NORM );
			(CSoundEnvelopeController::GetController()).Play( m_sndLockedOn, 1.0f, 100 );
			m_sndMotor = (CSoundEnvelopeController::GetController()).SoundCreate( filter, entindex(), CHAN_STATIC, "Weapon_Physgun.Scanning", ATTN_NORM );
			(CSoundEnvelopeController::GetController()).Play( m_sndMotor, 1.0f, 100 );
			m_sndLightObject = (CSoundEnvelopeController::GetController()).SoundCreate( filter, entindex(), CHAN_STATIC, "Weapon_Physgun.LightObject", ATTN_NORM );
			(CSoundEnvelopeController::GetController()).Play( m_sndLightObject, 1.0f, 100 );
			m_sndHeavyObject = (CSoundEnvelopeController::GetController()).SoundCreate( filter, entindex(), CHAN_STATIC, "Weapon_Physgun.HeavyObject", ATTN_NORM );
			(CSoundEnvelopeController::GetController()).Play( m_sndHeavyObject, 1.0f, 100 );
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
			CPASAttenuationFilter filter( this );

			float height = m_hObject->GetAbsOrigin().z - m_originalObjectPosition.z;

			// go from pitch 90 to 150 over a height of 500
			int pitch = 90 + (int)UTIL_LineFraction( height, 0, 500, 60 );

			assert(m_sndLockedOn!=NULL);
			if ( m_sndLockedOn != NULL )
			{
				(CSoundEnvelopeController::GetController()).SoundChangePitch( m_sndLockedOn, pitch, 0.0f );
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

			(CSoundEnvelopeController::GetController()).SoundChangeVolume( m_sndLightObject, fade * distance, 0.0f );

			(CSoundEnvelopeController::GetController()).SoundChangeVolume( m_sndHeavyObject, (1.0 - fade) * distance, 0.0f );
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

void CWeaponGravityGun::UpdateObject( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	Assert( pPlayer );

	CBaseEntity *pObject = m_hObject;
	if ( !pObject )
		return;

	if ( !m_gravCallback.UpdateObject( pPlayer, pObject ) )
	{
		DetachObject();
		return;
	}
}

void CWeaponGravityGun::DetachObject( void )
{
	if ( m_hObject )
	{
#ifndef CLIENT_DLL
		CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
		Pickup_OnPhysGunDrop( m_hObject, pOwner, DROPPED_BY_CANNON );
#endif

		IPhysicsObject *pPhysics = m_hObject->VPhysicsGetObject();
		if ( pPhysics )
		{
			PhysClearGameFlags( pPhysics, FVPHYSICS_PLAYER_HELD );
		}
		m_gravCallback.DetachEntity();
		m_hObject = NULL;
	}
}

void CWeaponGravityGun::AttachObject( CBaseEntity *pObject, IPhysicsObject *pPhysics, short physicsbone, const Vector& start, const Vector &end, float distance )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if( !pOwner )
		return;
	m_hObject = pObject;
	m_physicsBone = physicsbone;
	m_useDown = false;
	if ( pPhysics && pObject->GetMoveType() == MOVETYPE_VPHYSICS )
	{
		m_distance = distance;

		Vector worldPosition;
		pObject->WorldToEntitySpace( end, &worldPosition );
		m_worldPosition = worldPosition;
		m_gravCallback.AttachEntity( pOwner, pObject, pPhysics, physicsbone, pObject->GetAbsOrigin() );

		m_originalObjectPosition = pObject->GetAbsOrigin();

		pPhysics->Wake();
		PhysSetGameFlags( pPhysics, FVPHYSICS_PLAYER_HELD );

#ifndef CLIENT_DLL
		Pickup_OnPhysGunPickup( pObject, pOwner );
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
	return;
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
		if ( !m_active )
			return 0;

		C_BasePlayer *pOwner = ToBasePlayer( GetOwner() );

		if ( !pOwner )
			return 0;

		Vector points[3];
		QAngle tmpAngle;

		C_BaseEntity *pObject = m_hObject;
		//if ( pObject == NULL )
		//	return 0;

		GetAttachment( 1, points[0], tmpAngle );

		// a little noise 11t & 13t should be somewhat non-periodic looking
		//points[1].z += 4*sin( gpGlobals->curtime*11 ) + 5*cos( gpGlobals->curtime*13 );
		if ( pObject == NULL )
		{
			//points[2] = m_targetPosition;
			trace_t tr;
			TraceLine( &tr );
			points[2] = tr.endpos;
		}
		else
		{
			pObject->EntityToWorldSpace( m_worldPosition, &points[2] );
		}

		Vector forward, right, up;
		QAngle playerAngles = pOwner->EyeAngles();
		AngleVectors( playerAngles, &forward, &right, &up );
		if ( pObject == NULL )
		{
			Vector vecDir = points[2] - points[0];
			VectorNormalize( vecDir );
			points[1] = points[0] + 0.5f * (vecDir * points[2].DistTo(points[0]));
		}
		else
		{
			Vector vecSrc = pOwner->Weapon_ShootPosition( );
			points[1] = vecSrc + 0.5f * (forward * points[2].DistTo(points[0]));
		}
		
		IMaterial *pMat = materials->FindMaterial( "sprites/physbeam1", TEXTURE_GROUP_CLIENT_EFFECTS );
		if ( pObject )
			pMat = materials->FindMaterial( "sprites/physbeam", TEXTURE_GROUP_CLIENT_EFFECTS );
		Vector color;
		color.Init(1,1,1);

		float scrollOffset = gpGlobals->curtime - (int)gpGlobals->curtime;
		CMatRenderContextPtr pRenderContext( materials );
		pRenderContext->Bind( pMat );
		DrawBeamQuadratic( points[0], points[1], points[2], pObject ? 13/3.0f : 13/5.0f, color, scrollOffset );
		DrawBeamQuadratic( points[0], points[1], points[2], pObject ? 13/3.0f : 13/5.0f, color, -scrollOffset );

		IMaterial *pMaterial = materials->FindMaterial( "sprites/physglow", TEXTURE_GROUP_CLIENT_EFFECTS );

		color32 clr={0,64,255,255};
		if ( pObject )
		{
			clr.r = 186;
			clr.g = 253;
			clr.b = 247;
			clr.a = 255;
		}

		float scale = random->RandomFloat( 3, 5 ) * ( pObject ? 3 : 2 );

		// Draw the sprite
		pRenderContext->Bind( pMaterial );
		for ( int i = 0; i < 3; i++ )
		{
			DrawSprite( points[2], scale, scale, clr );
		}
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
	if ( !m_active )
		return;

	// Render our effects
	C_BasePlayer *pOwner = ToBasePlayer( GetOwner() );

	if ( !pOwner )
		return;

	Vector points[3];
	QAngle tmpAngle;

	C_BaseEntity *pObject = m_hObject;
	//if ( pObject == NULL )
	//	return;

	pBaseViewModel->GetAttachment( 1, points[0], tmpAngle );

	// a little noise 11t & 13t should be somewhat non-periodic looking
	//points[1].z += 4*sin( gpGlobals->curtime*11 ) + 5*cos( gpGlobals->curtime*13 );
	if ( pObject == NULL )
	{
		//points[2] = m_targetPosition;
		trace_t tr;
		TraceLine( &tr );
		points[2] = tr.endpos;
	}
	else
	{
		pObject->EntityToWorldSpace(m_worldPosition, &points[2]);
	}

	Vector forward, right, up;
	QAngle playerAngles = pOwner->EyeAngles();
	AngleVectors( playerAngles, &forward, &right, &up );
	Vector vecSrc = pOwner->Weapon_ShootPosition( );
	points[1] = vecSrc + 0.5f * (forward * points[2].DistTo(points[0]));
	
	IMaterial *pMat = materials->FindMaterial( "sprites/physbeam1", TEXTURE_GROUP_CLIENT_EFFECTS );
	if ( pObject )
		pMat = materials->FindMaterial( "sprites/physbeam", TEXTURE_GROUP_CLIENT_EFFECTS );
	Vector color;
	color.Init(1,1,1);

	// Now draw it.
	CViewSetup beamView = *view->GetPlayerViewSetup();

	Frustum dummyFrustum;
	render->Push3DView( beamView, 0, NULL, dummyFrustum );

	float scrollOffset = gpGlobals->curtime - (int)gpGlobals->curtime;
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->Bind( pMat );
#if 1
	// HACK HACK:  Munge the depth range to prevent view model from poking into walls, etc.
	// Force clipped down range
	pRenderContext->DepthRange( 0.1f, 0.2f );
#endif
	DrawBeamQuadratic( points[0], points[1], points[2], pObject ? 13/3.0f : 13/5.0f, color, scrollOffset );
	DrawBeamQuadratic( points[0], points[1], points[2], pObject ? 13/3.0f : 13/5.0f, color, -scrollOffset );

	IMaterial *pMaterial = materials->FindMaterial( "sprites/physglow", TEXTURE_GROUP_CLIENT_EFFECTS );

	color32 clr={0,64,255,255};
	if ( pObject )
	{
		clr.r = 186;
		clr.g = 253;
		clr.b = 247;
		clr.a = 255;
	}

	float scale = random->RandomFloat( 3, 5 ) * ( pObject ? 3 : 2 );

	// Draw the sprite
	pRenderContext->Bind( pMaterial );
	for ( int i = 0; i < 3; i++ )
	{
		DrawSprite( points[2], scale, scale, clr );
	}
#if 1
	pRenderContext->DepthRange( 0.0f, 1.0f );
#endif

	render->PopView( dummyFrustum );

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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CWeaponGravityGun::ItemPreFrame()
{
	BaseClass::ItemPreFrame();

#ifndef CLIENT_DLL
	// Update the object if the weapon is switched on.
	if( m_active )
	{
		UpdateObject();
	}
#endif
}


void CWeaponGravityGun::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if (!pOwner)
		return;

#ifdef ARGG
	// adnan
	// this is where we check if we're orbiting the object
	
	// if we're holding something and pressing use,
	//  then set us in the orbiting state
	//  - this will indicate to OverrideMouseInput that we should zero the input and update our delta angles
	//  UPDATE: not anymore.  now this just sets our state variables.
	CBaseEntity *pObject = m_hObject;
	if( pObject ) {

		if((pOwner->m_nButtons & IN_ATTACK) && (pOwner->m_nButtons & IN_USE) ) {
			m_gravCallback.m_bHasRotatedCarryAngles = true;
			
			// did we JUST hit use?
			//  if so, grab the current angles to begin with as the rotated angles
			if( !(pOwner->m_afButtonLast & IN_USE) ) {
				m_gravCallback.m_vecRotatedCarryAngles = pObject->GetAbsAngles();
			}

			m_bIsCurrentlyRotating = true;
		} else {
			m_gravCallback.m_bHasRotatedCarryAngles = false;

			m_bIsCurrentlyRotating = false;
		}
	} else {
		m_bIsCurrentlyRotating = false;

		m_gravCallback.m_bHasRotatedCarryAngles = false;
	}
	// end adnan
#endif

	if ( pOwner->m_nButtons & IN_ATTACK )
	{
#if defined( ARGG )
		if( (pOwner->m_nButtons & IN_USE) ) {
			pOwner->m_vecUseAngles = pOwner->pl.v_angle;
		}
#endif
		if ( pOwner->m_afButtonPressed & IN_ATTACK2 )
		{
			SecondaryAttack();
		}
		else if ( pOwner->m_nButtons & IN_ATTACK2 )
		{
			if ( m_active )
			{
				EffectDestroy();
				SoundDestroy();
			}
			WeaponIdle( );
			return;
		}
		PrimaryAttack();
	}
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
	if ( pOwner->m_afButtonPressed & IN_RELOAD )
	{
		Reload();
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

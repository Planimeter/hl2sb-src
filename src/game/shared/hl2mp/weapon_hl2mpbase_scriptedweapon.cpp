//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
#endif

#include "weapon_hl2mpbase_scriptedweapon.h"
#include "in_buttons.h"
#include "ammodef.h"
#include "luamanager.h"
#include "lbasecombatweapon_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_NETWORKCLASS_ALIASED( HL2MPScriptedWeapon, DT_HL2MPScriptedWeapon )

BEGIN_NETWORK_TABLE( CHL2MPScriptedWeapon, DT_HL2MPScriptedWeapon )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CHL2MPScriptedWeapon )
END_PREDICTION_DATA()

//=========================================================
//	>> CHLSelectFireScriptedWeapon
//=========================================================
BEGIN_DATADESC( CHL2MPScriptedWeapon )
END_DATADESC()



// LINK_ENTITY_TO_CLASS( weapon_hl2mpbase_scriptedweapon, CHL2MPScriptedWeapon );
// PRECACHE_WEAPON_REGISTER( weapon_hl2mpbase_scriptedweapon );

// These functions replace the macros above for runtime registration of
// scripted weapons.
#ifdef CLIENT_DLL
static C_BaseEntity *CCHL2MPScriptedWeaponFactory( void )
{
	return static_cast< C_BaseEntity * >( new CHL2MPScriptedWeapon );
};
#endif

#ifndef CLIENT_DLL
static CUtlDict< CEntityFactory<CHL2MPScriptedWeapon>*, unsigned short > m_EntityFactoryDatabase;
#endif

void RegisterScriptedWeapon( const char *className )
{
#ifdef CLIENT_DLL
	GetClassMap().Add( className, "CHL2MPScriptedWeapon", sizeof( CHL2MPScriptedWeapon ),
		&CCHL2MPScriptedWeaponFactory );
#else
	// Complain about duplicately defined scripted weapon names...
	unsigned short lookup = m_EntityFactoryDatabase.Find( className );
	if ( lookup != m_EntityFactoryDatabase.InvalidIndex() )
	{
		return;
	}

	// Andrew; This fixes months worth of pain and anguish.
	CEntityFactory<CHL2MPScriptedWeapon> *pFactory = new CEntityFactory<CHL2MPScriptedWeapon>( className );

	lookup = m_EntityFactoryDatabase.Insert( className, pFactory );
	Assert( lookup != m_EntityFactoryDatabase.InvalidIndex() );
#endif
	// BUGBUG: When attempting to precache weapons registered during runtime,
	// they don't appear as valid registered entities.
	// static CPrecacheRegister precache_weapon_(&CPrecacheRegister::PrecacheFn_Other, className);
}

#ifndef CLIENT_DLL
void ResetEntityFactoryDatabase( void )
{
	int c = m_EntityFactoryDatabase.Count(); 
	for ( int i = 0; i < c; ++i )
	{
		delete m_EntityFactoryDatabase[ i ];
	}
	m_EntityFactoryDatabase.RemoveAll();
}
#endif


acttable_t CHL2MPScriptedWeapon::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_PISTOL,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_PISTOL,			false },

	{ ACT_MP_RUN,						ACT_HL2MP_RUN_PISTOL,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_PISTOL,			false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },

	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },

	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_PISTOL,					false },
};

IMPLEMENT_ACTTABLE( CHL2MPScriptedWeapon );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHL2MPScriptedWeapon::CHL2MPScriptedWeapon( void )
{
	m_pLuaWeaponInfo = new FileWeaponInfo_t();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHL2MPScriptedWeapon::~CHL2MPScriptedWeapon( void )
{
	delete m_pLuaWeaponInfo;
}

void CHL2MPScriptedWeapon::InitScriptedWeapon( void )
{
#if defined ( LUA_SDK )
	if ( m_nRefCount != LUA_REFNIL )
		return;

	char className[ MAX_WEAPON_STRING ];
	Q_strncpy( className, GetClassname(), sizeof( className ) );
	Q_strlower( className );
	// Andrew; This redundancy is pretty annoying.
	Q_strncpy( m_pLuaWeaponInfo->szClassName, className, MAX_WEAPON_STRING );
	SetClassname( className );

	lua_getglobal( L, "weapon" );
	if ( lua_istable( L, -1 ) )
	{
		lua_getfield( L, -1, "Get" );
		if ( lua_isfunction( L, -1 ) )
		{
			lua_remove( L, -2 );
			lua_pushstring( L, GetClassname() );
			luasrc_pcall( L, 1, 1, 0 );
		}
		else
		{
			lua_pop( L, 2 );
		}
	}
	else
	{
		lua_pop( L, 1 );
	}

	m_nRefCount = luaL_ref( L, LUA_REGISTRYINDEX );

	BEGIN_LUA_CALL_WEAPON_METHOD( "Initialize" );
	END_LUA_CALL_WEAPON_METHOD( 0, 0 );
#endif
}

void CHL2MPScriptedWeapon::Precache( void )
{
	BaseClass::Precache();

	InitScriptedWeapon();

	// Precache models (preload to avoid hitch)
	m_iViewModelIndex = 0;
	m_iWorldModelIndex = 0;
	if ( GetViewModel() && GetViewModel()[0] )
	{
		m_iViewModelIndex = CBaseEntity::PrecacheModel( GetViewModel() );
	}
	if ( GetWorldModel() && GetWorldModel()[0] )
	{
		m_iWorldModelIndex = CBaseEntity::PrecacheModel( GetWorldModel() );
	}

#if defined ( LUA_SDK )
	BEGIN_LUA_CALL_WEAPON_METHOD( "Precache" );
	END_LUA_CALL_WEAPON_METHOD( 0, 0 );
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Get my data in the file weapon info array
//-----------------------------------------------------------------------------
const FileWeaponInfo_t &CHL2MPScriptedWeapon::GetWpnData( void ) const
{
	return *m_pLuaWeaponInfo;
}

const char *CHL2MPScriptedWeapon::GetViewModel( int ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "viewmodel" );
	lua_remove( L, -2 );

	RETURN_LUA_STRING();
#endif

	return BaseClass::GetViewModel();
}

const char *CHL2MPScriptedWeapon::GetWorldModel( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "playermodel" );
	lua_remove( L, -2 );

	RETURN_LUA_STRING();
#endif

	return BaseClass::GetWorldModel();
}

const char *CHL2MPScriptedWeapon::GetAnimPrefix( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "anim_prefix" );
	lua_remove( L, -2 );

	RETURN_LUA_STRING();
#endif

	return BaseClass::GetAnimPrefix();
}

const char *CHL2MPScriptedWeapon::GetPrintName( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "printname" );
	lua_remove( L, -2 );

	RETURN_LUA_STRING();
#endif

	return BaseClass::GetPrintName();
}

int CHL2MPScriptedWeapon::GetMaxClip1( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "clip_size" );
	lua_remove( L, -2 );

	RETURN_LUA_INTEGER();
#endif

	return BaseClass::GetMaxClip1();
}

int CHL2MPScriptedWeapon::GetMaxClip2( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "clip2_size" );
	lua_remove( L, -2 );

	RETURN_LUA_INTEGER();
#endif

	return BaseClass::GetMaxClip2();
}

int CHL2MPScriptedWeapon::GetDefaultClip1( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "default_clip" );
	lua_remove( L, -2 );

	RETURN_LUA_INTEGER();
#endif

	return BaseClass::GetDefaultClip1();
}

int CHL2MPScriptedWeapon::GetDefaultClip2( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "default_clip2" );
	lua_remove( L, -2 );

	RETURN_LUA_INTEGER();
#endif

	return BaseClass::GetDefaultClip2();
}


bool CHL2MPScriptedWeapon::IsMeleeWeapon() const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "MeleeWeapon" );
	lua_remove( L, -2 );

	if ( lua_gettop( L ) == 1 )
	{
		if ( lua_isnumber( L, -1 ) )
		{
			int res = ( luaL_checkinteger( L, -1 ) != 0 ) ? true : false;
			lua_pop(L, 1);
			return res;
		}
		else
			lua_pop(L, 1);
	}
#endif

	return BaseClass::IsMeleeWeapon();
}

int CHL2MPScriptedWeapon::GetWeight( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "weight" );
	lua_remove( L, -2 );

	RETURN_LUA_INTEGER();
#endif

	return BaseClass::GetWeight();
}

bool CHL2MPScriptedWeapon::AllowsAutoSwitchTo( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "autoswitchto" );
	lua_remove( L, -2 );

	if ( lua_gettop( L ) == 1 )
	{
		if ( lua_isnumber( L, -1 ) )
		{
			int res = ( luaL_checkinteger( L, -1 ) != 0 ) ? true : false;
			lua_pop(L, 1);
			return res;
		}
		else
			lua_pop(L, 1);
	}
#endif

	return BaseClass::AllowsAutoSwitchTo();
}

bool CHL2MPScriptedWeapon::AllowsAutoSwitchFrom( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "autoswitchfrom" );
	lua_remove( L, -2 );

	if ( lua_gettop( L ) == 1 )
	{
		if ( lua_isnumber( L, -1 ) )
		{
			int res = ( luaL_checkinteger( L, -1 ) != 0 ) ? true : false;
			lua_pop(L, 1);
			return res;
		}
		else
			lua_pop(L, 1);
	}
#endif

	return BaseClass::AllowsAutoSwitchFrom();
}

int CHL2MPScriptedWeapon::GetWeaponFlags( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "item_flags" );
	lua_remove( L, -2 );

	RETURN_LUA_INTEGER();
#endif

	return BaseClass::GetWeaponFlags();
}

int CHL2MPScriptedWeapon::GetSlot( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "bucket" );
	lua_remove( L, -2 );

	RETURN_LUA_INTEGER();
#endif

	return BaseClass::GetSlot();
}

int CHL2MPScriptedWeapon::GetPosition( void ) const
{
#if defined ( LUA_SDK )
	lua_getref( L, m_nRefCount );
	lua_getfield( L, -1, "bucket_position" );
	lua_remove( L, -2 );

	RETURN_LUA_INTEGER();
#endif

	return BaseClass::GetPosition();
}

const Vector &CHL2MPScriptedWeapon::GetBulletSpread( void )
{
	static Vector cone = VECTOR_CONE_3DEGREES;
	return cone;
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CHL2MPScriptedWeapon::PrimaryAttack( void )
{
#if defined ( LUA_SDK )
	BEGIN_LUA_CALL_WEAPON_METHOD( "PrimaryAttack" );
	END_LUA_CALL_WEAPON_METHOD( 0, 0 );
#endif
}

void CHL2MPScriptedWeapon::SecondaryAttack( void )
{
#if defined ( LUA_SDK )
	BEGIN_LUA_CALL_WEAPON_METHOD( "SecondaryAttack" );
	END_LUA_CALL_WEAPON_METHOD( 0, 0 );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
//-----------------------------------------------------------------------------
void CHL2MPScriptedWeapon::FireBullets( const FireBulletsInfo_t &info )
{
	if(CBasePlayer *pPlayer = ToBasePlayer ( GetOwner() ) )
	{
		pPlayer->FireBullets(info);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Reset our shots fired
//-----------------------------------------------------------------------------
bool CHL2MPScriptedWeapon::Deploy( void )
{
#if defined ( LUA_SDK )
	BEGIN_LUA_CALL_WEAPON_METHOD( "Deploy" );
	END_LUA_CALL_WEAPON_METHOD( 0, 1 );

	RETURN_LUA_BOOLEAN();
#endif

	return BaseClass::Deploy();
}



//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHL2MPScriptedWeapon::ItemPostFrame( void )
{
#if defined ( LUA_SDK )
	BEGIN_LUA_CALL_WEAPON_METHOD( "ItemPostFrame" );
	END_LUA_CALL_WEAPON_METHOD( 0, 0 );
#endif

	BaseClass::ItemPostFrame();
}



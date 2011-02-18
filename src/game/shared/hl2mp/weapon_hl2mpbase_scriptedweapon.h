//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "weapon_hl2mpbasehlmpcombatweapon.h"

#ifndef BASEHLCOMBATWEAPON_H
#define BASEHLCOMBATWEAPON_H
#ifdef _WIN32
#pragma once
#endif

#if defined( CLIENT_DLL )
	#define CHL2MPScriptedWeapon C_HL2MPScriptedWeapon
#endif

//=========================================================
// Scripted weapon base class
//=========================================================
class CHL2MPScriptedWeapon : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS( CHL2MPScriptedWeapon, CWeaponHL2MPBase );
	DECLARE_DATADESC();

	CHL2MPScriptedWeapon();

	bool			IsScripted( void ) const { return true; }
	
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();

	void			Precache( void );
	void			InitScriptedWeapon( void );

	void	PrimaryAttack( void );
	void	SecondaryAttack( void );

	// Default calls through to m_hOwner, but plasma weapons can override and shoot projectiles here.
	virtual void	ItemPostFrame( void );
	virtual void	FireBullets( const FireBulletsInfo_t &info );
	virtual bool	Deploy( void );

	virtual const Vector &GetBulletSpread( void );

public:

	// Weapon info accessors for data in the weapon's data file
	virtual const char		*GetViewModel( int viewmodelindex = 0 ) const;
	virtual const char		*GetWorldModel( void ) const;
	virtual const char		*GetAnimPrefix( void ) const;
	virtual int				GetMaxClip1( void ) const;
	virtual int				GetMaxClip2( void ) const;
	virtual int				GetDefaultClip1( void ) const;
	virtual int				GetDefaultClip2( void ) const;
	virtual int				GetWeight( void ) const;
	virtual bool			AllowsAutoSwitchTo( void ) const;
	virtual bool			AllowsAutoSwitchFrom( void ) const;
	virtual int				GetWeaponFlags( void ) const;
	virtual int				GetSlot( void ) const;
	virtual int				GetPosition( void ) const;
	virtual char const		*GetPrintName( void ) const;
	bool					IsMeleeWeapon() const;

private:
	
	CHL2MPScriptedWeapon( const CHL2MPScriptedWeapon & );

};

void RegisterScriptedWeapon( const char *szClassname );
void ResetEntityFactoryDatabase( void );

#endif // BASEHLCOMBATWEAPON_H

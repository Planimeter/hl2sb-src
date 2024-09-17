--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose:
--
--===========================================================================--

GM.Name       = "Deathmatch"
GM.Homepage   = "http://www.steampowered.com/"
GM.Developer  = "Valve"
GM.Manual     = nil

function GM:Initialize()
  self.m_bTeamPlayEnabled = cvar.FindVar( "mp_teamplay" ):GetBool()
end

function GM:Shutdown()
  -- Andrew; this is a Lua-side implemented hook. We have a proper C level hook
  -- call for Initialize which is called directly after the gamemode is loaded.
  -- While one might wonder why Shutdown isn't implemented at the C level as
  -- well, it's simply because it would be called within LevelShutdown, causing
  -- it's implementation to be redundant.
end

function GM:CalcPlayerView( pPlayer, eyeOrigin, eyeAngles, fov )
end

function GM:CheckGameOver()
end

function GM:ClientSettingsChanged( pPlayer )
end

function GM:CreateStandardEntities()
end

function GM:DeathNotice( pVictim, info )
end

function GM:FlWeaponRespawnTime( pWeapon )
end

function GM:FlWeaponTryRespawn( pWeapon )
end

function GM:GetGameDescription()
  if ( self:IsTeamplay() ) then
    return "Team " .. self.Name
  end
  return self.Name
end

function GM:GetMapRemainingTime()
  -- if timelimit is disabled, return 0
  if ( cvar.FindVar( "mp_timelimit" ):GetInt() <= 0 ) then
    return 0;
  end
end

function GM:GoToIntermission()
end

function GM:IsIntermission()
end

function GM:IsTeamplay()
  return self.m_bTeamPlayEnabled
end

function GM:LevelShutdown()
  self:Shutdown()
end

function GM:OnEntityCreated( pEntity )
end

function GM:PlayerKilled( pVictim, info )
end

function GM:PlayerPlayFootStep( pPlayer, vecOrigin, fvol, force )
end

function GM:PlayerRelationship( pPlayer, pTarget )
end

function GM:PlayerTraceAttack( info, vecDir, ptr )
end

function GM:PlayerUse( pPlayer )
end

function GM:Precache()
  _R.CBaseEntity.PrecacheScriptSound( "AlyxEmp.Charge" );
end

function GM:ShouldCollide( collisionGroup0, collisionGroup1 )
end

function GM:Think()
end

function GM:VecWeaponRespawnSpot( pWeapon )
end

--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose:
--
--===========================================================================--

GM.Name			= "Campaign"
GM.Homepage		= "http://www.andrewmcwatters.com/hl2sb/"
GM.Developer	= "Team Sandbox"
GM.Manual		= nil

function GM:Initialize()
if ( not _CLIENT ) then
  engine.ServerCommand( "mp_falldamage 1\n" )
  engine.ServerCommand( "mp_flashlight 1\n" )
end
end

function GM:FlWeaponRespawnTime( pWeapon )
  return -1
end

function GM:FlWeaponTryRespawn( pWeapon )
  return 0
end

function GM:PlayerPlayStepSound( pPlayer, vecOrigin, psurface, fvol, force )
	if ( gpGlobals.maxClients() > 1 and cvar.FindVar( "sv_footsteps" ):GetFloat() == 0 ) then
		return false;
	end

if ( _CLIENT ) then
	-- during prediction play footstep sounds only once
	if ( prediction.InPrediction() and not prediction.IsFirstTimePredicted() ) then
		return false;
	end
end

	if ( not psurface ) then
		return false;
	end

	local nSide = pPlayer:GetPlayerLocalData().m_nStepside;
	local stepSoundName = nSide ~= 0 and psurface.sounds.stepleft or psurface.sounds.stepright;
	if ( not stepSoundName ) then
		return false;
	end

	pPlayer:SetPlayerLocalData( "m_nStepside", nSide == 0 and 1 or 0 );

	local params;

	assert( nSide == 0 or nSide == 1 );

	if ( pPlayer:GetStepSoundCache()[ nSide ].m_usSoundNameIndex == stepSoundName ) then
		params = pPlayer:GetStepSoundCache()[ nSide ].m_SoundParameters;
	else
		local physprops = MoveHelper():GetSurfaceProps();
		local pSoundName = physprops:GetString( stepSoundName );
		local b
		b, params = _R.CBaseEntity.GetParametersForSound( pSoundName )
		if ( not b ) then
			return false;
		end

		-- Only cache if there's one option.  Otherwise we'd never here any other sounds
		if ( params.count == 1 ) then
			pPlayer:SetStepSoundCache( nSide, "m_usSoundNameIndex", stepSoundName );
			pPlayer:SetStepSoundCache( nSide, "m_SoundParameters", params );
		end
	end

	local filter = CRecipientFilter();
	filter:AddRecipientsByPAS( vecOrigin );

if not _CLIENT then
	-- in MP, server removes all players in the vecOrigin's PVS, these players generate the footsteps client side
	if ( gpGlobals.maxClients() > 1 ) then
		filter:RemoveRecipientsByPVS( vecOrigin );
	end
end

	local ep = {};
	ep.m_nChannel = 4; -- CHAN_BODY
	ep.m_pSoundName = params.soundname;
	ep.m_flVolume = fvol;
	ep.m_SoundLevel = params.soundlevel;
	ep.m_nFlags = 0;
	ep.m_nPitch = params.pitch;
	ep.m_pOrigin = vecOrigin;

	_R.CBaseEntity.EmitSound( filter, pPlayer:entindex(), ep );
	return false
end

function GM:OnEntityCreated( pEntity )
  -- TODO: Replace vehicles with vehicle spawners
end

function GM:WeaponShouldRespawn( pItem )
  pItem:AddSpawnFlags( 2^30 )
  -- return 2
end

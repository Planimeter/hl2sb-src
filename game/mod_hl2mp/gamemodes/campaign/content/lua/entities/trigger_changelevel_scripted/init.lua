--======== Copyleft © 2010-2013, Team Sandbox, Some rights reserved. ========--
--
-- Purpose:
--
--===========================================================================--

if ( not bit ) then
  require( "bit" )
end

local Msg = dbg.Msg
local bor = bit.bor
local Warning = dbg.Warning
local DevMsg = dbg.DevMsg

ENT.__base = "trigger_scripted"
ENT.__factory = "CBaseTrigger"

function ENT:InitScriptedTrigger()
  self:VPhysicsInitNormal( 2, 8, false )

  local w = self.m_vecMaxs.x - self.m_vecMins.x
  local l = self.m_vecMaxs.y - self.m_vecMins.y
  local h = self.m_vecMaxs.z - self.m_vecMins.z
  
  local mins = Vector( 0 - ( w / 2 ), 0 - ( l / 2 ), 0 - ( h / 2 ) )
  local maxs = Vector( w / 2, l / 2, h / 2 )

  self:SetCollisionBounds( mins, maxs )
  
  self:SetSolid( 2 )
  self:SetCollisionGroup( 1 )
  self:SetMoveType( 0 )
end

function ENT:Initialize()
  if ( not self.m_szMapName ) then
    Msg( "a trigger_changelevel_scripted doesn't have a map" );
  end

  if ( not self.m_szLandmarkName ) then
    Msg( "trigger_changelevel to " .. m_szMapName .. " doesn't have a landmark" );
  end

  self:InitScriptedTrigger()

  if ( not self:HasSpawnFlags(0x0002) ) then
    self.Touch = self.TouchChangeLevel;
  end

--  Msg( "TRANSITION: " .. self.m_szMapName .. " (" .. self.m_szLandmarkName .. ")\n" );

  if ( self:HasSpawnFlags( 0x0004 ) ) then
    self:VPhysicsInitStatic();
    self:RemoveSolidFlags( bor( 4, 8 ) );
    self.Touch = nil;
    return;
  end

  self.m_bTouched = false;
end

function ENT:FindLandmark( pLandmarkName )
  local pentLandmark;

  pentLandmark = gEntList.FindEntityByName( NULL, pLandmarkName );
  while ( pentLandmark ~= NULL ) do
    -- Found the landmark
    if ( pentLandmark:ClassMatches("info_landmark") ) then
      return pentLandmark;
    else
      pentLandmark = gEntList.FindEntityByName( pentLandmark, pLandmarkName );
    end
  end
  Warning( "Can't find landmark " .. tostring( pLandmarkName ) .. "\n" );
  return NULL;
end

function ENT:ChangeLevelNow( pActivator )
  local pLandmark;

  assert( self.m_szMapName );

  -- Some people are firing these multiple times in a frame, disable
  if ( self.m_bTouched ) then
    return;
  end

  self.m_bTouched = true;

  -- look for a landmark entity   
  pLandmark = self:FindLandmark( self.m_szLandmarkName );

  if ( pLandmark == NULL ) then
    return;
  end

  if ( cvar.FindVar( "g_debug_transitions" ):GetInt() ) then
    Msg( "CHANGE LEVEL: " .. tostring( self.m_szMapName ) .. " " .. tostring( self.m_szLandmarkName ) .. "\n" );
  end

  -- If we're debugging, don't actually change level
  if ( cvar.FindVar( "g_debug_transitions" ):GetInt() == 0 ) then
    engine.ChangeLevel( self.m_szMapName );
  end
end

function ENT:TouchChangeLevel( pOther )
  local pPlayer = ToBasePlayer(pOther);
  if ( not pOther:IsPlayer() ) then
    return;
  end

  if( pPlayer:IsSinglePlayerGameEnding() ) then
    -- Some semblance of deceleration, but allow player to fall normally.
    -- Also, disable controls.
    local vecVelocity = pPlayer:GetAbsVelocity();
    vecVelocity.x = vecVelocity.x * 0.5;
    vecVelocity.y = vecVelocity.y * 0.5;
    pPlayer:SetAbsVelocity( vecVelocity );
    pPlayer:AddFlag( 32 );
    return;
  end

  if ( not pPlayer:IsInAVehicle() and pPlayer:GetMoveType() == 8 ) then
    DevMsg("In scripted level transition: " .. tostring( self.m_szMapName ) .. " " .. tostring( self.m_szLandmarkName ) .. "\n" );
    return;
  end

  self:ChangeLevelNow( pOther );
end

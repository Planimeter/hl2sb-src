--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: 
--
--===========================================================================--

SWEP.printname				= "#HL2_SMG1"
SWEP.viewmodel				= "models/weapons/w_package.mdl"
SWEP.playermodel			= "models/weapons/w_package.mdl"
SWEP.anim_prefix			= "smg2"
SWEP.bucket					= 2
SWEP.bucket_position		= 0

SWEP.clip_size				= -1
SWEP.clip2_size				= -1

SWEP.default_clip			= -1
SWEP.default_clip2			= -1

SWEP.primary_ammo			= "None"
SWEP.secondary_ammo			= "None"

SWEP.weight					= 0
SWEP.item_flags				= 0

SWEP.SoundData				=
{
}

SWEP.m_acttable				=
{
	{ 1,						307,					false },
	{ 6,						308,					false },
};

-------------------------------------------------------------------------------
-- Purpose: 
-------------------------------------------------------------------------------
function SWEP:ItemPostFrame()
	-- Do nothing
end

-------------------------------------------------------------------------------
-- Purpose: Remove the citizen package if it's ever dropped
-------------------------------------------------------------------------------
function SWEP:Drop( vecVelocity )
	self:Drop( vecVelocity );
if not _CLIENT then
	UTIL.Remove( self );
end
end



--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Tests the implementation of the PlayerUpdateFlashlight hook.
--
--===========================================================================--

local AngleVectors = mathlib.AngleVectors

hook.add( "PlayerUpdateFlashlight", "PlayerUpdateWeaponFlashlight", function( pHL2MPPlayer, position, vecForward, vecRight, vecUp, nDistance )
	--if ( not input.CAM_IsThirdPerson() ) then
		local vm = pHL2MPPlayer:GetViewModel()
		if ( vm ~= NULL ) then
			local iAttachment = vm:LookupAttachment( "muzzle" );

			if ( iAttachment >= 0 ) then
				local vecOrigin = Vector();
				--Tony; EyeAngles will return proper whether it's local player or not.
				local eyeAngles = pHL2MPPlayer:EyeAngles();

				vm:GetAttachment( iAttachment, vecOrigin, eyeAngles );

				local vForward = Vector();
				AngleVectors( eyeAngles, vecForward, vecRight, vecUp );
				position = vecOrigin;
				return position, vecForward, vecRight, vecUp, nDistance
			end
		end
	--end
end )

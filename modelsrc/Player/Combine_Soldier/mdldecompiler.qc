$cd "C:\Users\Andrew\Documents\hl2sb-src\modelsrc\Combine_Soldier"
$modelname "Player\Combine_Soldier.mdl"
$model "Soldier" "Soldier_reference.smd"
$lod 10
 {
  replacemodel "Soldier_reference" "lod1_Soldier_reference"
}
$lod 25
 {
  replacemodel "Soldier_reference" "lod2_Soldier_reference"
}
$lod 40
 {
  replacemodel "Soldier_reference" "lod3_Soldier_reference"
}
$lod 60
 {
  replacemodel "Soldier_reference" "lod4_Soldier_reference"
}
$shadowlod
{
  replacemodel "Soldier_reference" "lod5_Soldier_reference"
}
$cdmaterials "models\Combine_Soldier\"
$texturegroup skinfamilies
{
	{ "combinesoldiersheet.vmt" }
	{ "combinesoldiersheet_shotgun.vmt" }
}
$hboxset "default"
$hbox 2 "ValveBiped.Bip01_Spine" -1.000  -4.740  -7.220  7.000  8.270  6.780
$hbox 1 "ValveBiped.Bip01_Head1" -1.750  -8.000  -3.500  7.250  4.000  3.500
$hbox 4 "ValveBiped.Bip01_L_UpperArm" -2.000  -2.750  -2.750  12.000  2.750  2.750
$hbox 4 "ValveBiped.Bip01_L_Forearm" 0.000  -2.440  -2.150  10.000  2.060  2.350
$hbox 4 "ValveBiped.Bip01_L_Hand" 0.250  -1.000  -2.000  5.750  1.000  2.000
$hbox 5 "ValveBiped.Bip01_R_UpperArm" -2.000  -2.750  -2.750  12.000  2.750  2.750
$hbox 5 "ValveBiped.Bip01_R_Forearm" 0.000  -2.250  -2.250  10.000  2.250  2.250
$hbox 5 "ValveBiped.Bip01_R_Hand" 0.500  -0.960  -2.330  5.500  1.040  2.610
$hbox 6 "ValveBiped.Bip01_L_Thigh" 1.070  -3.000  -3.000  18.920  5.000  4.000
$hbox 6 "ValveBiped.Bip01_L_Calf" 0.500  -3.500  -2.800  15.500  3.500  3.200
$hbox 6 "ValveBiped.Bip01_L_Foot" -0.500  -1.500  -2.000  5.500  4.500  2.500
$hbox 6 "ValveBiped.Bip01_L_Toe0" -0.250  -1.750  -2.190  3.250  0.750  2.460
$hbox 7 "ValveBiped.Bip01_R_Thigh" 1.000  -5.500  -3.500  19.000  3.500  3.500
$hbox 7 "ValveBiped.Bip01_R_Calf" 0.500  -4.010  -2.570  15.500  3.990  2.930
$hbox 7 "ValveBiped.Bip01_R_Foot" -1.250  -4.000  -2.250  5.250  2.000  2.250
$hbox 7 "ValveBiped.Bip01_R_Toe0" -1.000  -0.750  -2.600  3.000  1.750  2.100
$hbox 0 "ValveBiped.Bip01_Pelvis" -7.000  -4.500  -8.000  7.000  4.500  5.000
$hbox 0 "ValveBiped.Bip01_Spine2" -1.000  -4.250  -7.250  13.000  8.250  7.250
// Model uses material "combinesoldiersheet.vmt"
// Model uses material "combinesoldiersheet_shotgun.vmt"
$attachment "lefthand" "ValveBiped.Bip01_L_Hand" 0.00 -0.00 -0.00 rotate -0.00 -0.00 -0.00
$attachment "zipline" "ValveBiped.Bip01_Spine4" 3.00 -4.00 -0.00 rotate 0.00 -0.00 0.00
$attachment "eyes" "ValveBiped.Bip01_Head1" 4.50 -5.00 0.00 rotate 0.00 -90.00 -90.00
$attachment "beam_damage" "ValveBiped.Bip01_Spine2" 0.00 -0.00 0.00 rotate 0.00 -0.00 0.00
$attachment "anim_attachment_RH" "ValveBiped.Anim_Attachment_RH" 0.00 0.00 0.00 rotate -90.00 -90.00 0.00
$attachment "anim_attachment_LH" "ValveBiped.Anim_Attachment_LH" 0.00 0.00 -0.00 rotate -90.00 -90.00 0.00
$includemodel "combine_soldier_anims.mdl"
$includemodel "humans/male_shared.mdl"
$includemodel "humans/male_ss.mdl"
$includemodel "humans/male_gestures.mdl"
$includemodel "humans/male_postures.mdl"
$surfaceprop "flesh"
$keyvalues  { physgun_interactions { "onpickup" "boogie" "onlaunch" "spin_zaxis" "onfirstimpact" "bloodsplat"  } } 
$eyeposition -0.000 0.000 73.000
$illumposition -2.255 0.067 35.510
$sequence ragdoll "ragdoll" ACT_DIERAGDOLL 1 fps 30.00
$ikchain rhand ValveBiped.Bip01_R_Hand knee  0.707 0.707 0.000
$ikchain lhand ValveBiped.Bip01_L_Hand knee  0.707 0.707 0.000
$ikchain rfoot ValveBiped.Bip01_R_Foot knee  0.707 -0.707 0.000
$ikchain lfoot ValveBiped.Bip01_L_Foot knee  0.707 -0.707 0.000
$collisionjoints "phymodel.smd" {

	$mass 100.0
	$inertia 2.00
	$damping 0.01
	$rotdamping 0.40
	$rootbone "valvebiped.bip01_pelvis"
	$jointmerge "ValveBiped.Bip01_Pelvis" "ValveBiped.Bip01_Spine1"

	$jointconstrain "valvebiped.bip01_spine2" x limit -48.00 48.00 0.20
	$jointconstrain "valvebiped.bip01_spine2" y limit -25.00 25.00 0.20
	$jointconstrain "valvebiped.bip01_spine2" z limit -25.00 50.00 0.20

	$jointconstrain "valvebiped.bip01_r_upperarm" x limit -50.00 57.00 0.20
	$jointconstrain "valvebiped.bip01_r_upperarm" y limit -19.00 102.00 0.20
	$jointconstrain "valvebiped.bip01_r_upperarm" z limit -93.00 30.00 0.20

	$jointconstrain "valvebiped.bip01_l_upperarm" x limit -50.00 57.00 0.20
	$jointconstrain "valvebiped.bip01_l_upperarm" y limit -102.00 19.00 0.20
	$jointconstrain "valvebiped.bip01_l_upperarm" z limit -93.00 30.00 0.20

	$jointmassbias "valvebiped.bip01_l_forearm" 2.00
	$jointconstrain "valvebiped.bip01_l_forearm" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_forearm" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_forearm" z limit -120.00 4.00 0.20

	$jointmassbias "valvebiped.bip01_l_hand" 4.00
	$jointconstrain "valvebiped.bip01_l_hand" x limit -120.00 120.00 0.20
	$jointconstrain "valvebiped.bip01_l_hand" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_hand" z limit -57.00 70.00 0.20

	$jointmassbias "valvebiped.bip01_r_forearm" 2.00
	$jointconstrain "valvebiped.bip01_r_forearm" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_forearm" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_forearm" z limit -120.00 4.00 0.20

	$jointmassbias "valvebiped.bip01_r_hand" 4.00
	$jointconstrain "valvebiped.bip01_r_hand" x limit -120.00 120.00 0.20
	$jointconstrain "valvebiped.bip01_r_hand" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_hand" z limit -57.00 70.00 0.20

	$jointconstrain "valvebiped.bip01_r_thigh" x limit -25.00 39.00 0.20
	$jointconstrain "valvebiped.bip01_r_thigh" y limit -8.00 75.00 0.20
	$jointconstrain "valvebiped.bip01_r_thigh" z limit -97.00 32.00 0.20

	$jointmassbias "valvebiped.bip01_r_calf" 2.00
	$jointconstrain "valvebiped.bip01_r_calf" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_calf" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_r_calf" z limit -12.00 126.00 0.20

	$jointmassbias "valvebiped.bip01_head1" 2.00
	$jointconstrain "valvebiped.bip01_head1" x limit -20.00 20.00 0.20
	$jointconstrain "valvebiped.bip01_head1" y limit -25.00 25.00 0.20
	$jointconstrain "valvebiped.bip01_head1" z limit -13.00 30.00 0.20

	$jointconstrain "valvebiped.bip01_l_thigh" x limit -25.00 20.00 0.20
	$jointconstrain "valvebiped.bip01_l_thigh" y limit -73.00 6.00 0.20
	$jointconstrain "valvebiped.bip01_l_thigh" z limit -93.00 30.00 0.20

	$jointmassbias "valvebiped.bip01_l_calf" 2.00
	$jointconstrain "valvebiped.bip01_l_calf" x limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_calf" y limit 0.00 0.00 0.20
	$jointconstrain "valvebiped.bip01_l_calf" z limit -8.00 126.00 0.20

	$jointmassbias "valvebiped.bip01_l_foot" 4.00
	$jointconstrain "valvebiped.bip01_l_foot" x limit -1.00 -1.00 0.20
	$jointconstrain "valvebiped.bip01_l_foot" y limit -19.00 19.00 0.20
	$jointconstrain "valvebiped.bip01_l_foot" z limit -15.00 35.00 0.20

	$jointmassbias "valvebiped.bip01_r_foot" 4.00
	$jointconstrain "valvebiped.bip01_r_foot" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_foot" y limit -21.00 6.00 0.20
	$jointconstrain "valvebiped.bip01_r_foot" z limit -15.00 35.00 0.20
	$animatedfriction 1.000 1000.000 0.800 0.500 0.000
}
$bonemerge ValveBiped.Anim_Attachment_RH
$bonemerge ValveBiped.Bip01_R_Hand

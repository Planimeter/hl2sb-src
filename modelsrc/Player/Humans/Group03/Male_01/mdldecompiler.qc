$cd "C:\Users\Andrew\Documents\hl2sb-src\modelsrc\Humans\Group03\Male_01"
$modelname "Humans\Group03\Male_01.mdl"
$model "male_01" "male_01_reference.smd" {
     flexfile "mdldecompiler_expressions.vta" {
          flex "upper_right" frame 2
          flex "lower_right" frame 4
          flex "upper_left" frame 6
          flex "lower_left" frame 8
          flex "AU42" frame 9
          flexpair "AU1" 1.0 frame 10
          flexpair "AU2" 1.0 frame 11
          flexpair "AU4" 1.0 frame 12
          flexpair "AU1AU2" 1.0 frame 13
          flexpair "AU12" 1.0 frame 14
          flex "AU12AU25" frame 15
          flexpair "AU15" 1.0 frame 16
          flexpair "AU17" 1.0 frame 17
          flexpair "AU10" 1.0 frame 18
          flex "AU16" frame 19
          flexpair "AU6" 1.0 frame 20
          flexpair "AU9" 1.0 frame 21
          flexpair "AU25" 1.0 frame 22
          flexpair "AU18" 1.0 frame 23
          flexpair "AU22" 1.0 frame 24
          flexpair "AU20" 1.0 frame 25
          flex "AU32" frame 26
          flex "AU24" frame 27
          flex "AU38" frame 28
          flex "AU31" frame 29
          flexpair "AU26" 1.0 frame 30
          flexpair "AU27" 1.0 frame 31
          flexpair "AU26Z" 1.0 frame 32
     }
     eyeball righteye ValveBiped.Bip01_Head1 -1.360 -4.104 67.487 eyeball_r 1.000  4.000 van_facemap 0.680
     eyeball lefteye ValveBiped.Bip01_Head1 1.354 -4.104 67.479 eyeball_l 1.000  -4.000 van_facemap 0.680
     // If you uncomment the following lines the eyes will be closed
     // I'm including it anyway in case anyone wants to experiment
     // eyelid  upper_right "mdldecompiler_expressions.vta" lowerer 2 -0.166000 neutral 2 0.140500 raiser 2 0.210400 split 0.1 eyeball righteye
     // eyelid  lower_right "mdldecompiler_expressions.vta" lowerer 4 -0.339900 neutral 4 -0.256000 raiser 4 -0.100100  split 0.1 eyeball righteye
     // If you uncomment the following lines the eyes will be closed
     // I'm including it anyway in case anyone wants to experiment
     // eyelid  upper_left "mdldecompiler_expressions.vta" lowerer 6 -0.166000 neutral 6 0.140500 raiser 6 0.210400 split -0.1 eyeball lefteye
     // eyelid  lower_left "mdldecompiler_expressions.vta" lowerer 8 -0.339900 neutral 8 -0.256000 raiser 8 -0.100100  split -0.1 eyeball lefteye
     mouth 0 "mouth"  ValveBiped.Bip01_Head1 0.000 1.000 0.000
     flexcontroller eyelid right_lid_raiser "range" 0.000 1.000
     flexcontroller eyelid left_lid_raiser "range" 0.000 1.000
     flexcontroller eyelid right_lid_tightener "range" 0.000 1.000
     flexcontroller eyelid left_lid_tightener "range" 0.000 1.000
     flexcontroller eyelid right_lid_droop "range" 0.000 1.000
     flexcontroller eyelid left_lid_droop "range" 0.000 1.000
     flexcontroller eyelid right_lid_closer "range" 0.000 1.000
     flexcontroller eyelid left_lid_closer "range" 0.000 1.000
     flexcontroller eyelid half_closed "range" 0.000 1.000
     flexcontroller eyelid blink "range" 0.000 1.000
     flexcontroller brow right_inner_raiser "range" 0.000 1.000
     flexcontroller brow left_inner_raiser "range" 0.000 1.000
     flexcontroller brow right_outer_raiser "range" 0.000 1.000
     flexcontroller brow left_outer_raiser "range" 0.000 1.000
     flexcontroller brow right_lowerer "range" 0.000 1.000
     flexcontroller brow left_lowerer "range" 0.000 1.000
     flexcontroller nose right_cheek_raiser "range" 0.000 1.000
     flexcontroller nose left_cheek_raiser "range" 0.000 1.000
     flexcontroller nose wrinkler "range" 0.000 1.000
     flexcontroller nose dilator "range" 0.000 1.000
     flexcontroller mouth right_upper_raiser "range" 0.000 1.000
     flexcontroller mouth left_upper_raiser "range" 0.000 1.000
     flexcontroller mouth right_corner_puller "range" 0.000 1.000
     flexcontroller mouth left_corner_puller "range" 0.000 1.000
     flexcontroller mouth right_corner_depressor "range" 0.000 1.000
     flexcontroller mouth left_corner_depressor "range" 0.000 1.000
     flexcontroller mouth chin_raiser "range" 0.000 1.000
     flexcontroller phoneme right_part "range" 0.000 1.000
     flexcontroller phoneme left_part "range" 0.000 1.000
     flexcontroller phoneme right_puckerer "range" 0.000 1.000
     flexcontroller phoneme left_puckerer "range" 0.000 1.000
     flexcontroller phoneme right_funneler "range" 0.000 1.000
     flexcontroller phoneme left_funneler "range" 0.000 1.000
     flexcontroller phoneme right_stretcher "range" 0.000 1.000
     flexcontroller phoneme left_stretcher "range" 0.000 1.000
     flexcontroller phoneme bite "range" 0.000 1.000
     flexcontroller phoneme presser "range" 0.000 1.000
     flexcontroller phoneme tightener "range" 0.000 1.000
     flexcontroller phoneme jaw_clencher "range" 0.000 1.000
     flexcontroller phoneme jaw_drop "range" 0.000 1.000
     flexcontroller phoneme right_mouth_drop "range" 0.000 1.000
     flexcontroller phoneme left_mouth_drop "range" 0.000 1.000
     flexcontroller mouth smile "range" 0.000 1.000
     flexcontroller mouth lower_lip "range" 0.000 1.000
     flexcontroller head head_rightleft "range" -30.000 30.000
     flexcontroller head head_updown "range" -15.000 15.000
     flexcontroller head head_tilt "range" -15.000 15.000
     flexcontroller eyes eyes_updown "range" -30.000 30.000
     flexcontroller eyes eyes_rightleft "range" -30.000 30.000
     flexcontroller body body_rightleft "range" -30.000 30.000
     flexcontroller chest chest_rightleft "range" -30.000 30.000
     flexcontroller head head_forwardback "range" -0.200 0.200
     flexcontroller gesture gesture_updown "range" -1.000 1.000
     flexcontroller gesture gesture_rightleft "range" -1.000 1.000
    localvar upper_right_raiser
    %upper_right_raiser = ((( right_lid_raiser * ( 1.000 - ( right_lid_droop *  0.800))) * ( 1.000 -  right_lid_closer)) * ( 1.000 -  blink))
    localvar upper_right_neutral
    %upper_right_neutral = (((( 1.000 - ( right_lid_droop *  0.800)) * ( 1.000 -  right_lid_raiser)) * ( 1.000 -  right_lid_closer)) * ( 1.000 -  blink))
    localvar upper_right_lowerer
    %upper_right_lowerer = ( right_lid_closer + ( blink * ( 1.000 -  right_lid_closer)))
    localvar upper_left_raiser
    %upper_left_raiser = ((( left_lid_raiser * ( 1.000 - ( left_lid_droop *  0.800))) * ( 1.000 -  left_lid_closer)) * ( 1.000 -  blink))
    localvar upper_left_neutral
    %upper_left_neutral = (((( 1.000 - ( left_lid_droop *  0.800)) * ( 1.000 -  left_lid_raiser)) * ( 1.000 -  left_lid_closer)) * ( 1.000 -  blink))
    localvar upper_left_lowerer
    %upper_left_lowerer = ( left_lid_closer + ( blink * ( 1.000 -  left_lid_closer)))
    localvar lower_right_raiser
    %lower_right_raiser = ( right_lid_closer + (( blink *  0.500) * ( 1.000 -  right_lid_closer)))
    localvar lower_right_neutral
    %lower_right_neutral = (((( 1.000 -  right_lid_closer) * ( 1.000 - ( 0.500 *  blink))) * ( 1.000 - ( right_lid_tightener *  0.500))) * ( 1.000 - ( right_cheek_raiser *  0.250)))
    localvar lower_right_lowerer
    %lower_right_lowerer =  0.000
    localvar lower_left_raiser
    %lower_left_raiser = ( left_lid_closer + (( blink *  0.500) * ( 1.000 -  left_lid_closer)))
    localvar lower_left_neutral
    %lower_left_neutral = (((( 1.000 -  left_lid_closer) * ( 1.000 - ( 0.500 *  blink))) * ( 1.000 - ( left_lid_tightener *  0.500))) * ( 1.000 - ( left_cheek_raiser *  0.250)))
    localvar lower_left_lowerer
    %lower_left_lowerer =  0.000
    %AU1R = (( right_inner_raiser * ( 1.000 -  right_outer_raiser)) * ( 1.000 -  right_lowerer))
    %AU1L = (( left_inner_raiser * ( 1.000 -  left_outer_raiser)) * ( 1.000 -  left_lowerer))
    %AU2R = (( right_outer_raiser * ( 1.000 -  right_inner_raiser)) * ( 1.000 -  right_lowerer))
    %AU2L = (( left_outer_raiser * ( 1.000 -  left_inner_raiser)) * ( 1.000 -  left_lowerer))
    %AU1AU2R = (( right_inner_raiser *  right_outer_raiser) * ( 1.000 -  right_lowerer))
    %AU1AU2L = (( left_inner_raiser *  left_outer_raiser) * ( 1.000 -  left_lowerer))
    %AU4R =  right_lowerer
    %AU4L =  left_lowerer
    %AU6R =  right_cheek_raiser
    %AU6L =  left_cheek_raiser
    %AU9R =  wrinkler
    %AU9L =  wrinkler
    %AU38 =  dilator
    localvar right_open
    %right_open = ((((( smile + ( right_part * ( 1.000 -  smile))) +  right_puckerer) +  right_funneler) + ( right_upper_raiser *  0.500)) + ( lower_lip *  0.500))
    localvar left_open
    %left_open = ((((( smile + ( left_part * ( 1.000 -  smile))) +  left_puckerer) +  left_funneler) + ( left_upper_raiser *  0.500)) + ( lower_lip *  0.500))
    localvar right_lip_suppressor
    %right_lip_suppressor = ((( 1.000 /  %right_open) * ( 1.000 -  presser)) * ( 1.000 -  bite))
    localvar left_lip_suppressor
    %left_lip_suppressor = ((( 1.000 /  %left_open) * ( 1.000 -  presser)) * ( 1.000 -  bite))
    %AU10R = ( right_upper_raiser * (( right_upper_raiser *  0.500) *  %right_lip_suppressor))
    %AU10L = ( left_upper_raiser * (( left_upper_raiser *  0.500) *  %left_lip_suppressor))
    %AU25R = (( right_part * ( right_part *  %right_lip_suppressor)) * ( 1.000 -  smile))
    %AU25L = (( left_part * ( left_part *  %left_lip_suppressor)) * ( 1.000 -  smile))
    %AU12AU25 = ( smile * (( smile *  0.500) * ( %right_lip_suppressor +  %left_lip_suppressor)))
    %AU18R = ( right_puckerer * ( right_puckerer *  %right_lip_suppressor))
    %AU18L = ( left_puckerer * ( left_puckerer *  %left_lip_suppressor))
    %AU22R = ( right_funneler * ( right_funneler *  %right_lip_suppressor))
    %AU22L = ( left_funneler * ( left_funneler *  %left_lip_suppressor))
    %AU17R = (((( chin_raiser * ( 1.000 - ( jaw_drop *  0.500))) * ( 1.000 -  bite)) * ( 1.000 - ( 0.600 * (( %AU25R +  %AU18R) +  %AU22R)))) * ( 1.000 -  smile))
    %AU17L = (((( chin_raiser * ( 1.000 - ( jaw_drop *  0.500))) * ( 1.000 -  bite)) * ( 1.000 - ( 0.600 * (( %AU25L +  %AU18L) +  %AU22L)))) * ( 1.000 -  smile))
    localvar right_depressor_suppressor
    %right_depressor_suppressor = ((( 1.000 / ((((((( %AU18R *  0.800) + ( %AU22R *  0.800)) +  %AU17R) +  bite) +  right_corner_puller) +  right_stretcher) +  right_corner_depressor)) * ( 1.000 - ( jaw_drop *  0.500))) * ( 1.000 - ( smile *  0.500)))
    localvar left_depressor_suppressor
    %left_depressor_suppressor = ((( 1.000 / ((((((( %AU18L *  0.800) + ( %AU22L *  0.800)) +  %AU17L) +  bite) +  left_corner_puller) +  left_stretcher) +  left_corner_depressor)) * ( 1.000 - ( jaw_drop *  0.500))) * ( 1.000 - ( smile *  0.500)))
    %AU15R = ( right_corner_depressor * ( right_corner_depressor *  %right_depressor_suppressor))
    %AU15L = ( left_corner_depressor * ( left_corner_depressor *  %left_depressor_suppressor))
    localvar right_corner_suppressor
    %right_corner_suppressor = ((( 1.000 / ((((((( %AU18R *  0.800) + ( %AU22R *  0.800)) +  %AU17R) +  bite) +  right_corner_puller) +  right_stretcher) +  right_corner_depressor)) * ( 1.000 - ( jaw_drop *  0.500))) * ( 1.000 -  smile))
    localvar left_corner_suppressor
    %left_corner_suppressor = ((( 1.000 / ((((((( %AU18L *  0.800) + ( %AU22L *  0.800)) +  %AU17L) +  bite) +  left_corner_puller) +  left_stretcher) +  left_corner_depressor)) * ( 1.000 - ( jaw_drop *  0.500))) * ( 1.000 -  smile))
    %AU12R = ( right_corner_puller * ( right_corner_puller *  %right_corner_suppressor))
    %AU12L = ( left_corner_puller * ( left_corner_puller *  %left_corner_suppressor))
    %AU20R = ( right_stretcher * ( right_stretcher *  %right_corner_suppressor))
    %AU20L = ( left_stretcher * ( left_stretcher *  %left_corner_suppressor))
    %AU32 =  bite
    %AU24 = ( presser + (( 1.000 -  presser) *  tightener))
    %AU31 = ( jaw_clencher * ( jaw_clencher / ( jaw_clencher +  jaw_drop)))
    localvar right_drop_suppressor
    %right_drop_suppressor = ((( 1.000 / (( 0.500 *  %right_open) +  right_mouth_drop)) * ( 1.000 -  presser)) * ( 1.000 -  bite))
    localvar left_drop_suppressor
    %left_drop_suppressor = ((( 1.000 / (( 0.500 *  %left_open) +  left_mouth_drop)) * ( 1.000 -  presser)) * ( 1.000 -  bite))
    localvar right_drop
    %right_drop = ( right_mouth_drop * ( right_mouth_drop *  %right_drop_suppressor))
    localvar left_drop
    %left_drop = ( left_mouth_drop * ( left_mouth_drop *  %left_drop_suppressor))
    %AU26R = (( jaw_drop * ( 1.000 -  %right_drop)) * ( jaw_drop / ( jaw_clencher +  jaw_drop)))
    %AU26L = (( jaw_drop * ( 1.000 -  %left_drop)) * ( jaw_drop / ( jaw_clencher +  jaw_drop)))
    %AU27R = (( jaw_drop *  %right_drop) * ( jaw_drop / ( jaw_clencher +  jaw_drop)))
    %AU27L = (( jaw_drop *  %left_drop) * ( jaw_drop / ( jaw_clencher +  jaw_drop)))
    %AU16 = (( lower_lip * (( lower_lip *  0.250) * ( %right_lip_suppressor +  %left_lip_suppressor))) * ( 1.000 - ( 0.500 * ( %AU27R +  %AU27L))))
    localvar mouth
    %mouth = ((((((((((( %AU27R *  0.500) + ( %AU27L *  0.500)) + ( %AU22R *  0.350)) + ( %AU22L *  0.350)) + ( %AU18R *  0.250)) + ( %AU18L *  0.250)) + ( %AU25R *  0.350)) + ( %AU25L *  0.350)) + ( %AU32 *  0.500)) +  %AU12AU25) + ( %AU16 *  0.400))
}
$lod 7
 {
  replacemodel "male_01_reference" "lod1_male_01_reference"
}
$lod 14
 {
  replacemodel "male_01_reference" "lod2_male_01_reference"
}
$lod 24
 {
  replacemodel "male_01_reference" "lod3_male_01_reference"
}
$lod 35
 {
  replacemodel "male_01_reference" "lod4_male_01_reference"
}
$lod 60
 {
  replacemodel "male_01_reference" "lod5_male_01_reference"
}
$shadowlod
{
  replacemodel "male_01_reference" "lod6_male_01_reference"
}
$cdmaterials "models\Humans\Male\Group03\"
$cdmaterials "models\Humans\Male\"
$cdmaterials "models\humans\male\"
$hboxset "default"
$hbox 1 "ValveBiped.Bip01_Head1" -1.250  -6.500  -3.190  8.250  3.500  3.310
$hbox 4 "ValveBiped.Bip01_L_UpperArm" 0.000  -2.750  -2.750  12.510  1.750  2.250
$hbox 4 "ValveBiped.Bip01_L_Forearm" 0.000  -2.190  -2.380  13.000  1.810  1.620
$hbox 4 "ValveBiped.Bip01_L_Hand" 0.060  -2.000  -1.500  4.060  1.000  2.500
$hbox 5 "ValveBiped.Bip01_R_UpperArm" 0.000  -3.000  -2.250  12.390  2.000  2.750
$hbox 5 "ValveBiped.Bip01_R_Forearm" -0.500  -2.200  -1.550  12.500  1.800  2.450
$hbox 5 "ValveBiped.Bip01_R_Hand" 0.060  -2.000  -2.500  4.060  1.000  1.500
$hbox 6 "ValveBiped.Bip01_L_Thigh" 0.000  -3.750  -3.250  17.850  3.750  3.250
$hbox 6 "ValveBiped.Bip01_L_Calf" 0.000  -3.510  -3.280  15.640  3.490  2.720
$hbox 6 "ValveBiped.Bip01_L_Foot" -1.070  -2.000  -2.840  5.040  5.000  2.160
$hbox 6 "ValveBiped.Bip01_L_Toe0" -0.500  -3.000  -2.190  2.500  0.000  2.460
$hbox 7 "ValveBiped.Bip01_R_Thigh" 0.000  -3.750  -3.250  17.850  3.750  3.250
$hbox 7 "ValveBiped.Bip01_R_Calf" 0.000  -3.510  -2.820  15.640  3.490  3.180
$hbox 7 "ValveBiped.Bip01_R_Foot" -1.060  -2.010  -2.280  5.050  4.990  2.720
$hbox 7 "ValveBiped.Bip01_R_Toe0" -0.500  -3.000  -2.600  2.500  0.000  2.100
$hbox 3 "ValveBiped.Bip01_Pelvis" -7.500  -5.500  -6.000  7.500  8.500  5.000
$hbox 2 "ValveBiped.Bip01_Spine2" -2.500  -2.500  -7.000  14.500  7.500  7.000
// Model uses material "van_facemap.vmt"
// Model uses material "dark_eyeball_r.vmt"
// Model uses material "mouth.vmt"
// Model uses material "citizen_sheet.vmt"
// Model uses material "dark_eyeball_l.vmt"
$attachment "eyes" "ValveBiped.Bip01_Head1" 3.67 -4.32 0.00 rotate 0.00 -80.10 -90.00
$attachment "mouth" "ValveBiped.Bip01_Head1" 1.00 -6.50 -0.00 rotate 0.00 -80.00 -90.00
$attachment "chest" "ValveBiped.Bip01_Spine2" 5.00 4.00 -0.00 rotate -0.00 90.00 90.00
$attachment "forward" "ValveBiped.forward" -0.00 -0.00 -0.00 rotate 0.00 0.00 -0.00
$attachment "anim_attachment_RH" "ValveBiped.Anim_Attachment_RH" -0.00 0.00 -0.00 rotate -90.00 -90.00 0.00
$attachment "anim_attachment_LH" "ValveBiped.Anim_Attachment_LH" 0.00 0.00 -0.00 rotate -90.00 -90.00 0.00
$attachment "anim_attachment_head" "ValveBiped.Bip01_Head1" -0.00 -0.00 0.00 rotate -90.00 -90.00 0.00
$includemodel "humans/male_shared.mdl"
$includemodel "humans/male_ss.mdl"
$includemodel "humans/male_gestures.mdl"
$includemodel "humans/male_postures.mdl"
$surfaceprop "flesh"
$eyeposition -0.000 0.000 70.000
$illumposition -1.682 0.001 35.329
$sequence ragdoll "ragdoll" ACT_DIERAGDOLL 1 fps 30.00
$ikchain rhand ValveBiped.Bip01_R_Hand knee  0.707 0.707 0.000
$ikchain lhand ValveBiped.Bip01_L_Hand knee  0.707 0.707 0.000
$ikchain rfoot ValveBiped.Bip01_R_Foot knee  0.707 -0.707 0.000
$ikchain lfoot ValveBiped.Bip01_L_Foot knee  0.707 -0.707 0.000
$collisionjoints "phymodel.smd" {

	$mass 90.0
	$inertia 10.00
	$damping 0.01
	$rotdamping 1.50
	$rootbone "valvebiped.bip01_pelvis"
	$jointmerge "ValveBiped.Bip01_Pelvis" "ValveBiped.Bip01_Spine1"

	$jointconstrain "valvebiped.bip01_spine2" x limit -48.00 48.00 0.00
	$jointconstrain "valvebiped.bip01_spine2" y limit -25.00 25.00 0.00
	$jointconstrain "valvebiped.bip01_spine2" z limit -25.00 50.00 0.00

	$jointconstrain "valvebiped.bip01_r_upperarm" x limit -39.00 39.00 0.00
	$jointconstrain "valvebiped.bip01_r_upperarm" y limit -79.00 95.00 0.00
	$jointconstrain "valvebiped.bip01_r_upperarm" z limit -93.00 23.00 0.00

	$jointconstrain "valvebiped.bip01_l_upperarm" x limit -30.00 30.00 0.00
	$jointconstrain "valvebiped.bip01_l_upperarm" y limit -95.00 84.00 0.00
	$jointconstrain "valvebiped.bip01_l_upperarm" z limit -86.00 26.00 0.00

	$jointconstrain "valvebiped.bip01_l_forearm" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_forearm" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_forearm" z limit -149.00 4.00 0.00

	$jointconstrain "valvebiped.bip01_l_hand" x limit -37.00 37.00 0.00
	$jointconstrain "valvebiped.bip01_l_hand" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_hand" z limit -57.00 59.00 0.00

	$jointconstrain "valvebiped.bip01_r_forearm" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_forearm" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_forearm" z limit -149.00 4.00 0.00

	$jointconstrain "valvebiped.bip01_r_hand" x limit -60.00 60.00 0.00
	$jointconstrain "valvebiped.bip01_r_hand" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_hand" z limit -57.00 70.00 0.00

	$jointconstrain "valvebiped.bip01_r_thigh" x limit -12.00 12.00 0.00
	$jointconstrain "valvebiped.bip01_r_thigh" y limit -8.00 75.00 0.00
	$jointconstrain "valvebiped.bip01_r_thigh" z limit -97.00 32.00 0.00

	$jointconstrain "valvebiped.bip01_r_calf" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_calf" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_calf" z limit -12.00 126.00 0.00

	$jointconstrain "valvebiped.bip01_head1" x limit -20.00 20.00 0.00
	$jointconstrain "valvebiped.bip01_head1" y limit -25.00 25.00 0.00
	$jointconstrain "valvebiped.bip01_head1" z limit -13.00 30.00 0.00

	$jointconstrain "valvebiped.bip01_l_thigh" x limit -12.00 12.00 0.00
	$jointconstrain "valvebiped.bip01_l_thigh" y limit -73.00 6.00 0.00
	$jointconstrain "valvebiped.bip01_l_thigh" z limit -93.00 30.00 0.00

	$jointconstrain "valvebiped.bip01_l_calf" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_calf" y limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_calf" z limit -8.00 126.00 0.00

	$jointconstrain "valvebiped.bip01_l_foot" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_l_foot" y limit -19.00 19.00 0.00
	$jointconstrain "valvebiped.bip01_l_foot" z limit -15.00 35.00 0.00

	$jointconstrain "valvebiped.bip01_r_foot" x limit 0.00 0.00 0.00
	$jointconstrain "valvebiped.bip01_r_foot" y limit -25.00 6.00 0.00
	$jointconstrain "valvebiped.bip01_r_foot" z limit -15.00 35.00 0.00
	$animatedfriction 1.000 400.000 0.500 0.300 0.000
}

--========= Copyleft © 2010-2013, Team Sandbox, Some rights reserved. ============--
--
-- Purpose: 
--
-- $NoKeywords: $
--
--=============================================================================--
include( "../includes/extensions/table.lua" );
include( "../includes/extensions/keyvalues.lua" );
include( "../includes/extensions/vgui.lua" );

local vgui = vgui;

local CContentSubGames = {
	m_pHalfLife2CheckBox = INVALID_PANEL,
	m_pCounterStrikeCheckBox = INVALID_PANEL,
	m_pHalfLifeCheckBox = INVALID_PANEL,
	m_pDayOfDefeatCheckBox = INVALID_PANEL,
	m_pLostCoastCheckBox = INVALID_PANEL,
	m_pHalfLifeDeathmatchCheckBox = INVALID_PANEL,
	m_pEpisodicCheckBox = INVALID_PANEL,
	m_pPortalCheckBox = INVALID_PANEL,
	m_pEpisodeTwoCheckBox = INVALID_PANEL,
	m_pTeamFortressCheckBox = INVALID_PANEL
}

local g_GameCheckButtons =
{
	{ "m_pHalfLife2CheckBox",			220 },
	{ "m_pCounterStrikeCheckBox",		240 },
	{ "m_pHalfLifeCheckBox",			280 },
	{ "m_pDayOfDefeatCheckBox",			300 },
	{ "m_pLostCoastCheckBox",			340 },
	{ "m_pHalfLifeDeathmatchCheckBox",	360 },
	{ "m_pEpisodicCheckBox",			380 },
	{ "m_pPortalCheckBox",				400 },
	{ "m_pEpisodeTwoCheckBox",			420 },
	{ "m_pTeamFortressCheckBox",		440 }
};

-------------------------------------------------------------------------------
-- Purpose: 
-------------------------------------------------------------------------------
local function OnCheckButtonChecked( self )
	local _propertyDialog = self:GetParent():GetParent():GetParent()
	_R.PropertyDialog.EnableApplyButton( _propertyDialog, true )
end

-------------------------------------------------------------------------------
-- Purpose: Constructor
-------------------------------------------------------------------------------
function CContentSubGames:Init(parent)
	self.m_pHalfLife2CheckBox = vgui.CheckButton( 
		self, 
		"HalfLife2", 
		"Half-Life 2" );
	self.m_pHalfLife2CheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pHalfLife2CheckBox:AddActionSignalTarget( self.m_pHalfLife2CheckBox )

	self.m_pCounterStrikeCheckBox = vgui.CheckButton( 
		self, 
		"CounterStrike", 
		"Counter-Strike: Source" );
	self.m_pCounterStrikeCheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pCounterStrikeCheckBox:AddActionSignalTarget( self.m_pCounterStrikeCheckBox )

	self.m_pHalfLifeCheckBox = vgui.CheckButton( 
		self, 
		"HalfLife", 
		"Half-Life: Source" );
	self.m_pHalfLifeCheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pHalfLifeCheckBox:AddActionSignalTarget( self.m_pHalfLifeCheckBox )

	self.m_pDayOfDefeatCheckBox = vgui.CheckButton( 
		self, 
		"DayOfDefeat", 
		"Day of Defeat: Source" );
	self.m_pDayOfDefeatCheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pDayOfDefeatCheckBox:AddActionSignalTarget( self.m_pDayOfDefeatCheckBox )

	self.m_pLostCoastCheckBox = vgui.CheckButton( 
		self, 
		"LostCoast", 
		"Half-Life 2: Lost Coast" );
	self.m_pLostCoastCheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pLostCoastCheckBox:AddActionSignalTarget( self.m_pLostCoastCheckBox )

	self.m_pHalfLifeDeathmatchCheckBox = vgui.CheckButton( 
		self, 
		"HalfLifeDeathmatch", 
		"Half-Life Deathmatch: Source" );
	self.m_pHalfLifeDeathmatchCheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pHalfLifeDeathmatchCheckBox:AddActionSignalTarget( self.m_pHalfLifeDeathmatchCheckBox )

	self.m_pEpisodicCheckBox = vgui.CheckButton( 
		self, 
		"Episodic", 
		"Half-Life 2: Episode One" );
	self.m_pEpisodicCheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pEpisodicCheckBox:AddActionSignalTarget( self.m_pEpisodicCheckBox )

	self.m_pPortalCheckBox = vgui.CheckButton( 
		self, 
		"Portal", 
		"Portal" );
	self.m_pPortalCheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pPortalCheckBox:AddActionSignalTarget( self.m_pPortalCheckBox )

	self.m_pEpisodeTwoCheckBox = vgui.CheckButton( 
		self, 
		"EpisodeTwo", 
		"Half-Life 2: Episode Two" );
	self.m_pEpisodeTwoCheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pEpisodeTwoCheckBox:AddActionSignalTarget( self.m_pEpisodeTwoCheckBox )

	self.m_pTeamFortressCheckBox = vgui.CheckButton( 
		self, 
		"TeamFortress", 
		"Team Fortress 2" );
	self.m_pTeamFortressCheckBox.OnCheckButtonChecked = OnCheckButtonChecked
	self.m_pTeamFortressCheckBox:AddActionSignalTarget( self.m_pTeamFortressCheckBox )

	self:LoadControlSettings("Resource\\ContentSubGames.res");
end

-------------------------------------------------------------------------------
-- Purpose: 
-------------------------------------------------------------------------------
function CContentSubGames:OnResetData()
	local pMainFile = KeyValues( "gamecontent.txt" );
	if ( pMainFile:LoadFromFile( engine.GetGameDirectory() .. "/gamecontent.txt", "MOD" ) ) then
		local tMainFile = pMainFile:ToTable( true )
		if ( tMainFile[1].value and type( tMainFile[1].value ) == "table" ) then
			for i, pair in ipairs( tMainFile[1].value ) do
				local checkButtonName
				for j=1, #g_GameCheckButtons do
					if ( tonumber( pair.value ) == g_GameCheckButtons[j][2] ) then
						checkButtonName = g_GameCheckButtons[j][1]
					end
				end
				if ( checkButtonName ) then
					self[ checkButtonName ]:SetSelected( true )
				end
			end
		end
	end
	pMainFile:deleteThis();
end

-------------------------------------------------------------------------------
-- Purpose: 
-------------------------------------------------------------------------------
function CContentSubGames:OnOK(applyOnly)
	self:OnApplyChanges()
end

-------------------------------------------------------------------------------
-- Purpose: 
-------------------------------------------------------------------------------
function CContentSubGames:OnApplyChanges()
	local pGameContent = KeyValues( "GameContent" )
	local pFileSystemKey = pGameContent:CreateNewKey()
	pFileSystemKey:SetName( "FileSystem" )
	local checkButtonName
	local pCheckButton
	for i=1, #g_GameCheckButtons do
		checkButtonName = g_GameCheckButtons[i][1]
		pCheckButton = self[ checkButtonName ]
		if ( pCheckButton and pCheckButton:IsSelected() ) then
			local AppId = KeyValues( "AppId" )
			AppId:SetStringValue( tostring( g_GameCheckButtons[i][2] ) )
			pFileSystemKey:AddSubKey( AppId )
		end
	end
	pGameContent:SaveToFile( "gamecontent.txt", "MOD" )
	pGameContent:deleteThis()
end

vgui.register( CContentSubGames, "CContentSubGames", "PropertyPage" )

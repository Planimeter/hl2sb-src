--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Wraps the font type so handles are persistent through screen size
--          changes, and fonts work properly without having to manually
--          recreate them.
--
--===========================================================================--

if not _CLIENT then return end

require( "UTIL" )
require( "surface" )
require( "hook" )

local INVALID_FONT = INVALID_FONT
local setmetatable = setmetatable
local ipairs = ipairs
local type = type
local ComputeStringWidth = UTIL.ComputeStringWidth
local GetFontName = _R.IScheme.GetFontName
local CreateFont = surface.CreateFont
local DrawSetTextFont = surface.DrawSetTextFont
local GetCharABCwide = surface.GetCharABCwide
local GetCharacterWidth = surface.GetCharacterWidth
local GetFontAscent = surface.GetFontAscent
local GetFontTall = surface.GetFontTall
local GetTextSize = surface.GetTextSize
local IsFontAdditive = surface.IsFontAdditive
local SetFontGlyphSet = surface.SetFontGlyphSet

-------------------------------------------------------------------------------
-- _R.HFontContainer
-- Purpose: Class metatable
-------------------------------------------------------------------------------
_R.HFontContainer = {
  __index = {},
  __type = "fontcontainer"
}

-------------------------------------------------------------------------------
-- HFontContainerIndex
-- Purpose: Internal container index
-------------------------------------------------------------------------------
local HFontContainerIndex = 1

-------------------------------------------------------------------------------
-- HFontContainers
-- Purpose: Internal containers index
-------------------------------------------------------------------------------
local HFontContainers = {}

-------------------------------------------------------------------------------
-- HFontContainer()
-- Purpose: Creates a new HFontContainer
-- Output: HFontContainer
-------------------------------------------------------------------------------
function HFontContainer()
  local t = {
    index = 0,
    font = INVALID_FONT,
    windowsFontName = "",
    tall = 0,
    weight = 0,
    blur = 0,
    scanlines = 0,
    flags = 0,
    nRangeMin = nil,
    nRangeMax = nil
  }
  setmetatable( t, _R.HFontContainer )
  return t
end

-------------------------------------------------------------------------------
-- HFontContainer:__tostring()
-- Purpose: __tostring metamethod for HFontContainer
-------------------------------------------------------------------------------
function _R.HFontContainer:__tostring()
  return "HFontContainer: " .. self.index
end

hook.add( "OnScreenSizeChanged", "HFontContainerManager", function()
  for i, fontcontainer in ipairs( HFontContainers ) do
    fontcontainer.font = CreateFont()
    SetFontGlyphSet( fontcontainer.font,
                     fontcontainer.windowsFontName,
                     fontcontainer.tall,
                     fontcontainer.weight,
                     fontcontainer.blur,
                     fontcontainer.scanlines,
                     fontcontainer.flags,
                     fontcontainer.nRangeMin,
                     fontcontainer.nRangeMax )
  end
end )

function UTIL.ComputeStringWidth( font, str )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'ComputeStringWidth' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return ComputeStringWidth( font, str )
  elseif ( type == "fontcontainer" ) then
    return ComputeStringWidth( font.font, str )
  end
end

function _R.IScheme.GetFontName( font )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'GetFontName' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return GetFontName( font )
  elseif ( type == "fontcontainer" ) then
    return GetFontName( font.font )
  end
end

function surface.CreateFont()
  local fontcontainer = HFontContainer()
  fontcontainer.index = HFontContainerIndex
  fontcontainer.font = CreateFont()
  HFontContainers[ HFontContainerIndex ] = fontcontainer
  HFontContainerIndex = HFontContainerIndex + 1
  return fontcontainer
end

function surface.DrawSetTextFont( font )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'DrawSetTextFont' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return DrawSetTextFont( font )
  elseif ( type == "fontcontainer" ) then
    return DrawSetTextFont( font.font )
  end
end

function surface.GetCharABCwide( font, ch )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'GetCharABCwide' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return GetCharABCwide( font, ch )
  elseif ( type == "fontcontainer" ) then
    return GetCharABCwide( font.font, ch )
  end
end

function surface.GetCharacterWidth( font, ch )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'GetCharacterWidth' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return GetCharacterWidth( font, ch )
  elseif ( type == "fontcontainer" ) then
    return GetCharacterWidth( font.font, ch )
  end
end

function surface.GetFontAscent( font, ch )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'GetFontAscent' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return GetFontAscent( font, ch )
  elseif ( type == "fontcontainer" ) then
    return GetFontAscent( font.font, ch )
  end
end

function surface.GetFontTall( font )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'GetFontTall' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return GetFontTall( font )
  elseif ( type == "fontcontainer" ) then
    return GetFontTall( font.font )
  end
end

function surface.GetTextSize( font, text )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'GetTextSize' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return GetTextSize( font, text )
  elseif ( type == "fontcontainer" ) then
    return GetTextSize( font.font, text )
  end
end

function surface.IsFontAdditive( font )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'IsFontAdditive' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return IsFontAdditive( font )
  elseif ( type == "fontcontainer" ) then
    return IsFontAdditive( font.font )
  end
end

function surface.SetFontGlyphSet( font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax )
  local type = type( font )
  if ( type ~= "font" and type ~= "fontcontainer" ) then
    error( "bad argument #1 to 'SetFontGlyphSet' (font or fontcontainer expected, got " .. type .. ")", 2 )
  end
  if ( type == "font" ) then
    return SetFontGlyphSet( font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax )
  elseif ( type == "fontcontainer" ) then
    font.windowsFontName = windowsFontName
    font.tall = tall
    font.weight = weight
    font.blur = blur
    font.scanlines = scanlines
    font.flags = flags
    font.nRangeMin = nRangeMin
    font.nRangeMax = nRangeMax
    return SetFontGlyphSet( font.font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax )
  end
end

--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Lua Style Guide for Half-Life 2: Sandbox Standardization.
--
--===========================================================================--

-- Formatting
local function Indentation()
  print("Use two spaces for indentation.")
end

-- Naming
g_ExampleTable    = {} -- variable names with a larger scope should be more
local t           = {} -- descriptive than those with a smaller scope
t.m_iCount        = 100
t.m_flPercentage  = 94.75
t.m_strConvention = "Use Apps (original) Hungarian notation as your naming convention."
t.m_bCleanCode    = true -- Use CamelCase for naming as well
t.m_tTable        = {}
t.m_fnMsg         = print
t.m_flNil         = nil

_REFCOUNT = 10 -- As a convention, names starting with an underscope followed
               -- by uppercase letters (such as _VERSION) as reserved for
			   -- internal global variables use by Lua. In HL2:SB, you may use
			   -- this convention for your own internal needs.
			   
NUM_OBJECTS = "Unknown" -- Constants, particularly ones that are simple values,
                        -- are often given in ALL_CAPS, with words optionally
						-- separated by underscores
						
-- The variable "_" is commonly used as a placeholder when you want to ignore
-- the variable:
for _, v in pairs(t) do
  v = nil
end

-- i, k, v, and t are often used as follows:
for k, v in pairs(t) do
  if not v then
    k = nil
  end
end

for i, v in ipairs(t) do
  if i > 1 then
    break
  end
end

local mt = {}
mt.__newindex = function(t, k, v) end

-- self refers to the object a method is invoked on
function mt:GetClassname()
  return self.Classname or "Unknown"
end

-- Libraries and Features
-- Avoid using the debug library unless necessary, especially if trusted code
-- is being run.
-- Avoid deprecated features.

-- Scope
-- Use locals rather than globals whenever possible.
local x = 0
local function count()
  x = x + 1
  print(x)
end

-- Commenting
-- Limit commented lines to 80 columns. Break the lines into other single-line
-- comments rather than writing one block comment when making short comments.

-- Leave one line empty at the bottom of your script.

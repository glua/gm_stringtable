solution "gm_stringtable"
	language "C++"
	location "project"
	targetdir "bin"

	flags "StaticRuntime"
	architecture "x86"

	configurations {
		"Debug",
		"Release"
	}

	configuration "Debug"
		flags { "symbols" }

	configuration "Release"
		optimize "On"

	project "gm_stringtable"
		kind "SharedLib"
		
		files {
			"src/**.cpp",
			"src/**.h"
		}

		include "LuaInterface"
		include "SourceSDK/Tier0"

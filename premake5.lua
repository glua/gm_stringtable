assert(os.getenv("SOURCE_SDK"), "SOURCE_SDK environmental variable not set")

local SOURCE_SDK = os.getenv("SOURCE_SDK") .. "/mp/src"

solution "gm_stringtable"
	language "C++"
	location "project"
	targetdir "bin"

	flags { "StaticRuntime" }

	includedirs {
		"include",
		path.join(SOURCE_SDK, "common"),
		path.join(SOURCE_SDK, "public"),
		path.join(SOURCE_SDK, "public/tier0"),
		path.join(SOURCE_SDK, "public/tier1")
	}

	libdirs {
		path.join(SOURCE_SDK, "lib/public")
	}

	links {
		"tier0",
		"tier1"
	}
	

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

		defines { "GMMODULE" }

		if os.is "windows" then
			targetsuffix "_win32"
		elseif os.is "linux" then
			targetsuffix "_linux"
		elseif os.is "macosx" then
			targetsuffix "_osx"
		end
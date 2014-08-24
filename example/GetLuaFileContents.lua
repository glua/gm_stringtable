require "stringtable"

local client_lua_files = stringtable.Get "client_lua_files"

local function GetLuaFiles()
	local count = client_lua_files:Count()

	local ret = {}

	for i = 1, count do
		ret[i] = {
			Path = client_lua_files:GetString(i),
			CRC = client_lua_files:GetUserDataInt(i)
		}
	end

	return ret
end

local function GetLuaFileContents(crc)
	local fs = file.Open("cache/lua/" .. crc .. ".lua", "rb", "MOD")

	fs:Seek(4)

	local contents = util.Decompress(fs:Read(fs:Size() - 4))

	return contents:sub(1, -2) -- Trim trailing null
end


-- Prints the code of the 120th file the server has sent us

local target = GetLuaFiles()[120]

print("OPENING", target.Path)

print(GetLuaFileContents(target.CRC))
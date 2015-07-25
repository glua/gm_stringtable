
#include <string>
#include <sstream>
#include <stdio.h>

#include "GarrysMod/Lua/Interface.h"
#include "interface.h"
#include "networkstringtabledefs.h"

#include "vtable.h"

#include "proc.h"
#include "conv.h"

using namespace GarrysMod;

INetworkStringTableContainer *networktablecontainer;

typedef void *(__cdecl *luaL_checkudataFn)(lua_State *L, int narg, const char *tname);
luaL_checkudataFn luaL_checkudata;

//
// stringtable library functions
//
int stringtable_Get(lua_State *state) {
	INetworkStringTable *table = 0;

	switch (LUA->GetType(-1)) {
		case Lua::Type::STRING:
			table = networktablecontainer->FindTable(LUA->GetString(1));
			break;
		case Lua::Type::NUMBER:
			table = networktablecontainer->GetTable(int(LUA->GetNumber(1)) - 1); // -1 to make stringtables seem indexed from 1 for PUNY LUA USERS
			break;
	}

	if (!table)
		return 0;

	INetworkStringTable **ud = (INetworkStringTable **)LUA->NewUserdata(sizeof(INetworkStringTable*)); // Why is there no proper userdata binding Garry	?
		*ud = table;

		LUA->CreateMetaTableType("StringTable", Lua::Type::USERDATA);
	LUA->SetMetaTable(-2);

	return 1;
}

int stringtable_Count(lua_State *state) {
	LUA->PushNumber(networktablecontainer->GetNumTables());

	return 1;
}

//
// StringTable Meta-Methods
//
INetworkStringTable *GetStringTable(lua_State *state, int iStackPos) {
	return *(INetworkStringTable **)luaL_checkudata(state, 1, "StringTable");
}

int StringTable_tostring(lua_State *state) {
	INetworkStringTable *table = GetStringTable(state, 1);

	if (!table)
		return 0;

	std::ostringstream stream;
	stream << "StringTable [" << table->GetTableId() + 1 << "][" << table->GetTableName() << "]";

	std::string ret = stream.str();

	LUA->PushString(ret.c_str());

	return 1;
}

int StringTable_GetName(lua_State *state) {
	INetworkStringTable *table = GetStringTable(state, 1);

	if (!table)
		return 0;

	LUA->PushString(table->GetTableName());

	return 1;
}

int StringTable_GetString(lua_State *state) {
	INetworkStringTable *table = GetStringTable(state, 1);

	if (!table)
		return 0;

	int i = int(LUA->CheckNumber(2));

	LUA->PushString(table->GetString(i - 1));

	return 1;
}

int StringTable_GetUserData(lua_State *state) {
	INetworkStringTable *table = GetStringTable(state, 1);

	if (!table)
		return 0;

	int i = int(LUA->CheckNumber(2));
	int size;

	const void *data = table->GetStringUserData(i - 1, &size);

	if (!data)
		return 0;

	LUA->PushString((const char *)data, size);

	return 1;
}

int StringTable_GetUserDataInt(lua_State *state) {
	INetworkStringTable *table = GetStringTable(state, 1);

	if (!table)
		return 0;

	int i = int(LUA->CheckNumber(2));
	int size;

	const void *data = table->GetStringUserData(i - 1, &size);

	if (!data)
		return 0;

	LUA->PushNumber(*(unsigned int*)data);

	return 1;
}

int StringTable_Count(lua_State *state) {
	INetworkStringTable *table = GetStringTable(state, 1);

	if (!table)
		return 0;

	LUA->PushNumber(table->GetNumStrings());

	return 1;
}

int StringTable_Size(lua_State *state) {
	INetworkStringTable *table = GetStringTable(state, 1);

	if (!table)
		return 0;

	LUA->PushNumber(table->GetMaxStrings());

	return 1;
}


GMOD_MODULE_OPEN() {
	LUA->PushSpecial(Lua::SPECIAL_GLOB);

		LUA->GetField(-1, "CLIENT");
			bool CLIENT = LUA->GetBool();
		LUA->Pop();

		libsym_return code;
		if (CLIENT) {
			networktablecontainer = GetInterface<INetworkStringTableContainer *>("engine", INTERFACENAME_NETWORKSTRINGTABLECLIENT, &code);
		} else {
			networktablecontainer = GetInterface<INetworkStringTableContainer *>("engine", INTERFACENAME_NETWORKSTRINGTABLESERVER, &code);
		}

		if (!networktablecontainer) {
			char err[512];
			snprintf(err, 512, "(GM_STRINGTABLE) Could not find INetworkStringTableContainer, retcode = %i\n", code);
			LUA->ThrowError(err);
			return 0;
		}

		LUA->CreateTable();
			LUA->PushCFunction(stringtable_Get);
			LUA->SetField(-2, "Get");

			LUA->PushCFunction(stringtable_Count);
			LUA->SetField(-2, "Count");
		LUA->SetField(-2, "stringtable");

		LUA->CreateMetaTableType("StringTable", Lua::Type::USERDATA);
			LUA->Push(-1);
			LUA->SetField(-2, "__index");

			LUA->PushCFunction(StringTable_tostring);
			LUA->SetField(-2, "__tostring");

			LUA->PushString("StringTable");
			LUA->SetField(-2, "__type");

			LUA->PushCFunction(StringTable_GetName);
			LUA->SetField(-2, "GetName");

			LUA->PushCFunction(StringTable_GetString);
			LUA->SetField(-2, "GetString");

			LUA->PushCFunction(StringTable_GetUserData);
			LUA->SetField(-2, "GetUserData");

			LUA->PushCFunction(StringTable_GetUserDataInt);
			LUA->SetField(-2, "GetUserDataInt");

			LUA->PushCFunction(StringTable_Count);
			LUA->SetField(-2, "Count");

			LUA->PushCFunction(StringTable_Size);
			LUA->SetField(-2, "Size");
		LUA->Pop();

	LUA->Pop();


	libsym(&luaL_checkudata, "lua_shared", "luaL_checkudata");


	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}

#define LIB_NAME "defmc"
#define MODULE_NAME "defmc"

// include the Defold SDK
#include <dmsdk/sdk.h>

#include "defmc_private.h"

static int InitDefMc(lua_State* L)
{
	int top = lua_gettop(L);

	int result = DefMcPlatform_Init();

	if (result != 0)
		luaL_error(L, "Failed to init DefMc");

	assert(top == lua_gettop(L));
	return 0;
}

static int StartDefMc(lua_State* L)
{
	int top = lua_gettop(L);

    int result = DefMcPlatform_Start();
    if (result != 0)
        luaL_error(L, "Failed to start DefMc");

    assert(top == lua_gettop(L));
    return 0;
}

static int SampleAmplitude(lua_State* L)
{
    int top = lua_gettop(L);

	lua_pushnumber(L, 1337);
    // Assert that there is one item on the stack.
    assert(top + 1 == lua_gettop(L));

    // Return 1 item
    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"sample_amplitude", SampleAmplitude},
    {"init", InitDefMc},
    {"start", StartDefMc},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeDefMc(dmExtension::AppParams* params)
{
    printf("AppInitializeDefMc");
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeDefMc(dmExtension::Params* params)
{
    // Init Lua
	LuaInit(params->m_L);
    //InitDefMc(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeDefMc(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeDefMc(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}


// Defold SDK uses a macro for setting up extension entry points:
//
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)

DM_DECLARE_EXTENSION(defmc, LIB_NAME, AppInitializeDefMc, AppFinalizeDefMc, InitializeDefMc, 0, 0, FinalizeDefMc)
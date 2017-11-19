#define LIB_NAME "defmc"
#define MODULE_NAME "defmc"

// include the Defold SDK
#include <dmsdk/sdk.h>

#include "defmc_private.h"

// static int InitDefMc(lua_State* L)
// {
// 	int top = lua_gettop(L);

// 	int result = DefMcPlatform_Init();

// 	if (result != 0)
// 		luaL_error(L, "Failed to init DefMc");

// 	assert(top == lua_gettop(L));
// 	return 0;
// }

static int StartDefMc(lua_State* L)
{
    int top = lua_gettop(L);

    uint32_t sample_rate = 8000;
    uint32_t sample_delay = 75;
    if (top >= 1 && luaL_checknumber(L, 1)) {
        sample_rate = (uint32_t) lua_tonumber(L, 1);
        dmLogInfo("sample_rate: %i", sample_rate);
    }
    if (top >= 2 && luaL_checknumber(L, 2)) {
        sample_delay = (uint32_t) lua_tonumber(L, 2);
        dmLogInfo("sample_delay: %u", sample_delay);
    }
    int result = DefMcPlatform_Start(sample_rate, sample_delay);
    if (result != 0)
        luaL_error(L, "Failed to start DefMc");

    assert(top == lua_gettop(L));
    return 0;
}

static int StopDefMc(lua_State* L)
{
    int top = lua_gettop(L);

    int result = DefMcPlatform_Stop();
    if (result != 0)
        luaL_error(L, "Failed to stop DefMc");

    assert(top == lua_gettop(L));
    return 0;
}

static int SampleAmplitude(lua_State* L)
{
    int top = lua_gettop(L);

    float amp = 0.0f;
    int result = DefMcPlatform_SampleAmplitude(amp);
    if (result != 0)
        luaL_error(L, "Failed to sample amplitude DefMc");

	lua_pushnumber(L, amp);
    // Assert that there is one item on the stack.
    assert(top + 1 == lua_gettop(L));

    // Return 1 item
    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"sample_amplitude", SampleAmplitude},
    {"start", StartDefMc},
    {"stop", StopDefMc},
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
    //DefMcPlatform_Stop(params->m_L);
    return dmExtension::RESULT_OK;
}


// Defold SDK uses a macro for setting up extension entry points:
//
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)

DM_DECLARE_EXTENSION(defmc, LIB_NAME, AppInitializeDefMc, AppFinalizeDefMc, InitializeDefMc, 0, 0, FinalizeDefMc)
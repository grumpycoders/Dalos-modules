#include "LuaLoad.h"
#include "LuaTask.h"
#include "Buffer.h"
#include "Input.h"

using namespace Balau;

namespace {

class LuaLoad { };

enum LuaLoad_functions_t {
    LUALOAD
};

struct lua_functypes_t LuaLoad_functions[] = {
    { LUALOAD, "load", 1, 1, { BLUA_OBJECT | BLUA_STRING } },
    { -1, 0, 0, 0, 0 }
};

struct sLua_LuaLoad {
    static int LuaLoad_proceed_static(Lua & L, int n, int caller) {
        IO<Handle> h;
        enum {
            OPENING,
            CREATETASK,
            WAITTASK,
        } status;
        if (L.isobject()) {
            h = L.recast<Handle>();
            if (h.isA<Buffer>()) {
                L.load(h);
                return 0;
            }
            status = CREATETASK;
        } else {
            h = new Input(L.tostring());
            status = OPENING;
        }
        LuaExecFile * execFile = NULL;
        return L.yield(Future<int>([L, status, h, execFile]() mutable {
            switch (status) {
            case OPENING:
                h.asA<Input>()->open();
                status = CREATETASK;
            case CREATETASK:
                execFile = new LuaExecFile(h);
                status = WAITTASK;
                execFile->exec(L);
            case WAITTASK:
                execFile->throwError(); // will induce a memory leak in case of an actual error.
                delete execFile;
            }
            return 0;
        }));
    }
};

};

void registerLuaLoad(Lua & L) {
    CHECK_FUNCTIONS(LuaLoad);
    PUSH_FUNCTION(LuaLoad, LUALOAD);
}

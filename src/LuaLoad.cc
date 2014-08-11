#include <memory>

#include "LuaHandle.h"
#include "LuaLoad.h"
#include "LuaTask.h"
#include "Buffer.h"
#include "Input.h"

using namespace Balau;

namespace {

struct LuaLoad { void cleanup() { } };

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
            CLEANUP,
        } status;
        if (L.isobject()) {
            h = L.recast<LuaIO>()->getIO();
            if (h.isA<Buffer>()) {
                L.load(h);
                return 0;
            }
            status = CREATETASK;
        } else {
            h = new Input(L.tostring());
            status = OPENING;
        }
        std::shared_ptr<LuaExecFile> execFile(new LuaExecFile(h));
        return L.yield(Future<int>([L, status, h, execFile]() mutable {
            switch (status) {
            case OPENING:
                h.asA<Input>()->open();
                status = CREATETASK;
            case CREATETASK:
                execFile->exec(L);
                status = WAITTASK;
            case WAITTASK:
                execFile->throwError();
                status = CLEANUP;
            case CLEANUP:
                h->close();
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

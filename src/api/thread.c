#include "api.h"
#include "thread/thread.h"

static int threadRunner(void* data) {
  Thread* thread = (Thread*) data;

  lovrRetain(&thread->ref);
  mtx_lock(&thread->lock);
  thread->running = true;
  thread->error = NULL;
  mtx_unlock(&thread->lock);

  // Lua state
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  l_lovrInit(L);
  lua_setglobal(L, "lovr");

  if (luaL_loadbuffer(L, thread->body, strlen(thread->body), "thread") || lua_pcall(L, 0, 0, 0)) {
    thread->error = lua_tostring(L, -1);
    //lua_getglobal(lovr, "threaderror");
    lovrRelease(&thread->ref);
    return 1;
  }

  mtx_lock(&thread->lock);
  thread->running = false;
  mtx_unlock(&thread->lock);
  lovrRelease(&thread->ref);
  return 0;
}

int l_lovrThreadInit(lua_State* L) {
  lua_newtable(L);
  luaL_register(L, NULL, lovrThreadModule);
  luax_registertype(L, "Thread", lovrThread);
  return 1;
}

int l_lovrThreadNewThread(lua_State* L) {
  const char* body = luaL_checkstring(L, 1);
  Thread* thread = lovrThreadCreate(threadRunner, body);
  luax_pushtype(L, Thread, thread);
  lovrRelease(&thread->ref);
  return 1;
}

const luaL_Reg lovrThreadModule[] = {
  { "newThread", l_lovrThreadNewThread },
  { NULL, NULL }
};

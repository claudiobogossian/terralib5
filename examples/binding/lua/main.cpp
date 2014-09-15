// Lua
#include <lua.hpp>

// STL
#include <stdlib.h>
#include <stdio.h>


int
main(void)
{
  int status, result, i;
  double sum;
  lua_State *L;

  /*
   * All Lua contexts are held in this structure. We work with it almost
   * all the time.
  */
  L = luaL_newstate();

  luaL_openlibs(L); /* Load Lua libraries */

  /* Load the file containing the script we are going to run */
  status = luaL_loadfile(L, "example.lua");
  if (status)
  {
    /* If something went wrong, error message is at the top of */
    /* the stack */
    fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
    exit(1);
  }

  /* By what name is the script going to reference our table? */
  lua_setglobal(L, "foo");

  /* Ask Lua to run our little script */
  result = lua_pcall(L, 0, LUA_MULTRET, 0);
  if (result)
  {
    fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
    exit(1);
  }

  lua_close(L);   /* Cya, Lua */

  return 0;
}

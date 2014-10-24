/*
 * Defining a typemap for std::auto_ptr
 */

%define AUTO_PTR_TYPEMAPS(TYPE...)

%typemap(out) (std::auto_ptr< TYPE >) %{
  SWIG_NewPointerObj(L, (void *) $result.release(), $descriptor(TYPE *), 1); 
  SWIG_arg++;
%}

%template () std::auto_ptr< TYPE >;

%enddef


/*
 * Defining a typemap for std::map< std::string, std::string >
 */

%typemap(in) std::map< std::string, std::string >, const std::map< std::string, std::string > & (std::map< std::string, std::string > aux)
{
  lua_pushnil(L);
  
  while(lua_next(L, -2) != 0)
  {
    if(lua_isstring(L, -1) && lua_isstring(L, -2))
    {
      std::string key(lua_tostring(L, -2)), 
                  value(lua_tostring(L, -1)); 
      
      aux[key] = value;
    }
    
    lua_pop(L, 1);
  }
  
  $1 = &aux;
}

namespace std 
{
  template<class T> class auto_ptr { };
}
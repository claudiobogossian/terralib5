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

/*
 * Defining a typemap for std::vector< std::string >
 */

%typemap(in) std::vector< std::string >, const std::vector< std::string > & (std::vector< std::string > aux)
{
  lua_pushnil(L);
  
  while(lua_next(L, -2) != 0)
  {
    if(lua_isstring(L, -1))
    {
      std::string value(lua_tostring(L, -1)); 
      aux.push_back(value);
    }
    
    lua_pop(L, 1);
  }
  
  $1 = &aux;
}

%typemap(out) std::vector<std::string>
{
  size_t s = $result.size();
  
  lua_createtable(L, 0, 1);
  
  for(size_t i = 0; i < s; i++)
    lua_pushstring(L, $result[i].c_str());
  
  return (int) s;
}

%template (StrVector) std::vector <std::string>;

namespace std 
{
  template<class T> class auto_ptr { };
}
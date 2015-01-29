/*
 * Defining a typemap for std::auto_ptr
 */

%define AUTO_PTR_TYPEMAPS(TYPE...)

%typemap(out) (std::auto_ptr< TYPE >) %{
%}

%template () std::auto_ptr< TYPE >;

%enddef

namespace std 
{
  template<class T> class auto_ptr { };
}
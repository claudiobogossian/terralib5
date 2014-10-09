%include exception.i

/*
 * Defining a type map for std::auto_ptr
 */

%define AUTO_PTR_TYPEMAPS(TYPE...)

%typemap(out) (std::auto_ptr< TYPE >) %{
  SWIG_NewPointerObj(L, (void *) $result.release(), $descriptor(TYPE *), 1); 
  SWIG_arg++;
%}

%template () std::auto_ptr< TYPE >;

%enddef

namespace std 
{
  template<class T> class auto_ptr { };
}

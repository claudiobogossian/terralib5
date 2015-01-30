/*
 * Defining a typemap for std::auto_ptr
 */

%define AUTO_PTR_TYPEMAPS(TYPE...)

%typemap(out) (std::auto_ptr< TYPE >) %{
  r_ans = SWIG_R_NewPointerObj(SWIG_as_voidptr(result.release()), $descriptor(TYPE *), R_SWIG_EXTERNAL |  0 );
%}

%template () std::auto_ptr< TYPE >;

%enddef

namespace std 
{
  template<class T> class auto_ptr { };
}
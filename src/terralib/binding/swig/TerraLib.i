%module terralib

#define TEGEOMEXPORT

%include <attribute.i>
%include <exception.i>
%include <std_except.i>
%include <std_string.i>
%include <std_vector.i>
%include <windows.i>

#ifdef SWIGR
  #include R/R.i
#endif

%{
// TerraLib
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
%}

%include "../../geometry/Coord2D.h"
%include "../../geometry/Envelope.h"

/* 
* File: TerraLibLUA.i 
* 
*/ 
 
%module terralib_mod_binding_lua 

#define TECOMMONEXPORT

%include "terralib/common/Singleton.h"

namespace te {
  namespace common {
    %template(TeSingleton) Singleton < TerraLib >;
  }
}

typedef te::common::Singleton< TerraLib > TeSingleton;

%include stl.i
%include typemaps.i

%apply unsigned int *INOUT {std::size_t& size}
%apply const string& { const std::string& }

%include "terralib/common/Enums.h"  
%include "terralib/common/TerraLib.h"

%{  
#include <terralib/Config.h>
#include <terralib/common/Enums.h>  
#include <terralib/common/TerraLib.h>
%}

/* Include Geometry module in bind. */   
%include common/Geometry.i

/* 
* File: TerraLibR.i 
* 
*/ 
 
%module terralib_mod_binding_r 

#define TECOMMONEXPORT

%include "terralib/common/Singleton.h"

%include stl.i
%include typemaps.i

%include r/typemaps.i

namespace te 
{
  namespace common 
  {
    %template(TeSingleton) Singleton < TerraLib >;
  }
}

typedef te::common::Singleton< TerraLib > TeSingleton;

%apply unsigned int *INOUT {std::size_t& size}
%apply const string& { const std::string& }
%apply te::common::Exception { Exception }

%ignore as_long_string;
%ignore as_short_string;
%ignore get(const std::pair<int, int>& typeMap);

%feature("compactdefaultargs") te::rst::RasterSummaryManager::get;

%{  
#include <terralib/Config.h>
#include <terralib/common/Enums.h>  
#include <terralib/common/TerraLib.h>
#include <terralib/common/Exception.h>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

/* Defining a function for generating randomic ids. */
static std::string GetRandomicId()
{
  //Generates a randon id to the data source
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  return boost::uuids::to_string(u);
}

%}

%include "terralib/common/Enums.h"  
%include "terralib/common/TerraLib.h"


/* Include Common module to the bind. */
%include common/Common.i

/* Include SRS module to the bind. */
%include common/SRS.i

/* Include Data Type module to the bind. */
%include common/DataType.i

/* Include Geometry module to the bind. */   
%include common/Geometry.i

/* Include Raster module to the bind. */   
%include common/Raster.i

/* Include Data Access module to the bind. */
%include common/DataAccess.i 

/* Include Plugin module to the bind. */
%include common/Plugin.i 

/* Include Spatial Temporal module to the bind. */
%include common/ST.i

// Wrap function
std::string GetRandomicId();

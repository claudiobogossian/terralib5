/*
 * File: Maptools.i
 *
 * Map Tools module
 */

#define TEMAPEXPORT

namespace te {
  namespace common {
    %template(LayerSerializeSingleton) Singleton < te::map::serialize::Layer >;
  }
}

typedef te::common::Singleton< te::map::serialize::Layer > LayerSerializeSingleton;

%{
#include "terralib/maptools/Config.h"
#include "terralib/maptools/Enums.h"
#include "terralib/maptools/AbstractLayer.h"
#include "terralib/maptools/DataSetLayer.h"
#include "terralib/maptools/serialization/xml/Layer.h"
%}

%nspace te::map::AbstractLayer;
%nspace te::map::DataSetLayer;
%nspace te::map::serialize::Layer;

%include "terralib/maptools/Enums.h"
%include "terralib/maptools/AbstractLayer.h"
%include "terralib/maptools/DataSetLayer.h"
%include "terralib/maptools/serialization/xml/Layer.h"
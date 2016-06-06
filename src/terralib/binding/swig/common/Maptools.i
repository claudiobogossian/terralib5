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

%ignore te::map::AbstractLayer::setId;
%ignore te::map::AbstractLayer::setTitle;
%ignore te::map::AbstractLayer::getAncestors;
%ignore te::map::AbstractLayer::hasVisibilityChanged;
%ignore te::map::AbstractLayer::setVisibilityAsChanged;
%ignore te::map::AbstractLayer::updateVisibilityOfAncestors;
%ignore te::map::AbstractLayer::updateVisibility;
%ignore te::map::AbstractLayer::setExtent;
%ignore te::map::AbstractLayer::setSRID;
%ignore te::map::AbstractLayer::select;
%ignore te::map::AbstractLayer::getSelected;
%ignore te::map::AbstractLayer::deselect;
%ignore te::map::AbstractLayer::clearSelected;
%ignore te::map::AbstractLayer::getStyle;
%ignore te::map::AbstractLayer::setStyle;
%ignore te::map::AbstractLayer::getGrouping;
%ignore te::map::AbstractLayer::setGrouping;
%ignore te::map::AbstractLayer::getChart;
%ignore te::map::AbstractLayer::setChart;
%ignore te::map::AbstractLayer::getSchema;
%ignore te::map::AbstractLayer::getData;
%ignore te::map::AbstractLayer::getType;
%ignore te::map::AbstractLayer::draw;
%ignore te::map::AbstractLayer::getGeometries;
%ignore te::map::AbstractLayer::setRenderer;

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
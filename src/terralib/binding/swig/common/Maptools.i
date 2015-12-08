/*
 * File: ST.i
 *
 * Map Tools module
 */

#define TEMAPEXPORT

%{
#include "terralib/maptools/Config.h"
#include "terralib/maptools/Enums.h"
#include "terralib/maptools/AbstractLayer.h"
#include "terralib/maptools/DataSetLayer.h"
%}

%nspace te::map::AbstractLayer;
%nspace te::map::DataSetLayer;

%include "terralib/maptools/Enums.h"
%include "terralib/maptools/AbstractLayer.h"
%include "terralib/maptools/DataSetLayer.h"
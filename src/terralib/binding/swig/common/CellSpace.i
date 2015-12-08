/*
 * File: ST.i
 *
 * Cellular Space module
 */

#define TECELLSPACEEXPORT

%{
#include "terralib/cellspace/Module.h"
#include "terralib/cellspace/CellSpaceOperations.h"
#include "terralib/maptools/DataSetLayer.h"
%}

%nspace te::cellspace::CellularSpacesOperations;
%nspace te::cellspace::Module;

%include "terralib/cellspace/Module.h"
%include "terralib/cellspace/CellSpaceOperations.h"
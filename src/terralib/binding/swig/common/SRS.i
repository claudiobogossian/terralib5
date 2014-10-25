/*
 * File: SRS.i
 *
 * Spatial Reference System
 */
 
#define TESRSEXPORT

// This methods were not exported for the bind.
%ignore te::srs::SpatialReferenceSystemManager::getIterators;
%ignore te::srs::SpatialReferenceSystemManager::getSpatialReferenceSystem;

%nspace te::srs::SpatialReferenceSystem;
%nspace te::srs::SpatialReferenceSystemManager;

%{
#include "terralib/srs/SpatialReferenceSystem.h"
#include "terralib/srs/SpatialReferenceSystemManager.h"
%}

%include "terralib/srs/SpatialReferenceSystem.h"
%include "terralib/srs/SpatialReferenceSystemManager.h"

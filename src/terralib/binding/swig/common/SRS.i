/*
 * File: SRS.i
 *
 * Spatial Reference System
 */
 
#define TESRSEXPORT

// This methods were not exported for the bind.
%ignore te::srs::SpatialReferenceSystemManager::getIterators;
%ignore te::srs::SpatialReferenceSystemManager::getSpatialReferenceSystem;

//Python doesn't work with namespaces
#ifndef SWIGPYTHON

%nspace te::srs::SpatialReferenceSystem;
%nspace te::srs::SpatialReferenceSystemManager;

#endif // SWIGPYTHON

%{
#include "terralib/srs/SpatialReferenceSystem.h"
#include "terralib/srs/SpatialReferenceSystemManager.h"
%}

%include "terralib/srs/SpatialReferenceSystem.h"
%include "terralib/srs/SpatialReferenceSystemManager.h"

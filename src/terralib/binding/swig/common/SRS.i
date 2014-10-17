/*
 * File: SRS.i
 *
 * Spatial Reference System
 */
 
 #define TESRSEXPORT
 
 %ignore te::srs::SpatialReferenceSystemManager::getIterators;
 %ignore te::srs::SpatialReferenceSystemManager::getSpatialReferenceSystem;
 
 %{
 #include "terralib/srs/SpatialReferenceSystem.h"
 #include "terralib/srs/SpatialReferenceSystemManager.h"
 %}
 
 %include "terralib/srs/SpatialReferenceSystem.h"
 %include "terralib/srs/SpatialReferenceSystemManager.h"

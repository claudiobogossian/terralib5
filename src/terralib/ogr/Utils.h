/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/ogr/Utils.h
   
  \brief Utility functions for OGR support.
*/

#ifndef __TERRALIB_OGR_INTERNAL_UTILS_H
#define __TERRALIB_OGR_INTERNAL_UTILS_H

// TerraLib
#include "Config.h"
#include "../srs/Config.h"

// OGR
#include <ogr_core.h>

// Forward declarations
class OGRGeometry;
class OGREnvelope;
class OGRSpatialReference;
class OGRFeatureDefn;
class OGRFieldDefn;

namespace te
{
  namespace da
  {
    class DataSetType;
  }

  namespace gm
  { 
    class Envelope;
    class Geometry; 
  } 
}

namespace te
{
  namespace ogr
  {
    /*!
      \brief It converts the OGR Geometry to TerraLib Geometry.

      \param ogrGeom A valid OGR Geometry.

      \return A valid TerraLib Geometry.

      \exception Exception It throws an exception if the OGR geometry can not be converted.

      \note It uses the WKB to create the TerraLib Geometry  
      \note The caller of this function will take the ownership of the returned TerraLib Geometry.
    */
    TEOGREXPORT te::gm::Geometry* Convert2TerraLib(OGRGeometry* ogrGeom);

    /*!
      \brief It converts the TerraLib Geometry to OGR Geometry.

      \param teGeom A valid TerraLib Geometry.

      \return A valid TerraLib geometry.

      \exception Exception It throws an exception if the TerraLib geometry can not be converted.

      \note It uses the WKB to create the TerraLib Geometry  
      \note The caller of this function will take the ownership of the returned OGR Geometry.
    */
    TEOGREXPORT OGRGeometry* Convert2OGR(const te::gm::Geometry* teGeom);

    /*!
      \brief It converts the OGR Envelope to TerraLib Envelope.

      \param env A valid OGR Envelope.

      \return A valid TerraLib Envelope.

      \note The caller of this function will take the ownership of the returned TerraLib Envelope.
    */
    TEOGREXPORT te::gm::Envelope* Convert2TerraLib(const OGREnvelope* env);

    /*!
      \brief It converts the TerraLib Envelope to OGR Envelope.

      \param env A valid TerraLib Envelope.

      \return A valid OGR Envelope.

      \note The caller of this function will take the ownership of the returned OGR Envelope.
    */
    TEOGREXPORT OGREnvelope* Convert2OGR(const te::gm::Envelope* env);

    /*!
      \brief It converts the OGR Projection to TerraLib Projection.

      \param osrs A valid OGR Projection.

      \return An SRS id recognized by TerraLib or an indication of unknown SRS (TE_UNKNOWN_SRS)
    */
    TEOGREXPORT int Convert2TerraLibProjection(OGRSpatialReference* osrs);

    /*!
      \brief It converts the TerraLib Projection to OGR Projection.

      \param srid A valid TerraLib Projection id.

      \return A valid OGR Spatial Reference System.

      \exception Exception It throws an exception if the SRS can not be imported using OGR.

      \note The caller of this function will take the ownership of the returned OGR Spatial Reference System.
    */
    TEOGREXPORT OGRSpatialReference* Convert2OGRProjection(int srid);
    
    /*!
     \brief It inserts a OGR Feature Definition to TerraLib DataSet Type
     
     \param featDef A valid OGR Feature Definition.
     \param dt      Pointer to a TerraLib dataset type previously created. Do not pass NULL.
     \param srs    Optional parameter to set the SRS of geometry fields.
     
     \exception Exception It throws an exception if the feature definition can not be converted.
     
     \note It uses the method that converts a OGR Field Definition to TerraLib Property Type. 
     \note It uses the method that converts a OGR Geometry Type to TerraLib Geometry Type.
     */
    TEOGREXPORT void Convert2TerraLib(OGRFeatureDefn* featDef, te::da::DataSetType* dt, int srs=TE_UNKNOWN_SRS);

    /*!
      \brief It converts the OGR Feature Definition to TerraLib DataSet Type

      \param featDef A valid OGR Feature Definition.
      \param srs    Optional parameter to set the SRS of geometry fields.

      \return A valid TerraLib DataSet Type.

      \exception Exception It throws an exception if the feature definition can not be converted.

      \note It uses the method that converts a OGR Field Definition to TerraLib Property Type. 
      \note It uses the method that converts a OGR Geometry Type to TerraLib Geometry Type.
      \note The caller of this function will take the ownership of the returned TerraLib DataSet Type.
    */
    TEOGREXPORT te::da::DataSetType* Convert2TerraLib(OGRFeatureDefn* featDef, int srs=TE_UNKNOWN_SRS);

    /*!
      \brief It converts the TerraLib DataSet Type to OGR Feature Definition

      \param dt A valid TerraLib DataSet Type.

      \return A valid OGR Feature Definition.

      \exception Exception It throws an exception if the data set type can not be converted.

       \note It uses the method that converts a TerraLib Property Type to OGR Field Definition. 
       \note The caller of this function will take the ownership of the returned OGR Feature Definition.
    */
    TEOGREXPORT OGRFeatureDefn* Convert2OGR(te::da::DataSetType* dt);

    /*!
      \brief It converts the OGR Field Definition to TerraLib Property Type

      \param fieldDef A valid OGR Field Definition.

      \return A valid TerraLib Property Type.

      \exception Exception It throws an exception if the data type is not supported by TerraLib.

      \note The caller of this function will take the ownership of the returned TerraLib Property Type.
    */
    TEOGREXPORT te::dt::Property* Convert2TerraLib(OGRFieldDefn* fieldDef);
    
    /*!
      \brief It converts the TerraLib Property Type to OGR Field Definition

      \param pt A valid TerraLib Property Type.

      \return A valid OGR Field Definition.

      \exception Exception It throws an exception if the data type is not supported by OGR.
    */
    TEOGREXPORT OGRFieldDefn* Convert2OGR(te::dt::Property* p);

    /*!
      \brief It converts the OGR Geometry Type element to TerraLib Geometry Type

      \param ogrGeomType A valid OGR Geometry Type element.

      \return A valid TerraLib Geometry Type
    */
    TEOGREXPORT te::gm::GeomType Convert2TerraLib(OGRwkbGeometryType ogrGeomType);

    /*!
      \brief It converts the TerraLib Geometry Type element to OGR Geometry Type

      \param geomType A valid TerraLib Geometry Type.

      \return A valid OGR Geometry Type element

      \exception Exception It throws an exception if the geometry type is not supported by OGR.
    */
    TEOGREXPORT OGRwkbGeometryType Convert2OGR(te::gm::GeomType geomType);

    /*!
      \brief It tries extract the driver name used by OGR Library based on the given path.

      \param path The path that will be consulted.

      \return The driver name used by OGR Library or an empty string if not possible extract the driver name.
    */
    std::string GetDriverName(const std::string& path);
    
    /*!
     \brief It returns the list of OGR Drivers available.
     
     \param filterCreate Set TRUE to indicate to retrieve only the drivers with the capacity to create datasources.
     
     \return A list of OGR Drivers.
     */
    TEOGREXPORT std::vector<std::string> GetOGRDrivers(bool filterCreate = false);
    
    TEOGREXPORT std::string GetOGRConnectionInfo(const std::map<std::string, std::string>& connInfo);

    TEOGREXPORT std::string RemoveSpatialSql(const std::string& sql);

  } // end namespace ogr
}   // end namespace te

#endif  // __TERRALIB_OGR_INTERNAL_UTILS_H

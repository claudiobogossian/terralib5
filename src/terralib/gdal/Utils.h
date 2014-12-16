/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.
 
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
 \file terralib/gdal/Utils.h
 
 \brief Utilitary functions to access GDAL and match some of its concepts to TerraLib concepts.
 */

#ifndef __TERRALIB_GDAL_INTERNAL_UTILS_H
#define __TERRALIB_GDAL_INTERNAL_UTILS_H

// TerraLib
#include "../raster/BandProperty.h"
#include "../raster/Enums.h"
#include "../raster/Raster.h"
#include "Config.h"
#include "Band.h"
#include "Raster.h"

// GDAL
#include <gdal_priv.h>

// STL
#include <map>
#include <string>
#include <vector>
#include <set>

#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>

namespace te
{
  namespace gm {class Envelope; class Geometry;}
  
  namespace rst {class Grid; class RasterProperty; }
  
  namespace gdal
  {
    /*!
     \brief GDAL driver metadata.
     */    
    struct DriverMetadata
    {
      std::string m_driverName;  //!< Driver name (driver description).
      std::string m_extension; //!< File extension (DMD_EXTENSION).
      std::string m_longName; //!< File long name (DMD_LONGNAME).
      bool m_subDatasetsSupport; //!< true if the driver has support for sub-datasets (DMD_SUBDATASETS).
    };
    
    /*!
     \brief It translates a GDAL DataType to a TerraLib DataType.
     */
    inline int GetTeDataType(GDALDataType gt)
    {
      switch(gt)
      {
        case GDT_Byte : return te::dt::UCHAR_TYPE;
        case GDT_UInt16 : return te::dt::UINT16_TYPE;
        case GDT_Int16 : return te::dt::INT16_TYPE;
        case GDT_UInt32 : return te::dt::UINT32_TYPE;
        case GDT_Int32 : return te::dt::INT32_TYPE;
        case GDT_Float32 : return te::dt::FLOAT_TYPE;
        case GDT_Float64 : return te::dt::DOUBLE_TYPE;
          
        case GDT_CInt16 : return te::dt::CINT16_TYPE;
        case GDT_CInt32 : return te::dt::CINT32_TYPE;
        case GDT_CFloat32 : return te::dt::CFLOAT_TYPE;
        case GDT_CFloat64 : return te::dt::CDOUBLE_TYPE;
          
        default : return te::dt::UNKNOWN_TYPE;
      }     
    }
    
    /*!
     \brief It translates a TerraLib DataType to a GDAL DataType.
     */
    inline GDALDataType GetGDALDataType(int tet)
    {
      switch(tet)
      {
        case te::dt::UCHAR_TYPE : return GDT_Byte;
        case te::dt::CHAR_TYPE : return GDT_Byte;
        case te::dt::UINT16_TYPE : return GDT_UInt16;
        case te::dt::INT16_TYPE : return GDT_Int16;
        case te::dt::UINT32_TYPE : return GDT_UInt32;
        case te::dt::INT32_TYPE : return GDT_Int32;
        case te::dt::FLOAT_TYPE : return GDT_Float32;
        case te::dt::DOUBLE_TYPE : return GDT_Float64;
          
        case te::dt::CINT16_TYPE : return GDT_CInt16;
        case te::dt::CINT32_TYPE : return GDT_CInt32;
        case te::dt::CFLOAT_TYPE : return GDT_CFloat32;
        case te::dt::CDOUBLE_TYPE : return GDT_CFloat64;
          
        default : return GDT_Unknown;
      }
    }
    
    /*!
     \brief It translates a GDAL ColorInterpretation to a TerraLib ColorInterpretation.
     */
    inline te::rst::ColorInterp GetTeColorInterpretation(GDALColorInterp gci)
    {
      switch(gci)
      {
        case GCI_GrayIndex : return te::rst::GrayIdxCInt;
        case GCI_PaletteIndex : return te::rst::PaletteIdxCInt;
        case GCI_RedBand : return te::rst::RedCInt;
        case GCI_GreenBand : return te::rst::GreenCInt;
        case GCI_BlueBand : return te::rst::BlueCInt;
        case GCI_AlphaBand : return te::rst::AlphaCInt;
        case GCI_HueBand : return te::rst::HueCInt;
        case GCI_SaturationBand : return te::rst::SatCInt;
        case GCI_LightnessBand : return te::rst::LigCInt;
        case GCI_CyanBand : return te::rst::CyanCInt;
        case GCI_MagentaBand : return te::rst::MagentaCInt;
        case GCI_YellowBand : return te::rst::YellowCInt;
        case GCI_BlackBand : return te::rst::KeyCInt;
        case GCI_YCbCr_YBand : return te::rst::YCInt;
        case GCI_YCbCr_CbBand : return te::rst::CbCInt;
        case GCI_YCbCr_CrBand : return te::rst::CrCInt;
        default : return te::rst::UndefCInt;
      }
    }
    
    /*!
     \brief It translates a TerraLib ColorInterpretation to a GDAL ColorInterpretation.
     */
    inline GDALColorInterp GetGDALColorInterpretation(te::rst::ColorInterp ci)
    {
      switch(ci)
      {
        case te::rst::GrayIdxCInt : return GCI_GrayIndex;
        case te::rst::PaletteIdxCInt : return GCI_PaletteIndex;
        case te::rst::RedCInt : return GCI_RedBand;
        case te::rst::GreenCInt : return GCI_GreenBand;
        case te::rst::BlueCInt : return GCI_BlueBand;
        case te::rst::AlphaCInt : return GCI_AlphaBand;
        case te::rst::HueCInt : return GCI_HueBand;
        case te::rst::SatCInt : return GCI_SaturationBand;
        case te::rst::LigCInt : return GCI_LightnessBand;
        case te::rst::CyanCInt : return GCI_CyanBand;
        case te::rst::MagentaCInt : return GCI_MagentaBand;
        case te::rst::YellowCInt : return GCI_YellowBand;
        case te::rst::KeyCInt : return GCI_BlackBand;
        case te::rst::YCInt : return GCI_YCbCr_YBand;
        case te::rst::CbCInt : return GCI_YCbCr_CbBand;
        case te::rst::CrCInt : return  GCI_YCbCr_CrBand;
        default : return GCI_Undefined;
      }
    }
    
    /*!
     \brief It translates a GDAL Pallete Interpretation to a TerraLib Pallete Interpretation.
     */
    inline te::rst::PaletteInterpretation GetTePaletteInterpretation(GDALPaletteInterp gpi)
    {
      switch (gpi)
      {
        case GPI_Gray : return te::rst::GrayPalInt;
        case GPI_RGB : return te::rst::RGBPalInt;
        case GPI_CMYK : return te::rst::CMYKPalInt;
        case GPI_HLS : return te::rst::HSLPalInt;
        default : return te::rst::UndefPalInt;
      }
    }
    /*!
     \brief Gets the grid definition from a GDAL dataset.
     \param gds A pointer to a GDAL dataset.
     \return A pointer to the grid definition from a GDAL dataset. Caller takes its ownership.
     */
    TEGDALEXPORT te::rst::Grid* GetGrid(GDALDataset* gds);
    
    /*!
     \brief Gets the list of bands definition from a GDAL dataset.
     \param gds A pointer to a GDAL dataset.
     \param bprops A reference to a vector to be filled with the bands description extracted from a dataset.
     \note The caller of this method must take the ownership of the returned properties.
     */
    TEGDALEXPORT void GetBandProperties(GDALDataset* gds, std::vector<te::rst::BandProperty*>& bprops);
    
    /*!
     \brief Gets the properties of a single band from a GDAL dataset.
     
     \param gband  A pointer to a GDAL Raster Band.
     
     \param bandIndex The band index (starting from 0).
     
     \return A band property.
     
     \note The caller of this method must take the ownership of the returned properties.
     */
    te::rst::BandProperty* GetBandProperty(GDALRasterBand* gband, const unsigned int bandIndex );
    
    /*!
     \brief Gets the list of bands from a GDAL dataset.
     
     \param rst    A pointer to the raster.
     \param bands  A reference to a vector to be filled with the bands extracted from a dataset.
     \note The caller of this method must take the ownership of the returned properties.
     */
    void GetBands(te::gdal::Raster* rst, std::vector<te::gdal::Band*>& bands);
    
    /*!
     \brief Gets the complete description from a GDAL dataset.
     \param strAccessInfo A a string to be used by GDAL to access the raster.
     \return A pointer to the raster description from a GDAL dataset. Caller takes its ownership.
     */
    te::rst::RasterProperty* GetRasterProperty(std::string strAccessInfo);
    
    /*!
     \brief Creates a raster data using GDAL.
     
     \param g          Raster grid info.
     \param bands      Band info.
     \param optParams  A vector of optional parameters that are valid only for some data formats.
     
     \return A pointer to a GDALDataset if it succeeds or a NULL pointer otherwise. Caller is responsible for closing it.
     
     \exception Exception It throws an exception if the raster can not be created.
     */
    GDALDataset* CreateRaster(te::rst::Grid* g, const std::vector<te::rst::BandProperty*>& bands, const std::map<std::string, std::string>& optParams);
    
    /*!
     \brief Creates a raster data using GDAL.
     \param name       The name of the dataset to create. UTF-8 encoded.
     \param g          Raster grid info.
     \param bands      Band info.
     \param optParams  A vector of optional parameters that are valid only for some data formats.
     
     \return A pointer to a GDALDataset if it succeeds or a NULL pointer otherwise. Caller is responsible for closing it.
     
     \exception Exception It throws an exception if the raster can not be created.
     */
    GDALDataset* CreateRaster(const std::string& name, te::rst::Grid* g, const std::vector<te::rst::BandProperty*>& bands, const std::map<std::string, std::string>& optParams);
    
    /*!
     \brief Gets the extent of a raster data decoded by GDAL.
     \param strAccessInfo A a string to be used by GDAL to access the raster.
     \return A pointer to raster extent. Caller takes its ownership.
     */ 
    te::gm::Envelope* GetExtent(std::string strAccessInfo);
    
    /*! 
     \brief Get a handle to a raster file.
     \param filename   File name (path included).
     \param policy     The access permission requested.
     \return A pointer to GDAL dataset if it succeeds or a NULL pointer otherwise.
     */
    GDALDataset* GetRasterHandle(std::string strAccessInfo, te::common::AccessPolicy policy = te::common::RAccess);
    
    /*! 
     \brief Returns a PostGIS connection string from the set connection information.
     The connection string is to be used as a dataset name in GDAL data model. See also http://trac.osgeo.org/gdal/wiki/frmts_wtkraster.html.
     \param connInfo   The connection parameters.
     \return Returns a PostGIS connection string from the set connection information.
     */
    std::string MakePGConnectionStr(const std::map<std::string, std::string>& dsInfo);
    
    /*!
     \brief Reprojects a raster to another SRS.
     \param rin  The input raster file. Do not pass a null pointer.
     \param rout The new output raster. Do not pass a null pointer.
     \return true If the reprojection was done or false otherwise.
     */
    bool ReprojectRaster(te::rst::Raster const * const rin, te::rst::Raster* rout);
    
    /*!
     \brief It returns the GDAL driver name associated to a data source name.
     
     \param dsName the name of the file that represents the data source.
     
     \return the GDAL driver name, or its identifier if succeeds and a null string otherwise.
     */
    std::string GetDriverName(const std::string& dsName);
    
    /*!
     \brief It returns a GDAL connection string from the given map.
     
     \param connInfo An associative conteiner with data source connection info.
     
     \return a GDAL connection string from the given map.
     
     \exception Exception It throws an exception if no connection info exists in the input map.
     */
    std::string GetGDALConnectionInfo(const std::map<std::string, std::string>& connInfo);
    
    /*!
     \brief It returns the Sub DataSet name from the given name or the same name.
     
     \param name The Full SubDataSet string name.
     
     \param driverName The driver name.
     
     \return The Sub DataSet name from the given name.
     */
    std::string GetSubDataSetName(const std::string& name, const std::string& driverName);
    
    /*!
     \brief It returns true if GDAL recognizes the given SRS id.
     
     \param srid The SRS identifier.
     
     \return true if GDAL recognizes the given SRS id or false otherwise.
     */
    bool RecognizesSRID(unsigned int srid);
    
    /*!
     \brief Vectorizes a given raster band, using GDALPolygonize function.
     
     \param band         The band to vectorize.
     \param geometries   A reference to a vector of geometries. Will be filled with geometries found in band.
     */
    TEGDALEXPORT void Vectorize(GDALRasterBand* band, std::vector<te::gm::Geometry*>& geometries);
    
    /*!
     \brief Rasterizes a given vector of geometries, using GDALRasterizeGeometries function.
     
     \param geometries   A vector of geometries to be rasterized.
     \param outraster    A reference to the GDAL dataset where the rasterized geometries will be drawn.
     */
    TEGDALEXPORT void Rasterize(std::vector<te::gm::Geometry*> geometries, GDALDataset* outraster);

    /*!
     \brief Returns true if the given URI is related to a sub-dataset.
     \param uri The given URI.
     \return true if the given URI is related to a sub-dataset.
     */
    bool IsSubDataSet( const std::string& uri );
    
    /*!
     \brief It returns the parent dataset name from a Sub DataSet name.
     \param subDataSetName The Full SubDataSet string name.
     \return the parent dataset name from a Sub DataSet name.
     \note If the given name does not refers to a sub-dataset it will be returned.
     */
    std::string GetParentDataSetName(const std::string& subDataSetName);
    
    /*!
     \brief Returns a reference to a static mutex initialized when this module is initialized.
     \return Returns a reference to a static mutex initialized when this module is initialized.
     */
    TEGDALEXPORT boost::mutex& getStaticMutex();   
    
    /*!
     \brief Returns metadata from all registered GDAL drivers (key: driver name).
     \return Metadata from all registered GDAL drivers (key: driver name).
     */    
    const std::map< std::string, DriverMetadata >& GetGDALDriversMetadata();
    
    /*!
     \brief Returns a map all GDAL supported Upper-case extensions to their respective driver names.
     \return Returns a map all GDAL supported Upper-case extensions to their respective driver names.
     */    
    const std::multimap< std::string, std::string >& GetGDALDriversUCaseExt2DriversMap();    
        
  } // end namespace gdal
} // end namespace te
#endif

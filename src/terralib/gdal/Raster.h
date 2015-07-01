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
 \file terralib/gdal/Raster.h

 \brief This is a class that represents a GDAL Raster.
 */

#ifndef __TERRALIB_GDAL_INTERNAL_RASTER_H
#define __TERRALIB_GDAL_INTERNAL_RASTER_H

//TerraLib
#include "../raster/Raster.h"
#include "Band.h"
#include "Config.h"
#include "DataSetUseCounter.h"

#include <string>
#include <memory>

#include <gdal_priv.h>

// Forward declaration
class GDALDataset;
class GDALRasterBand;

namespace te
{
  namespace gdal
  {
    // Forward declaration
    class Band;

    /*!
     \class Raster

     \brief This class represents Raster data.

     This class is a concrete implementation of a Raster using the GDAL library to access
     the data.

     \sa te::rst::Raster
     */
    class TEGDALEXPORT Raster : public te::rst::Raster
    {
    public:

      Raster();

      /*!
       \brief Constructor.

       \param rinfo  The information needed to open the raster.
       \param p      Access Policy.
       */
      Raster(const std::string& rinfo, te::common::AccessPolicy p = te::common::RAccess);

      /*!
       \brief Constructor.

       \param grid        The grid definition. The Raster will take its ownership.
       \param bprops      A vector of band properties, one for each band. The Raster will take its ownership.
       \param optparams   Extra information to create the raster. See GDAL documentation for more information.
       Parameters include NBANDS, BANDSTYPE, NCOLS, NROWS, RESX, RESY, SRID, ULX, ULY.
       \param p           Access Policy.
       */
      Raster(te::rst::Grid* grid, const std::vector<te::rst::BandProperty*>& bprops, const std::map<std::string, std::string>& optParams, te::common::AccessPolicy p = te::common::RAccess);

      /*!
       \brief Constructor.

       \param gdataset    A GDAL dataset handler.
       \param p           Access Policy.
       */
      //Raster(GDALDataset* gdataset, te::common::AccessPolicy p = te::common::RAccess);

      Raster(const Raster& rhs);

      ~Raster();

      void open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess);

      std::map<std::string, std::string> getInfo() const;

      std::size_t getNumberOfBands() const;

      int getBandDataType(std::size_t i) const;

      const te::rst::Band* getBand(std::size_t i) const;

      te::rst::Band* getBand(std::size_t i);

      const te::rst::Band& operator[](std::size_t i) const;

      te::rst::Band& operator[](std::size_t i);

      /*! \brief Returns the raster GDAL handler. */
      GDALDataset* getGDALDataset() const;

      te::dt::AbstractData* clone() const;

      Raster& operator=(const Raster& rhs);

      te::rst::Raster* resample(int method, int scale, const std::map<std::string, std::string>& rinfo) const;

      /*!
       \note When the parameter USE_TERRALIB_REPROJECTION = TRUE in rinfo, the default reprojection will be called.
       Otherwise this method will call the GDAL reprojection method. The rinfo must define a GDAL compatible raster.
       The parameter m (interpolation method) is not used in GDAL implementation.
       */
      te::rst::Raster* transform(int srid, double llx, double lly, double urx, double ury, double resx, double resy, const std::map<std::string, std::string>& rinfo, int m = 0) const;

      void transform(te::rst::Raster* outRaster);

      /*!
       \note GDAL driver extended method.
       */
      void create(te::rst::Grid* g,
                  const std::vector<te::rst::BandProperty*> bands,
                  const std::map<std::string, std::string>& rinfo,
                  void* h, void (*deleter)(void*));
      
      bool createMultiResolution( const unsigned int levels, const te::rst::InterpolationMethod interpMethod );
      
      bool removeMultiResolution(); 
      
      unsigned int getMultiResLevelsCount() const;
      
      te::rst::Raster* getMultiResLevel( const unsigned int level ) const;
      
    protected :
      
      /*!
       \brief Constructor.

       \param multiResolutionLevel Level of a multi-resolution pyramid.
       \param uRI         The raster URI.
       \param policy      The raster access policy. 
       */
      Raster( const unsigned int multiResolutionLevel, const std::string& uRI, 
              const te::common::AccessPolicy& policy );

    private:

      GDALDataset* m_gdataset;                             //!< Gdal data set handler.
      std::vector<Band*> m_bands;                          //!< The vector of available bands in the raster.
      void (*m_deleter)(void*);                            //!< A pointer to a deleter function, if the buffer needs to be deleted by this object.
      std::string m_myURI;                                 //!< This instance URI;
      std::auto_ptr<DataSetUseCounter> m_dsUseCounterPtr;  //!< Dataset use counter pointer.
    };
  } // end namespace gdal
}   // end namespace te

#endif  // __TERRALIB_GDAL_INTERNAL_RASTER_H

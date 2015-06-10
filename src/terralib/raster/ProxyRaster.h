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
  \file terralib/raster/ProxyRaster.h

  \brief A proxy class for bands of several rasters over the same region.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_PROXYRASTER_H
#define __TERRALIB_RASTER_INTERNAL_PROXYRASTER_H

// TerraLib
#include "Raster.h"

namespace te
{
  namespace rst
  {
    /*!
      \class ProxyRaster

      \brief A proxy class for bands of several rasters over the same region.
      
      \ingroup rst
    */
    class TERASTEREXPORT ProxyRaster: public Raster
    {
      public:

        /*!
          \brief Constructor from a set of rasters.

          The bands of the proxy raster will be taken from the input set of raster.

          \param rasters  A vector with the list of raster covering the same region.

          \pre All rasters must cover the same region, have the same grid parameters.

          \note It is desired, but not required, that all raster have the same block size.
        */
        ProxyRaster(const std::vector<RasterPtr>& rasters);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand side Raster.
        */
        ProxyRaster(const ProxyRaster& rhs);

        /*! \brief Destructor. */
        ~ProxyRaster();

        void swap(std::size_t first, std::size_t second);

        void open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess);

        std::map<std::string, std::string> getInfo() const;

        std::size_t getNumberOfBands() const;

        int getBandDataType(std::size_t i) const;

        const Band* getBand(std::size_t i) const;

        Band* getBand(std::size_t i);

        const Band& operator[](std::size_t i) const;

        Band& operator[](std::size_t i);

        te::dt::AbstractData* clone() const { return 0; }
        
        bool createMultiResolution( const unsigned int levels, const InterpolationMethod interpMethod )
        {
          return false;
        };
        
        bool removeMultiResolution() { return false; }; 
        
        unsigned int getMultiResLevelsCount() const
        {
          return 0;        
        }
        
        Raster* getMultiResLevel( const unsigned int level ) const
        {
          return 0;         
        }        

      protected:

        std::vector<RasterPtr> m_rasters;           //!< The list of proxed rasters.
        std::vector<Band*> m_bands;                 //!< The proxy raster bands.
        std::map<std::string, std::string> m_rinfo; //!< Proxy raster additional information.
    };

    typedef boost::shared_ptr<ProxyRaster> ProxyRasterPtr;

  } // end namespace rst
}   // end namespace te

#endif //__TERRALIB_RASTER_INTERNAL_PROXYRASTER_H

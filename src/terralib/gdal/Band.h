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
 \file terralib/gdal/Band.h
 
 \brief It gives access to values in one band (dimension) of a raster.
 */

#ifndef __TERRALIB_GDAL_INTERNAL_BAND_H
#define __TERRALIB_GDAL_INTERNAL_BAND_H

// TerraLib
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/BlockUtils.h"
#include "../raster/Raster.h"
#include "Config.h"

// STL
#include <complex>
#include <string>
#include <vector>

// GDAL
#include <gdal.h>

// Forward declaration
class GDALRasterBand;

namespace te
{
  namespace gdal
  {
    // Forward declaration.
    class Raster;
    
    /*!
     \class Band
     
     \brief This class represents raster band description.
     
     This class is a concrete implementation of a Raster Band using the GDAL library to access
     the data.
     
     \sa te::rst::Band, te::rst::BandProperty
     */
    class TEGDALEXPORT Band : public te::rst::Band
    {
    public:
      
      /*!
       \brief Constructor
       
       \param rstPtr A pointer to the parent raster.
       \param idx This band index.
       \param gdalRasterBandPtr The GDAL band related to this terralib band.
       
       \warning The caller is responsible for providing correct values for the range [c x r].
       */      
      Band( Raster* rstPtr, std::size_t idx, GDALRasterBand* gdalRasterBandPtr );
      
      Band(const Band& rhs);
      
      ~Band();
      
      te::rst::Raster* getRaster() const;
      
      Band& operator=(const Band& rhs);
      
      void getValue(unsigned int c, unsigned int r, double& value) const;
      
      void setValue(unsigned int c, unsigned int r, const double value);
      
      void getIValue(unsigned int c, unsigned int r, double& value) const;
      
      void setIValue(unsigned int c, unsigned int r, const double value);
      
      void getValue(unsigned int c, unsigned int r, std::complex<double>& value) const { te::rst::Band::getValue(c, r, value); }
      
      void setValue(unsigned int c, unsigned int r, const std::complex<double>& value) { te::rst::Band::setValue(c, r, value); }
      
      void read(int x, int y, void* buffer) const;
      
      void* read(int /*x*/, int /*y*/);
      
      void write(int x, int y, void* buffer);
      
      void getValueFromBlock(void* block, unsigned int pos, std::complex<double>& value) const;
      
      void getValueFromBlock(void* block, unsigned int pos, double& value) const;
      
    private:
      
      /*!
       \brief Places the buffer in position adequate to obtain row/column values.
       
       \param c  The column in band to manipulate value.
       \param r  The row in band to manipulate value.
       
       \warning The caller is responsible for providing correct values for the range [c x r].
       */
      int placeBuffer(unsigned c, unsigned r) const;
      
      /*! \brief The type of function used to insert/extract data from buffers. */
      //typedef void (*ManipulateBufferValueFPtr)(int index, void* buffer, double* value);
      
    private:
      
      te::rst::Raster* m_raster;               //!< The Raster associated to this band.
      GDALRasterBand* m_rasterBand;            //!< GDAL Raster band.
      te::rst::GetBufferValueFPtr m_getBuff;   //!< A pointer to a function that helps to extract a double or complex value from a specific buffer data type (char, int16, int32, float, ...).
      te::rst::GetBufferValueFPtr m_getBuffI;  //!< A pointer to a function that helps to extract the imaginary part value from a specific buffer data type (cint16, cint32, cfloat, cdouble).
      te::rst::SetBufferValueFPtr m_setBuff;   //!< A pointer to a function that helps to insert a double or complex value into a specific buffer data type (char, int16, int32, float, ...).
      te::rst::SetBufferValueFPtr m_setBuffI;  //!< A pointer to a function that helps to insert the imaginary part value into a specific buffer data type (cint16, cint32, cfloat, cdouble).
      GDALDataType m_gdaltype;                 //!< The GDAL Data type.
      void* m_buffer;                          //!< An internal buffer.
      mutable int m_x;                         //!< Actual x buffer position.
      mutable int m_y;                         //!< Actual y buffer position.
      mutable bool m_update_buffer;            //!< Flag to update buffer.
      
      // te::gdal::Band::getValue/setValue internal variables
      mutable int m_currX;                     //!< Block x position.
      mutable int m_currY;                     //!< Block y position.
      mutable int m_currC;                     //!< Block column position.
      mutable int m_currR;                     //!< Block row position.
      mutable int m_i;                         //!< Block index.
    };
  } // end namespace gdal
}   // end namespace te

#endif  // __TERRALIB_GDAL_INTERNAL_BAND_H


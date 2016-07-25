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
  \file terralib/idl/IdlBand.h
 
  \brief A adaptor to an external existent raster in a idl array form.
*/

#ifndef __TERRALIB_IDL_INTERNAL_IDLBAND_H
#define __TERRALIB_IDL_INTERNAL_IDLBAND_H

// TerraLib
#include "Config.h"
#include "../raster/Band.h"
#include "../raster/BlockUtils.h"

#include <cstddef>
#include <cassert>

namespace te
{  
  namespace idl
  {
// Forward declaration
    class Raster;

    /*!
      \class IdlBand

      \brief A adaptor to an external existent raster in a idl array form.
      
      \details A adaptor to an external existent raster in a idl array form.
    */
    class TEIDLEXPORT IdlBand : public te::rst::Band
    {
      public:

        /*!
          \brief Constructor.

          \param rasterPtr The parent raster pointer.
          
          \param idx The band index.
          
          \param allBandsDataPtr A pointer to the unique global raster data.
          
          \param dataType The pixel data type (terralib type).
          
          \param nBands The parent raster number of bands.
          
          \param nLines The parent raster number of lines.
          
          \param nCols The parent raster number of columns.          
        */        
        IdlBand( const te::rst::Raster* rasterPtr, const void* allBandsDataPtr, 
          std::size_t idx, int dataType, const unsigned int nBands,
          const unsigned int nLines, const unsigned int nCols );

        ~IdlBand();

        inline te::rst::Raster* getRaster() const
        {
          return m_rasterPtr;
        };

        void getValue(unsigned int c, unsigned int r, double& value) const;

        void setValue(unsigned int c, unsigned int r, const double value);

        void getIValue(unsigned int c, unsigned int r, double& value) const;

        void setIValue(unsigned int c, unsigned int r, const double value);

        void read(int x, int y, void* buffer) const;

        inline void* read(int x, int y)
        {
          return m_allBandsDataPtr;
        };

        void write(int x, int y, void* buffer);

      protected :
        
        te::rst::Raster* m_rasterPtr; //!< A pointer to the parent raster instance.
        
        void* m_allBandsDataPtr; //!< Data buffer pointer.
        
        unsigned int m_nLines; //!< The parent raster number of lines.
        
        unsigned int m_nCols; //!< The parent raster number of columns.
        
        unsigned int m_nBands; //!< The parent raster number of bands.
        
        te::rst::GetBufferValueFPtr m_getBuff;   //!< A pointer to a function that helps to extract a double or complex value from a specific buffer data type (char, int16, int32, float, ...).
        te::rst::GetBufferValueFPtr m_getBuffI;  //!< A pointer to a function that helps to extract the imaginary part value from a specific buffer data type (cint16, cint32, cfloat, cdouble).
        te::rst::SetBufferValueFPtr m_setBuff;   //!< A pointer to a function that helps to insert a double or complex value into a specific buffer data type (char, int16, int32, float, ...).
        te::rst::SetBufferValueFPtr m_setBuffI;  //!< A pointer to a function that helps to insert the imaginary part value into a specific buffer data type (cint16, cint32, cfloat, cdouble).
        
        // Variable used by setValue/getValue methods
        mutable unsigned int m_bufferOffset;  
        
      private :

        IdlBand();
        
        IdlBand(const IdlBand& );
    };

  } // end namespace idl
}   // end namespace te

#endif  // __TERRALIB_IDL_INTERNAL_IDLBAND_H


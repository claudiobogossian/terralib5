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
  \file terralib/idl/IdlRaster.h

  \brief A adaptor to an external existent raster in a idl array form.
*/

#ifndef __TERRALIB_IDL_INTERNAL_CACHEDRASTER_H
#define __TERRALIB_IDL_INTERNAL_CACHEDRASTER_H

#include "Config.h"
#include "IdlBand.h"

#include "../raster/Raster.h"

#include <idl_export.h>

#include <vector>
#include <cassert>

namespace te
{
  namespace idl
  {
    /*!
      \class IdlRaster

      \brief A adaptor to an external existent raster in a idl array form.
      
      \details A adaptor to an external existent raster in a idl array form.
    */
    class TEIDLEXPORT IdlRaster: public te::rst::Raster
    {
      public:

        /*!
          \brief Constructor.
          
          \param idlValuePointer A pointer to a valid idl array structure.
          
          \param takeBufferOwnership if True the data buffer will be deleted by this raster instance, when necessary.
        */
        IdlRaster( const IDL_VPTR& idlValuePointer, const bool takeBufferOwnership );
         

        ~IdlRaster();
        

        std::map<std::string, std::string> getInfo() const;

        inline std::size_t getNumberOfBands() const
        {
          return m_bands.size();
        };

        inline int getBandDataType(std::size_t i) const
        {
          return m_dataType;
        };        

        inline const te::rst::Band* getBand(std::size_t i) const
        {
          assert( i < m_bands.size() );
          return m_bands[ i ];
        };

        inline te::rst::Band* getBand(std::size_t i)
        {
          assert( i < m_bands.size() );
          return m_bands[ i ];
        };

        inline const te::rst::Band& operator[](std::size_t i) const
        {
          assert( i < m_bands.size() );
          return *(m_bands[ i ]);
        };        

        inline te::rst::Band& operator[](std::size_t i)
        {
          assert( i < m_bands.size() );
          return *(m_bands[ i ]);
        };        
        
        bool createMultiResolution( const unsigned int levels, const te::rst::InterpolationMethod interpMethod )
        {
          return false;
        };
        
        bool removeMultiResolution() { return false; }; 
        
        unsigned int getMultiResLevelsCount() const
        {
          return 0;        
        }
        
        te::rst::Raster* getMultiResLevel( const unsigned int level ) const
        {
          return 0;         
        }         

      protected :
        
        unsigned char* m_allBandsDataPtr; //!< Data buffer pointer.
        
        bool m_takeBufferOwnership; //!< If true, the data buffer will be deleted when necessary.
        
        int m_dataType; //!< Data type used by all bands.
        
        std::vector< te::idl::IdlBand* > m_bands; //!< Internal raster bands.        
        
      private :
        
        IdlRaster();
        
        IdlRaster( const te::rst::Raster& rhs );
        
        IdlRaster(te::rst::Grid* grid, te::common::AccessPolicy p = te::common::RAccess);
        
        void open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess);
        
        te::dt::AbstractData* clone() const;
    };

  } // end namespace idl
}   // end namespace te

#endif //__TERRALIB_IDL_INTERNAL_CACHEDRASTER_H

/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/memory/ExpansibleRaster.h

  \brief A raster (stored in memory and eventually swapped to disk) where it is possible to dynamically add lines/columns/bands.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_EXPANSIBLERASTER_H
#define __TERRALIB_MEMORY_INTERNAL_EXPANSIBLERASTER_H

// TerraLib
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "Config.h"
#include "ExpansibleBand.h"
#include "ExpansibleBandBlocksManager.h"

namespace te
{
  namespace mem
  {
    /*!
      \class ExpansibleRaster

      \brief A raster (stored in memory and eventually swapped to disk) where it is possible to dynamically add lines/columns/bands.

      \note The first band blocking scheme will be taken as reference for the other bands.

      \note Adding lines/columns may add extra lines/columns to correctly fit the internal blocking structure.

      \note The geographic limits will be automatically adjust following the requested expansion.
    */
    class TEMEMORYEXPORT ExpansibleRaster: public te::rst::Raster
    {
      public:
        
        ExpansibleRaster();

        /*!
          \brief Constructor.

          \param maxMemPercentUsed The maximum free memory percentual to use valid range: [1:100].
          \param grid              The grid definition. The Raster will take its ownership.
          \param bandsProperties   The bands propeties (the raster will take their ownership);
        */
        ExpansibleRaster( const unsigned char maxMemPercentUsed, 
          te::rst::Grid* grid, 
          const std::vector<te::rst::BandProperty*> bandsProperties );

        /*!
          \brief Constructor.

          \param grid   The grid definition. The Raster will take its ownership.

          \param bandsProperties The bands propeties (the raster will take their ownership);

          \param maxNumberOfRAMBlocks The maximum number of RAM blocks.
        */
        ExpansibleRaster( te::rst::Grid* grid, 
                          const std::vector<te::rst::BandProperty*> bandsProperties,
                          const unsigned int maxNumberOfRAMBlocks );

        ~ExpansibleRaster();

        void open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess);

        std::map<std::string, std::string> getInfo() const;

        std::size_t getNumberOfBands() const
        {
          return m_bands.size();
        };

        int getBandDataType(std::size_t i) const
        {
          assert( i < m_bands.size() );
          return m_bands[ i ]->getProperty()->m_type;
        };        

        const te::rst::Band* getBand(std::size_t i) const
        {
          assert( i < m_bands.size() );
          return m_bands[ i ];
        };

        te::rst::Band* getBand(std::size_t i)
        {
          assert( i < m_bands.size() );
          return m_bands[ i ];
        };

        const te::rst::Band& operator[](std::size_t i) const
        {
          assert( i < m_bands.size() );
          return *(m_bands[ i ]);
        };

        te::rst::Band& operator[](std::size_t i)
        {
          assert( i < m_bands.size() );
          return *(m_bands[ i ]);
        };

        te::dt::AbstractData* clone() const;
        
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

        /*!
          \brief New lines will be added at the top of the raster (before the first line).

          \param number The number of lines to add.

          \return true if OK, false on errors.
        */
        bool addTopLines( const unsigned int number );
        
        /*!
          \brief New lines will be added at the bottom of the raster (after de the last line).
          
          \param number The number of lines to add.
          
          \return true if OK, false on errors.
        */
        bool addBottomLines( const unsigned int number );

        /*!
          \brief New columns will be added at the left of the raster (before the first column).

          \param number The number of columns to add.

          \return true if OK, false on errors.
        */
        bool addLeftColumns( const unsigned int number );

        /*!
          \brief New columns will be added at the right of the raster (after the last column).

          \param number The number of columns to add.

          \return true if OK, false on errors.
        */
        bool addRightColumns( const unsigned int number );

        /*!
          \brief New bands will be added at the top of the raster (before the first band).

          \param number The number of bands to add.

          \return true if OK, false on errors.
        */
        bool addTopBands( const unsigned int number );

        /*!
          \brief New bands will be added at the bottom of the raster (after de the last band).

          \param number The number of bands to add.

          \return true if OK, false on errors.
        */
        bool addBottomBands( const unsigned int number );

      protected :

        std::vector<ExpansibleBand*> m_bands; //!< Internal raster bands.

        ExpansibleBandBlocksManager m_blocksManager; //!< Internal blocks manager.

        /*! \brief Free all allocated internal resources and go back to the initial state. */
        void free();

        /*!
          \brief Fill all blocks with dummy values.
        */
        void dummyFillAllBlocks();

        /*!
          \brief Fill the required blocks with dummy values.

          \param blocksCoords The blocks coords.
        */
        void dummyFillBlocks( const std::vector<ExpansibleBandBlocksManager::BlockIndex3D>& blocksCoords );

      private :

        ExpansibleRaster( const Raster &rhs );

        ExpansibleRaster(te::rst::Grid* grid, te::common::AccessPolicy p = te::common::RAccess);
    };

  } // end namespace mem
}   // end namespace te

#endif //__TERRALIB_MEMORY_INTERNAL_EXPANSIBLERASTER_H

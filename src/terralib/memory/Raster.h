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
  \file terralib/memory/Raster.h

  \brief A raster class for memory.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_RASTER_H
#define __TERRALIB_MEMORY_INTERNAL_RASTER_H

// TerraLib
#include "../raster/Raster.h"
#include "Config.h"

namespace te
{
  namespace mem
  {
    /*!
      \class Raster

      \brief A raster class for memory.

      \sa te::rst::Raster
    */
    class TEMEMORYEXPORT Raster: public te::rst::Raster
    {
      public:

        Raster();

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

        te::dt::AbstractData* clone() const;

        /*!
          \note In-Memory driver extended method.
        */
        void close();

        /*!
          \note In-Memory driver extended method.
        */
        void create(te::rst::Grid* g,
                    const std::vector<te::rst::BandProperty*> bands,
                    const std::map<std::string, std::string>& rinfo,
                    void* h, void (*deleter)(void*));
        
        bool createMultiResolution( const unsigned int levels, const te::rst::InterpolationMethod interpMethod )
        {
          return false;
        };
        
        unsigned int getMultiResLevelsCount() const
        {
          return 0;        
        }
        
        te::rst::Raster* getMultiResLevel( const unsigned int level ) const
        {
          return 0;         
        }        

      private:

        std::vector<te::rst::Band*> m_bands;     //!< The list of data bands.
        void (*m_deleter)(void*);                //!< A pointer to a deleter function, if the buffer needs to be deleted by this object.
        void* m_externalBuffer;                  //!< An external buffer.
    };

  } // end namespace mem
}   // end namespace te

#endif //__TERRALIB_MEMORY_INTERNAL_RASTER_H

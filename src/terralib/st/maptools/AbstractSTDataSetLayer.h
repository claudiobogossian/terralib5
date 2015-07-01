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
  \file terralib/st/maptools/AbstractSTDataSetLayer.h

  \brief  This file has the AbstractSTDataSetLayer class that provides an abstract interface
          for layers that contains a DataSet with spatiotemporal data. 
*/

#ifndef __TERRALIB_ST_INTERNAL_ABSTRACTSTDATASETLAYER_H
#define __TERRALIB_ST_INTERNAL_ABSTRACTSTDATASETLAYER_H

// ST
#include "../Config.h"

// TerraLib
#include "../../datatype/Enums.h"
#include "../../maptools/AbstractLayer.h"

// Forward declaration
namespace te { namespace dt { class DateTime; class DateTimePeriod; } }

namespace te
{

  namespace st
  {
    /*!
      \class AbstractSTDataSetLayer

      \brief A layer with reference to a dataset that contains spatiotemporal data.

      \sa AbstractLayer
    */
    class TESTEXPORT AbstractSTDataSetLayer : public te::map::AbstractLayer
    {
      public:

        /*!
          \brief It initializes a new layer.

          \param parent The parent layer (NULL if it has no parent).
        */
        AbstractSTDataSetLayer(AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new layer.

          \param id     The layer id.
          \param parent The parent layer (NULL if it has no parent).
        */
        AbstractSTDataSetLayer(const std::string& id, AbstractLayer* parent = 0);
                
        /*!
          \brief It initializes a new Layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer.
          \param parent The parent layer (NULL if it has no parent).
        */
        AbstractSTDataSetLayer(const std::string& id, const std::string& title, AbstractLayer* parent = 0);
              

        /*! \brief Virtual Destructor. */
        virtual ~AbstractSTDataSetLayer() {};
                
        /*!
          \brief It returns the layer temporal extent.

          \return The layer temporal extent.
        */
        virtual te::dt::DateTimePeriod* getTemporalExtent() const = 0;
        
        /*!
          \brief It gets the dataset using a temporal filter over the phenomenon time property.

          The possible temporal relations are: 1. AFTER; 
          2. AFTER | EQUALS; 3. BEFORE;  4. BEFORE | EQUALS; 5. DURING; 6. EQUALS 

          \param dt       A given datetime to be used as a temporal filter when retrieving datasets.
          \param r        The temporal relation to be used during the filter.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return A pointer to a new DataSet. 

          \note The caller will take the ownership of the returned pointer.
          \note When the temporal relation is DURING, dt must be a time period.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSet> getData( const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING,
                                               te::common::TraverseType travType = te::common::FORWARDONLY, 
                                               te::common::AccessPolicy rwRole = te::common::RAccess) const = 0;

        /*!
          \brief  It gets the dataset using a temporal filter over the phenomenon time property
                  and a spatial filter over the observed geometry property.

          The possible temporal relations are: 1. AFTER; 
          2. AFTER | EQUALS; 3. BEFORE;  4. BEFORE | EQUALS; 5. DURING; 6. EQUALS 

          \param dt       A given datetime to be used as a temporal filter when retrieving datasets.
          \param tr       The temporal relation to be used during the filter.
          \param e        A rectangle to be used as a spatial filter when retrieving datasets.
          \param sr       The spatial relation to be used during the filter.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return A pointer to a new DataSet. 

          \note The caller will take the ownership of the returned pointer.
          \note When the temporal relation is DURING, dt must be a time period.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSet> getData( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                               const te::gm::Envelope& e, te::gm::SpatialRelation sr = te::gm::INTERSECTS,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               te::common::AccessPolicy rwRole = te::common::RAccess) const = 0;

        /*!
          \brief  It gets the dataset using a temporal filter over the phenomenon time property
                  and a spatial filter over the observed geometry property.

          The possible temporal relations are: 1. AFTER; 
          2. AFTER | EQUALS; 3. BEFORE;  4. BEFORE | EQUALS; 5. DURING; 6. EQUALS 

          \param dt       A given datetime to be used as a temporal filter when retrieving datasets.
          \param tr       The temporal relation to be used during the filter.
          \param g        A geometry to be used as a spatial filter when retrieving datasets.
          \param sr       The spatial relation to be used during the filter.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return A pointer to a new DataSet. 

          \note The caller will take the ownership of the returned pointer.
          \note When the temporal relation is DURING, dt must be a time period.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSet> getData( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                               const te::gm::Geometry& g, te::gm::SpatialRelation sr = te::gm::INTERSECTS,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               te::common::AccessPolicy rwRole = te::common::RAccess) const = 0;
    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_ABSTRACTSTDATASETLAYER_H


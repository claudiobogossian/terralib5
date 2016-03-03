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
  \file terralib/edit/Feature.h
   
  \brief This class represents a geographic feature.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_FEATURE_H
#define __TERRALIB_EDIT_INTERNAL_FEATURE_H

// TerraLib
#include "Config.h"
#include "Utils.h"

// STL
#include <map>

namespace te
{
// Forward declarations
  namespace da
  {
    class ObjectId;
  }

  namespace dt
  {
    class AbstractData;
  }

  namespace gm
  {
    class Geometry;
  }

  namespace edit
  {
    class TEEDITEXPORT Feature
    {
      public:

        Feature();

        Feature(te::da::ObjectId* id);

        Feature(te::da::ObjectId* id, te::gm::Geometry* geom, OperationType operation);

        ~Feature();

        void set(te::da::ObjectId* id, te::gm::Geometry* geom, OperationType operation);

        void setId(te::da::ObjectId* id);

        void setGeometry(te::gm::Geometry* geom);

        void setData(const std::map<std::size_t, te::dt::AbstractData*>& data);

        void setOperation(OperationType operation);

        void setCoords(std::vector<te::gm::Coord2D> coords);

        te::da::ObjectId* getId() const;

        te::gm::Geometry* getGeometry() const;

        const std::map<std::size_t, te::dt::AbstractData*>& getData() const;

        OperationType getOperationType() const;

        std::vector<te::gm::Coord2D> getCoords() const;

        bool isEquals(te::da::ObjectId* id);

        Feature* clone() const;

      private:

        te::da::ObjectId* m_id;
        te::gm::Geometry* m_geom;
        std::map<std::size_t, te::dt::AbstractData*> m_data;
        OperationType m_operationType;
        std::vector<te::gm::Coord2D> m_coords;

    };

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_FEATURE_H

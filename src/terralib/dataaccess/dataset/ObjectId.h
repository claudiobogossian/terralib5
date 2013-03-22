/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/dataset/ObjectId.h

  \brief This class represents an unique id for a data set element.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_OBJECTID_H
#define __TERRALIB_DATAACCESS_INTERNAL_OBJECTID_H

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <string>

namespace te
{
// Forward declaration
  namespace dt { class AbstractData; }

  namespace da
  {
    /*!
      \class ObjectId

      \brief This class represents an unique id for a data set element.

      \sa AbstractData, ObjectIdSet
    */
    class TEDATAACCESSEXPORT ObjectId
    {
      public:

        /*! \brief Default constructor. */
        ObjectId();

        /*! \brief Destructor. */
        ~ObjectId();

        /*!
          \brief It gets the properties values used to uniquely identify a data set element.
          
          \return A map from property indexes to their value.

          \note The caller will NOT take the ownership of the returned pointers.
        */
        const std::map<std::size_t, te::dt::AbstractData*>& getValue() const;

        /*!
          \brief It gets the properties values used to uniquely identify a data set element as string.

          \return A string that represents the set of values.
        */
        std::string getValueAsString() const;

        /*!
          \brief It adds a property value to uniquely identify a data set element.

          \param index The property index.
          \param data  The value of the property.

          \note This class will take the ownership of the given pointer.
        */
        void addValue(const std::size_t& index, te::dt::AbstractData* data);

      private:

        std::map<std::size_t, te::dt::AbstractData*> m_data; //!< A map of property indexes to abstract data.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_OBJECTID_H

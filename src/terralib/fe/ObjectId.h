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
  \file ObjectId.h
  
  \brief An object identifier is meant to represent a unique identifier for an object instance within the context of the web service or database that is serving the object.
 */

#ifndef __TERRALIB_FE_INTERNAL_ABSTRACTID_H
#define __TERRALIB_FE_INTERNAL_ABSTRACTID_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace fe
  {
    /*!
      \class ObjectId
      
      \brief An object identifier is meant to represent a unique identifier for an object instance within the context of the web service or database that is serving the object.

      The Filter Encoding specification does not define
      a specific element for identifying objects but instead
      defines the abstract element Id as the head of an XML
      substitution group that may be used to define an object
      identifier element for specific object types.
      <br>
      Remember that:
      <ul>
      <li>GML 3: uses GmlObjectId tag.</li>
      <li>GML 2: uses FeatureId tag.</li>
      </ul>

      \sa Filter
     */
    class TEFEEXPORT ObjectId
    {
      public:

        /*!
          \enum OidType

          \brief The type of object identification used.
         */
        enum OidType
        {
          GMLOBJECTID,  /*!< Using GML 3 format for oid (default). */
          FEATUREID     /*!< using GML 2 format for oid. */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new ObjectId.

          \param id      The object identification value. Must be a non empty value.
          \param oidType The type of object identification used.
         */
        ObjectId(const char* id, OidType oidType);

        /*! \brief Destructor. */
        ~ObjectId();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the feature identification value.

          \param id The feature identification value.
         */
        void setId(const std::string& id);

        /*!
          \brief It returns the feature identification value.

          \return The feature identification value.
         */
        const std::string& getId() const;

        /*!
          \brief It sets the format type used by the feature identification value.

          \param t The format type used by the feature identification value.
         */
        void setOidType(OidType t);

        /*!
          \brief It returns the format type used by the feature identification value.

          \return The format type used by the feature identification value.
         */
        OidType getOidType() const;

        //@}

      private:

        std::string m_id;    //!< The id in a string format.
        OidType m_oidType;   //!< May be one of: GmlObjectId, FeatureId, ObjectId, RecordId.
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_ABSTRACTID_H


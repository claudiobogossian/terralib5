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
  \file terralib/geometry/GeometryProperty.h

  \brief Geometric property.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYPROPERTY_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYPROPERTY_H

// TerraLib
#include "../datatype/SimpleProperty.h"
#include "Config.h"
#include "Enums.h"


namespace te
{
  namespace gm
  {
// Forward declarations
    class Envelope;

    /*!
      \class GeometryProperty

      \brief Geometric property.

      \ingroup geometry

      \sa SimpleProperty
     */
    class TEGEOMEXPORT GeometryProperty : public te::dt::SimpleProperty
    {
      public:

        /** @name Basic Methods
         *  Basic methods on a GeometryProperty.
         */
        //@{

        /*!
          \brief It constructs a new GeometryProperty.

          This constructor will set:
          <ul>
          <li>the associated SRS to 0;</li>
          <li>the subtype of geometry to te::gm::UnknownGeometryProperty;</li>
          <li>the coordinate dimension to 2;</li>
          <li>the extent to NULL.</li>
          </ul>

          \param name         The geometric property name.
          \param isRequired   Tells if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.
          
          \post The GeometryProperty will take the ownership of the defaultValue.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        GeometryProperty(const std::string& name,
                         bool isRequired = false,
                         std::string* defaultValue = 0,
                         unsigned int id = 0,
                         te::dt::Property* parent = 0);

        /*!
          \brief It constructs a new GeometryProperty.

          \param name         The geometric property name.
          \param srid         The SRS of the geometries.
          \param t            The subtype of geometries.
          \param isRequired   Indicates if the property is required (mandatory) or not.
          \param defaultValue The default value to be used, if none is informed.
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.

          \post The GeometryProperty will take the ownership of the defaultValue.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        GeometryProperty(const std::string& name,
                         int srid,
                         te::gm::GeomType subtype,
                         bool isRequired = false,
                         std::string* defaultValue = 0,
                         unsigned int id = 0,
                         te::dt::Property* parent = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy that would be used to copy from.
        */
        GeometryProperty(const GeometryProperty& rhs);

        /*! \brief Destructor. */
        ~GeometryProperty();

        /*!
          \brief It returns the spatial reference system identifier associated to this property.

          \return The SRS id associated to the property.
        */
        int getSRID() const { return m_srid; }

        /*!
          \brief It sets the spatial reference system identifier associated to this property.

          \param srid The SRS id associated to the property.
        */
        void setSRID(int srid) { m_srid = srid; }

        /*!
          \brief It returns the geometry subtype allowed for the property.

          \return The geometry subtype.
        */
        GeomType getGeometryType() const { return m_geomType; }

        /*!
          \brief It sets the geometry subtype.
          
          \param t The geometry subtype.
        */
        void setGeometryType(GeomType t) { m_geomType = t; }

        //@}

        /** @name Implementation From Superclass Property
         *  Methods re-implemented from the superclass Property.
         */
        //@{

        /*!
          \brief It returns a clone of the object.

          The new property will NOT have associations to other elements.

          \return A clone of the object.
        */
        te::dt::Property* clone() const;

        //@}

      private:

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
        */
        GeometryProperty& operator=(const GeometryProperty& rhs);

      protected:

        int m_srid;             //!< The SRS associated to the property.
        GeomType m_geomType;    //!< The geometry type.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYPROPERTY_H


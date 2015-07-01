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
  \file terralib/fe/BinarySpatialOp.h
  
  \brief A class for binary spatial operators.  
 */

#ifndef __TERRALIB_FE_INTERNAL_BINARYSPATIALOP_H
#define __TERRALIB_FE_INTERNAL_BINARYSPATIALOP_H

// TerraLib
#include "SpatialOp.h"

namespace te
{
  namespace gm { class Geometry; }
  namespace gml { class Envelope; }

  namespace fe
  {
// Forward declarations
    class PropertyName;

    /*!
      \class BinarySpatialOp
      
      \brief A class for binary spatial operators.

      Binary spatial operators are used to test whether
      the value of a geometric property, referenced using the name
      of the property, and a literal geometric value satisfy the
      spatial relationship implied by the operator.<br>
      The following operators are defined in the Filter
      Encoding specification:
      <ul>
      <li>Equals</li>
      <li>Disjoint</li>
      <li>Touches</li>
      <li>Within</li>
      <li>Overlaps</li>
      <li>Crosses</li>
      <li>Intersects</li>
      <li>Contains</li>
      </ul>

      \ingroup fe

      \sa SpatialOp, PropertyName      
     */
    class TEFEEXPORT BinarySpatialOp : public SpatialOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes the BinarySpatialOp.

          \param opName The operator name.

          \note The BinarySpatialOp will not take the ownership of the given name.
         */
        BinarySpatialOp(const char* opName = 0);

        /*!
          \brief It initializes the BinarySpatialOp.

          \param opName The operator name.
          \param p      The property name.
          \param g      The geometry value.

          \note The BinarySpatialOp will not take the ownership of the given name.

          \note The BinarySpatialOp will take the ownership of the given operands.
         */
        BinarySpatialOp(const char* opName, PropertyName* p, te::gm::Geometry* g);

        /*!
          \brief It initializes the BinarySpatialOp.

          \param opName The operator name.
          \param p      The property name.
          \param e      The envelope value.

          \note The BinarySpatialOp will not take the ownership of the given name.

          \note The BinarySpatialOp will take the ownership of the given operands.
         */
        BinarySpatialOp(const char* opName, PropertyName* p, te::gml::Envelope* e);

        /*! \brief Destructor. */
        virtual ~BinarySpatialOp();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the property name.

          \param p The property name.

          \note It will take the ownership of the property name.
         */
        void setProperty(PropertyName* p);

        /*!
          \brief It returns the property name.

          \return The property name.
         */
        PropertyName* getProperty() const;

        /*!
          \brief It sets the geometry.

          \param g The geometry.

          \note It will take the ownership of the geometry.
         */
        void setGeometry(te::gm::Geometry* g);

        /*!
          \brief It returns the geometry.

          \return The geometry.
         */
        te::gm::Geometry* getGeometry() const;

        /*!
          \brief It sets the envelope.

          \param e The envelope.

          \note It will take the ownership of the envelope.
         */
        void setEnvelope(te::gml::Envelope* e);

        /*!
          \brief It returns the envelope.

          \return The envelope.
         */
        te::gml::Envelope* getEnvelope() const;

        //@}

      protected:

        PropertyName* m_property;  //!< A geometric property. (Mandatory)
        te::gm::Geometry* m_geom;  //!< A literal geometric value. (It must be omitted if m_envelope is given)
        te::gml::Envelope* m_box;  //!< A convenient and more compact way of encoding the very common bounding box constraint. (It must be omitted ig m_box is given)
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_BINARYSPATIALOP_H

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
  \file terralib/fe/BBOXOp.h
  
  \brief A convenient and more compact way of encoding the very common bounding box constraint based on an envelope.  
 */

#ifndef __TERRALIB_FE_INTERNAL_BBOXOP_H
#define __TERRALIB_FE_INTERNAL_BBOXOP_H

// TerraLib
#include "SpatialOp.h"

namespace te
{
  namespace gml { class Envelope; }

  namespace fe
  {
// Forward declarations
    class PropertyName;

    /*!
      \class BBOXOp
      
      \brief A convenient and more compact way of encoding the very common bounding box constraint based on an envelope.

      It is equivalent to the spatial operation
      Not Disjoint, meaning that the BBOX operator
      should identify all geometries that spatially
      interact with the box. If the optional
      m_property element is not specified,
      the calling service must determine which
      spatial property is the spatial key and
      apply the BBOX operator accordingly. For
      feature types that has a single spatial
      property, this is a trivial matter. For
      feature types that have multiple spatial
      properties, the calling service either
      knows which spatial property is the spatial
      key, or the calling service generates an exception
      indicating that the feature contains multiple spatial
      properties and the m_property element must be specified.
      Of course, a client application always has the
      options of avoiding the exceptions by calling
      the DescribeFeatureType operation to get a description
      of the feature type.
      
      \ingroup fe

      \sa SpatialOp, PropertyName      
     */
    class TEFEEXPORT BBOXOp : public SpatialOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new BBOXOp. */
        BBOXOp();

        /*!
          \brief It initializes a new BBOXOp.

          \param p The property name.
          \param e The envelope.

          \note It will take the ownership of the property name and envelope.
         */
        BBOXOp(PropertyName* p, te::gml::Envelope* e);

        /*! \brief Destructor. */
        ~BBOXOp();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets property name.

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

      private:

        PropertyName* m_property;         //!< A geometric property. (theoretically it is optional, but it is required by the XML schema)
        te::gml::Envelope* m_envelope;    //!< Bounding box constraint. (Mandatory)
    };   

  } // end namespace fe
}     // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_BBOXOP_H

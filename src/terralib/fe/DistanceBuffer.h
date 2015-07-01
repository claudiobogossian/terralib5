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
  \file DistanceBuffer.h
  
  \brief Distance buffer operator.  
 */

#ifndef __TERRALIB_FE_INTERNAL_DISTANCEBUFFER_H
#define __TERRALIB_FE_INTERNAL_DISTANCEBUFFER_H

// TerraLib
#include "SpatialOp.h"

namespace te
{
// Forward declarations
  namespace common { class Distance; }
  namespace gm { class Geometry; }

  namespace fe
  {
// Forward declarations
    class PropertyName;

    /*!
      \class DistanceBuffer
      
      \brief Distance buffer operator.

      The following operators are defined in the Filter
      Encoding specification:
      <ul>
      <li>DWithin</li>
      <li>Beyond</li>
      </ul>

      \ingroup fe

      \sa SpatialOp, PropertyName, Distance      
     */
    class TEFEEXPORT DistanceBuffer : public SpatialOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes the DistanceBuffer operator.

          \param opName The operator name.

          \note The DistanceBuffer will not take the ownership of the given name.
         */
        DistanceBuffer(const char* opName = 0);

        /*!
          \brief It initializes the DistanceBuffer operator.

          \param opName The operator name.

          \note The DistanceBuffer will not take the ownership of the given name.

          \note The DistanceBuffer will take the ownership of the given operands.
         */
        DistanceBuffer(const char* opName,
                       PropertyName* p,
                       te::gm::Geometry* g,
                       te::common::Distance* d);

        /*! \brief Destructor. */
        virtual ~DistanceBuffer();

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
          \brief It sets the geometry.

          \param g The geometry.

          \note It will take the ownership of the geometry.
         */
        void setGeometry(te::gm::Geometry* g);

        /*! \brief It returns the geometry.

            \return The geometry.
         */
        te::gm::Geometry* getGeometry() const;

        /*!
          \brief It sets the distance.

          \param d The distance.

          \note It will take the ownership of the distance.
         */
        void setDistance(te::common::Distance* d);

        /*!
          \brief It returns the distance.

          \return The distance.
         */
        te::common::Distance* getDistance() const;

        //@}

      protected:

        PropertyName* m_property;     //!< A geometric property. (Mandatory, if m_geom is not specified)
        te::gm::Geometry* m_geom;     //!< Geometry literal. (Mandatory, if m_property is not specified)
        te::common::Distance* m_d;    //!< Distance. (mandatory)
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_DISTANCEBUFFER_H


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
  \file terralib/dataaccess/query/ST_DistanceBuffer.h

  \brief Spatial distance buffer operator.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ST_DISTANCEBUFFER_H
#define __TERRALIB_DATAACCESS_INTERNAL_ST_DISTANCEBUFFER_H

// TerraLib
#include "Function.h"

// STL
#include <memory>

namespace te
{
  // Forward declarations
  namespace common { class Distance; }
  namespace gm { class Geometry; }

  namespace da
  {
    /*!
      \class ST_DistanceBuffer
      
      \brief Spatial Distance Buffer operator.

      This function is just a syntatic-suggar.

      \sa Function
    */
    class TEDATAACCESSEXPORT ST_DistanceBuffer : public Function
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param name The distance buffer operator name.
          \param e    The expression.
          \param g    The geometry literal.
          \param d    Distance.

          \note The operator will take the ownership of the given arguments.
        */
        ST_DistanceBuffer(const std::string& name, Expression* e, te::gm::Geometry* g, te::common::Distance* d);

        /*!
          \brief Constructor.

          \param name The distance buffer operator name.
          \param e    The expression.
          \param g    The geometry literal.
          \param d    Distance.
        */
        ST_DistanceBuffer(const std::string& name, const Expression& e, const te::gm::Geometry& g, const te::common::Distance& d);

         /*! \brief Copy constructor. */
        ST_DistanceBuffer(const ST_DistanceBuffer& rhs);
        
        /*! \brief Destructor. */
        ~ST_DistanceBuffer();

        /*! Assignment operator.  */
        ST_DistanceBuffer& operator=(const ST_DistanceBuffer& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;

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

      protected:

        std::auto_ptr<te::gm::Geometry> m_geom;     //!< Geometry literal.
        std::auto_ptr<te::common::Distance> m_d;    //!< Distance.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ST_DISTANCEBUFFER_H


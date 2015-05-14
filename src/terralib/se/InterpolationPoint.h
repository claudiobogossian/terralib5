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
  \file terralib/se/InterpolationPoint.h

  \brief They are used to define a graph of points.
*/

#ifndef __TERRALIB_SE_INTERNAL_INTERPOLATIONPOINT_H
#define __TERRALIB_SE_INTERNAL_INTERPOLATIONPOINT_H

// TerraLib
#include "../fe/Expression.h"
#include "Config.h"

namespace te
{
  namespace se
  {
// Forward declarations
    class ParameterValue;

    /*!
      \class InterpolationPoint
      
      \brief They are used to define a graph of points.

      \sa Interpolate, te::fe::Expression, ParameterValue
    */
    class TESEEXPORT InterpolationPoint : public te::fe::Expression
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new InterpolationPoint.  */
        InterpolationPoint();

        /*!
          \brief Copy constructor.

          \param rhs The other InterpolationPoint.
        */
        InterpolationPoint(const InterpolationPoint& rhs);

        /*! \brief Destructor. */
        ~InterpolationPoint();

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        virtual InterpolationPoint* clone() const;

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setData(const double& d);
        double getData() const;

        void setValue(ParameterValue* v);
        ParameterValue* getValue() const;

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No assignment operator allowed.

          \param rhs The other InterpolationPoint.

          \return A reference for this.
        */
        InterpolationPoint& operator=(const InterpolationPoint& rhs);

        //@}

      private:

        double m_data;           //!< Mandatory.
        ParameterValue* m_value; //!< Mandatory.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_INTERPOLATIONPOINT_H

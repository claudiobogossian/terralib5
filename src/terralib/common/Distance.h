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
  \file terralib/common/Distance.h

  \brief A given distance has a measurement and a unit-of-measure.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_DISTANCE_H
#define __TERRALIB_COMMON_INTERNAL_DISTANCE_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \class Distance
      
      \brief A given distance has a measurement and a unit-of-measure.
     */
    class TECOMMONEXPORT Distance
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new Distance.

          \param value   The value associated to the measure.
          \param uomIdx  An index to a valid unit-of-measure. See the Unit-of-measure manager class for more information on it.
         */
        Distance(const double& value, std::size_t uomIdx);

        /*! \brief Destructor. */
        ~Distance();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setDistance(const double& value);

        const double& getDistance() const;

        void setUom(std::size_t uom);

        std::size_t getUom() const;

        //@}

      private:

        double m_value;
        std::size_t m_uom;
    };    

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_DISTANCE_H

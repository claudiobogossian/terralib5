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
  \file terralib/common/AbstractParameters.h

  \brief Abstract parameters base interface.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_ABSTRACTPARAMETERS_H
#define __TERRALIB_COMMON_INTERNAL_ABSTRACTPARAMETERS_H

// TerraLib
#include "Config.h"
#include "Exception.h"

namespace te
{
  namespace common
  {
    /*!
      \class AbstractParameters

      \brief Abstract parameters base interface.
    */
    class TECOMMONEXPORT AbstractParameters
    {
      public:

        /*! \brief Constructor. */
        AbstractParameters();

        /*! \brief Virtual destructor. */
        virtual ~AbstractParameters();

        /*! \brief Clear all internal allocated resources and reset the parameters instance to its initial state. */
        virtual void reset() throw( te::common::Exception ) = 0;

        /*!
          \brief Create a clone copy of this instance.

          \return A clone copy of this instance.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual AbstractParameters* clone() const = 0;
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_ABSTRACTPARAMETERS_H


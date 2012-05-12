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
  \file terralib/common/progress/ProgressParams.h
 
  \brief The ProgressParams is a struct with all common attributes of a progress bar.
*/

#ifndef __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSPARAMS_H
#define __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSPARAMS_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \struct ProgressParams

      \brief The ProgressParams is a struct with progress bar common attributes.

      \sa ProgressManager, AbstractProgress

      \todo
    */
    struct ProgressParams
    {
      int m_minValue;         //!< Range minimum value.
      int m_maxValue;         //!< Range maximum value.
      int m_totalSteps;       //!< Progress total steps.
      int m_currentStep;      //!< Current value.
      int m_currentPropStep;  //!< Current proportional value (0 - 100).
      std::string m_message;  //!< Progress message.
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_ABSTRACTPROGRESS_H

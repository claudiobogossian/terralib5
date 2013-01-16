/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/ApplicationController.h

  \brief The singleton controller of a TerraLib application.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_APPLICATIONCONTROLLER_H
#define __TERRALIB_QT_AF_INTERNAL_APPLICATIONCONTROLLER_H

// Terralib
#include "../../common/Singleton.h"
#include "BaseApplicationController.h"

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class ApplicationController

        \brief The singleton controller of a TerraLib application.

        \sa BaseApplicationController

        \todo Avaliar se os singletons nao estao gerando mais do que uma instancia, devido ao ponto de inclusao!
      */
      class ApplicationController : public te::common::Singleton<BaseApplicationController>
      {
        friend class te::common::Singleton<BaseApplicationController>;

        private:

          /*! \brief Default constructor. */
          ApplicationController();

          /*! \brief Destructor. */
          ~ApplicationController();
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_APPLICATIONCONTROLLER_H


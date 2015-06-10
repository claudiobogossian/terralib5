/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*! 
  \file terralib/qt/plugins/ApplicationListener.h

  \brief A listener of application framework. It is used to listen events sent by the application framework.
*/

#ifndef __TERRALIB_QT_PLUGINS_EDIT_INTERNAL_APPLICATIONLISTENER_H
#define __TERRALIB_QT_PLUGINS_EDIT_INTERNAL_APPLICATIONLISTENER_H

// Terralib
#include "../../../common/Singleton.h"

// Qt
#include <QObject>

namespace te
{
  namespace qt
  {
// Forward declaration
    namespace af
    {
      namespace evt
      {
        struct Event;
      }
    }

    namespace plugins
    {
      namespace edit
      {
        /*!
          \class ApplicationListener
        
          \brief A listener of application framework. It is used to listen events sent by the application framework.
        */
        class ApplicationListener : public QObject, public te::common::Singleton<ApplicationListener>
        {
          Q_OBJECT

          friend class te::common::Singleton<ApplicationListener>;

          protected:

            /*! \brief It initializes the singleton instance of the application framework listener. */
            ApplicationListener();

            /*! \brief Singleton destructor. */
            ~ApplicationListener();

          protected slots:

            void onApplicationTriggered(te::qt::af::evt::Event* e);
        };

      } // end namespace edit
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif // __TERRALIB_QT_PLUGINS_EDIT_INTERNAL_APPLICATIONLISTENER_H

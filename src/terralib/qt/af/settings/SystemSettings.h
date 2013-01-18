/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/settings/SystemSettings.h

  \brief A widget for setting user preferences.
*/

#ifndef __TERRALIB_QT_AF_SYSTEMSETTINGS_H
#define __TERRALIB_QT_AF_SYSTEMSETTINGS_H

// QT
#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>

// TerraLib
#include "../Config.h"

namespace Ui { class SystemSettingsForm; }

namespace te
{
  namespace qt
  {
    namespace af
    {
      class ComponentWidget;
      /*!
        \class SystemSettings

        \brief A widget for setting user preferences.
      */
      class TEQTAFEXPORT SystemSettings : public QDialog
      {
        Q_OBJECT

        public:

          SystemSettings(QWidget* parent = 0);

          ~SystemSettings();

          public slots:

            void on_itemClicked(QListWidgetItem * current);

        private:

          std::auto_ptr<Ui::SystemSettingsForm> m_ui;

          ComponentWidget* m_curComp;

      };

    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_AF_SYSTEMSETTINGS_H


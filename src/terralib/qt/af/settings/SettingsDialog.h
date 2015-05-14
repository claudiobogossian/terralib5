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
  \file SettingsDialog.h

  \brief A for customization of the application components.
*/

#ifndef __TERRALIB_QT_AF_SETTINGSDIALOG_H
#define __TERRALIB_QT_AF_SETTINGSDIALOG_H

#include "../Config.h"

// Qt
#include <QDialog>
#include <QMap>

// Forward declarations
namespace Ui
{
  class SettingsDialogForm;
}

class QListWidgetItem;

namespace te
{
  namespace qt
  {
    namespace af
    {
      class TEQTAFEXPORT SettingsDialog : public QDialog
      {
        Q_OBJECT

        public:

          SettingsDialog (QWidget* parent=0);

          ~SettingsDialog();

        protected slots:

          void	settingsChanged (QListWidgetItem* current, QListWidgetItem* previous);

          void updateApplyButtonState(const bool& state);

        protected:

          QMap<QString, int> m_widPos;

        private:

          Ui::SettingsDialogForm* m_ui;
      };
    }
  }
}

#endif //__TERRALIB_QT_AF_SETTINGSDIALOG_H

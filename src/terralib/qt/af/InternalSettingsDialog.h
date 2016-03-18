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
\file terralib/qt/af/InternalSettingsDialog.h

\brief A Dialog to be used to inform about internal settings.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_INTERNALSETTINGSDIALOG_H
#define __TERRALIB_QT_AF_INTERNAL_INTERNALSETTINGSDIALOG_H

#include "Config.h"

//Qt
#include <QDialog>

namespace Ui {
  class InternalSettingsDialogForm;
}

namespace te
{
  namespace qt
  {
    namespace af
    {
      class TEQTAFEXPORT InternalSettingsDialog : public QDialog
      {
        Q_OBJECT

        public:

          InternalSettingsDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          virtual ~InternalSettingsDialog();

        protected:

          void setPaths();

          void setModules();

          void setPlugins();

          void setSystemInfo();

        private:

          Ui::InternalSettingsDialogForm* m_ui;
      };
    }
  }
}

#endif //__TERRALIB_QT_AF_INTERNAL_INTERNALSETTINGSDIALOG_H

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
  \file ProjectWidget.h

  \brief A frame for setting project options.
*/

#ifndef __TERRALIB_QT_AF_PROJECTWIDGET_H
#define __TERRALIB_QT_AF_PROJECTWIDGET_H

// TerraLib
#include "../Config.h"
#include "AbstractSettingWidget.h"

// Forwar declaration
namespace Ui 
{ 
  class ProjectWidgetForm; 
}

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class ProjectWidget

        \brief A frame for setting grid options.
      */
      class TEQTAFEXPORT ProjectWidget : public AbstractSettingWidget
      {
        Q_OBJECT

        public:

          ProjectWidget(QWidget* parent = 0);

          ~ProjectWidget();

          void saveChanges();

          void resetState();

          virtual void getHelpInformations(QString& ns, QString& helpFile);

        protected slots:

          void authorChanged(const QString& text);

          void maxSavedChanged(int i);

          void onOpeningOptionChanged();

        private:

          Ui::ProjectWidgetForm* m_ui;
      };

    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif // __TERRALIB_QT_AF_PROJECTWIDGET_H

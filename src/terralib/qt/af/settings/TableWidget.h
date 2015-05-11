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
  \file TableWidget.h

  \brief A frame for setting Table options.
*/

#ifndef __TERRALIB_QT_AF_TABLEWIDGET_H
#define __TERRALIB_QT_AF_TABLEWIDGET_H

// TerraLib
#include "../Config.h"
#include "AbstractSettingWidget.h"

// Forwar declaration
namespace Ui 
{ 
  class TableWidgetForm; 
}

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class TableWidget

        \brief A frame for setting Table options.
      */
      class TEQTAFEXPORT TableWidget : public AbstractSettingWidget
      {
        Q_OBJECT

        public:

          TableWidget(QWidget* parent = 0);

          ~TableWidget();

          virtual void saveChanges();

          virtual void resetState();

          virtual void getHelpInformations(QString& ns, QString& helpFile);

        public slots:

          void onApplyButtonClicked();

          void onAlternateRowColorsGroupBoxClicked();

          void onPrimaryPushButtonClicked();

          void onSecondaryPushButtonClicked();

        private:

          Ui::TableWidgetForm* m_ui;
          QColor m_primaryColor;
          QColor m_secondaryColor;

      };

    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif // __TERRALIB_QT_AF_TABLEWIDGET_H


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
  \file terralib/qt/widgets/utils/DoubleListWidget.h

  \brief ...
*/

#ifndef __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_DOUBLELISTWIDGET_H
#define __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_DOUBLELISTWIDGET_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

class QListWidgetItem;

namespace Ui { class DoubleListWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DoubleListWidget

        \brief ...
      */
      class TEQTWIDGETSEXPORT DoubleListWidget : public QWidget
      {
        Q_OBJECT

        public:

          DoubleListWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DoubleListWidget();

          Ui::DoubleListWidgetForm* getForm() const;

          void setLeftLabel(std::string value);

          void setRightLabel(std::string value);

          void setInputValues(std::vector<std::string> values);

          void setOutputValues(std::vector<std::string> values);

          void setFixedOutputValues(std::vector<std::string> values, std::string iconName);

          std::vector<std::string> getOutputValues();

          void clearInputValues();

          void clearOutputValues();

        protected slots:

          void onAddToolButtonPressed();

          void onAddAllToolButtonPressed();

          void onRemoveToolButtonPressed();

          void onRemoveAllToolButtonPressed();

          void onLeftListItemSelectionChanged();

          void onRightListItemSelectionChanged();

          void onUpToolButtonPressed();

          void onDownToolButtonPressed();

        signals:

          void itemChanged();

        private:

          std::auto_ptr<Ui::DoubleListWidgetForm> m_ui;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_DOUBLELISTWIDGET_H


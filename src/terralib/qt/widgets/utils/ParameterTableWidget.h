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
  \file terralib/qt/widgets/utils/ParameterTableWidget.h

  \brief ...
*/

#ifndef __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_PARAMETERTABLEWIDGET_H
#define __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_PARAMETERTABLEWIDGET_H

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <memory>
#include <string>

// Qt
#include <QWidget>

namespace Ui { class ParameterTableWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ParameterTableWidget

        \brief ...
      */
      class TEQTWIDGETSEXPORT ParameterTableWidget : public QWidget
      {
        Q_OBJECT

        public:

          ParameterTableWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ParameterTableWidget();

          Ui::ParameterTableWidgetForm* getForm() const;

          void add(const std::string& name, const std::string& value);

          std::map<std::string, std::string> getMap() const;

        protected slots:

          void addToolButtonPressed();

          void removeToolButtonPressed();

          void editToolButtonPressed();

        private:

          std::auto_ptr<Ui::ParameterTableWidgetForm> m_ui;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_PARAMETERTABLEWIDGET_H


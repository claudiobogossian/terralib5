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
  \file terralib/qt/widgets/utils/ListWidget.h

  \brief ...
*/

#ifndef __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_LISTWIDGET_H
#define __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_LISTWIDGET_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

class QListWidgetItem;

namespace Ui { class ListWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ListWidget

        \brief ...
      */
      class TEQTWIDGETSEXPORT ListWidget : public QWidget
      {
        Q_OBJECT

        public:

          ListWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ListWidget();

          Ui::ListWidgetForm* getForm() const;

          void setLabel(std::string value);

          std::vector<std::string> getValues();

        protected slots:

          void onAddToolButtonPressed();

          void onRemoveToolButtonPressed();

          void onEditToolButtonPressed();

          void onUpToolButtonPressed();

          void onDownToolButtonPressed();

          void onListPressed(QListWidgetItem* item);

        private:

          std::auto_ptr<Ui::ListWidgetForm> m_ui;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_DOUBLELISTWIDGET_H


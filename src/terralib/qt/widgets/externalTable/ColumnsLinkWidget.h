/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file  terralib/qt/widgets/externalTable/ColumnsLinkWidget.h

  \brief A class used to define the column used to connect two datasets
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_COLUMNSLINKWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_COLUMNSLINKWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class ColumnsLinkWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class ColumnsLinkWidget;
      /*!
        \class ColumnsLinkWidget

        \brief This class is a wizard page with the ColumnsLinkWidget component.
      */
      class TEQTWIDGETSEXPORT ColumnsLinkWidget : public QWidget
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor

            It constructs a ColumnsLinkWidget.

            \param parent this widget's parent
            \param f Window flags used to configure this widget

          */
          ColumnsLinkWidget(QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~ColumnsLinkWidget();

        signals:

          void itemChanged();

        private:

          std::auto_ptr<Ui::ColumnsLinkWidgetForm>  m_ui;  //!< The widget's form.
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_COLUMNSLINKWIDGET_H
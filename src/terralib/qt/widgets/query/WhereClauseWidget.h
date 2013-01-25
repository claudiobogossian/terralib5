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
  \file terralib/qt/widgets/query/WhereClauseWidget.h

  \brief This file has the DataSetWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_WHERECLAUSEWIDGET_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_WHERECLAUSEWIDGET_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWidget>


namespace Ui { class WhereClauseWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class WhereClauseWidget

        \brief This class is used to define the WHERE object of select operation.
      */
      class TEQTWIDGETSEXPORT WhereClauseWidget : public QWidget
      {
        Q_OBJECT

        public:

          WhereClauseWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~WhereClauseWidget();

          Ui::WhereClauseWidgetForm* getForm() const;


        public:

          void setAttributeList(const std::vector<std::string>& vec);


        public slots:

          void onAddWhereClausePushButtonClicked();

          void onRemoveWhereClausePushButtonClicked();


        private:

          std::auto_ptr<Ui::WhereClauseWidgetForm> m_ui;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_WHERECLAUSEWIDGET_H

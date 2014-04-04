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
  \file terralib/qt/widgets/query/OrderByWidget.h

  \brief This file has the OrderByWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_ORDERBYWIDGET_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_ORDERBYWIDGET_H

// TerraLib
#include "../../../dataaccess/query/OrderBy.h"
#include "../Config.h"

// STL
#include <memory>
#include <map>

// Qt
#include <QWidget>


namespace Ui { class OrderByWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class OrderByWidget

        \brief This class is used to define the ORDER BY object of select operation.
      */
      class TEQTWIDGETSEXPORT OrderByWidget : public QWidget
      {
        Q_OBJECT

        public:

          OrderByWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~OrderByWidget();

          Ui::OrderByWidgetForm* getForm() const;

          te::da::OrderBy* gerOrderBy();


        public:

          void setAttributeList(const std::vector<std::string>& vec);


        protected:

          void getOrderItems(std::vector<std::pair<std::string, std::string> >& list); // pair namer, order type

        public slots:

          void onAddOrderPushButton();

          void onRemoveOrderPushButton();


        private:

          std::auto_ptr<Ui::OrderByWidgetForm> m_ui;

          std::map<std::string, int > m_orderTypes;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_ORDERBYWIDGET_H
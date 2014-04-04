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
  \file terralib/qt/widgets/query/DataSetWidget.h

  \brief This file has the DataSetWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_DATASETWIDGET_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_DATASETWIDGET_H

// TerraLib
#include "../../../dataaccess/query/From.h"
#include "../Config.h"

// STL
#include <memory>
#include <vector>
#include <string>

// Qt
#include <QWidget>

// BOOST
#include <boost/ptr_container/ptr_vector.hpp>


namespace Ui { class DataSetWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataSetWidget

        \brief This class is used to define the FROM object of select operation.
      */
      class TEQTWIDGETSEXPORT DataSetWidget : public QWidget
      {
        Q_OBJECT

        public:

          DataSetWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DataSetWidget();

          Ui::DataSetWidgetForm* getForm() const;

          te::da::From* getFrom();

          void setFrom(const te::da::From* from);

        public:

          void setDataSetNames(const std::vector<std::string>& names);

          void setDataSetNames(std::vector<std::pair<std::string, std::string> >& list); // pair dsName, dsAlias

          void getDataSetNames(std::vector<std::pair<std::string, std::string> >& list); // pair dsName, dsAlias

        public slots:

          void onDataSetComboBoxActivated(const QString& value);

          void onAddDataSetPushButtonClicked();

          void onRemoveDataSetPushButtonClicked();


        signals:

          void itemChanged();


        private:

         std::auto_ptr<Ui::DataSetWidgetForm> m_ui;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_DATASETWIDGET_H


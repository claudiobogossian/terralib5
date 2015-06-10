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
  \file terralib/qt/widgets/query/QueryBuilderWizard.h

  \brief A Qt dialog that allows users to create query builder based on TerraLib query framework.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYBUILDERWIZARD_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYBUILDERWIZARD_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWizard>

// Boost
#include <boost/bimap.hpp>

// Forward declaration
namespace Ui { class QueryBuilderWizardForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DoubleListWidget;

      class TEQTWIDGETSEXPORT QueryBuilderWizard : public QWizard
      {
        Q_OBJECT

          enum QueryBuilderOperator
          {
            QueryBuilderOperator_Plus,
            QueryBuilderOperator_Minus,
            QueryBuilderOperator_Multi,
            QueryBuilderOperator_Div,
            QueryBuilderOperator_Equal,
            QueryBuilderOperator_Dif,
            QueryBuilderOperator_Great,
            QueryBuilderOperator_GreatEqual,
            QueryBuilderOperator_Less,
            QueryBuilderOperator_LessEqual
          };

          enum QueryBuilderConnector
          {
            QueryBuilderConnector_AND,
            QueryBuilderConnector_OR,
            QueryBuilderConnector_NOT,
            QueryBuilderConnector_LIKE,
            QueryBuilderConnector_NONE
          };

          enum QueryBuilderOrder
          {
            QueryBuilderOrder_ASC,
            QueryBuilderOrder_DESC
          };

        public:

          QueryBuilderWizard(QWidget* parent);

          ~QueryBuilderWizard();

          virtual bool validateCurrentPage();

        protected:

          void getDataSourceIds();
          void getDataSets();
          void getProperties();
          void getOperators();
          void getConnectors();
          void getOrders();

          //check functions
          bool checkDataSourcePage();
          bool checkDataSetPage();
          bool checkPropertiesPage();
          bool checkWhereClausePage();
          bool checkGroupByPage();
          bool checkOrderByPage();

          bool buildQuery();

        public slots:

          void onDataSourceComboBoxActivated(const QString& value);
          void onDataSetComboBoxActivated(const QString& value);
          void onAddDataSetPushButtonClicked();
          void onRemoveDataSetPushButtonClicked();
          void onAddWhereClausePushButtonClicked();
          void onRemoveWhereClausePushButtonClicked();
          void onAddOrderPushButton();
          void onRemoveOrderPushButton();

        private:

          std::auto_ptr<Ui::QueryBuilderWizardForm> m_ui;
          std::auto_ptr<DoubleListWidget> m_propertyList;
          std::auto_ptr<DoubleListWidget> m_groupByList;

          boost::bimap<int, std::string> m_bimapOperator;
          boost::bimap<int, std::string> m_bimapConnector;
          boost::bimap<int, std::string> m_bimapOrder;
      };


    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYBUILDERWIZARD_H

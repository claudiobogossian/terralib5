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
  \file terralib/qt/widgets/query/WhereClauseWidget.h

  \brief This file has the DataSetWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_WHERECLAUSEWIDGET_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_WHERECLAUSEWIDGET_H

// TerraLib
#include "../Config.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/query/Expression.h"
#include "../../../dataaccess/query/Where.h"
#include "../../../maptools/AbstractLayer.h"

// STL
#include <memory>
#include <map>
#include <set>

// Qt
#include <QComboBox>
#include <QWidget>


namespace Ui { class WhereClauseWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      struct ExpressionProperty
      {
        /*! \brief Default constructor. */
        ExpressionProperty()
        {
          m_isAttributeCriteria = false;
          m_isSpatialCriteria = false;
          m_isPropertyValue = false;
          m_isValueValue = false;

          m_property = "";
          m_operator = "";
          m_value = "";

          m_expression = 0;
          m_valuesComboBox = 0;
        }

        ~ExpressionProperty()
        {
          delete m_expression;
        }

        bool m_isAttributeCriteria;
        bool m_isSpatialCriteria;

        bool m_isPropertyValue;
        bool m_isValueValue;

        std::string m_property;
        std::string m_operator;
        std::string m_value;

        te::da::Expression* m_expression;

        QComboBox* m_valuesComboBox;
      };

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

          te::da::Where* getWhere();

          std::string getWhereString();


        public:

          void setDataSource(const te::da::DataSourcePtr& ds);

          void setLayerList(std::list<te::map::AbstractLayerPtr>& layerList);

          void setFromItems(std::vector<std::pair<std::string, std::string> > vec);

          void setAttributeList(const std::vector<std::string>& vec);

          void setGeomAttributeList(const std::vector<std::string>& vec, int srid);

          void setOperatorsList(const std::vector<std::string>& vec);

          void setSpatialOperatorsList(const std::vector<std::string>& vec);

          void setConnectorsList(const std::vector<std::string>& vec);

          void clear();

          void resetInterface();


        public slots:

          void onAddWhereClausePushButtonClicked();

          void onRemoveWhereClausePushButtonClicked();

          void onValuePropertyRadioButtonClicked();

          void onClearAllPushButtonClicked();

          void onRestrictValueComboBoxActivated(QString value);

          void onComboBoxActivated(QString value);

        protected:

          te::da::Expression* getExpression(const QString& value, const std::string& propName);

          void copyCombo(QComboBox* input, QComboBox* output, std::string curValue = "");

          QStringList getPropertyValues(std::string propertyName);


        private:

          std::auto_ptr<Ui::WhereClauseWidgetForm> m_ui;

          te::da::DataSourcePtr m_ds;

          std::vector<std::pair<std::string, std::string> > m_fromItems;

          std::map<int, ExpressionProperty*> m_mapExp;

          std::map< QComboBox*, std::pair<int, int> > m_comboMap;

          QStringList m_connectorsList;

          int m_count;
          int m_srid;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_WHERECLAUSEWIDGET_H

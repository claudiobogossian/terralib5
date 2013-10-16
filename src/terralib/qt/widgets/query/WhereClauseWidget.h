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
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/query/Where.h"
#include "../../../maptools/AbstractLayer.h"

// STL
#include <memory>
#include <map>

// Qt
#include <QtGui/QWidget>


namespace Ui { class WhereClauseWidgetForm; }

namespace te
{
  namespace da { class Expression; }

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

          te::da::Where* getWhere();


        public:

          void setDataSource(const te::da::DataSourcePtr& ds);

          void setLayerList(std::list<te::map::AbstractLayerPtr>& layerList);

          void setFromItems(std::vector<std::pair<std::string, std::string> > vec);

          void setAttributeList(const std::vector<std::string>& vec);

          void setGeomAttributeList(const std::vector<std::string>& vec, int srid);

          void setOperatorsList(const std::vector<std::string>& vec);

          void setSpatialOperatorsList(const std::vector<std::string>& vec);

          void setConnectorsList(const std::vector<std::string>& vec);


        public slots:

          void onAddWhereClausePushButtonClicked();

          void onRemoveWhereClausePushButtonClicked();

          void onValuePropertyRadioButtonClicked();

        protected:

          te::da::Expression* getExpression(const QString& value, const std::string& propName);


        private:

          std::auto_ptr<Ui::WhereClauseWidgetForm> m_ui;

          te::da::DataSourcePtr m_ds;

          std::vector<std::pair<std::string, std::string> > m_fromItems;

          std::map<int, te::da::Expression*> m_mapExp;

          int m_count;
          int m_srid;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_WHERECLAUSEWIDGET_H

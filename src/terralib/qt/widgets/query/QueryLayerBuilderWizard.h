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
  \file terralib/qt/widgets/query/QueryLayerBuilderWizard.h

  \brief A Qt dialog that allows users to create query builder based on TerraLib query framework.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYLAYERBUILDERWIZARD_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYLAYERBUILDERWIZARD_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// Qt
#include <QWizard>

namespace te
{
  namespace da { class Select; }
  namespace qt
  {
    namespace widgets
    {
      class DataSourceSelectorWizardPage;
      class DataSetWizardPage;
      class FieldsWizardPage;
      class GroupByWizardPage;
      class LayerAttributesWizardPage;
      class OrderByWizardPage;
      class WhereClauseWizardPage;

      class TEQTWIDGETSEXPORT QueryLayerBuilderWizard : public QWizard
      {
        public:

          QueryLayerBuilderWizard(QWidget* parent);

          ~QueryLayerBuilderWizard();

          virtual bool validateCurrentPage();

          void setDataSource(const te::da::DataSourcePtr& ds);

          void setLayerList(std::list<te::map::AbstractLayerPtr>& layerList);

          te::da::Select getSelectQuery();

          te::map::AbstractLayerPtr getQueryLayer();

        protected:

          void addPages();

          void getDataSets();

          void getProperties();

          void getQueryCapabilities();

        private:

          std::auto_ptr<te::qt::widgets::DataSourceSelectorWizardPage> m_dataSourcePage;
          std::auto_ptr<te::qt::widgets::DataSetWizardPage> m_dataSetPage;
          std::auto_ptr<te::qt::widgets::FieldsWizardPage> m_fieldPage;
          std::auto_ptr<te::qt::widgets::GroupByWizardPage> m_groupByPage;
          std::auto_ptr<te::qt::widgets::LayerAttributesWizardPage> m_layerAttrPage;
          std::auto_ptr<te::qt::widgets::OrderByWizardPage> m_orderByPage;
          std::auto_ptr<te::qt::widgets::WhereClauseWizardPage> m_whereClausePage;

          te::da::DataSourcePtr m_ds;
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYLAYERBUILDERWIZARD_H

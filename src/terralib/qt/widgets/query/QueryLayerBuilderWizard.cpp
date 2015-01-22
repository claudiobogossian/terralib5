/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/query/QueryLayerBuilderWizard.cpp

  \brief A Qt dialog that allows users to create query builder based on TerraLib query framework.
*/

// TerraLib
#include "QueryLayerBuilderWizard.h"

// TerraLib 
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/query/QueryCapabilities.h"
#include "../../../dataaccess/query/Select.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/QueryLayer.h"
#include "../../../se/Utils.h"
#include "../../widgets/help/HelpPushButton.h"
#include "../datasource/selector/DataSourceSelectorWizardPage.h"
#include "../datasource/selector/DataSourceSelectorWidget.h"
#include "../utils/DoubleListWidget.h"
#include "DataSetWidget.h"
#include "DataSetWizardPage.h"
#include "FieldsWizardPage.h"
#include "GroupByWizardPage.h"
#include "LayerAttributesWizardPage.h"
#include "OrderByWidget.h"
#include "OrderByWizardPage.h"
#include "WhereClauseWidget.h"
#include "WhereClauseWizardPage.h"

// STL
#include <cassert>

// Qt
#include <QMessageBox>
#include <QIcon>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


te::qt::widgets::QueryLayerBuilderWizard::QueryLayerBuilderWizard(QWidget* parent)
  : QWizard(parent)
{
  m_ds.reset();

  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  //this->setFixedSize(640, 480);
  this->setWindowTitle(tr("Query Layer Builder"));

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("widgets/query/query_layer.html");

  addPages();
}

te::qt::widgets::QueryLayerBuilderWizard::~QueryLayerBuilderWizard()
{

}

bool te::qt::widgets::QueryLayerBuilderWizard::validateCurrentPage()
{
  if(currentPage() ==  m_dataSourcePage.get())
  {
    std::list<te::da::DataSourceInfoPtr> list = m_dataSourcePage->getSelectorWidget()->getSelecteds();

    if(list.empty())
    {
      return false;
    }

    te::da::DataSourceInfoPtr dsInfo = *list.begin();
    te::da::DataSourcePtr dataSource = te::da::DataSourceManager::getInstance().get(dsInfo->getId(), dsInfo->getType(), dsInfo->getConnInfo());

    if(!dataSource->isOpened())
      dataSource->open();

    setDataSource(dataSource);

    return m_dataSourcePage->isComplete();
  }
  else if(currentPage() ==  m_dataSetPage.get())
  {
    getProperties();

    //used to get distinct values from a selected property
    std::vector<std::pair<std::string, std::string> > vec;
    m_dataSetPage->getWidget()->getDataSetNames(vec);
    m_whereClausePage->getWidget()->setFromItems(vec);

    return m_dataSetPage->isComplete();
  }
  else if(currentPage() ==  m_fieldPage.get())
  {
    return m_fieldPage->isComplete();
  }
  else if(currentPage() ==  m_whereClausePage.get())
  {
    return m_whereClausePage->isComplete();
  }
  else if(currentPage() ==  m_groupByPage.get())
  {
    return m_groupByPage->isComplete();
  }
  else if(currentPage() ==  m_orderByPage.get())
  {
    return m_orderByPage->isComplete();
  }
  else if(currentPage() ==  m_layerAttrPage.get())
  {
    bool res = m_layerAttrPage->isComplete();

    if(!res)
      QMessageBox::warning(this, tr("Warning"), tr("Layer name not defined."));

    return res;
  }

  return false;
}

void te::qt::widgets::QueryLayerBuilderWizard::setDataSource(const te::da::DataSourcePtr& ds)
{
  assert(ds);

  m_ds = ds;

  m_whereClausePage->getWidget()->setDataSource(m_ds);

  getDataSets();

  getQueryCapabilities();
}

void te::qt::widgets::QueryLayerBuilderWizard::setLayerList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  if(m_whereClausePage.get())
    m_whereClausePage->getWidget()->setLayerList(layerList);
}

te::da::Select te::qt::widgets::QueryLayerBuilderWizard::getSelectQuery()
{
  //fields
  te::da::Fields* fields = m_fieldPage->getFields();

  //from
  te::da::From* from = m_dataSetPage->getWidget()->getFrom();

  //where
  te::da::Where* w = m_whereClausePage->getWidget()->getWhere();

  //groupby
  te::da::GroupBy* groupBy = m_groupByPage->getGroupBy();

  //order
  te::da::OrderBy* orderBy = m_orderByPage->getWidget()->gerOrderBy();

  //build the select object
  te::da::Select s(fields, from, w, groupBy, orderBy);

  return s;
}

te::map::AbstractLayerPtr te::qt::widgets::QueryLayerBuilderWizard::getQueryLayer()
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  std::string title = m_layerAttrPage->getLayerName();

  te::da::Select* s = new te::da::Select(getSelectQuery());

  te::map::QueryLayerPtr layer(new te::map::QueryLayer(id, title));
  layer->setDataSourceId(m_ds->getId());
  layer->setRendererType("QUERY_LAYER_RENDERER");
  layer->setQuery(s);
  layer->computeExtent();

  // SRID
  std::auto_ptr<const te::map::LayerSchema> schema(layer->getSchema());
  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(schema.get());
  layer->setSRID(gp->getSRID());
  
  // style
  layer->setStyle(te::se::CreateFeatureTypeStyle(gp->getGeometryType()));

  return layer;
}

void te::qt::widgets::QueryLayerBuilderWizard::addPages()
{
  m_dataSourcePage.reset(new te::qt::widgets::DataSourceSelectorWizardPage(this));
  m_dataSetPage.reset(new te::qt::widgets::DataSetWizardPage(this));
  m_fieldPage.reset(new te::qt::widgets::FieldsWizardPage(this));
  m_groupByPage.reset(new te::qt::widgets::GroupByWizardPage(this));
  m_layerAttrPage.reset(new te::qt::widgets::LayerAttributesWizardPage(this));
  m_orderByPage.reset(new te::qt::widgets::OrderByWizardPage(this));
  m_whereClausePage.reset(new te::qt::widgets::WhereClauseWizardPage(this));

  addPage(m_dataSourcePage.get());
  addPage(m_dataSetPage.get());
  addPage(m_fieldPage.get());
  addPage(m_whereClausePage.get());
  addPage(m_groupByPage.get());
  addPage(m_orderByPage.get());
  addPage(m_layerAttrPage.get());
}

void te::qt::widgets::QueryLayerBuilderWizard::getDataSets()
{
  std::string dsId = m_ds->getId();

  std::vector<std::string> datasetNames;

  te::da::GetDataSetNames(datasetNames, dsId);

  m_dataSetPage->getWidget()->setDataSetNames(datasetNames);
}

void te::qt::widgets::QueryLayerBuilderWizard::getProperties()
{
  //get the dataset names
  std::vector<std::string> datasetNames = m_ds->getDataSetNames();

  std::vector<std::pair<std::string, std::string> > dataSetSelecteds;

  m_dataSetPage->getWidget()->getDataSetNames(dataSetSelecteds);

  std::vector<std::string> inputProperties;
  std::vector<std::string> geomProperties;

  int srid = 0;

  //get properties for each data set
  for(size_t t = 0; t < dataSetSelecteds.size(); ++t)
  {
    //alias name
    std::string alias = dataSetSelecteds[t].second;

    //data set name
    std::string dataSetName = dataSetSelecteds[t].first;

    //get datasettype
    std::auto_ptr<te::da::DataSetType> dsType(0);

    for(unsigned int i = 0; i < datasetNames.size(); ++i)
    {
      if(datasetNames[i] == dataSetName)
        dsType = m_ds->getDataSetType(datasetNames[i]);
    }

    if(dsType.get())
    {
      for(size_t i = 0; i < dsType->size(); ++i)
      {
        std::string propName = dsType->getProperty(i)->getName();
        std::string fullName = alias + "." + propName;

        if(dsType->getProperty(i)->getType() == te::dt::GEOMETRY_TYPE)
        {
          te::gm::GeometryProperty* geomProp = dynamic_cast<te::gm::GeometryProperty*>(dsType->getProperty(i));
          
          if(geomProp)
            srid = geomProp->getSRID();

          geomProperties.push_back(fullName);
        }
        else
          inputProperties.push_back(fullName);
      }
    }
  }

  //set values in other pages
  m_fieldPage->getWidget()->setInputValues(inputProperties);
  m_fieldPage->getWidget()->clearOutputValues();
  m_fieldPage->getWidget()->setFixedOutputValues(geomProperties, "geometry");
  m_groupByPage->getWidget()->setInputValues(inputProperties);
  m_whereClausePage->getWidget()->setAttributeList(inputProperties);
  m_whereClausePage->getWidget()->setGeomAttributeList(geomProperties, srid);
  m_orderByPage->getWidget()->setAttributeList(inputProperties);
}

void te::qt::widgets::QueryLayerBuilderWizard::getQueryCapabilities()
{

  te::da::DataSourceCapabilities dsCap = m_ds->getCapabilities();

  te::da::QueryCapabilities queryCap = dsCap.getQueryCapabilities();

  std::vector<std::string> vecOperators;

  std::set<std::string>::iterator it;

  //Arithmetic Operators
  it = queryCap.getArithmeticOperators().begin();

  while(it != queryCap.getArithmeticOperators().end())
  {
    vecOperators.push_back(*it);

    ++it;
  }

  //Comparsion Operators
  it = queryCap.getComparsionOperators().begin();

  while(it != queryCap.getComparsionOperators().end())
  {
    vecOperators.push_back(*it);

    ++it;
  }

  m_whereClausePage->getWidget()->setOperatorsList(vecOperators);

  //Spatial Operators
  std::vector<std::string> vecSpatialOperators;

  it = queryCap.getSpatialTopologicOperators().begin();

  while(it != queryCap.getSpatialTopologicOperators().end())
  {
    vecSpatialOperators.push_back(*it);

    ++it;
  }

  m_whereClausePage->getWidget()->setSpatialOperatorsList(vecSpatialOperators);

  //Logical Operators
  std::vector<std::string> vecConnectors;
  
  it = queryCap.getLogicalOperators().begin();

  while(it != queryCap.getLogicalOperators().end())
  {
    vecConnectors.push_back(*it);

    ++it;
  }

  m_whereClausePage->getWidget()->setConnectorsList(vecConnectors);
}

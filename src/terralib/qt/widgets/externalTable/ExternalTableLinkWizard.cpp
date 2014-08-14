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
  \file terralib/qt/widgets/query/ExternalTableLinkWizard.cpp

  \brief A Qt dialog that allows users to create query builder based on TerraLib query framework.
*/

// TerraLib
#include "ExternalTableLinkWizard.h"

// TerraLib 
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/query/QueryCapabilities.h"
#include "../../../dataaccess/query/Select.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/DataSetAdapterLayer.h"
#include "../../../maptools/QueryLayer.h"
#include "../../../se/Utils.h"
#include "../../widgets/help/HelpPushButton.h"
#include "../utils/DoubleListWidget.h"
#include "../query/DataSetWidget.h"
#include "../query/DataSetWizardPage.h"
#include "../query/FieldsWizardPage.h"
#include "../query/LayerAttributesWizardPage.h"
#include "ColumnsLinkWizardPage.h"
#include "ColumnsLinkWidget.h"
#include "ui_DataSetWidgetForm.h"

// STL
#include <cassert>

// Qt
#include <QIcon>
#include <QMessageBox>
#include <QToolButton>


// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::qt::widgets::ExternalTableLinkWizard::ExternalTableLinkWizard(QWidget* parent)
  : QWizard(parent)
{
  m_ds.reset();

  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setMinimumSize(640, 480);
  this->setWindowTitle(tr("External Table Link"));

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

//  helpButton->setPageReference("widgets/externalTable/linking.html");

  addPages();
}

te::qt::widgets::ExternalTableLinkWizard::~ExternalTableLinkWizard()
{

}

bool te::qt::widgets::ExternalTableLinkWizard::validateCurrentPage()
{
  if(currentPage() ==  m_dataSetPage.get())
  {
    getProperties();

    //used to get distinct values from a selected property
    std::vector<std::pair<std::string, std::string> > vec;
    m_dataSetPage->getWidget()->getDataSetNames(vec);

    //m_whereClausePage->getWidget()->setFromItems(vec);

    return m_dataSetPage->isComplete();
  }
  else if(currentPage() ==  m_fieldPage.get())
  {
    return m_fieldPage->isComplete();
  }
  else if(currentPage() ==  m_columnsLinkPage.get())
  {
    return m_columnsLinkPage->isComplete();
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

void te::qt::widgets::ExternalTableLinkWizard::setInputLayer(te::map::AbstractLayerPtr inLayer)
{
  assert(inLayer);

  m_inputLayer.reset((te::map::DataSetLayer*)inLayer.get());
  m_ds = te::da::DataSourceManager::getInstance().find(m_inputLayer->getDataSourceId());
  m_ds->getType();

//  m_whereClausePage->getWidget()->setDataSource(m_ds);

  getDataSets();
  //getQueryCapabilities();
}

te::da::Select te::qt::widgets::ExternalTableLinkWizard::getSelectQuery()
{
  //fields
  te::da::Fields* fields = m_fieldPage->getFields();

  //from
  te::da::From* from = m_dataSetPage->getWidget()->getFrom();

  //where
//  te::da::Where* w = m_whereClausePage->getWidget()->getWhere();

  //build the select object
  te::da::Select s(fields, from);

  return s;
}

te::map::AbstractLayerPtr te::qt::widgets::ExternalTableLinkWizard::getQueryLayer()
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  std::string title = m_layerAttrPage->getLayerName();

  te::da::Select* s = new te::da::Select(getSelectQuery());

  te::map::QueryLayerPtr layer(new te::map::QueryLayer(id, title));
  layer->setDataSourceId(m_ds->getId());
  layer->setRendererType("ABSTRACT_LAYER_RENDERER");
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

void te::qt::widgets::ExternalTableLinkWizard::addPages()
{
  m_dataSetPage.reset(new te::qt::widgets::DataSetWizardPage(this));
  m_dataSetPage->setSubTitle(tr("Select the dataset to be linked."));

  m_fieldPage.reset(new te::qt::widgets::FieldsWizardPage(this));
  m_columnsLinkPage.reset(new te::qt::widgets::ColumnsLinkWizardPage(this));
  m_layerAttrPage.reset(new te::qt::widgets::LayerAttributesWizardPage(this));

  addPage(m_dataSetPage.get());
  addPage(m_fieldPage.get());
  addPage(m_columnsLinkPage.get());
  addPage(m_layerAttrPage.get());
}

void te::qt::widgets::ExternalTableLinkWizard::getDataSets()
{
  std::string dsId = m_ds->getId();

  std::vector<std::string> datasetNames;

  te::da::GetDataSetNames(datasetNames, dsId);

  for(std::size_t i = 0; i < datasetNames.size(); ++i)
  {
    if(datasetNames[i] == m_inputLayer->getDataSetName())
    {
      std::string alias = datasetNames[i];

      int pos = datasetNames[i].find(".");
      if(pos != std::string::npos)
        alias = datasetNames[i].substr(pos + 1, datasetNames[i].size() - 1);

      //Including the input layer on the dataset page list
      int newrow = m_dataSetPage->getWidget()->getForm()->m_dataSetTableWidget->rowCount();

      //new entry
      m_dataSetPage->getWidget()->getForm()->m_dataSetTableWidget->insertRow(newrow);

      QTableWidgetItem* itemDataSet = new QTableWidgetItem(QString::fromStdString(datasetNames[i]));
      m_dataSetPage->getWidget()->getForm()->m_dataSetTableWidget->setItem(newrow, 1, itemDataSet);

      QTableWidgetItem* itemAlias = new QTableWidgetItem(QString::fromStdString(alias));
      m_dataSetPage->getWidget()->getForm()->m_dataSetTableWidget->setItem(newrow, 2, itemAlias);

      m_dataSetPage->getWidget()->getForm()->m_dataSetTableWidget->resizeColumnToContents(0);
    }
  }

  m_dataSetPage->getWidget()->setDataSetNames(datasetNames);
}

void te::qt::widgets::ExternalTableLinkWizard::getProperties()
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

  //m_whereClausePage->getWidget()->setAttributeList(inputProperties);
  //m_whereClausePage->getWidget()->setGeomAttributeList(geomProperties, srid);
}

void te::qt::widgets::ExternalTableLinkWizard::getQueryCapabilities()
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

//  m_whereClausePage->getWidget()->setOperatorsList(vecOperators);

  //Spatial Operators
  std::vector<std::string> vecSpatialOperators;

  it = queryCap.getSpatialTopologicOperators().begin();

  while(it != queryCap.getSpatialTopologicOperators().end())
  {
    vecSpatialOperators.push_back(*it);

    ++it;
  }

//  m_whereClausePage->getWidget()->setSpatialOperatorsList(vecSpatialOperators);

  //Logical Operators
  std::vector<std::string> vecConnectors;
  
  it = queryCap.getLogicalOperators().begin();

  while(it != queryCap.getLogicalOperators().end())
  {
    vecConnectors.push_back(*it);

    ++it;
  }

//  m_whereClausePage->getWidget()->setConnectorsList(vecConnectors);
}

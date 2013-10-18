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
  \file terralib/qt/widgets/query/QueryDialog.h

  \brief This file defines a class for a Query Dialog Dialog.
*/

// TerraLib
#include "../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../../dataaccess/query/QueryCapabilities.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../maptools/DataSetLayer.h"
#include "QueryDialog.h"
#include "WhereClauseWidget.h"
#include "ui_QueryDialogForm.h"

// Qt
#include <QGridLayout>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::QueryDialog::QueryDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::QueryDialogForm)
{
  m_ui->setupUi(this);

//build form
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  m_whereClauseWidget.reset( new te::qt::widgets::WhereClauseWidget(m_ui->m_widget));
  layout->addWidget(m_whereClauseWidget.get());
  layout->setContentsMargins(0,0,0,0);

//connectors
  connect(m_ui->m_inputLayerComboBox, SIGNAL(activated(QString)), this, SLOT(onInputLayerActivated(QString)));
}

te::qt::widgets::QueryDialog::~QueryDialog()
{
}

te::qt::widgets::WhereClauseWidget* te::qt::widgets::QueryDialog::getWidget()
{
  return m_whereClauseWidget.get();
}

void te::qt::widgets::QueryDialog::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_whereClauseWidget->setLayerList(layerList);

  m_ui->m_inputLayerComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

    if(dsLayer)
      m_ui->m_inputLayerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }

  if(m_ui->m_inputLayerComboBox->count() > 0)
  {
    QString s = m_ui->m_inputLayerComboBox->currentText();

    onInputLayerActivated(s);
  }
}

te::da::Where* te::qt::widgets::QueryDialog::getWhere()
{
  return m_whereClauseWidget->getWhere();
}

void te::qt::widgets::QueryDialog::onInputLayerActivated(QString value)
{
  m_whereClauseWidget->clear();

// get input layer
  int idxLayer = m_ui->m_inputLayerComboBox->currentIndex();

  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(idxLayer, Qt::UserRole);

  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());

// set data source
  std::string dataSourceName = dsLayer->getDataSourceId();

  te::da::DataSourcePtr dsPtr = te::da::GetDataSource(dataSourceName);

  if(!dsPtr->isOpened())
      dsPtr->open();

  m_whereClauseWidget->setDataSource(dsPtr);

//get capabilities
  std::vector<std::string> vecOperators;
  std::vector<std::string> vecSpatialOperators;
  std::vector<std::string> vecConnectors;

  te::da::DataSourceCapabilities dsCap = dsPtr->getCapabilities();
  te::da::QueryCapabilities queryCap = dsCap.getQueryCapabilities();

  std::set<std::string>::iterator it;

  it = queryCap.getArithmeticOperators().begin();
  while(it != queryCap.getArithmeticOperators().end())
  {
    vecOperators.push_back(*it);
    ++it;
  }

  it = queryCap.getComparsionOperators().begin();
  while(it != queryCap.getComparsionOperators().end())
  {
    vecOperators.push_back(*it);
    ++it;
  }

  m_whereClauseWidget->setOperatorsList(vecOperators);

  it = queryCap.getSpatialTopologicOperators().begin();
  while(it != queryCap.getSpatialTopologicOperators().end())
  {
    vecSpatialOperators.push_back(*it);
    ++it;
  }

  m_whereClauseWidget->setSpatialOperatorsList(vecSpatialOperators);

  it = queryCap.getLogicalOperators().begin();
  while(it != queryCap.getLogicalOperators().end())
  {
    vecConnectors.push_back(*it);
    ++it;
  }

  m_whereClauseWidget->setConnectorsList(vecConnectors);

//get data set information
  std::auto_ptr<te::da::DataSetType> dsType = dsLayer->getSchema();
  
  std::string dsName = dsType->getName();
  int pos = dsName.find(".");
  std::string aliasName = dsName.substr(pos + 1, dsName.size() - 1);

  std::vector<std::pair<std::string, std::string> > list;
  list.push_back(std::pair<std::string, std::string> (dsName, aliasName));

  m_whereClauseWidget->setFromItems(list);

//get properties
  std::vector<std::string> inputProperties;
  std::vector<std::string> geomProperties;

  for(size_t i = 0; i < dsType->size(); ++i)
  {
    std::string propName = dsType->getProperty(i)->getName();
    std::string fullName = aliasName + "." + propName;

    if(dsType->getProperty(i)->getType() == te::dt::GEOMETRY_TYPE)
      geomProperties.push_back(fullName);
    else
      inputProperties.push_back(fullName);
  }

  m_whereClauseWidget->setAttributeList(inputProperties);
  m_whereClauseWidget->setGeomAttributeList(geomProperties, dsLayer->getSRID());
}


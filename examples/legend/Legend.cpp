/*  Copyright (C) 2001-2011 National Institute For Space Research (INPE) - Brazil.

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

// Qt
#include <QMessageBox>
#include <QGridLayout>

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/qt/widgets.h>
#include <terralib/qt/widgets/layer/Legend.h>

#include "Legend.h"

Legend::Legend(QWidget* parent)
  : m_ds(0), m_t(0), m_rootLayer(0), m_layerModel(0),
    m_layerView(0), QDialog(parent)
{
  // Get a connection to a data source and a data source name
  std::map<std::string, std::string> connInfo;
  connInfo["host"] = "atlas.dpi.inpe.br";
  //connInfo["host"] = "localhost";
  connInfo["user"] = "postgres";
  connInfo["password"] = "sitim110";
  connInfo["dbname"] = "terralib4";
  connInfo["connect_timeout"] = "4"; 

  m_ds = te::da::DataSourceFactory::make("PostGIS");
  m_ds->open(connInfo);

  // Get a data source catalog loader to access the datasource catalog
  m_t = m_ds->getTransactor();
  te::da::DataSourceCatalogLoader* catalogLoader = m_t->getCatalogLoader();
    
  // Load the catalog to find out the information in the data source (only the schemas)
  //catalogLoader->loadCatalog();

  // Get the dataset names of the data source
  std::vector<std::string*> datasets;
  catalogLoader->getDataSets(datasets);

  // Sort the dataset names
  QStringList dataSetList;

  size_t numDataSets = datasets.size();
  for (size_t i = 0; i < numDataSets; ++i)
    dataSetList << (*datasets[i]).c_str();

  dataSetList.sort();

  // Mount a layer tree where each element corresponds to a dataset
  std::string id;
  std::string dataSetName;
  te::map::Layer* layer;

  m_rootLayer = new te::map::FolderLayer("0", "Layers");

  for(size_t i = 0; i < numDataSets; ++i)
  {
    id = te::common::Convert2String(i+1);
    dataSetName = dataSetList[i].toStdString();
    
    layer = new te::map::Layer(id, dataSetName, m_rootLayer);
    layer->setDataSource(m_ds);
    layer->setDataSetName(dataSetName);
  }

  // Create the layer explorer model
  m_layerModel = new te::qt::widgets::LayerExplorerModel(m_rootLayer, 0);

  // Create the layer view and set its model
  m_layerView = new te::qt::widgets::LayerExplorer(this);
  m_layerView->setModel(m_layerModel);

  QObject::connect(m_layerModel, SIGNAL(dragDropEnded(const QModelIndex&, const QModelIndex&)),
                   m_layerView, SLOT(updateCurrentIndex(const QModelIndex&, const QModelIndex&)));

  m_layerView->setDragEnabled(true);
  m_layerView->setAcceptDrops(true);
  m_layerView->setDropIndicatorShown(true);

  QGridLayout* mainLayout = new QGridLayout;
  mainLayout->addWidget(m_layerView);
  setLayout(mainLayout);

  QAction* editLegendAction = new QAction(QObject::tr("Edit Legend..."), m_layerView);
  editLegendAction->setStatusTip(QObject::tr("Edit Legend"));
  QObject::connect(editLegendAction, SIGNAL(triggered()), this, SLOT(editLegendSlot()));

  QList<QAction*> actionsList;
  actionsList << editLegendAction;

  m_layerView->setActionsList(actionsList);
}

Legend::~Legend()
{
}

void Legend::editLegendSlot()
{
  QModelIndex currentIndex = m_layerView->currentIndex();

  te::qt::widgets::LayerItem* layerItem = static_cast<te::qt::widgets::LayerItem*>(m_layerModel->getItem(currentIndex));

  te::qt::widgets::Legend legendDialog(layerItem);

  if(legendDialog.exec() != QDialog::Accepted)
    return;

  // Reset the model to take into account changes in the tree of layers
  m_layerModel->resetModel();
}

void Legend::closeEvent(QCloseEvent* /*e*/)
{
  delete m_rootLayer;
  delete m_ds;
}

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
#include <QComboBox>
#include <QMessageBox>
#include <QMenu>

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/qt/widgets.h>

#include "DataGrid.h"

DataGrid::DataGrid(QWidget* parent)
  : m_ds(0), m_transactor(0), m_catalogLoader(0),
    m_dataGridOp(0), m_opTableView(0), m_dataSetType(0),
    m_dataSet(0), QDialog(parent)
{
  setupUi(this);

  // Instantiate the class GridDataOperation
  m_dataGridOp = new te::map::DataGridOperation();

  // Instantiate the table view that will accomplish data grid operations
  m_opTableView = new te::qt::widgets::DataGridView(dataGridGroupBox);
  gridLayout_2->addWidget(m_opTableView, 0, 0, 1, 1);
  m_opTableView->setVisible(false);

  // Instantiate the data grid model
  m_dataGridModel = new te::qt::widgets::DataGridModel();
  //m_tableView->setModel(m_dataGridModel);

  // Set the server names available
  serverComboBox->addItem("atlas.dpi.inpe.br");
  serverComboBox->addItem("localhost");

  // Set the actions for the menu of the vertical header of the grid
  QAction* promotePointedRowsAction = new QAction(QObject::tr("Promote the Pointed Rows"), m_opTableView);
  promotePointedRowsAction->setStatusTip(QObject::tr("Promote the rows pointed"));
  QObject::connect(promotePointedRowsAction, SIGNAL(triggered()), m_opTableView, SLOT(promotePointedRows()));

  QAction* promoteQueriedRowsAction = new QAction(QObject::tr("Promote the Queried Rows"), m_opTableView);
  promoteQueriedRowsAction->setStatusTip(QObject::tr("Promote the rows queried"));
  QObject::connect(promoteQueriedRowsAction, SIGNAL(triggered()), m_opTableView, SLOT(promoteQueriedRows()));

  te::qt::widgets::HeaderView* vertHeaderView = m_opTableView->getVerticalHeaderView();
  QMenu* vertHeaderMenu = new QMenu(vertHeaderView);
  vertHeaderMenu->addAction(promotePointedRowsAction);
  vertHeaderMenu->addAction(promoteQueriedRowsAction);

  vertHeaderView->setVerticalHeaderMenu(vertHeaderMenu);

  // Set the actions for the menu of the horizontal header of the grid
  QAction* sortAscAction = new QAction(QObject::tr("Sort Selected Columns in Ascendent Order"), m_opTableView);
  sortAscAction->setStatusTip(QObject::tr("Sort the selected columns in ascendent order"));
  QObject::connect(sortAscAction, SIGNAL(triggered()), m_opTableView, SLOT(sortColumnsInAscendentOrder()));

  QAction* sortDescAction = new QAction(QObject::tr("Sort Selected Columns in Descendent Order"), m_opTableView);
  sortDescAction->setStatusTip(QObject::tr("Sort the selected columns in descendent order"));
  QObject::connect(sortDescAction, SIGNAL(triggered()), m_opTableView, SLOT(sortColumnsInDescendentOrder()));

  te::qt::widgets::HeaderView* horizHeaderView = m_opTableView->getHorizontalHeaderView();
  QMenu* horizHeaderMenu = new QMenu(horizHeaderView);
  horizHeaderMenu->addAction(sortAscAction);
  horizHeaderMenu->addAction(sortDescAction);

  horizHeaderView->setHorizontalHeaderMenu(horizHeaderMenu);

  // Set the actions for the menu of the grid viewport
  QAction* removeAllSelectionsAction = new QAction(QObject::tr("Remove All Selections"), m_opTableView);
  removeAllSelectionsAction->setStatusTip(QObject::tr("Remove All Selections"));
  QObject::connect(removeAllSelectionsAction, SIGNAL(triggered()), m_opTableView, SLOT(removeAllSelections()));

  QMenu* viewportMenu = new QMenu(m_opTableView);
  viewportMenu->addAction(removeAllSelectionsAction);
  m_opTableView->setViewportMenu(viewportMenu);

  serverComboBoxChanged(serverComboBox->currentText());

  // Connect the signals/slots
  connect(serverComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(serverComboBoxChanged(const QString&)));
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
}

DataGrid::~DataGrid()
{
  // Release the transactor
  if (m_transactor)
   delete m_transactor;
}

void DataGrid::serverComboBoxChanged(const QString& server)
{
  if (m_transactor)
   delete m_transactor;

  if(m_ds)
  {
    m_ds->close();
    delete m_ds;
  } 
  
  std::map<std::string, std::string> connInfo;
  connInfo["host"] = server.toStdString();
  connInfo["user"] = "postgres";
  connInfo["password"] = "sitim110";
  connInfo["dbname"] = "terralib4";
  connInfo["connect_timeout"] = "4";

  try
  {
    m_ds = te::da::DataSourceFactory::make("PostGIS");
    m_ds->open(connInfo);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(this, tr("Connection to the Data Source Failed"), e.what());
    return;
  }

  // Get a data source catalog loader to access the datasource catalog
  m_transactor = m_ds->getTransactor();
  m_catalogLoader = m_transactor->getCatalogLoader();
    
  // Load the catalog to find out the information in the data source (only the schemas)
  m_catalogLoader->loadCatalog();

  // Get the dataset names of the data source
  std::vector<std::string*> datasets;
  m_catalogLoader->getDataSets(datasets);

  dataSetComboBox->clear();

  // Fill alphabetically the dataSetCombobox with the dataset names of the data source
  QStringList dataSetList;

  size_t numDataSets = datasets.size();
  for (size_t i = 0; i < numDataSets; ++i)
    dataSetList << (*datasets[i]).c_str();

  dataSetList.sort();
  dataSetComboBox->addItems(dataSetList);
}

void DataGrid::okPushButtonClicked()
{
  setCursor(Qt::WaitCursor);

  if(m_dataSetType)
  {
    delete m_dataSetType;
    m_dataSetType = 0;
  }

  if(m_dataSet)
  {
    delete m_dataSet;
    m_dataSet = 0;
  }

  std::string dataSetName = dataSetComboBox->currentText().toStdString();

  // Get the DataSetType and the DataSet associated to the dataset selected
  if(dataSetComboBox->currentText().isEmpty() == false)
  {
    m_dataSetType = m_catalogLoader->getDataSetType(dataSetName, true);
    m_dataSetType->setCatalog(m_ds->getCatalog());
    m_dataSet = m_transactor->getDataSet(dataSetName);
  }

  // If the data set type has geometry, get the position of the geometry column
  size_t geometryColumn;

  if(m_dataSetType->hasGeom() == true)
    geometryColumn = m_dataSetType->getDefaultGeomPropertyPos();

  // Check if there will be operations on the data grid
  if(dataGridOperationCheckBox->isChecked())
  {
    m_dataGridOp->init(m_dataSetType, m_dataSet);
    m_dataGridModel->setDataGridOperation(m_dataGridOp);
    m_dataGridModel->init(m_dataSetType, m_dataSet);

    dataGridTableView->setVisible(false);
    m_opTableView->setVisible(true);
    m_opTableView->setModel(m_dataGridModel);
    m_opTableView->hideColumn(geometryColumn);
    m_opTableView->viewport()->update();
  }
  else
  {
    m_dataGridModel->setDataGridOperation(0);
    m_dataGridModel->init(m_dataSetType, m_dataSet);

    m_opTableView->setVisible(false); 
    dataGridTableView->setVisible(true);
    dataGridTableView->setModel(m_dataGridModel);
    dataGridTableView->hideColumn(geometryColumn);
    dataGridTableView->viewport()->update();
  }

  unsetCursor();
}

void DataGrid::closeEvent(QCloseEvent* /*e*/)
{
  if(m_dataGridOp)
  {
    m_dataGridModel->setDataGridOperation(0);
    delete m_dataGridOp;
    m_dataGridOp = 0;
  }

  // Release the transactor
  if(m_transactor)
  {
    delete m_transactor;
    m_transactor = 0;
  }

  if(m_ds)
  {
    m_ds->close();
    delete m_ds;
  }
}

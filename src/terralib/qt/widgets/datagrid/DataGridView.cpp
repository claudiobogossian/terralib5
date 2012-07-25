/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file DataGridView.cpp

  \brief It is used to display the data contents of a dataset.
 */

// TerraLib
#include "../../../dataaccess.h"
#include "../../../maptools/DataGridOperation.h"
#include "../../../qt/widgets.h"

// STL
#include <algorithm>

//Qt
#include <QtGui/QApplication>
#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>
#include <QtGui/QScrollBar>
#include <QtGui/QMessageBox>

te::qt::widgets::DataGridView::DataGridView(QWidget* parent)
  : QTableView(parent),
    m_dataGridOp(0),
    m_itemDelegate(0),
    m_viewportMenu(0)
    
{
  // Set the grid selection mode to MultiSelection
  setSelectionMode(QAbstractItemView::MultiSelection);

  /******* Set the horizontal header view of this table view***********/
  m_horizHeaderView = new te::qt::widgets::HeaderView(Qt::Horizontal, this);
  setHorizontalHeader(m_horizHeaderView);

  /******* Set the vertical header view of this table view***********/
  m_vertHeaderView = new te::qt::widgets::HeaderView(Qt::Vertical, this);
  setVerticalHeader(m_vertHeaderView);

  // Set the delegate for the view
  m_itemDelegate = new te::qt::widgets::DataGridItemDelegate();
  setItemDelegate(m_itemDelegate);

  // Set the signal/slot connections
  const QObject* vertHeader = (const QObject*)this->verticalHeader();
  QObject::connect(vertHeader, SIGNAL(sectionClicked(int)), this, SLOT(rowClicked(int)));

  const QObject* horizHeader = (const QObject*)this->horizontalHeader();
  connect(horizHeader, SIGNAL(sectionClicked(int)), this, SLOT(columnClicked(int)));
  connect(horizHeader, SIGNAL(columnMoved(int, int)), this, SLOT(columnMoved(int, int)));
}

te::qt::widgets::DataGridView::~DataGridView()
{
}

void te::qt::widgets::DataGridView::setViewportMenu(QMenu* menu)
{
  if (m_viewportMenu != 0)
    delete m_viewportMenu;

  m_viewportMenu = menu;
}

te::qt::widgets::HeaderView* te::qt::widgets::DataGridView::getHorizontalHeaderView()
{
  return m_horizHeaderView;
}

te::qt::widgets::HeaderView* te::qt::widgets::DataGridView::getVerticalHeaderView()
{
  return m_vertHeaderView;
}

void te::qt::widgets::DataGridView::setModel(QAbstractItemModel* model)
{
  te::qt::widgets::DataGridModel* dataGridModel = static_cast<te::qt::widgets::DataGridModel*>(model);

  m_dataGridOp = dataGridModel->getDataGridOperation();

  // Set the data grid for the header views of this view.
  m_horizHeaderView->setDataGridOperation(m_dataGridOp);
  m_vertHeaderView->setDataGridOperation(m_dataGridOp);

  // Set the data grid for the item delegate of this view.
  m_itemDelegate->setDataGridOperation(m_dataGridOp);

  QTableView::setModel(model);
}

void te::qt::widgets::DataGridView::updateTableViewSelectionStatus()
{
  // Get the selection model of the table view
  QItemSelectionModel* selectionModel = this->selectionModel();

  // Get the model of the table view
  QAbstractItemModel* model = this->model();

  // Clear all the selections on the table view
  selectionModel->clearSelection();

  // Get the number of table rows
  size_t numRows = m_dataGridOp->getNumberOfRows();

  // Get the number of table columns
  size_t numColumns = m_dataGridOp->getNumberOfColumns();

  // Set the pairs of rows selected that are contiguous
  std::vector<std::pair<int, int> > seqPairs;
  std::vector<int> selectedRows = m_dataGridOp->getSelectedRows();

  getSequencePairs(selectedRows, seqPairs);

  QModelIndex topLeft, bottomRight;

  // Select the rows on the table view
  QItemSelection rowItemSelection;
  for (size_t i = 0; i < seqPairs.size(); ++i)
  {
    topLeft = model->index(seqPairs[i].first, 0);
    bottomRight = model->index(seqPairs[i].second, numColumns - 1);
    rowItemSelection.select(topLeft, bottomRight);
  }
  selectionModel->select(rowItemSelection, QItemSelectionModel::Select);

  // Set the pairs of columns selected that are contiguous
  seqPairs.clear();
  std::vector<int> selectedColumns = m_dataGridOp->getSelectedColumns();

  getSequencePairs(selectedColumns, seqPairs);

  QItemSelection columnItemSelection;
  for (size_t i = 0; i < seqPairs.size(); ++i)
  {
    topLeft = model->index(0, seqPairs[i].first);
    bottomRight = model->index(numRows - 1, seqPairs[i].second);
    columnItemSelection.select(topLeft, bottomRight);
  }
  selectionModel->select(columnItemSelection, QItemSelectionModel::Select);
}

void te::qt::widgets::DataGridView::rowClicked(int clickedVisualRow)
{
  static int prevClickedVisualRow = -1; // previous visual row clicked

  Qt::KeyboardModifiers keyboardModifiers = QApplication::keyboardModifiers();

  if (keyboardModifiers == 0 || keyboardModifiers == Qt::ControlModifier)
  {
    // Toggle the pointed status of the clicked row
    std::vector<int> visRows;
    visRows.push_back(clickedVisualRow);
    m_dataGridOp->toggleRowsPointingStatus(visRows);
  }
  else if (keyboardModifiers == Qt::ShiftModifier)
  {
    if (clickedVisualRow == prevClickedVisualRow)
      return;

    // Set the range of rows between the last previous row clicked and
    // the row where the mouse was released.
    std::vector<int> visualRows;

    int lowerRow, upperRow;

    if (clickedVisualRow > prevClickedVisualRow)
    {
      lowerRow = prevClickedVisualRow + 1;
      upperRow = clickedVisualRow;
    }
    else if (clickedVisualRow < prevClickedVisualRow)
    {
      lowerRow = clickedVisualRow;
      upperRow = prevClickedVisualRow - 1;
    }

    for (int i = lowerRow; i <= upperRow; ++i)
      visualRows.push_back(i);

    // Get the status of the previous visual row clicked.
    // It will be used to set the status of the range of rows.
    te::map::DataGridOperation::SelectionStatus prevClickedRowStatus = m_dataGridOp->getVisualRowStatus(prevClickedVisualRow);

    if (prevClickedRowStatus == te::map::DataGridOperation::POINTED ||
        prevClickedRowStatus == te::map::DataGridOperation::POINTED_AND_QUERIED)
      m_dataGridOp->setRowsAsPointed(visualRows);
    else
      m_dataGridOp->removePointedStatusOfRows(visualRows);
  }

  // Update the table view selection status on account of the row clicked.
  updateTableViewSelectionStatus();
  
  // Store the static previous row clicked variable
  prevClickedVisualRow = clickedVisualRow;

  // Update the viewport
  viewport()->update();
}

void te::qt::widgets::DataGridView::columnClicked(int clickedVisualColumn)
{
  static int prevClickedVisualColumn = -1;  // previous visual column clicked

  Qt::KeyboardModifiers keyboardModifiers = QApplication::keyboardModifiers();

  if (keyboardModifiers == 0 || keyboardModifiers == Qt::ControlModifier)
  {   
    // Toggle the selection status of the clicked column
    m_dataGridOp->toggleColumnPointingStatus(clickedVisualColumn);
  }
  else if (keyboardModifiers == Qt::ShiftModifier)
  {
    if (clickedVisualColumn == prevClickedVisualColumn)
      return;

    // Set the range of columns between the last previous column clicked and
    // the column where the mouse was released.
    std::vector<int> visualColumns;

    int lowerColumn, upperColumn;

    if (clickedVisualColumn > prevClickedVisualColumn)
    {
      lowerColumn = prevClickedVisualColumn + 1;
      upperColumn = clickedVisualColumn;
    }
    else if (clickedVisualColumn < prevClickedVisualColumn)
    {
      lowerColumn = clickedVisualColumn;
      upperColumn = prevClickedVisualColumn - 1;
    }

    for (int i = lowerColumn; i <= upperColumn; ++i)
      visualColumns.push_back(i);

    // Get the status of the previous column clicked.
    // It will be used to set the status of the range of columns.
    bool prevClickedColumnStatus = m_dataGridOp->getVisualColumnStatus(prevClickedVisualColumn);

    if (prevClickedColumnStatus == true)
      m_dataGridOp->setColumnsAsPointed(visualColumns);
    else
      m_dataGridOp->removePointedStatusOfColumns(visualColumns);
  }

  // Update the table view selection status on account of the column clicked.
  updateTableViewSelectionStatus();

  // Store the static previous column clicked variable.
  prevClickedVisualColumn = clickedVisualColumn;

  // Update the view
  viewport()->update();
}

void te::qt::widgets::DataGridView::promotePointedRows()
{
  // Promote the visual rows that are pointed, or "pointed and queried".
  m_dataGridOp->promoteRows(te::map::DataGridOperation::POINTED);

  // Update the table view selection status on account of the promotion of the pointed rows.
  updateTableViewSelectionStatus();

  verticalScrollBar()->setValue(0);
  viewport()->update();
}

void te::qt::widgets::DataGridView::promoteQueriedRows()
{
  // Promote the visual rows that are queried, or "pointed and queried".
  m_dataGridOp->promoteRows(te::map::DataGridOperation::QUERIED);

  // Update the table view selection satus on account of the promotion of the queried rows.
  updateTableViewSelectionStatus();

  verticalScrollBar()->setValue(0);
  viewport()->update();
}

void te::qt::widgets::DataGridView::removeAllSelections()
{
  m_dataGridOp->removeRowSelections();
  m_dataGridOp->removeColumnSelections();

  // Clear the table view selection model
  selectionModel()->clearSelection();

  viewport()->update();
}

void te::qt::widgets::DataGridView::sortColumnsInAscendentOrder()
{
  sortColumns("ASC");
}

void te::qt::widgets::DataGridView::sortColumnsInDescendentOrder()
{
  sortColumns("DESC");
}

void te::qt::widgets::DataGridView::columnMoved(int fromPos, int toPos)
{
  // Move the column in memory from the fromPos to toPos, and get the
  // logical geomtry column before the moving operation
  int prevLogicalGeometryColumn = m_dataGridOp->moveColumn(fromPos, toPos);

  // Update the table view selection status because of the column position change.
  updateTableViewSelectionStatus();

  // If there is a geometry column, check if the visual geometry column is different from the previous one;
  // in positive case, make them be shown or hidden as appropriate.
  if(prevLogicalGeometryColumn == -1)
  {
    viewport()->update();
    return;
  }

  int prevVisualGeometryColumn = m_dataGridOp->getVisualColumn(prevLogicalGeometryColumn);

  int visualGeometryColumn = m_dataGridOp->getVisualColumn(m_dataGridOp->getLogicalGeometryColumn());

  if (visualGeometryColumn != prevVisualGeometryColumn)
  {
    showColumn(prevVisualGeometryColumn);
    hideColumn(visualGeometryColumn);
  }

  viewport()->update();
}

void te::qt::widgets::DataGridView::getSequencePairs(std::vector<int>& sections, std::vector<std::pair<int, int> >& seqPairs)
{
  int numSections = sections.size();

  if (numSections == 0)
    return;

  std::pair<int, int> seqPair;

  std::sort(sections.begin(), sections.end());

  for (int i = 0; i < numSections; ++i)
  {
    if (i == 0)
    {
      seqPair.first = sections[i];
      seqPair.second = seqPair.first;
    }
    else
    {
      if (sections[i] == sections[i-1] + 1)
        seqPair.second = sections[i];
      else
      {
        seqPairs.push_back(seqPair);

        seqPair.first = sections[i];
        seqPair.second = seqPair.first;
      }
    }
  }

  if (seqPairs.empty() == true || seqPairs[seqPairs.size() - 1].second != seqPair.second)
    seqPairs.push_back(seqPair);
}

void te::qt::widgets::DataGridView::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() == Qt::LeftButton)
  {
    const QPoint& p = e->pos();
    QModelIndex clickedIndex = indexAt(p);

    if (clickedIndex.row() < 0 || clickedIndex.column() < 0)
      return QTableView::mouseReleaseEvent(e);

    if (m_dataGridOp->getVisualRowStatus(clickedIndex.row()) == te::map::DataGridOperation::DESELECTED &&
        m_dataGridOp->getVisualColumnStatus(clickedIndex.column()) == te::map::DataGridOperation::DESELECTED)
        return QTableView::mouseReleaseEvent(e);
    else
      selectionModel()->select(clickedIndex, QItemSelectionModel::Select);
  }
  else if (e->button() == Qt::RightButton)
  {
    if (m_viewportMenu != 0)
      m_viewportMenu->exec(e->globalPos());
  }

  return QTableView::mouseReleaseEvent(e);
}

void te::qt::widgets::DataGridView::sortColumns(std::string order)
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  std::vector<int> selectedColumns = m_dataGridOp->getSelectedColumns();
  if(selectedColumns.size() == 0)
  {
    QMessageBox::information(this, tr("Sort Operation"), tr("Select column(s) in the grid to be sorted!"));
    QApplication::restoreOverrideCursor();
    return;
  }

  // Set a query to the data source to get a data set ordered in ascendent or descendent order
  te::da::DataSetType* dataSetType = m_dataGridOp->getDataSetType();
  te::da::DataSource* ds = dataSetType->getCatalog()->getDataSource();

  std::string columns;
  for(size_t i = 0; i < selectedColumns.size(); ++i)
  {
    if(i != 0)
      columns += ",";

    int logicalColumn = m_dataGridOp->getLogicalColumn(selectedColumns[i]);
    std::string colName = dataSetType->getProperty(logicalColumn)->getName();

    columns += colName;
  }

  te::da::PrimaryKey* pk = dataSetType->getPrimaryKey();
  const std::vector<te::dt::Property*>& pkProperties = pk->getProperties();

  std::string pkCols;
  for(size_t i = 0; i < pkProperties.size(); ++i)
  {
    if(i != 0)
      pkCols += ",";

    pkCols += pkProperties[i]->getName();
  }

  std::string sql = "SELECT " + pkCols + " FROM ";
  sql += dataSetType->getName();
  sql += " ORDER BY " + columns + " " + order;

  te::da::DataSet* sortedDataSet;

  te::da::DataSourceTransactor* t = ds->getTransactor();

  try
  {
    sortedDataSet = t->query(sql);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(this, tr("Sort Operation"), e.what());
    QApplication::restoreOverrideCursor();
    return;
  }

  m_dataGridOp->sortColumns(sortedDataSet);
  updateTableViewSelectionStatus();

  verticalScrollBar()->setValue(0);
  viewport()->update();

  delete t;

  QApplication::restoreOverrideCursor();
}


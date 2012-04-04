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


// TerraLib
#include "../common.h"
#include "../dataaccess.h"

#include "DataGridOperation.h"

te::map::DataGridOperation::DataGridOperation() :
  m_lineWidth(1),
  m_contourWidth(1),
  m_ptMarkerType(te::map::MarkerFourRays),
  m_ptWidth(16),
  m_polygonOpacity(255)
{
  // Initialize the selection colors
  m_deselectedColor.setColor(220 ,0, 0, 255);
  m_pointedColor.setColor(80, 240, 100, 255);
  m_queriedColor.setColor(210, 210, 0, 255);
  m_PointedAndQueriedColor.setColor(255, 255, 0, 255);
  m_contourColor.setColor(20, 20, 20, 255);
}

te::map::DataGridOperation::~DataGridOperation()
{
}

void te::map::DataGridOperation::init(te::da::DataSetType* dt, te::da::DataSet* dataSet)
{
  clear();

  m_dt = dt;
  m_dataSet = dataSet;

  // Initialize the variables related to the data grid rows
  m_numRows = m_dataSet->size();

  m_visualRowStatus.resize(m_numRows);
  m_visualToLogicalRows.resize(m_numRows);
  m_logicalToVisualRows.resize(m_numRows);

  for (int i = 0; i < m_numRows; ++i)
  {
    m_visualRowStatus[i] = DESELECTED;
    m_visualToLogicalRows[i] = i;
    m_logicalToVisualRows[i] = i;
  }

  // Initialize the variables related to the data grid columns
  m_numColumns = m_dt->size();

  m_visualColumnStatus.resize(m_numColumns);
  m_visualToLogicalColumns.resize(m_numColumns);
  m_logicalToVisualColumns.resize(m_numColumns);
  
  for (int i = 0; i < m_numColumns; ++i)
  {
    m_visualColumnStatus[i] = DESELECTED;
    m_visualToLogicalColumns[i] = i;
    m_logicalToVisualColumns[i] = i;
  }

  // Set the geometry column
  m_logicalGeometryColumn = dt->getDefaultGeomPropertyPos();

  // Build the dataset items to the logical rows relationship
  buildDataSetItemToLogicalRowRelation();
}

te::da::DataSetType* te::map::DataGridOperation::getDataSetType()
{
  return m_dt;
}

void te::map::DataGridOperation::setDataSetType(te::da::DataSetType* dt)
{
  m_dt = dt;
}

te::da::DataSet* te::map::DataGridOperation::getDataSet()
{
  return m_dataSet;
}

void te::map::DataGridOperation::setDataSet(te::da::DataSet* d)
{
  m_dataSet = d;
}

int te::map::DataGridOperation::getNumberOfRows() const
{
  return m_numRows;
}

int te::map::DataGridOperation::getNumberOfColumns() const
{
  return m_numColumns;
}

te::map::DataGridOperation::SelectionStatus te::map::DataGridOperation::getVisualRowStatus(int visualRow) const
{
  return m_visualRowStatus[visualRow];
}

void te::map::DataGridOperation::setVisualRowStatus(int visualRow, SelectionStatus status)
{
  m_visualRowStatus[visualRow] = status;
}

int te::map::DataGridOperation::getLogicalRow(int visualRow) const
{
  return m_visualToLogicalRows[visualRow];
}

void te::map::DataGridOperation::setLogicalRow(int visualRow, int logicalRow)
{
  m_visualToLogicalRows[visualRow] = logicalRow;
}

int te::map::DataGridOperation::getVisualRow(int logicalRow) const
{
  return m_logicalToVisualRows[logicalRow];
}

void te::map::DataGridOperation::setVisualRow(int logicalRow, int visualRow)
{
  m_logicalToVisualRows[logicalRow] = visualRow;
}

bool te::map::DataGridOperation::getVisualColumnStatus(int visualColumn) const
{
  return m_visualColumnStatus[visualColumn];
}

void te::map::DataGridOperation::setVisualColumnStatus(int visualColumn, bool status)
{
  m_visualColumnStatus[visualColumn] = status;
}

int te::map::DataGridOperation::getLogicalColumn(int visualColumn) const
{
  return m_visualToLogicalColumns[visualColumn];
}

void te::map::DataGridOperation::setLogicalColumn(int visualColumn, int logicalColumn)
{
  m_visualToLogicalColumns[visualColumn] = logicalColumn;
}

int te::map::DataGridOperation::getVisualColumn(int logicalColumn) const
{
  return m_logicalToVisualColumns[logicalColumn];
}

void te::map::DataGridOperation::setVisualColumn(int logicalColumn, int visualColumn)
{
  m_logicalToVisualColumns[logicalColumn] = visualColumn;
}

size_t te::map::DataGridOperation::getLogicalGeometryColumn() const
{
  return m_logicalGeometryColumn;
}

int te::map::DataGridOperation::getLogicalRow(const std::string& featureId) const
{
  std::map<std::string, int>::const_iterator it = m_dataSetItemToLogicalRowMap.find(featureId);

  if(it != m_dataSetItemToLogicalRowMap.end())
    return it->second;

  return -1;
}

const std::string& te::map::DataGridOperation::getDataSetId(int logicalRow) const
{
//  assert(logicalRow > 0 && logicalRow < (int)m_dataSet->size());
  assert(logicalRow >= 0 && logicalRow < (int)m_dataSet->size()); //lauro
  return m_logicalRowToDataSetItemMap[logicalRow];
}

const te::color::RGBAColor& te::map::DataGridOperation::getDeselectedColor() const
{
  return m_deselectedColor;
}

void te::map::DataGridOperation::setDeselectedColor(const te::color::RGBAColor& color)
{
  m_deselectedColor = color;

  size_t size = m_visualRowStatus.size();
  m_theChanged.clear();
  for(size_t i = 0; i < size; ++i)
  {
    if (m_visualRowStatus[i] == DESELECTED)
      m_theChanged.insert(getDataSetId(getLogicalRow(i)));
  }
}

const te::color::RGBAColor& te::map::DataGridOperation::getPointedColor() const
{
  return m_pointedColor;
}

void te::map::DataGridOperation::setPointedColor(const te::color::RGBAColor& color)
{
  m_pointedColor = color;

  size_t size = m_visualRowStatus.size();
  m_theChanged.clear();
  for(size_t i = 0; i < size; ++i)
  {
    if (m_visualRowStatus[i] == POINTED)
      m_theChanged.insert(getDataSetId(getLogicalRow(i)));
  }
}

const te::color::RGBAColor& te::map::DataGridOperation::getQueriedColor() const
{
  return m_queriedColor;
}

void te::map::DataGridOperation::setQueriedColor(const te::color::RGBAColor& color)
{
  m_queriedColor = color;

  size_t size = m_visualRowStatus.size();
  m_theChanged.clear();
  for(size_t i = 0; i < size; ++i)
  {
    if (m_visualRowStatus[i] == QUERIED)
      m_theChanged.insert(getDataSetId(getLogicalRow(i)));
  }
}

const te::color::RGBAColor& te::map::DataGridOperation::getPointedAndQueriedColor() const
{
  return m_PointedAndQueriedColor;
}

void te::map::DataGridOperation::setPointedAndQueriedColor(const te::color::RGBAColor& color)
{
  m_PointedAndQueriedColor = color;

  size_t size = m_visualRowStatus.size();
  m_theChanged.clear();
  for(size_t i = 0; i < size; ++i)
  {
    if (m_visualRowStatus[i] == POINTED_AND_QUERIED)
      m_theChanged.insert(getDataSetId(getLogicalRow(i)));
  }
}

te::map::DataGridOperation::SelectionStatus te::map::DataGridOperation::getDataSetSelectionStatus(const std::string& featureId)
{
  int row = getLogicalRow(featureId);

  return m_visualRowStatus[m_logicalToVisualRows[row]];
}

std::vector<int> te::map::DataGridOperation::getSelectedRows()
{
  std::vector<int> selected;

  for (int i = 0; i < m_numRows; ++i)
  {
    if (m_visualRowStatus[i] != DESELECTED)
      selected.push_back(i);
  }

  return selected;
}

std::vector<int> te::map::DataGridOperation::getSelectedColumns()
{
  std::vector<int> selected;

  for (int i = 0; i < m_numColumns; ++i)
  {
    if (m_visualColumnStatus[i] == true)
      selected.push_back(i);
  }

  return selected;
}

void te::map::DataGridOperation::toggleRowsPointingStatus(const std::vector<int>& visualRows)
{
  int visualRow;
  std::vector<int>::const_iterator it;

  m_theChanged.clear();
  for(it = visualRows.begin(); it != visualRows.end(); ++it)
  {
    visualRow = *it;
    if (m_visualRowStatus[visualRow] == DESELECTED)
      m_visualRowStatus[visualRow] = POINTED;
    else if (m_visualRowStatus[visualRow] == POINTED)
      m_visualRowStatus[visualRow] = DESELECTED;
    else if (m_visualRowStatus[visualRow] == QUERIED)
      m_visualRowStatus[visualRow] = POINTED_AND_QUERIED;
    else if (m_visualRowStatus[visualRow] == POINTED_AND_QUERIED)
      m_visualRowStatus[visualRow] = QUERIED;

    m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
  }
}

void te::map::DataGridOperation::toggleColumnPointingStatus(int visualColumn)
{
  if (m_visualColumnStatus[visualColumn] == false)
    m_visualColumnStatus[visualColumn] = true;
  else
    m_visualColumnStatus[visualColumn] = false;
}

void te::map::DataGridOperation::addRowsToPointed(const std::vector<int>& visualRows)
{
  int visualRow;

  m_theChanged.clear();
  for (size_t i = 0; i < visualRows.size(); ++i)
  {
    visualRow = visualRows[i];

    if (m_visualRowStatus[visualRow] == DESELECTED)
    {
      m_visualRowStatus[visualRow] = POINTED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
    else if (m_visualRowStatus[visualRow] == QUERIED)
    {
      m_visualRowStatus[visualRow] = POINTED_AND_QUERIED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
  }
}

void te::map::DataGridOperation::setRowsAsPointed(const std::vector<int>& visualRows)
{
  int visualRow;
  size_t size = visualRows.size();
  std::set<int> pointeds;

  m_theChanged.clear();
  for (size_t i = 0; i < size; ++i)
  {
    visualRow = visualRows[i];

    if (m_visualRowStatus[visualRow] == DESELECTED)
    {
      m_visualRowStatus[visualRow] = POINTED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
    else if (m_visualRowStatus[visualRow] == QUERIED)
    {
      m_visualRowStatus[visualRow] = POINTED_AND_QUERIED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
    pointeds.insert(visualRow);
  }

  std::set<int>::iterator it;
  size = m_visualToLogicalRows.size();
  for (size_t i = 0; i < size; ++i)
  {
    if(pointeds.find(i) == pointeds.end())
    {
      if (m_visualRowStatus[i] == POINTED)
      {
        m_visualRowStatus[i] = DESELECTED;
        m_theChanged.insert(getDataSetId(getLogicalRow(i)));
      }
      else if (m_visualRowStatus[i] == POINTED_AND_QUERIED)
      {
        m_visualRowStatus[i] = QUERIED;
        m_theChanged.insert(getDataSetId(getLogicalRow(i)));
      }
    }
  }
}

void te::map::DataGridOperation::setColumnsAsPointed(const std::vector<int>& visualColumns)
{
  for (size_t i = 0; i < visualColumns.size(); ++i)
    m_visualColumnStatus[visualColumns[i]] = true;
}

void te::map::DataGridOperation::removePointedStatusOfRows(const std::vector<int>& visualRows)
{
  int visualRow;

  m_theChanged.clear();
  for (size_t i = 0; i < visualRows.size(); ++i)
  {
    visualRow = visualRows[i];

    if (m_visualRowStatus[visualRow] == POINTED)
    {
      m_visualRowStatus[visualRow] = DESELECTED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
    else if (m_visualRowStatus[visualRow] == POINTED_AND_QUERIED)
    {
      m_visualRowStatus[visualRow] = QUERIED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
  }
}

void te::map::DataGridOperation::removePointedStatusOfAllRows()
{
  m_theChanged.clear();
  for (int i = 0; i < m_numRows; ++i)
  {
    if (m_visualRowStatus[i] == POINTED)
    {
      m_visualRowStatus[i] = DESELECTED;
      m_theChanged.insert(getDataSetId(getLogicalRow(i)));
    }
    else if (m_visualRowStatus[i] == POINTED_AND_QUERIED)
    {
      m_visualRowStatus[i] = QUERIED;
      m_theChanged.insert(getDataSetId(getLogicalRow(i)));
    }
  }
}

void te::map::DataGridOperation::removePointedStatusOfColumns(const std::vector<int>& visualColumns)
{
  for (size_t i = 0; i < visualColumns.size(); ++i)
    m_visualColumnStatus[visualColumns[i]] = false;
}

void te::map::DataGridOperation::addRowsToQueried(const std::vector<int>& visualRows)
{
  int visualRow;

  m_theChanged.clear();
  for (size_t i = 0; i < visualRows.size(); ++i)
  {
    visualRow = visualRows[i];

    if (m_visualRowStatus[visualRow] == DESELECTED)
    {
      m_visualRowStatus[visualRow] = QUERIED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
    else if (m_visualRowStatus[visualRow] == POINTED)
    {
      m_visualRowStatus[visualRow] = POINTED_AND_QUERIED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
  }
}

void te::map::DataGridOperation::setRowsAsQueried(const std::vector<int>& visualRows)
{
  int visualRow;

  m_theChanged.clear();
  for (size_t i = 0; i < visualRows.size(); ++i)
  {
    visualRow = visualRows[i];

    if (m_visualRowStatus[visualRow] == DESELECTED)
    {
      m_visualRowStatus[visualRow] = QUERIED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
    else if (m_visualRowStatus[visualRow] == POINTED)
    {
      m_visualRowStatus[visualRow] = POINTED_AND_QUERIED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
  }
}

void te::map::DataGridOperation::removeQueriedStatusOfRows(const std::vector<int>& visualRows)
{
  int visualRow;

  m_theChanged.clear();
  for (size_t i = 0; i < visualRows.size(); ++i)
  {
    visualRow = visualRows[i];

    if (m_visualRowStatus[visualRow] == QUERIED)
    {
      m_visualRowStatus[visualRow] = DESELECTED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
    else if (m_visualRowStatus[visualRow] == POINTED_AND_QUERIED)
    {
      m_visualRowStatus[visualRow] = POINTED;
      m_theChanged.insert(getDataSetId(getLogicalRow(visualRow)));
    }
  }
}

void te::map::DataGridOperation::removeQueriedStatusOfAllRows()
{
  m_theChanged.clear();
  for (int i = 0; i < m_numRows; ++i)
  {
    if (m_visualRowStatus[i] == QUERIED)
    {
      m_visualRowStatus[i] = DESELECTED;
      m_theChanged.insert(getDataSetId(getLogicalRow(i)));
    }
    else if (m_visualRowStatus[i] == POINTED_AND_QUERIED)
    {
      m_visualRowStatus[i] = POINTED;
      m_theChanged.insert(getDataSetId(getLogicalRow(i)));
    }
  }
}

void te::map::DataGridOperation::removeRowSelections()
{
  m_theChanged.clear();
  for (int i = 0; i < m_numRows; ++i)
  {
    if(m_visualRowStatus[i] != DESELECTED)
    {
      m_visualRowStatus[i] = DESELECTED;
      m_theChanged.insert(getDataSetId(getLogicalRow(i)));
    }
  }
}

void te::map::DataGridOperation::removeColumnSelections()
{
  for (int i = 0; i < m_numColumns; ++i)
      m_visualColumnStatus[i] = false;
}

void te::map::DataGridOperation::promoteRows(SelectionStatus status)
{
  // Get the logical rows and their status for the visual rows that are to be promoted,
  // according to the selection status given, as well as the logical rows and their status for the 
  // visual rows that are selected but do not have the selection status given, and
  // the logical rows that are deselected.

  std::vector<int> toBePromotedLogicalRows;
  std::vector<SelectionStatus> toBePromotedLogicalRowStatus;

  std::vector<int> selectedLogicalRows;
  std::vector<SelectionStatus> selectedLogicalRowStatus;

  std::vector<int> deselectedLogicalRows;

  int logicalRow;

  if (status == POINTED)
  {
    for (int i = 0; i < m_numRows; ++i)
    {
      logicalRow = m_visualToLogicalRows[i];

      if (m_visualRowStatus[i] == POINTED)
      {
        toBePromotedLogicalRows.push_back(logicalRow);
        toBePromotedLogicalRowStatus.push_back(POINTED);
      }
      else if (m_visualRowStatus[i] == POINTED_AND_QUERIED)
      {
        toBePromotedLogicalRows.push_back(logicalRow);
        toBePromotedLogicalRowStatus.push_back(POINTED_AND_QUERIED);
      }
      else if (m_visualRowStatus[i] == QUERIED)
      {
        selectedLogicalRows.push_back(logicalRow);
        selectedLogicalRowStatus.push_back(QUERIED);
      }
      else
        deselectedLogicalRows.push_back(logicalRow);
    }
  }
  else if (status == QUERIED)
  {
    for (int i = 0; i < m_numRows; ++i)
    {
      logicalRow = m_visualToLogicalRows[i];

      if (m_visualRowStatus[i] == QUERIED)
      {
        toBePromotedLogicalRows.push_back(logicalRow);
        toBePromotedLogicalRowStatus.push_back(QUERIED);
      }
      else if (m_visualRowStatus[i] == POINTED_AND_QUERIED)
      {
        toBePromotedLogicalRows.push_back(logicalRow);
        toBePromotedLogicalRowStatus.push_back(POINTED_AND_QUERIED);
      }
      else if (m_visualRowStatus[i] == POINTED)
      {
        selectedLogicalRows.push_back(logicalRow);
        selectedLogicalRowStatus.push_back(POINTED);
      }
      else
        deselectedLogicalRows.push_back(logicalRow);
    }
  }
  else if (status == POINTED_AND_QUERIED)
  {
    for (int i = 0; i < m_numRows; ++i)
    {
      logicalRow = m_visualToLogicalRows[i];

      if (m_visualRowStatus[i] == POINTED_AND_QUERIED)
      {
        toBePromotedLogicalRows.push_back(logicalRow);
        toBePromotedLogicalRowStatus.push_back(POINTED_AND_QUERIED);
      }
      else if (m_visualRowStatus[i] == POINTED)
      {
        selectedLogicalRows.push_back(logicalRow);
        selectedLogicalRowStatus.push_back(POINTED);
      }
      else if (m_visualRowStatus[i] == QUERIED)
      {
        selectedLogicalRows.push_back(logicalRow);
        selectedLogicalRows.push_back(QUERIED);
      }
      else
        deselectedLogicalRows.push_back(logicalRow);
    }
  }

  // Make the logical rows to be promoted as the first visual rows, and insert these first visual rows
  // into the affected visual rows set.
  int numToBePromotedRows = toBePromotedLogicalRows.size();

  for (int i = 0; i < numToBePromotedRows; ++i)
  {
    m_visualRowStatus[i] = toBePromotedLogicalRowStatus[i];
    m_visualToLogicalRows[i] = toBePromotedLogicalRows[i];
    m_logicalToVisualRows[m_visualToLogicalRows[i]] = i;
  }

  // Make the selected logical rows to be the following visual rows.
  int numSelectedRows = selectedLogicalRows.size();

  int visualRow;

  int offset = numToBePromotedRows;
  for (int i = 0; i < numSelectedRows; ++i)
  {
    visualRow = offset + i;
    m_visualRowStatus[visualRow] = selectedLogicalRowStatus[i];
    m_visualToLogicalRows[visualRow] = selectedLogicalRows[i];
    m_logicalToVisualRows[m_visualToLogicalRows[visualRow]] = visualRow;
  }

  // Make the deselected logical rows to be the following visual rows.
  int numDeselectedRows = deselectedLogicalRows.size();

  offset = numToBePromotedRows + numSelectedRows;
  for (int i = 0; i < numDeselectedRows; ++i)
  {
    visualRow = offset + i;
    m_visualRowStatus[visualRow] = DESELECTED;
    m_visualToLogicalRows[visualRow] = deselectedLogicalRows[i];
    m_logicalToVisualRows[m_visualToLogicalRows[visualRow]] = visualRow;
  }
}

void te::map::DataGridOperation::sortColumns(te::da::DataSet* sortedDataSet)
{
  te::da::PrimaryKey* pk = m_dt->getPrimaryKey();
  const std::vector<te::dt::Property*>& pkProperties = pk->getProperties();

  std::vector<int> tempVisualToLogicalRows(m_numRows);
  std::vector<int> tempLogicalToVisualRows(m_numRows);
  std::vector<SelectionStatus> tempVisualRowStatus(m_numRows);
  int logicalRow;

  int row = 0;
  while(sortedDataSet->moveNext())
  {
    std::string dataSetItem;

    for (size_t i = 0; i < pkProperties.size(); ++i)
      dataSetItem += sortedDataSet->getAsString(pkProperties[i]->getName());

    // Get the logical row of the dataSetItem
    logicalRow = m_dataSetItemToLogicalRowMap[dataSetItem];

    // Set the new visual to logical row mapping and the new visual row status
    tempVisualToLogicalRows[row] = logicalRow;
    tempLogicalToVisualRows[logicalRow] = row;
    tempVisualRowStatus[row] = m_visualRowStatus[m_logicalToVisualRows[logicalRow]];

    ++row;
  }

  m_visualToLogicalRows = tempVisualToLogicalRows;
  m_logicalToVisualRows = tempLogicalToVisualRows;
  m_visualRowStatus = tempVisualRowStatus;
}


int te::map::DataGridOperation::moveColumn(int fromPos, int toPos)
{
  // Store the visual geometry column
  int prevLogicalGeometryColumn = m_logicalGeometryColumn;

  if (toPos == fromPos)
    return prevLogicalGeometryColumn;

  // Store the fromPos logical column, and the fromPos selection status
  int fromLogicalColumn = m_visualToLogicalColumns[fromPos];
  bool fromColumnStatus = m_visualColumnStatus[fromPos];

  // Shift the visual to logical and logical to visual mappings, and the visual
  // column selection status for the columns between fromPos and toPos.
  if (fromPos > toPos)
  {
    for (int i = fromPos; i > toPos; --i)
    {
      m_visualToLogicalColumns[i] = m_visualToLogicalColumns[i-1];
      m_logicalToVisualColumns[m_visualToLogicalColumns[i]] = i;
      m_visualColumnStatus[i] = m_visualColumnStatus[i-1];
    }
  }
  else if (fromPos < toPos)
  {
    for (int i = fromPos; i < toPos; ++i)
    {
      m_visualToLogicalColumns[i] = m_visualToLogicalColumns[i+1];
      m_logicalToVisualColumns[m_visualToLogicalColumns[i]] = i;
      m_visualColumnStatus[i] = m_visualColumnStatus[i+1];
    }        
  }

  // Set the toPos visual to logical and logical to visual mappings,
  // and the toPos visual column selection status
  m_visualToLogicalColumns[toPos] = fromLogicalColumn;
  m_logicalToVisualColumns[fromLogicalColumn] = toPos;
  m_visualColumnStatus[toPos] = fromColumnStatus;

  return prevLogicalGeometryColumn;
}

std::string te::map::DataGridOperation::getColumnFullName(size_t logicalColumn)
{
  const te::da::DataSetType* featureType = m_dataSet->getType();
  const std::string& featureName = featureType->getName();
  const std::string& propertyName = featureType->getProperty(logicalColumn)->getName();

  std::string columnName = featureName + "." + propertyName;

  return columnName;
}

const std::set<std::string>& te::map::DataGridOperation::getTheChanged()
{
  return m_theChanged;
}

void te::map::DataGridOperation::addToChanged(std::string id)
{
  m_theChanged.insert(id);
}

void te::map::DataGridOperation::clear()
{
  m_visualRowStatus.clear(); 
  m_visualToLogicalRows.clear();         
  m_logicalToVisualRows.clear();         

  m_visualColumnStatus.clear();         
  m_visualToLogicalColumns.clear();      
  m_logicalToVisualColumns.clear();      

  m_dataSetItemToLogicalRowMap.clear();
  m_logicalRowToDataSetItemMap.clear();  
}

void te::map::DataGridOperation::buildDataSetItemToLogicalRowRelation()
{
  // Set the mapping between the dataSetItem and the logical row, and
  // the mapping between the logical row and the dataSetItem.
  te::da::PrimaryKey* pk = m_dt->getPrimaryKey();
  if(pk == 0)
    return;
  const std::vector<te::dt::Property*>& pkProperties = pk->getProperties();

  int row = 0;
  m_logicalRowToDataSetItemMap.resize(m_numRows);

  while(m_dataSet->moveNext())
  {
    std::string dataSetItem;

    for (size_t i = 0; i < pkProperties.size(); ++i)
      dataSetItem += m_dataSet->getAsString(pkProperties[i]->getName());

    m_dataSetItemToLogicalRowMap[dataSetItem] = row;
    m_logicalRowToDataSetItemMap[row] = dataSetItem;

    ++row;
  }
}


void te::map::DataGridOperation::setLineWidth(int w)
{
  m_lineWidth = w;
}

int te::map::DataGridOperation::getLineWidth()
{
  return m_lineWidth;
}

void te::map::DataGridOperation::setContourWidth(int w)
{
  m_contourWidth = w;
}
void te::map::DataGridOperation::setContourColor(const te::color::RGBAColor& c)
{
  m_contourColor = c;
}

te::color::RGBAColor te::map::DataGridOperation::getContourColor()
{
  return m_contourColor;
}

int te::map::DataGridOperation::getContourWidth()
{
  return m_contourWidth;
}

void te::map::DataGridOperation::setPtMarkerType(te::map::PtMarkerType t)
{
  m_ptMarkerType = t;
}

te::map::PtMarkerType te::map::DataGridOperation::getPtMarkerType()
{
  return m_ptMarkerType;
}

void te::map::DataGridOperation::setPtWidth(int w)
{
  m_ptWidth = w;
}

int te::map::DataGridOperation::getPtWidth()
{
  return m_ptWidth;
}

void te::map::DataGridOperation::setPolygonOpacity(int opac)
{
  m_polygonOpacity = opac;
}

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
  m_defaultColor(220 ,0, 0, 120),
  m_pointedColor(80, 240, 100, 120),
  m_queriedColor(210, 210, 0, 120),
  m_pointedAndQueriedColor(255, 255, 0, 120),
  m_pointMarkerType(te::map::MarkerFourRays),
  m_pointIcon(0),
  m_pointIconSize(0),
  m_pointIconImageType(te::map::PNG),
  m_pointWidth(16),
  m_pointColor(255, 0, 255, 100),
  m_pointPatternRGBA(0),
  m_pointPatternRGBANCols(0),
  m_pointPatternRGBANRows(0),
  m_lineWidth(1),
  m_lineColor(0, 0, 255, 100),
  m_linePatternIcon(0),
  m_linePatternIconSize(0),
  m_linePatternIconImageType(te::map::PNG),
  m_linePatternRGBA(0),
  m_linePatternRGBANCols(0),
  m_linePatternRGBANRows(0),
  m_polygonContourWidth(1),
  m_polygonContourColor(20, 20, 20, 100),
  m_polygonContourPatternIconImageType(te::map::PNG),
  m_polygonContourPatternIcon(0),
  m_polygonContourPatternIconSize(0),
  m_polygonContourPatternRGBA(0),
  m_polygonContourPatternRGBANCols(0),
  m_polygonContourPatternRGBANRows(0),
  m_polygonFillColor(220 ,0, 0, 100),
  m_polygonMarkerType(te::map::MarkerNone),
  m_polygonFillMarkerColor(100, 20, 200, 100),
  m_polygonPatternIcon(0),
  m_polygonPatternIconSize(0),
  m_polygonPatternIconImageType(te::map::PNG),
  m_polygonPatternWidth(16),
  m_polygonPatternRGBA(0),
  m_polygonPatternRGBANCols(0),
  m_polygonPatternRGBANRows(0)
{
}

te::map::DataGridOperation::~DataGridOperation()
{
  delete []m_pointIcon;
  for(int i = 0; i < m_pointPatternRGBANRows; ++i)
    delete []m_pointPatternRGBA[i];
  delete []m_pointPatternRGBA;
  
  delete []m_linePatternIcon;
  for(int i = 0; i < m_linePatternRGBANRows; ++i)
    delete []m_linePatternRGBA[i];
  delete []m_linePatternRGBA;

  delete []m_polygonContourPatternIcon;
  for(int i = 0; i < m_polygonContourPatternRGBANRows; ++i)
    delete []m_polygonContourPatternRGBA[i];
  delete []m_polygonContourPatternRGBA;

  delete []m_polygonPatternIcon;
  for(int i = 0; i < m_polygonPatternRGBANRows; ++i)
    delete []m_polygonPatternRGBA[i];
  delete []m_polygonPatternRGBA;
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

const te::color::RGBAColor& te::map::DataGridOperation::getDefaultColor() const
{
  return m_defaultColor;
}

void te::map::DataGridOperation::setDefaultColor(const te::color::RGBAColor& color)
{
  m_defaultColor = color;

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
  return m_pointedAndQueriedColor;
}

void te::map::DataGridOperation::setPointedAndQueriedColor(const te::color::RGBAColor& color)
{
  m_pointedAndQueriedColor = color;

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

void te::map::DataGridOperation::setPointMarkerType(te::map::PtMarkerType t)
{
  m_pointMarkerType = t;
  if(m_pointMarkerType != te::map::MarkerNone && m_pointMarkerType != te::map::MarkerDot && m_pointMarkerType != te::map::MarkerPixel && m_pointMarkerType != te::map::MarkerPattern)
  {
    delete []m_pointIcon;
    m_pointIcon = 0;
  }
}

te::map::PtMarkerType te::map::DataGridOperation::getPointMarkerType()
{
  return m_pointMarkerType;
}

void te::map::DataGridOperation::setPointIcon(char* icon)
{
  if(m_pointIcon != icon)
    delete []m_pointIcon;
  m_pointIcon = icon;

  m_pointMarkerType = te::map::MarkerPattern;
}

char* te::map::DataGridOperation::getPointIcon()
{
  return m_pointIcon;
}
void te::map::DataGridOperation::setPointIconSize(int s)
{
  m_pointIconSize = s;
}

int te::map::DataGridOperation::getPointIconSize()
{
  return m_pointIconSize;
}

void te::map::DataGridOperation::setPointIconImageType(te::map::ImageType t)
{
  m_pointIconImageType = t;
}

te::map::ImageType te::map::DataGridOperation::getPointIconImageType()
{
  return m_pointIconImageType;
}

void te::map::DataGridOperation::setPointWidth(int w)
{
  m_pointWidth = w;
}

int te::map::DataGridOperation::getPointWidth()
{
  return m_pointWidth;
}

void te::map::DataGridOperation::setPointColor(const te::color::RGBAColor& cor)
{
  m_pointColor = cor;
}

te::color::RGBAColor te::map::DataGridOperation::getPointColor()
{
  return m_pointColor;
}

void te::map::DataGridOperation::setPointPatternRGBA(te::color::RGBAColor** patternRGBA)
{
  if(m_pointPatternRGBA != patternRGBA)
  {
    for(int i = 0; i < m_pointPatternRGBANRows; ++i)
      delete []m_pointPatternRGBA[i];
    delete []m_pointPatternRGBA;
  }

  m_pointPatternRGBA = patternRGBA;
}

te::color::RGBAColor** te::map::DataGridOperation::getPointPatternRBGA()
{
  return m_pointPatternRGBA;
}

void te::map::DataGridOperation::setPointPatternRGBANCols(int n)
{
  m_pointPatternRGBANCols = n;
}

int te::map::DataGridOperation::getPointPatternRGBANCols()
{
  return m_pointPatternRGBANCols;
}

void te::map::DataGridOperation::setPointPatternRGBANRows(int n)
{
  m_pointPatternRGBANRows = n;
}

int te::map::DataGridOperation::getPointPatternRGBANRows()
{
  return m_pointPatternRGBANRows;
}

void te::map::DataGridOperation::setLineWidth(int w)
{
  m_lineWidth = w;
}

int te::map::DataGridOperation::getLineWidth()
{
  return m_lineWidth;
}

void te::map::DataGridOperation::setLineColor(const te::color::RGBAColor& cor)
{
  m_lineColor = cor;
}

te::color::RGBAColor te::map::DataGridOperation::getLineColor()
{
  return m_lineColor;
}

void te::map::DataGridOperation::setLinePatternIcon(char* icon)
{
  if(m_linePatternIcon != icon)
    delete []m_linePatternIcon;
  m_linePatternIcon = icon;
}

char* te::map::DataGridOperation::getLinePatternIcon()
{
  return m_linePatternIcon;
}

void te::map::DataGridOperation::setLinePatternIconSize(int s)
{
  m_linePatternIconSize = s;
}

int te::map::DataGridOperation::getLinePatternIconSize()
{
  return m_linePatternIconSize;
}

void te::map::DataGridOperation::setLinePatternIconImageType(te::map::ImageType t)
{
  m_linePatternIconImageType = t;
}

te::map::ImageType te::map::DataGridOperation::getLinePatternIconImageType()
{
  return m_linePatternIconImageType;
}

void te::map::DataGridOperation::setLinePatternRGBA(te::color::RGBAColor** patternRGBA)
{
  if(m_linePatternRGBA != patternRGBA)
  {
    for(int i = 0; i < m_linePatternRGBANRows; ++i)
      delete []m_linePatternRGBA[i];
    delete []m_linePatternRGBA;
  }

  m_linePatternRGBA = patternRGBA;
}

te::color::RGBAColor** te::map::DataGridOperation::getLinePatternRBGA()
{
  return m_linePatternRGBA;
}

void te::map::DataGridOperation::setLinePatternRGBANCols(int n)
{
  m_linePatternRGBANCols = n;
}

int te::map::DataGridOperation::getLinePatternRGBANCols()
{
  return m_linePatternRGBANCols;
}

void te::map::DataGridOperation::setLinePatternRGBANRows(int n)
{
  m_linePatternRGBANRows = n;
}

int te::map::DataGridOperation::getLinePatternRGBANRows()
{
  return m_linePatternRGBANRows;
}

void te::map::DataGridOperation::setPolygonContourWidth(int w)
{
  m_polygonContourWidth = w;
}

int te::map::DataGridOperation::getPolygonContourWidth()
{
  return m_polygonContourWidth;
}

void te::map::DataGridOperation::setPolygonContourColor(const te::color::RGBAColor& cor)
{
  m_polygonContourColor = cor;
}

te::color::RGBAColor te::map::DataGridOperation::getPolygonContourColor()
{
  return m_polygonContourColor;
}

void te::map::DataGridOperation::setPolygonContourPatternIconImageType(te::map::ImageType t)
{
  m_polygonContourPatternIconImageType = t;
}

te::map::ImageType te::map::DataGridOperation::getPolygonContourPatternIconImageType()
{
  return m_polygonContourPatternIconImageType;
}

void te::map::DataGridOperation::setPolygonContourPatternIcon(char* icon)
{
  if(m_polygonContourPatternIcon != icon)
    delete []m_polygonContourPatternIcon;
  m_polygonContourPatternIcon = icon;
}

char* te::map::DataGridOperation::getPolygonContourPatternIcon()
{
  return m_polygonContourPatternIcon;
}

void te::map::DataGridOperation::setPolygonContourPatternIconSize(int s)
{
  m_polygonContourPatternIconSize = s;
}

int te::map::DataGridOperation::getPolygonContourPatternIconSize()
{
  return m_polygonContourPatternIconSize;
}

void te::map::DataGridOperation::setPolygonContourPatternRGBA(te::color::RGBAColor** patternColors)
{
  if(m_polygonContourPatternRGBA != patternColors)
  {
    for(int i = 0; i < m_polygonContourPatternRGBANRows; ++i)
      delete []m_polygonContourPatternRGBA[i];
    delete []m_polygonContourPatternRGBA;
  }

  m_polygonContourPatternRGBA = patternColors;
}

te::color::RGBAColor** te::map::DataGridOperation::getPolygonContourPatternRGBA()
{
  return m_polygonContourPatternRGBA;
}

void te::map::DataGridOperation::setPolygonContourPatternRGBANCols(int n)
{
  m_polygonContourPatternRGBANCols = n;
}

int te::map::DataGridOperation::getPolygonContourPatternRGBANCols()
{
  return m_polygonContourPatternRGBANCols;
}

void te::map::DataGridOperation::setPolygonContourPatternRGBANRows(int n)
{
  m_polygonContourPatternRGBANRows = n;
}

int te::map::DataGridOperation::getPolygonContourPatternRGBANRows()
{
  return m_polygonContourPatternRGBANRows;
}

void te::map::DataGridOperation::setPolygonFillColor(const te::color::RGBAColor& cor)
{
  m_polygonFillColor = cor;
}

te::color::RGBAColor te::map::DataGridOperation::getPolygonFillColor()
{
  return m_polygonFillColor;
}

void te::map::DataGridOperation::setPolygonMarkerType(te::map::PtMarkerType t)
{
  m_polygonMarkerType = t;
}

te::map::PtMarkerType te::map::DataGridOperation::getPolygonMarkerType()
{
  return m_polygonMarkerType;
}
void te::map::DataGridOperation::setPolygonFillMarkerColor(const te::color::RGBAColor& cor)
{
  m_polygonFillMarkerColor = cor;
}

te::color::RGBAColor te::map::DataGridOperation::getPolygonFillMarkerColor()
{
  return m_polygonFillMarkerColor;
}

void te::map::DataGridOperation::setPolygonPatternIcon(char* icon)
{
  if(m_polygonPatternIcon != icon)
    delete []m_polygonPatternIcon;
  m_polygonPatternIcon = icon;
}

char* te::map::DataGridOperation::getPolygonPatternIcon()
{
  return m_polygonPatternIcon;
}

void te::map::DataGridOperation::setPolygonPatternIconSize(int s)
{
  m_polygonPatternIconSize = s;
}

int te::map::DataGridOperation::getPolygonPatternIconSize()
{
  return m_polygonPatternIconSize;
}

void te::map::DataGridOperation::setPolygonPatternIconImageType(te::map::ImageType t)
{
  m_polygonPatternIconImageType = t;
}

te::map::ImageType te::map::DataGridOperation::getPolygonPatternIconImageType()
{
  return m_polygonPatternIconImageType;
}

void te::map::DataGridOperation::setPolygonPatternWidth(int w)
{
  m_polygonPatternWidth = w;
}

int te::map::DataGridOperation::getPolygonPatternWidth()
{
  return m_polygonPatternWidth;
}

void te::map::DataGridOperation::setPolygonPatternRGBA(te::color::RGBAColor** patternRGBA)
{
  if(m_polygonPatternRGBA != patternRGBA)
  {
    for(int i = 0; i < m_polygonPatternRGBANRows; ++i)
      delete []m_polygonPatternRGBA[i];
    delete []m_polygonPatternRGBA;
  }

  m_polygonPatternRGBA = patternRGBA;
}

te::color::RGBAColor** te::map::DataGridOperation::getPolygonPatternRBGA()
{
  return m_polygonPatternRGBA;
}

void te::map::DataGridOperation::setPolygonPatternRGBANCols(int n)
{
  m_polygonPatternRGBANCols = n;
}

int te::map::DataGridOperation::getPolygonPatternRGBANCols()
{
  return m_polygonPatternRGBANCols;
}

void te::map::DataGridOperation::setPolygonPatternRGBANRows(int n)
{
  m_polygonPatternRGBANRows = n;
}

int te::map::DataGridOperation::getPolygonPatternRGBANRows()
{
  return m_polygonPatternRGBANRows;
}

#include "DataSetTable.h"

//TerraLib include files
#include <terralib/dataaccess.h>

te::map::DataSetTable::DataSetTable(te::da::DataSet* dset) :
te::map::AbstractTable(),
m_dataSet(dset)
{
}

te::map::DataSetTable::~DataSetTable()
{
}

void te::map::DataSetTable::resetTable()
{
  te::map::AbstractTable::resetTable();

  for(size_t i = 0; i < m_dataSet->getNumProperties(); i++)
  {
    m_presentOrder.push_back(i);
    m_colVisiblity.push_back(true);
  }

  m_currentRow = 0;
  m_nextRow = m_currentRow+1;

  m_dataSet->moveFirst();
}

size_t te::map::DataSetTable::numRows()
{
	return m_dataSet->size();
}

std::string te::map::DataSetTable::dataAsString(size_t row, size_t column)
{
  if(row >= numRows())
    throw te::common::Exception("row index out of boundaries.");

  if(column >= numColumns())
    throw te::common::Exception("row index out of boundaries.");

  size_t logCol = getLogicalColumn(column);

  size_t c = m_presentOrder[logCol];

  if(row != m_currentRow)
  {
    if(row == m_nextRow)
    {
      if(!m_dataSet->moveNext())
        throw te::common::Exception("Fail to move next on data set.");
    }
    else if(!m_dataSet->move(row))
      throw te::common::Exception("Fail to move dataSet.");

    m_currentRow = row;
    m_nextRow = m_currentRow+1;
  }

  return m_dataSet->getAsString((int) c);
}

std::string te::map::DataSetTable::getColumnName(size_t column) const
{
  size_t c = getLogicalColumn(column);

  std::string pName = m_dataSet->getPropertyName(m_presentOrder[c]);

  return pName;
}

te::gm::Geometry* te::map::DataSetTable::getGeometry(const size_t& row) const
{
  if(row >= m_dataSet->size())
    throw te::common::Exception("Row index out of boundaries.");

  if(!m_geoColumns.empty())
  {
    size_t col = *m_geoColumns.begin();

    if(row != m_currentRow)
      if(!m_dataSet->move(row))
        throw te::common::Exception("Fail to move dataSet forward.");

    te::gm::Geometry* g = m_dataSet->getGeometry((int) col);

    if(row != m_currentRow)
      if(!m_dataSet->move(m_currentRow))
        throw te::common::Exception("Fail to move dataSet backward.");

    return g;
  }

  return AbstractTable::getGeometry(row);
}

std::set<size_t> te::map::DataSetTable::findGeoColsPositions() const
{
  std::set<size_t> cols;

  for(size_t i = 0; i < m_dataSet->getNumProperties(); i++)
  {
    if(m_dataSet->getPropertyDataType(i) == te::dt::GEOMETRY_TYPE)
      cols.insert(i);
  }

  return cols;
}

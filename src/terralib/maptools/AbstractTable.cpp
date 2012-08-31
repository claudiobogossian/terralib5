#include "AbstractTable.h"
#include <terralib/common/Exception.h>

//STL include files
#include <algorithm>

te::map::AbstractTable::AbstractTable() :
m_separator('#')
{
}

te::map::AbstractTable::AbstractTable(const AbstractTable & source)
{
  m_presentOrder = source.m_presentOrder;
  m_colVisiblity = source.m_colVisiblity;
  m_nextRow = source.m_nextRow;
  m_currentRow = source.m_currentRow;
  m_absPKeys = source.m_absPKeys;
  m_pkeys_2rows = source.m_pkeys_2rows; 
  m_geoColumns = source.m_geoColumns;
  m_separator = source.m_separator;
}

te::map::AbstractTable::~AbstractTable()
{
  m_pkeys_2rows.clear();
}

te::map::AbstractTable & te::map::AbstractTable::operator=(const AbstractTable & source)
{
  m_presentOrder = source.m_presentOrder;
  m_colVisiblity = source.m_colVisiblity;
  m_nextRow = source.m_nextRow;
  m_currentRow = source.m_currentRow;
  m_absPKeys = source.m_absPKeys;
  m_pkeys_2rows = source.m_pkeys_2rows; 
  m_separator = source.m_separator;
  m_geoColumns = source.m_geoColumns;

  return *this;
}

void te::map::AbstractTable::setPKeySeparator(const char& separator)
{
  m_separator = separator;
}

char te::map::AbstractTable::getPKeySeparator() const
{
  return m_separator;
}

void te::map::AbstractTable::resetTable()
{
  m_presentOrder.clear();
  m_colVisiblity.clear();
  m_pkeys_2rows.clear(); 

  m_geoColumns = findGeoColsPositions();

  m_currentRow = 0;
  m_nextRow = 0;
}

size_t te::map::AbstractTable::numColumns() const
{
  size_t nCols = std::count(m_colVisiblity.begin(), m_colVisiblity.end(), true);

  return nCols;
}

void te::map::AbstractTable::moveColumn(size_t fromPos, size_t toPos)
{
  size_t s = numColumns();

  if(fromPos < 0 || fromPos >= s)
    throw te::common::Exception("fromPos index out of boundaries.");
  if(toPos < 0 || toPos >= s)
    throw te::common::Exception("toPos index out of boundaries.");;

  size_t value = m_presentOrder[fromPos];

  m_presentOrder.erase(m_presentOrder.begin()+fromPos);

  m_presentOrder.insert(m_presentOrder.begin()+toPos, value);
}

void te::map::AbstractTable::setColumnVisibility(size_t column, bool visible)
{
  if(visible)
    m_colVisiblity[column] = visible;
  else
  {
    size_t c = getLogicalColumn(column);
    m_colVisiblity[c] = visible;
  }
}

void te::map::AbstractTable::setGeoColumnsVisible(bool visibile)
{
  std::set<size_t> gCols = findLogicalGeoColumns();
  std::set<size_t>::iterator it;

  for(it = gCols.begin(); it != gCols.end(); ++it)
    m_colVisiblity[*it] = visibile;
}

void te::map::AbstractTable::setPKeysColumns(const std::vector<size_t> & cols)
{
  std::vector<size_t>::const_iterator it;
  size_t nCols = numColumns();

  for(it = cols.begin(); it != cols.end(); ++it)
    if(*it >= nCols)
      te::common::Exception("Primary key out of boundaries");

  m_absPKeys = cols;

  cleanPreprocessPKeys();
}

std::vector<size_t> te::map::AbstractTable::getPKeysColumns() const
{
  return m_absPKeys;
}

void te::map::AbstractTable::preprocessPKeys(char separator)
{
  m_separator = separator;

  if(!m_pkeys_2rows.empty())
    return;

  size_t nPkeys = m_absPKeys.size();
  size_t nRows = numRows();

  for(size_t i=0; i<nRows; i++)
    m_pkeys_2rows[map2PKey(i)] = i;

  //{
  //  std::string pkey;

  //  for(size_t j=0; j<nPkeys; j++)
  //  {
  //    if(j > 0)
  //      pkey += separator;

  //    pkey += dataAsString(i, m_absPKeys[j]);
  //  }

  //  m_pkeys_2rows[pkey] = i;
  //}
}

void te::map::AbstractTable::cleanPreprocessPKeys()
{
  m_pkeys_2rows.clear();
}

std::string te::map::AbstractTable::map2PKey(size_t row)
{
  if(row >= numRows())
    throw te::common::Exception("Row out of boundaries.");

  size_t nPkeys = m_absPKeys.size();
  std::string pkey;

  for(size_t j=0; j<nPkeys; j++)
  {
    if(j > 0)
      pkey += m_separator;

    pkey += dataAsString(row, m_absPKeys[j]);
  }

  return pkey;
}

int te::map::AbstractTable::map2Row(std::string pkey) const
{
  std::map<std::string, size_t>::const_iterator it = m_pkeys_2rows.find(pkey);
  return ((it == m_pkeys_2rows.end()) ? -1 : ((int)it->second));
}

size_t te::map::AbstractTable::getLogicalColumn(size_t column) const
{
  size_t cols = numColumns();

  if(cols < 0 || column >= cols)
    throw te::common::Exception("Index out of boundaries.");

  if(m_colVisiblity[column])
    return column;

  for(size_t i=column; i<m_colVisiblity.size(); i++)
    if(m_colVisiblity[i])
      return i;

  throw te::common::Exception("Index out of boundaries.");
}

std::set<size_t> te::map::AbstractTable::findLogicalGeoColumns() const
{
  std::set<size_t> cols;
  std::set<size_t>::const_iterator it;

  for(it = m_geoColumns.begin(); it != m_geoColumns.end(); ++it)
  {
    std::vector<size_t>::const_iterator colIt = std::find(m_presentOrder.begin(), m_presentOrder.end(), *it);

    if(colIt != m_presentOrder.end())
      cols.insert(colIt - m_presentOrder.begin());
  }

  return cols;
}

#include "PromoTable.h"

te::map::PromoTable::PromoTable(te::map::AbstractTable* table) :
te::common::Decorator<te::map::AbstractTable>(table, true)
{
  resetTable();
}

te::map::PromoTable::~PromoTable()
{
  m_logRows_pos.clear();
}

void te::map::PromoTable::promoteRows(const std::vector<std::string>& pkeys)
{
  std::vector<std::string>::const_reverse_iterator pkIt;
  size_t pos=0;

  resetRowsOrder();

  for(pkIt=pkeys.rbegin(); pkIt != pkeys.rend(); ++pkIt)
  {
    size_t dsPos = map2Row(*pkIt);

    if(dsPos < 0)
      continue;

    if(m_logRows_pos[dsPos] != dsPos)
    {
      size_t aux = dsPos;

      while(m_logRows_pos[aux] != dsPos)
        aux++;

      dsPos = aux;
    }

    size_t value = m_logRows_pos[dsPos];

    m_logRows_pos.erase(m_logRows_pos.begin()+dsPos);
    m_logRows_pos.insert(m_logRows_pos.begin()+pos, value);

    pos++;
  }
}
			
void te::map::PromoTable::resetTable()
{
  m_decorated->resetTable();

  te::map::AbstractTable::operator=(*m_decorated);

  m_logRows_pos.clear();
}

size_t te::map::PromoTable::numRows()
{
  return m_decorated->numRows();
}

std::string te::map::PromoTable::dataAsString(size_t row, size_t column)
{
  size_t logRow = (m_logRows_pos.empty()) ? row : m_logRows_pos[row];

  return m_decorated->dataAsString(logRow, column);
}
			
std::string te::map::PromoTable::getColumnName(size_t column) const
{
  return m_decorated->getColumnName(column);
}

std::set<size_t> te::map::PromoTable::findGeoColsPositions() const
{
  return m_decorated->findGeoColsPositions();
}

void te::map::PromoTable::resetRowsOrder()
{
  size_t nRows = numRows();

  if(m_logRows_pos.empty())
    m_logRows_pos.resize(nRows);

  for(size_t i=0; i<nRows; i++)
    m_logRows_pos[i] = i;
}

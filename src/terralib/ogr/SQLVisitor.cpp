#include "SQLVisitor.h"

#include "../common/StringUtils.h"
#include "../geometry/Envelope.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/GroupBy.h"
#include "../dataaccess/query/Having.h"
#include "../dataaccess/query/LiteralEnvelope.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/Where.h"


te::ogr::SQLVisitor::SQLVisitor(const te::da::SQLDialect& dialect, std::string& sql) :
te::da::SQLVisitor(dialect, sql),
  m_bbox(0)
{
}

void te::ogr::SQLVisitor::visit(const te::da::LiteralEnvelope& visited)
{
  m_bbox = visited.getValue();

  m_sql += "BBOX)";
}

void te::ogr::SQLVisitor::visit(const te::da::Select& visited)
{
  m_sql += "SELECT FID, ";

  if(visited.getDistinct())
  {
    te::da::SQLVisitor::visitDistinct(*(visited.getDistinct()));
    m_sql += " ";
  }

  if(visited.getFields())
  {
    te::da::SQLVisitor::visit(*(visited.getFields()));
    m_sql += " ";
  }

  if(visited.getFrom())
  {
    te::da::SQLVisitor::visit(*(visited.getFrom()));
    m_sql += " ";
  }

  if(visited.getWhere())
  {
    m_sql += "WHERE ";
    visited.getWhere()->getExp()->accept(*this);
    m_sql += " ";
  }

  if(visited.getGroupBy())
  {
    te::da::SQLVisitor::visit(*(visited.getGroupBy()));
    m_sql += " ";
  }

  if(visited.getHaving())
  {
    m_sql += "HAVING ";
    visited.getHaving()->getExp()->accept(*this);
    m_sql += " ";
  }

  if(visited.getOrderBy())
    te::da::SQLVisitor::visit(*(visited.getOrderBy()));
}

te::gm::Envelope* te::ogr::SQLVisitor::getMBR()
{
  return m_bbox;
}

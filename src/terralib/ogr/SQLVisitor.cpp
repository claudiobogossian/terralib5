#include "SQLVisitor.h"

#include "../common/StringUtils.h"
#include "../geometry/Envelope.h"
#include "../dataaccess/query/LiteralEnvelope.h"


te::ogr::SQLVisitor::SQLVisitor(const te::da::SQLDialect& dialect, std::string& sql) :
te::da::SQLVisitor(dialect, sql)
{
}

void te::ogr::SQLVisitor::visit(const te::da::LiteralEnvelope& visited)
{
  te::gm::Envelope* e = visited.getValue();

  m_sql += "BuildMBR(";
  m_sql += te::common::Convert2String(e->m_llx, 15);
  m_sql += ", ";
  m_sql += te::common::Convert2String(e->m_lly, 15);
  m_sql += ", ";
  m_sql += te::common::Convert2String(e->m_urx, 15);
  m_sql += ", ";
  m_sql += te::common::Convert2String(e->m_ury, 15);
  m_sql += ")";
}

#include "Dialect.h"
#include "MapToolsExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/fe.h>
#include <terralib/maptools.h>

// STL
#include <iostream>

void Filter2Query()
{
  /* Creating an OGC Filter Expression */
  
  // (1): nome = 'MINAS GERAIS'
  te::fe::PropertyName* state = new te::fe::PropertyName("state");
  te::fe::Literal* stateName = new te::fe::Literal("MINAS GERAIS");
  te::fe::BinaryComparisonOp* stateEqual = new te::fe::BinaryComparisonOp(te::fe::Globals::sm_propertyIsEqualTo, state, stateName);

  // (2): populacao < '2.000'
  te::fe::PropertyName* pop = new te::fe::PropertyName("population");
  te::fe::Literal* popValue = new te::fe::Literal("2.000");
  te::fe::BinaryComparisonOp* popLessThan = new te::fe::BinaryComparisonOp(te::fe::Globals::sm_propertyIsLessThan, pop, popValue);

  // (3): Joins the expression (1) and (2) using a binary logic operator AND
  te::fe::BinaryLogicOp* andOp = new te::fe::BinaryLogicOp(te::fe::Globals::sm_and, stateEqual, popLessThan);

  // (4): cidade = 'SERITINGA'
  te::fe::PropertyName* city = new te::fe::PropertyName("city");
  te::fe::Literal* cityName = new te::fe::Literal("SERITINGA");
  te::fe::BinaryComparisonOp* cityEqual = new te::fe::BinaryComparisonOp(te::fe::Globals::sm_propertyIsEqualTo, city, cityName);
  
  // (5): Joins the expression (3) and (4) using a binary logic operator OR
  te::fe::BinaryLogicOp* orOp = new te::fe::BinaryLogicOp(te::fe::Globals::sm_or, andOp, cityEqual);

  // We have a Filter!
  te::fe::Filter* filter = new te::fe::Filter;
  filter->setOp(orOp); // (state = 'MINAS GERAIS' AND populacao < '2.000') OR (city = 'SERITINGA')

  // -----------------------------------------

  /* Let's convert! */

  // Gets an enconder...
  te::map::QueryEncoder converter;
  // ... and convert it to a TerraLib Expression!
  te::da::Expression* exp = converter.getExpression(filter);

  // Let's see the result...
  te::da::SQLDialect* dialect = createDialect(); // a hard-coded dialect
  
  std::string sql;
  te::da::SQLVisitor sqlConverter(*dialect, sql);
  exp->accept(sqlConverter);

  std::cout << "Conversion result is: " << sql << std::endl;
  std::cout << "Press Enter..." << std::endl;
  std::cin.get();

  delete filter;
  delete exp;
  delete dialect;
}

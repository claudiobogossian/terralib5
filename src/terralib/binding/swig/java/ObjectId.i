%{
#include "terralib/dataaccess/dataset/ObjectId.h"
#include "terralib/dataaccess/dataset/ObjectIdSet.h"
%}

namespace std 
{
	%template(ObjectIdVector) vector <te::da::ObjectId*>;
}

#ifdef SWIGJAVA
// ---------------------------------
// Java Documentation
// ---------------------------------

%typemap(javaimports) te::da::ObjectId "
/** 
 * This class represents an unique id for a data set element.
 */"

%javamethodmodifiers te::da::ObjectId::getValueAsString() const"
/**
 * It gets the properties values used to uniquely identify a data set element as string.
 *
 * @return A string that represents the set of values.
 */
  public";
#endif

%ignore te::da::ObjectId::ObjectId;
%ignore te::da::ObjectId::getValue;
%ignore te::da::ObjectId::addValue;
%ignore te::da::ObjectId::clone;
%ignore te::da::ObjectId::operator<;

%include "terralib/dataaccess/dataset/ObjectId.h"

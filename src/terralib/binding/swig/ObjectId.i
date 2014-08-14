%{
#include "terralib/dataaccess/dataset/ObjectId.h"
#include "terralib/dataaccess/dataset/ObjectIdSet.h"
%}

namespace std 
{
	%template(ObjectIdVector) vector <te::da::ObjectId*>;
}

%ignore te::da::ObjectId::ObjectId;
%ignore te::da::ObjectId::getValue;
%ignore te::da::ObjectId::addValue;
%ignore te::da::ObjectId::clone;
%ignore te::da::ObjectId::operator<;

%include "terralib/dataaccess/dataset/ObjectId.h"

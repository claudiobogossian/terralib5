%warnfilter(401) DataSet;

%{
// TerraLib includes
#include "terralib/dataaccess/dataset/DataSet.h"
#include "terralib/dataaccess/utils/Utils.h"
%}

namespace std {
 %template(CharEncodingVector) vector<te::common::CharEncoding>;
}

%nodefaultctor te::da::DataSet;

%ignore te::da::DataSet::getExtent;
%ignore te::da::DataSet::getInt16;
%ignore te::da::DataSet::getInt32;
%ignore te::da::DataSet::getInt32;
%ignore te::da::DataSet::getInt64;
%ignore te::da::DataSet::getInt64;
%ignore te::da::DataSet::getByteArray;
%ignore te::da::DataSet::getGeometry;
%ignore te::da::DataSet::getRaster;
%ignore te::da::DataSet::getDateTime;
%ignore te::da::DataSet::getArray;
%ignore te::da::DataSet::getValue;

%ignore te::common::LoggerConfigurationType;
%ignore te::common::MeasureType;
%ignore te::common::Hemisphere;
%ignore te::common::MachineByteOrder;
%ignore te::common::CharacterEncodingType;

%ignore te::dt::DateTimeType;
%ignore te::dt::DateTimeResolution;
%ignore te::dt::TemporalRelation;
%ignore te::dt::BasicRelation;
%ignore te::dt::StringType;

%include "terralib/common/CharEncodingConv.h"
%include "terralib/common/Enums.h"
%include "terralib/dataaccess/dataset/DataSet.h"
%include "terralib/datatype/Enums.h"

%newobject te::da::DataSet::getWindow();
%newobject te::da::DataSet::getPath();
%newobject te::da::DataSet::getObjectId(const std::vector<std::string>& colsNames);
%newobject te::da::DataSet::getObjectIds(const std::vector<std::string>& colsNames);

%extend te::da::DataSet {
	te::gm::Envelope* getWindow()
	{
		size_t gp = te::da::GetFirstSpatialPropertyPos(self);
		return self->getExtent(gp).release();
	}
	
	te::gm::Geometry* getPath()
	{
		size_t gp = te::da::GetFirstSpatialPropertyPos(self);
		return self->getGeometry(gp).release();
	}
	
	te::da::ObjectId* getObjectId(const std::vector<std::string>& colsNames)
	{
		return te::da::GenerateOID(self, colsNames);
	}
	
	std::vector<te::da::ObjectId*> getObjectIds(const std::vector<std::string>& colsNames)
	{
		std::vector<te::da::ObjectId*> res;
		
		std::auto_ptr<te::da::ObjectIdSet> oids;
		oids.reset(te::da::GenerateOIDSet(self, colsNames));
		
		std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;
		
		for(it=oids->begin(); it!=oids->end(); ++it)
			res.push_back((*it)->clone());
		
		return res;
	}
}

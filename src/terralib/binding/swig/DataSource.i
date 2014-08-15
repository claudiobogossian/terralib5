%warnfilter(401) DataSource;

%{
// TerraLib includes
#include "terralib/dataaccess/datasource/DataSource.h"
#include "terralib/dataaccess/datasource/DataSourceManager.h"

// Boost includes
#include <boost/uuid/random_generator.hpp>
#include "boost/uuid/uuid_io.hpp"
%}

namespace std {
 %template(DataSourceVector) vector<te::da::DataSource*>;
}

%nodefaultctor te::da::DataSource;

%ignore te::da::DataSource::changePropertyDefinition;
%ignore te::da::DataSource::changePropertiesDefinitions;
%ignore te::da::DataSource::setId;
%ignore te::da::DataSource::getTransactor;
%ignore te::da::DataSource::getCapabilities;
%ignore te::da::DataSource::getDialect;
%ignore te::da::DataSource::getDataSet;
%ignore te::da::DataSource::query;
%ignore te::da::DataSource::execute;
%ignore te::da::DataSource::escape;
%ignore te::da::DataSource::getDataSetType;
%ignore te::da::DataSource::getProperties;
%ignore te::da::DataSource::getProperty;
%ignore te::da::DataSource::addProperty;
%ignore te::da::DataSource::getPrimaryKey;
%ignore te::da::DataSource::addPrimaryKey;
%ignore te::da::DataSource::getForeignKey;
%ignore te::da::DataSource::addForeignKey;
%ignore te::da::DataSource::getUniqueKey;
%ignore te::da::DataSource::addUniqueKey;
%ignore te::da::DataSource::getCheckConstraint;
%ignore te::da::DataSource::addCheckConstraint;
%ignore te::da::DataSource::getIndex;
%ignore te::da::DataSource::createDataSet;
%ignore te::da::DataSource::addIndex;
%ignore te::da::DataSource::getSequence;
%ignore te::da::DataSource::getExtent;
%ignore te::da::DataSource::addSequence;
%ignore te::da::DataSource::getIndex;
%ignore te::da::DataSource::add;
%ignore te::da::DataSource::remove;
%ignore te::da::DataSource::update;
%ignore te::da::DataSource::create;

%{
  static te::da::DataSource* MakeDataSource(const std::string& dsType, const std::map<std::string, std::string>& connInfo)
  {
    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string dsId = boost::uuids::to_string(u);
	
	return te::da::DataSourceManager::getInstance().get(dsId, dsType, connInfo).get();
  } 
  
  static te::da::DataSource* GetDataSource(const std::string& dSrcId)
  {
	return te::da::DataSourceManager::getInstance().find(dSrcId).get();
  }
  
  static std::vector<te::da::DataSource*> GetRegisteredDataSources()
  {
	te::da::DataSourceManager::iterator it;
	te::da::DataSourceManager::iterator dBegin = te::da::DataSourceManager::getInstance().begin();
	te::da::DataSourceManager::iterator dEnd = te::da::DataSourceManager::getInstance().end();
	std::vector<te::da::DataSource*> res;
	
	for(it=dBegin; it!=dEnd; ++it)
		res.push_back(it->second.get());
	
	return res;
  }
%}


%include "terralib/dataaccess/datasource/DataSource.h";

%newobject te::da::DataSource::newDataSet(const std::string& name);

%extend te::da::DataSource {
	te::da::DataSet* newDataSet(const std::string& name)
	{
		return self->getDataSet(name).release();
	}
}

// Wrap function
te::da::DataSource* MakeDataSource(const std::string& dsType, const std::map<std::string, std::string>& connInfo);
te::da::DataSource* GetDataSource(const std::string& dSrcId);
std::vector<te::da::DataSource*> GetRegisteredDataSources();

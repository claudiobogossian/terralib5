/*
 * File: DataAccess.i
 *
 * Data Access
 */

#define TEDATAACCESSEXPORT

namespace te {
  namespace common {
    %template(DSMgerSingleton) Singleton < te::da::DataSourceManager >;
  }
}

typedef te::common::Singleton< te::da::DataSourceManager > DSMgerSingleton;

AUTO_PTR_TYPEMAPS(te::dt::DateTime)
AUTO_PTR_TYPEMAPS(te::dt::AbstractData)
AUTO_PTR_TYPEMAPS(te::dt::ByteArray)
AUTO_PTR_TYPEMAPS(te::dt::Array)
AUTO_PTR_TYPEMAPS(te::da::DataSet)
AUTO_PTR_TYPEMAPS(te::da::DataSource)
AUTO_PTR_TYPEMAPS(te::da::DataSourceTransactor)
AUTO_PTR_TYPEMAPS(te::da::Sequence)
AUTO_PTR_TYPEMAPS(te::da::CheckConstraint)
AUTO_PTR_TYPEMAPS(te::da::ForeignKey)
AUTO_PTR_TYPEMAPS(te::da::PrimaryKey)
AUTO_PTR_TYPEMAPS(te::da::UniqueKey)
AUTO_PTR_TYPEMAPS(te::da::Index)
AUTO_PTR_TYPEMAPS(te::dt::Property)
AUTO_PTR_TYPEMAPS(te::da::DataSetTypeCapabilities)
AUTO_PTR_TYPEMAPS(te::da::DataSetType)
AUTO_PTR_TYPEMAPS(te::gm::Envelope)
AUTO_PTR_TYPEMAPS(te::gm::Geometry)
AUTO_PTR_TYPEMAPS(te::rst::Raster)

%nodefaultctor te::da::DataSourceFactory;

%{
#include "terralib/dataaccess/dataset/DataSet.h"
#include "terralib/dataaccess/dataset/DataSetTypeCapabilities.h"
#include "terralib/dataaccess/dataset/DataSetType.h"
#include "terralib/dataaccess/dataset/ObjectId.h"
#include "terralib/dataaccess/dataset/ObjectIdSet.h"
#include "terralib/dataaccess/datasource/DataSource.h"
#include "terralib/dataaccess/datasource/DataSourceTransactor.h"
#include "terralib/dataaccess/datasource/DataSourceInfo.h"
#include "terralib/dataaccess/datasource/DataSourceManager.h"
#include "terralib/dataaccess/datasource/DataSourceFactory.h"

using namespace te::dt;

static void OpenDataSource(const te::da::DataSourceInfo& info)
{
  te::da::DataSourceManager::getInstance().open(info.getId(), info.getType(), info.getConnInfo());
}

%}

%nspace te::da::DataSetType;
%nspace te::da::DataSet;
%nspace te::da::ObjectId;
%nspace te::da::ObjectIdSet;
%nspace te::da::DataSource;
%nspace te::da::DataSourceInfo;
%nspace te::da::DataSourceManager;
%nspace te::da::DataSourceFactory;

%include "terralib/dataaccess/dataset/DataSetType.h"
%include "terralib/dataaccess/dataset/DataSet.h"
%include "terralib/dataaccess/dataset/ObjectId.h"
%include "terralib/dataaccess/dataset/ObjectIdSet.h"
%include "terralib/dataaccess/datasource/DataSource.h"
%include "terralib/dataaccess/datasource/DataSourceInfo.h"
%include "terralib/dataaccess/datasource/DataSourceManager.h"
%include "terralib/dataaccess/datasource/DataSourceFactory.h"

%newobject te::da::DataSourceFactory::make(const std::string& dsType);

//Wrap function
void OpenDataSource(const te::da::DataSourceInfo& info);

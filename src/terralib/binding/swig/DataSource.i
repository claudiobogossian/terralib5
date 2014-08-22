%warnfilter(401) DataSource;

%{
// TerraLib includes
#include "terralib/dataaccess/datasource/DataSource.h"
#include "terralib/dataaccess/datasource/DataSourceManager.h"
#include "terralib/maptools/DataSetLayer.h"
#include "terralib/qt/widgets/layer/utils/DataSet2Layer.h"

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

#ifdef SWIGJAVA
// ---------------------------------
// Java Documentation
// ---------------------------------
%typemap(javaimports) te::da::DataSource "
/** 
 *  An abstract class for data providers like a DBMS, Web Services or a regular file.
 *
 * A data source is the fundamental class of the data access module and
 * it may represent a data repository. Among other things, a repository can be:
 * <ul>
 * <li>a PostgreSQL database</li>
 * <li>an Oracle database</li>
 * <li>an OGC Web Feature Service</li>
 * <li>a directory of ESRI shape-files</li>
 * <li>a data stream</li>
 * </ul>
 *
 * Each data source is characterized by a set of parameters that can be used
 * to set up an access channel to its underlying repository. This information
 * is referred as the data source connection information.
 *
 * A data source exposes the data contained in it as a collection of datasets.
 * The information about the data stored in a data source may be retrieved.
 *
 * Besides the descriptive information about the underlying data repository,
 * each data source also provides information about their capabilities.
 * @see br.inpe.terralib5.DataSet
 */"

%javamethodmodifiers te::da::DataSource::getId() const"
/** 
 * An identification value for the data source.
 *
 * @return The data source identification.
 */
 public";

%javamethodmodifiers te::da::DataSource::getType() const"
/** 
 * It returns the data source type name (in UPPER CASE). Ex: POSTGIS, SQLITE, WFS, WMS, or MYSQL.
 * <p>
 * Note that each data source driver must have a unique name.
 * <p>
 * This method is thread-safe!
 *
 * @return The data source type name. Ex: POSTGIS, SQLITE, WFS, WMS, or MYSQL.
 */
 public";

%javamethodmodifiers te::da::DataSource::getConnectionInfo() const"
/** 
 * It returns the set of parameters used to set up the access channel to the underlying repository.
 * <p>
 * This is the connection information used by a data source in order to enter in an operational mode,
 * when the open method is called.
 * <p>
 * The key-value-pairs (kvp) may contain information about: maximum number of accepted connections,
 * user name and password required for establishing a connection, the url of a service, or any other
 * information needed by the data source to operate. This information is dependent on the data source
 * driver, so check the driver documentation for any additional information about the kvp.
 * <p>
 * This method is thread-safe!
 *
 * @return An associative container (key-value-pair) with information about the data source.
 */
 public";

%javamethodmodifiers te::da::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo) "
/** 
 * It sets the connection information to be used when connecting to the data source.
 *
 * @param connInfo Key-value-pairs (kvp) with the connection information.
 */
 public";

%javamethodmodifiers te::da::DataSource::open() "
/** 
 * It opens the data source and makes it ready for using.
 * <p>
 * If the subclass needs to open a connection to a database server, 
 * to open a file, or to get information from a Web Service, this
 * method can do this kind of job to prepare the data source to be
 * in an operational mode. It will use the connection information
 * provided by the setConnectionInfo methods.
 * <p>
 * This method is not thread-safe!
 *
 * @throws java.io.IOException An exception can be thrown, if the data source cannot be opened.
 */
 public";

%javamethodmodifiers te::da::DataSource::close() "
/** 
 * It closes the data source and clears all the resources used by its internal communication channel.
 * <p>
 * This method closes any connection, any opened file, and releases any other resources.
 * <p>
 * This method is not thread-safe!
 *
 * @throws java.io.IOException An exception can be thrown, if the data source cannot be closed.
 */
 public";

%javamethodmodifiers te::da::DataSource::isOpened() const"
/** 
 * It returns true if the data source is opened, otherwise it returns false.
 * <p>
 * This method will not check if the data source is available for using;
 * it will just answer if the data source has already been opened.
 * If you want to know if the data source is available for using,
 * check the isValid() method.
 * <p>
 * This method not thread-safe!
 *
 * @return It returns true if the data source is opened; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::isValid() const"
/** 
 * It checks if the data source is valid (available for using).
 * <p>
 * For a DBMS, it will check the opened connections.
 * For a WFS client, it will check if the server is reachable.
 * For a file, it will check if it can be read.
 * <p>
 * This method is not thread-safe!
 *
 * @return It returns true if the data source is available; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::isDataSetNameValid(const std::string& datasetName) "
/** 
 * It checks if the given dataset name is valid.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName A dataset name whose validity will be checked.
 *
 * @return True, if the name is valid according to the data source rules.
 */
 public";

%javamethodmodifiers te::da::DataSource::isPropertyNameValid(const std::string& propertyName) "
/** 
 * It checks if the given property name is valid.
 *
 * This method is thread-safe!
 *
 * @param propertyName A property name whose validity will be checked.
 *
 * @return True, if the name is valid according to the data source rules.
 */
 public";

%javamethodmodifiers te::da::DataSource::getDataSetNames() "
/** 
 * It gets the dataset names available in the data source.
 * <p>
 * Note that each dataset in the data source must have a unique name. For example, in a DBMS the name
 * may contain the schema name before the table name separated by a dot notation (\".\").
 * <p>
 * This method is thread-safe!
 *
 * @return The dataset names available in the data source.
 *
 * @throws java.io.IOException An exception can be thrown, if the dataset names could not be retrieved.
 */
 public";

%javamethodmodifiers te::da::DataSource::getNumberOfDataSets() "
/** 
 * It retrieves the number of data sets available in the data source.
 * <p>
 * This method is thread-safe!
 *
 * @throws java.io.IOException An exception can be thrown, if the number of datasets could not be retrieved.
 *
 * @return The number of data sets available in the data source.
 */
 public";

%javamethodmodifiers te::da::DataSource::getPropertyNames(const std::string& datasetName) "
/** 
 * It gets the property names of the given dataset.
 * <p>
 * Note that each dataset in the data source must have a unique name. For example, in a DBMS the name
 * may contain the schema name before the table name separated by a dot notation (\".\").
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @throws java.io.IOException An exception can be thrown, if the property names of the dataset could not be retrieved.
 *
 * @return The property names of the dataset.
 */
 public";

%javamethodmodifiers te::da::DataSource::getNumberOfProperties(const std::string& datasetName) "
/** 
 * It gets the number of properties of the given dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @throws java.io.IOException An exception can be thrown, if the number of dataset properties could not be retrieved.
 *
 * @return The number of dataset properties.
 */
 public";

%javamethodmodifiers te::da::DataSource::propertyExists(const std::string& datasetName, const std::string& name) "
/** 
 * It checks if a property with the given name exists in the dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName  The dataset name.
 *
 * @param name The property name.
 *
 * @throws java.io.IOException An exception can be thrown, if the existence of the dataset property could not be obtained.
 *
 * @return True, if the property exists in the dataset; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::dropProperty(const std::string& datasetName, const std::string& name) "
/** 
 * It removes a property from the given dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset from where the given property will be removed.
 *
 * @param name The property to be removed from the dataset.
 *
 * @throws java.io.IOException An exception can be thrown, if the dataset property could not be removed.
 */
 public";

%javamethodmodifiers te::da::DataSource::renameProperty(const std::string& datasetName, const std::string& propertyName, const std::string& newPropertyName) "
/** 
 * It renames a property of the given dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset containing the property to be renamed.
 *
 * @param propertyName The property to be renamed from the dataset.
 *
 * @param newPropertyName The new property name.
 *
 * @throws java.io.IOException An exception can be thrown, if the dataset property could not be renamed.
 */
 public";

%javamethodmodifiers te::da::DataSource::primaryKeyExists(const std::string& datasetName, const std::string& name) "
/** 
 * It checks if a primary key exists in the dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @param name The primary key name.
 *
 * @throws java.io.IOException An exception can be thrown, if the existence of the primary key could not be determined.
 *
 * @return True, if a primary key exists in the dataset; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::dropPrimaryKey(const std::string& datasetName) "
/** 
 * It removes the primary key constraint from the dataset schema.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName  The dataset from where the primary key will be removed.
 *
 * @throws java.io.IOException An exception can be thrown, if the primary key could not be dropped from the dataset schema.
 */
 public";

%javamethodmodifiers te::da::DataSource::getForeignKeyNames(const std::string& datasetName) "
/** 
 * It gets the foreign key names of the given dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @throws java.io.IOException An exception can be thrown, if the foreign key names could not be retrieved.
 *
 * @return The foreign key names of the dataset.
 */
 public";

%javamethodmodifiers te::da::DataSource::foreignKeyExists(const std::string& datasetName, const std::string& name) "
/** 
 * It checks if a foreign key with the given name exists in the data source.
 *
 * @param datasetName The dataset name.
 *
 * @param name The foreign key name.
 *
 * @throws java.io.IOException An exception can be thrown, if the existence of the foreign key could not be obtained.
 *
 * @return True, if the foreign key exists in the dataset; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::dropForeignKey(const std::string& datasetName, const std::string& fkName) "
/** 
 * It removes the foreign key constraint from the dataset schema.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset where the foreign key will be removed.
 *
 * @param fkName The foreign key to be removed.
 *
 * @throws java.io.IOException An exception can be thrown, if the foreign key could not be removed from the dataset schema.
 */
 public";

%javamethodmodifiers te::da::DataSource::getUniqueKeyNames(const std::string& datasetName) "
/** 
 * It gets the unique key names of the given dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @throws java.io.IOException An exception can be thrown, if the unique key names could not be obtained.
 *
 * @return The unique key names of the dataset.
 */
 public";

%javamethodmodifiers te::da::DataSource::uniqueKeyExists(const std::string& datasetName, const std::string& name) "
/** 
 * It checks if a unique key with the given name exists in the dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 * 
 * @param name The unique key name.
 *
 * @throws java.io.IOException An exception can be thrown, if the existence of the unique key could not be determined.
 *
 * @return True, if the unique key exists in the dataset; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::dropUniqueKey(const std::string& datasetName, const std::string& name) "
/** 
 * It removes the unique key constraint from the dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset from where the unique key will be removed.
 *
 * @param name The unique key constraint name.
 *
 * @throws java.io.IOException An exception can be thrown, if the unique key could not be removed from the dataset schema.
 */
 public";

%javamethodmodifiers te::da::DataSource::getCheckConstraintNames(const std::string& datasetName) "
/** 
 * It gets the check constraint names of the given dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @throws java.io.IOException An exception can be thrown, if the check constraint names could not be retrieved.
 *
 * @return The check constraint names of the dataset.
 */
 public";

%javamethodmodifiers te::da::DataSource::checkConstraintExists(const std::string& datasetName, const std::string& name) "
/** 
 * It checks if a check-constraint with the given name exists in the data source.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @param name The check-constraint name.
 *
 * @throws java.io.IOException An exception can be thrown, if the existence of the check constraint could not be determined.
 *
 * @return True, if the check-constraint exists in the dataset; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::dropCheckConstraint(const std::string& datasetName, const std::string& name) "
/** 
 * It removes the check constraint from the dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset from where the check constraint will be removed.
 * 
 * @param name The check constraint to be removed.
 *
 * @throws java.io.IOException An exception can be thrown, if the check constraint could not be removed from the dataset schema.
 */
 public";

%javamethodmodifiers te::da::DataSource::getIndexNames(const std::string& datasetName) "
/** 
 * It gets the index names of the given dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @throws java.io.IOException An exception can be thrown, if the index names could not be retrieved.
 *
 * @return The index names of the given dataset.
 */
 public";

%javamethodmodifiers te::da::DataSource::indexExists(const std::string& datasetName, const std::string& name) "
/** 
 * It checks if an index with the given name exists in the dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @param name The index name.
 *
 * @throws java.io.IOException An exception can be thrown, if the index existence could not be determined.
 *
 * @return True, if the index exists in the dataset; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::dropIndex(const std::string& datasetName, const std::string& idxName) "
/** 
 * It removes the index from the given dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset where the index will be removed.
 *
 * @param idxName The index to be removed.
 *
 * @throws java.io.IOException An exception can be thrown, if the index could not be removed from the dataset schema.
 */
 public";

%javamethodmodifiers te::da::DataSource::getSequenceNames() "
/** 
 * It gets the sequence names available in the data source.
 * <p>
 * Note that each sequence in the data source must have a unique name. For example, in a DBMS the name
 * may contain the schema name before the sequence name separated by a dot notation (\".\").
 * <p>
 * This method is thread-safe!
 *
 * @throws java.io.IOException An exception can be thrown, if the sequence names could not be retrieved.
 *
 * @return The sequence names of the data source.
 */
 public";

%javamethodmodifiers te::da::DataSource::sequenceExists(const std::string& name) "
/** 
 * It checks if a sequence with the given name exists in the data source.
 * <p>
 * This method is thread-safe!
 *
 * @param name The sequence name.
 *
 * @throws java.io.IOException An exception can be thrown, if the index existence could not be determined.
 *
 * @return True, if the sequence exists in the data source; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::dropSequence(const std::string& name) "
/** 
 * It removes the sequence from the data source.
 * <p>
 * This method is thread-safe!
 *
 * @param name The sequence that will be removed.
 *
 * @throws java.io.IOException An exception can be thrown, if the sequence could not be removed from the data source.
 */
 public";

%javamethodmodifiers te::da::DataSource::getNumberOfItems(const std::string& datasetName) "
/** 
 * It retrieves the number of items of the given dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param datasetName The dataset name.
 *
 * @throws java.io.IOException An exception can be thrown, if the number of items of the dataset could not be retrieved.
 *
 * @return The number of items of the given dataset.
 */
 public";

%javamethodmodifiers te::da::DataSource::hasDataSets() "
/** 
 * It checks if the data source has any dataset.
 * <p>
 * This method is thread-safe!
 *
 * @throws java.io.IOException An exception can be thrown, if it is not possible to check if the data source has datasets .
 *
 * @return True, if the data source has datasets; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::dataSetExists(const std::string& name) "
/** 
 * It checks if a dataset with the given name exists in the data source.
 * <p>
 * This method is thread-safe!
 *
 * @param name The dataset name.
 *
 * @throws java.io.IOException An exception can be thrown, if the existence of a dataset in the data source could not be determined.
 *
 * @return True, if the dataset exists in the data source; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::cloneDataSet(const std::string& name, const std::string& cloneName, const std::map<std::string, std::string>& options) "
/** 
 * It clones the dataset in the data source.
 * <p>
 * This method is thread-safe!
 *
 * @param name The dataset to be cloned.
 *
 * @param cloneName The name of the cloned dataset.
 *
 * @param options A list of optional modifiers. It is driver specific.
 *
 * @throws java.io.IOException An exception can be thrown, if the dataset schema could not be cloned.
 */
 public";

%javamethodmodifiers te::da::DataSource::dropDataSet(const std::string& name) "
/** 
 * It removes the dataset schema from the data source.
 * <p>
 * This method is thread-safe!
 *
 * @param name The dataset name whose schema will be removed from the data source.
 *
 * @throws java.io.IOException An exception can be thrown, if the dataset could not be removed from the data source.
 */
 public";

%javamethodmodifiers te::da::DataSource::renameDataSet(const std::string& name, const std::string& newName) "
/** 
 * It renames a dataset.
 * <p>
 * This method is thread-safe!
 *
 * @param name The name of the dataset to be renamed.
 *
 * @param newName The new dataset name.
 *
 * @throws java.io.IOException An exception can be thrown, if the dataset could not be renamed.
 */
 public";

%javamethodmodifiers te::da::DataSource::getEncoding() "
/** 
 * It return the DataOurce current encoding.
 *
 * @return The DataSource current encoding.
 */
 public";

%javamethodmodifiers te::da::DataSource::drop(const std::string& dsType, const std::map<std::string, std::string>& dsInfo) "
/** 
 * It removes a data source identified by its connection information and the driver type.
 * <p>
 * Note that no other instance of the data source to be removed can be opened, when calling this method.
 * <p>
 * This method is thread-safe!
 *
 * @param dsType The data source type name (example: POSTGIS, ORACLE, SQLITE).
 *
 * @param dsInfo The connection information for removing the data source.
 *
 * @throws java.io.IOException An exception can be thrown, if the data source could not be removed.
 */
 public";

%javamethodmodifiers te::da::DataSource::exists(const std::string& dsType, const std::map<std::string, std::string>& dsInfo) "
/** 
 * It checks if the data source exists with the connection information and the driver type.
 * <p>
 * This method is thread-safe!
 *
 * @param dsType The data source type name (example: POSTGIS, ORACLE, SQLITE).
 *
 * @param dsInfo The data source information.
 *
 * @throws java.io.IOException An exception can be thrown, if the data source exists in the driver.
 *
 * @return True, if the data source exists; otherwise, it returns false.
 */
 public";

%javamethodmodifiers te::da::DataSource::getDataSourceNames(const std::string& dsType, const std::map<std::string, std::string>& info) "
/** 
 * It returns the data source names available in the driver.
 *
 * @param dsType The type name of the data source(example: PostGIS, Oracle, WFS).
 *
 * @param dsInfo The information about the data sources.
 *
 * @return The data source names available.
 * 
 * @throws java.io.IOException An exception can be thrown, if the list of data source names could not be retrieved.
 */
 public";

%javamethodmodifiers te::da::DataSource::getEncodings(const std::string& dsType, const std::map<std::string, std::string>& info) "
/** 
 * It gets the encoding names of the data source.
 * 
 * @param dsType The data source type name (example: PostGIS, Oracle, WFS).
 *
 * @param dsInfo The data source information.
 *
 * @throws java.io.IOException An exception can be thrown, if the encoding names could not be retrieved.
 * 
 * @return The encoding types of the data source.
 */
 public";

%javamethodmodifiers te::da::DataSource::newDataSet(const std::string& name) "
/**
 * Retrieves the dataset named as <i>name</i>.
 *
 * @param name The name of the dataset.
 *
 * @return The data set searched or <i>null</i> if no data set was found.
 */
 public";
  
%javamethodmodifiers MakeDataSource(const std::string& dsType, const std::map<std::string, std::string>& connInfo) "
/** 
 * Creates and opens an existent data source.
 * <p>
 * See the documentation of the data source plugins of the TerraLib for details of <i>dsType</i> and <i>connInfo</i>.
 *
 * @param dsType Name of the driver of the data source. 
 *
 * @param connInfo Information about the data source.
 */
 public";

%javamethodmodifiers GetDataSource(const std::string& dSrcId) "
/**
 * Retrieves a previously opened data source.
 *
 * @param dSrcId Identifier of the data source to be searched.
 *
 * @return A data source if encountered or <i>null</i>.
 */
 public";

%javamethodmodifiers GetDataSetsAsLayers(te::da::DataSource* ds) "
/**
 * Returns all data sets in the data source as layers, each one as a single layer.
 * <p>
 * Note that all layers are visible.
 * @param ds The data source to be processed.
 *
 * @return A set of layers of all data sets.
 */
 public";
 
#endif

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
  
  static std::vector<te::map::AbstractLayer*> GetDataSetsAsLayers(te::da::DataSource* ds)
  {
    std::vector<te::map::AbstractLayer*> res;
    std::vector<std::string> dsets = ds->getDataSetNames();
    std::vector<std::string>::iterator it;
    
    for(it=dsets.begin(); it!=dsets.end(); ++it)
    {
      te::qt::widgets::DataSet2Layer converter(ds->getId());
      std::auto_ptr<te::da::DataSetType> dt = ds->getDataSetType(*it);
      te::da::DataSetTypePtr dtpt(dt.release());
      te::map::AbstractLayerPtr lp = converter(dtpt);
      te::map::AbstractLayer* l = lp.get();
      l->attach();
      l->setVisibility(te::map::VISIBLE);
      res.push_back(l);
    }
    
    return res;
  }
%}

%include "terralib/dataaccess/datasource/DataSource.h";

%newobject te::da::DataSource::newDataSet(const std::string& name);
%newobject GetDataSetsAsLayers(te::da::DataSource* ds);

%extend te::da::DataSource {
	te::da::DataSet* newDataSet(const std::string& name)
	{
		return self->getDataSet(name).release();
	}
}

// Wrap function
te::da::DataSource* MakeDataSource(const std::string& dsType, const std::map<std::string, std::string>& connInfo);
te::da::DataSource* GetDataSource(const std::string& dSrcId);
std::vector<te::map::AbstractLayer*> GetDataSetsAsLayers(te::da::DataSource* ds);

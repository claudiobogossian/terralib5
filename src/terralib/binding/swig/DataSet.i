%warnfilter(401) DataSet;

#ifdef SWIGJAVA
// ---------------------------------
// Java Documentation
// ---------------------------------

%typemap(javaimports) te::da::DataSet "
/** 
 * A dataset is the unit of information manipulated by the data access module of TerraLib.
 * <p>
 * A dataset is a container for a data collection with an internal pointer
 * pointing to a specific item in the collection. There are move methods
 * that can be used to set the position of this internal pointer.
 * <p>
 * When the dataset is created, its internal pointer
 * points to a sentinel location before the first item
 * of the collection. So, in order to retrieve data
 * from this collection one of the move methods must be called before.
 * <p>
 * Each item of the dataset is composed by any
 * number of properties. You have special methods to
 * query the property information (name, data type, constraints).
 * <p>
 * The individual value of a property of an item in the collection,
 * can be retrieved by an appropriated get method. These methods
 * retrieve the value by an integer index identifying the property position or
 * by a string with the property name.
 * <p>
 * When the dataset is the result of a query involving several
 * datasets, you can find out the dataset that gave the original
 * dataset name of a specific property.
 * <p>
 * A dataset can be connected or disconnected. A connected dataset, after its creation through
 * the data source transactor, continues to depend on the connection given by its associated
 * data source. Differently, a disconnected dataset, after its creation, no more depends of the
 * connection given by the data source, and it continues to live after the connection has been
 * released to the data source.
 * <p>
 * Note that a geometric or raster property is represented just like any other data type.
 * Our design also allows a dataset to have multiple geometric or raster properties.
 *
 * @see br.inpe.terralib5.DataSource
 */"

%javamethodmodifiers te::da::DataSet::getTraverseType() const"
/**
 * Returns the traverse type supported by the DataSet.
 *
 * @return The traverse type supported by the DataSet.
 *
 * @see br.inpe.terralib5.TraverseType
 */
  public";

%javamethodmodifiers te::da::DataSet::getAccessPolicy() const"
/**
 * It returns the read and write permission associated to the dataset.
 *
 * @return The read and write permission associated to the dataset.
 *
 * @see br.inpe.terralib5.AccessPolicy
 */
  public";

%javamethodmodifiers te::da::DataSet::getNumProperties() const"
/**
 * It returns the number of properties that composes an item of the dataset.
 *
 * @return The number of properties that composes an item of the dataset.
 */
  public";

%javamethodmodifiers te::da::DataSet::getPropertyDataType(std::size_t i) const"
/**
 * It returns the underlying data type of the property at position pos.
 *
 * @param i The property position of interest.
 *
 * @return The underlying data type of the property at position pos.
 */
  public";

%javamethodmodifiers te::da::DataSet::getPropertyName(std::size_t i) const"
/**
 * It returns the property name at position pos.
 *
 * @param i The property name at the position of interest.
 *
 * @return The property name at position pos.
 */
  public";

%javamethodmodifiers te::da::DataSet::getPropertyCharEncoding(std::size_t i) const"
/**
 * It returns the property character encoding at position pos.
 * Note that the property at position <i>pos</i> must be a textual property.
 *
 * @param i The property the position of interest.
 *
 * @return The property character encoding.
 *
 * @throws java.io.IOException It throws an exception if the property at position pos is not a textual property.
 */
  public";

%javamethodmodifiers te::da::DataSet::getDatasetNameOfProperty(std::size_t i) const"
/**
 * It returns the underlying dataset name of the property at position pos.
 *
 * @param i The property position of interest.
 *
 * @return The underlying dataset name of the property at position pos.
 */
  public";

%javamethodmodifiers te::da::DataSet::isEmpty() const"
/**
 * It returns true if the collection is empty.
 *
 * @return True, if the collection is empty.
 */
  public";

%javamethodmodifiers te::da::DataSet::isConnected() const"
/**
 * It returns true if the dataset is connected and false if it is disconnected.
 *
 * A dataset can be connected or disconnected. A connected dataset, after its creation through
 * the data source transactor, continues to depend on the connection given by its associated
 * data source. Differently, a disconnected dataset, after its creation, no more depends of the
 * connection given by the data source, and it continues to live after the connection has been
 * released to the data source.
 *
 * @return True, if the dataset is connected, or false if it is disconnected.
 */
  public";

%javamethodmodifiers te::da::DataSet::size() const"
/**
 * It returns the collection size, if it is known.
 * It may return std::string::npos if the size is not known,
 * or it would be too costly to compute it.
 *
 * @return The size of the collection, if it is known.
 */
  public";

%javamethodmodifiers te::da::DataSet::moveNext() "
/**
 * It moves the internal pointer to the next item of the collection.
 *
 * You always has to call this method in order to move the internal pointer to the first
 * item in the collection. This method can be used to traverse a dataset.
 * Note that all dataset types support this method: FORWARDONLY, BIDIRECTIONAL and RANDOM.
 *
 * @return True if the internal pointer is on a valid item, or false otherwise.
 */
  public";

%javamethodmodifiers te::da::DataSet::movePrevious() "
/**
 * It moves the internal pointer to the previous item of the collection.
 * Note that this method is not supported by FORWARDONLY datasets.
 *
 * @return True, if the internal pointer (cursor position) is on a valid item, or false otherwise.
 */
  public";

%javamethodmodifiers te::da::DataSet::moveBeforeFirst() "
/**
 * It moves the internal pointer to a position before the first item in the collection.
 * Note that this method is not supported by FORWARDONLY datasets.
 *
 * @return True, if it was possible to move to a sentinel position before the first item in the collection.
 */
  public";

%javamethodmodifiers te::da::DataSet::moveFirst() "
/**
 * It moves the internal pointer to the first item in the collection.
 * Note that this method is not supported by FORWARDONLY datasets.
 *
 * @return True, if it was possible to move to the first item in the collection.
 */
  public";

%javamethodmodifiers te::da::DataSet::moveLast() "
/**
 * It sets the dataset internal pointer to the last item in the collection.
 *
 * @return True, if it was possible to move to the last item in the collection.
 */
  public";

%javamethodmodifiers te::da::DataSet::move(std::size_t i) "
/**
 * It moves the dataset internal pointer to a given position.
 * Note that the first item in the collection starts at address 0.
 *
 * @param i The position the dataset internal pointer must be set up.
 *
 * @return True, if it was possible to move the dataset reading to the given position.
 */
  public";

%javamethodmodifiers te::da::DataSet::isAtBegin() const"
/**
 * It tells if the dataset internal pointer is on the first element of the collection or not.
 *
 * @return True if the dataset internal pointer is on the first element otherwise it returns false.
 */
  public";

%javamethodmodifiers te::da::DataSet::isBeforeBegin() const"
/**
 * It tells if the dataset internal pointer is in a position before the first element of the collection or not.
 *
 * @return True, if the dataset internal pointer is in a position before the first element otherwise it returns false.
 */
  public";

%javamethodmodifiers te::da::DataSet::isAtEnd() const"
/**
 * It tells if the dataset internal pointer is on the last element of the collection.
 *
 * @return True, if the dataset internal pointer is on the last element otherwise it returns false.
 */
  public";

%javamethodmodifiers te::da::DataSet::isAfterEnd() const"
/**
 * It tells if the dataset internal pointer is on the sentinel position after the last element of the collection or not.
 *
 * @return True, if the dataset internal pointer is on the sentinel position after the last element otherwise it returns false.
 */
  public";

%javamethodmodifiers te::da::DataSet::getChar(std::size_t i) const"
/**
 * Method for retrieving a signed character attribute value (1 byte long).
 *
 * @param i The attribute index.
 *
 * @return The signed character attribute value (1 byte long) in the given position.
 */
  public";

%javamethodmodifiers te::da::DataSet::getChar(const std::string& name) const"
/**
 * Method for retrieving a signed character attribute value (1 byte long).
 *
 * @param name The attribute name.
 *
 * @return The signed character attribute value (1 byte long) with the given name.
 */
  public";

%javamethodmodifiers te::da::DataSet::getUChar(std::size_t i) const"
/**
 * Method for retrieving an unsigned character attribute value (1 byte long).
 *
 * @param i The attribute index.
 *
 * @return The unsigned character attribute value (1 byte long) in the given position.
 */
  public";

%javamethodmodifiers te::da::DataSet::getUChar(const std::string& name) const"
/**
 * Method for retrieving an unsigned character attribute value (1 byte long).
 *
 * @param name The attribute name.
 *
 * @return The unsigned character attribute value (1 byte long) with the given name.
 */
  public";

%javamethodmodifiers te::da::DataSet::getBool(std::size_t i) const"
/**
 * Method for retrieving a boolean attribute value.
 *
 * @param i The attribute index.
 *
 * @return The boolean attribute value in the given position.
 */
  public";

%javamethodmodifiers te::da::DataSet::getBool(const std::string& name) const"
/**
 * Method for retrieving a boolean attribute value.
 *
 * @param name The attribute name.
 *
 * @return The boolean attribute value with the given name.
 */
  public";

%javamethodmodifiers te::da::DataSet::getFloat(std::size_t i) const"
/**
 * Method for retrieving a float attribute value.
 *
 * @param i The attribute index.
 *
 * @return The float attribute value in the given position.
 */
  public";

%javamethodmodifiers te::da::DataSet::getFloat(const std::string& name) const"
/**
 * Method for retrieving a float attribute value.
 *
 * @param name The attribute name.
 *
 * @return The float attribute value with the given name.
 */
  public";

%javamethodmodifiers te::da::DataSet::getDouble(std::size_t i) const"
/**
 * Method for retrieving a double attribute value.
 *
 * @param i The attribute index.
 *
 * @return The double attribute value in the given position.
 */
  public";

%javamethodmodifiers te::da::DataSet::getDouble(const std::string& name) const"
/**
 * Method for retrieving a double attribute value.
 *
 * @param name The attribute name.
 *
 * @return The double attribute value with the given name.
 */
  public";

%javamethodmodifiers te::da::DataSet::getNumeric(std::size_t i) const"
/**
 * Method for retrieving a numeric attribute value.
 *
 * @param i The attribute index.
 *
 * @return The numeric attribute value in the given position.
 */
  public";

%javamethodmodifiers te::da::DataSet::getNumeric(const std::string& name) const"
/**
 * Method for retrieving a numeric attribute value.
 *
 * @param name The attribute name.
 *
 * @return The numeric attribute value with the given name.
 */
  public";

%javamethodmodifiers te::da::DataSet::getString(std::size_t i) const"
/**
 * Method for retrieving a string value attribute.
 *
 * @param i The attribute index.
 *
 * @return The string attribute value in the given position.
 */
  public";

%javamethodmodifiers te::da::DataSet::getString(const std::string& name) const"
/**
 * Method for retrieving a string attribute value.
 *
 * @param name The attribute name.
 *
 * @return The string attribute value with the given name.
 */
  public";

%javamethodmodifiers te::da::DataSet::getAsString(std::size_t i) const"
/**
 * Method for retrieving a data value as a string plain representation.
 *
 * Note that it is safe to call this method for any data type, the data source implementation will take care of how to convert the internal representation to a string.
 *
 * @param i The attribute index.
 *
 * @return The attribute value in a string format.
 */
  public";

%javamethodmodifiers te::da::DataSet::getAsString(std::size_t i, int precision) const"
/**
 * Method for retrieving a data value as a string plain representation.
 *
 * Note that it is safe to call this method for any data type, the data source implementation will take care of how to convert the internal representation to a string.
 *
 * @param i The attribute index.
 *
 * @param precision The precision in the conversion.
 * 
 * @return The attribute value in a string format.
 */
  public";

%javamethodmodifiers te::da::DataSet::getAsString(const std::string& name) const"
/**
 * Method for retrieving a data value as a string plain representation.
 *
 * Note that it is safe to call this method for any data type, the data source implementation will take care of how to convert the internal representation to a string.
 *
 * @param name The attribute name.
 *
 * @return The attribute value in a string format.
 */
  public";

%javamethodmodifiers te::da::DataSet::getAsString(const std::string& name, int precision) const"
/**
 * Method for retrieving a data value as a string plain representation.
 *
 * Note that it is safe to call this method for any data type, the data source implementation will take care of how to convert the internal representation to a string.
 *
 * @param name The attribute name.
 *
 * @param precision The precision in the conversion.
 *
 * @return The attribute value in a string format.
 */
  public";

%javamethodmodifiers te::da::DataSet::isNull(std::size_t i) const"
/**
 * It checks if the attribute value is <i>null</i>.
 *
 * @param i The attribute index.
 *
 * @return True if the attribute value is <i>null</i>.
 */
  public";

%javamethodmodifiers te::da::DataSet::isNull(const std::string& name) const"
/**
 * It checks if the attribute value is <i>null</i>.
 *
 * @param name The attribute name.
 *
 * @return True if the attribute value is <i>null</i>.
 */
  public";
  
%javamethodmodifiers te::da::DataSet::getWindow() "
/**
 * Returns the bounding rectangle of the DataSet.
 *
 * @return The bounding rectangle of the DataSet.
 */
  public";
  
%javamethodmodifiers te::da::DataSet::getPath() "
/**
 * Returns the geometry of the current feature.
 * <ul>
 *  <li> Paths with just one point are points.</li>
 *  <li> Paths that haves more than once points and aren't closed are lines.</li>
 *  <li> Paths that are closed are polygons.</li>
 * </ul>
 *
 * @return The geometry of the current feature.
 */
  public";
 
%javamethodmodifiers te::da::DataSet::getObjectId(const std::vector<std::string>& colsNames) "
/**
 * Returns, defined by <i>colsNames</i>, the ObjectId of the current feature.
 * Note that the select columns must identify uniquely each row of the DataSet.
 *
 * @param colsNames Names of columns that compose the ObjectId.
 *
 * @return The ObjectId of the current feature.
 *
 * @see br.inpe.terralib5.ObjectId
 */
  public";
 
%javamethodmodifiers te::da::DataSet::getObjectIds(const std::vector<std::string>& colsNames) "
/**
 * Returns a vector of ObjectId, defined by <i>colsNames</i>, of each row of the DataSet.
 *
 * @param colsNames Names of the columns that compose the ObjectId.
 *
 * @see br.inpe.terralib5.ObjectId
 */
  public";
#endif

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

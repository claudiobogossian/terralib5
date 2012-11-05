/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/dataaccess/dataset/DataSetItem.h

  \brief A single element in a data collection represented by a dataset is called DataSetItem.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASETITEM_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASETITEM_H

// TerraLib
#include "../../datatype/Enums.h"
#include "../Config.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/cstdint.hpp>

// Forward declarations
namespace te { namespace dt { class Array; class ByteArray; class DateTime; } }
namespace te { namespace gm { class Geometry; } }
namespace te { namespace rst { class Raster; } }

namespace te
{
  namespace da
  {
// Forward declarations
    class DataSet;
    class DataSetType;

    /*!
      \class DataSetItem

      \brief A single element in a data collection represented by a dataset is called DataSetItem.

      A dataset is a collection of dataset items.
      Each dataset item can be a set of primitive values or more complex values (objects and structs),
      which can be used to represent a single row of a database table,
      a single raster data, a collection of raster data or even other datasets.

      In order to know what are the components of a dataset item you can query
      its type definition (DataSetType).

      To retrieve the component values of a data item, there are
      getter methods that can access safely the data using a specific
      data type.

      \sa DataSet, DataSetType
    */
    class TEDATAACCESSEXPORT DataSetItem
    {
      public:

        /*! \brief Default constructor. */
        DataSetItem() {}

        /*! \brief Virtual destructor. */
        virtual ~DataSetItem() {}

        /** @name DataSetItem Methods
         *  Methods for obtaining information about a given dataset item.
         */
        //@{

        /*!
          \brief It returns the description of this dataset.

          \return The description of this dataset.

          \note The caller of this method will NOT take the ownership of the returned information.
        */
        virtual DataSetType* getType() = 0;

        /*!
          \brief It returns the description of this dataset.

          \return The description of this dataset.

          \note The caller of this method will NOT take the ownership of the returned information.
        */
        virtual const DataSetType* getType() const = 0;

        /*!
          \brief It returns a pointer to the collection where the dataset is contained.

          \return A pointer to the collection where the dataset is contained.

          \note The caller of this method will NOT take the ownership of the returned information.
        */
        virtual DataSet* getParent() const = 0;

        //@}

        /** @name DataSetItem Element Properties
         *  Methods for retrieving/setting the dataset item component values.
         */
        //@{

        /*!
          \brief Method for retrieving a signed character attribute value (1 byte long).

          \param i The attribute index.

          \return The signed character attribute value (1 byte long) in the given position.
        */
        virtual char getChar(int i) const = 0;

        /*!
          \brief Method for retrieving a signed character attribute value (1 byte long).

          \param name The attribute name.

          \return The signed character attribute value (1 byte long) with the given name.
        */
        virtual char getChar(const std::string& name) const;

        /*!
          \brief Method for setting a signed character attribute value (1 byte long).

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy.
        */
        virtual void setChar(int i, char value) = 0;

        /*!
          \brief Method for setting a signed character attribute value (1 byte long).

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setChar(const std::string& name, char value);

        /*!
          \brief Method for retrieving an unsigned character attribute value (1 byte long).

          \param i The attribute index.

          \return The unsigned character attribute value (1 byte long) in the given position.
        */
        virtual unsigned char getUChar(int i) const = 0;

        /*!
          \brief Method for retrieving an unsigned character attribute value (1 byte long).

          \param name The attribute name.

          \return The unsigned character attribute value (1 byte long) with the given name.
        */
        virtual unsigned char getUChar(const std::string& name) const;

        /*!
          \brief Method for setting an unsigned character attribute value (1 byte long).

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy.
        */
        virtual void setUChar(int i, unsigned char value) = 0;

        /*!
          \brief Method for setting an unsigned character attribute value (1 byte long).

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setUChar(const std::string& name, unsigned char value);
        
        /*!
          \brief Method for retrieving a 16-bit integer attribute value (2 bytes long).

          \param i The attribute index.

          \return The 16-bit integer attribute value (2 bytes long) in the given position.
        */
        virtual boost::int16_t getInt16(int i) const = 0;

        /*!
          \brief Method for retrieving a 16-bit integer attribute value (2 bytes long).

          \param name The attribute name.

          \return The 16-bit integer attribute value (2 bytes long) with the given name.
        */
        virtual boost::int16_t getInt16(const std::string& name) const;

        /*!
          \brief Method for setting a 16-bit integer attribute value (2 bytes long).

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy.
        */
        virtual void setInt16(int i, boost::int16_t value) = 0;

        /*!
          \brief Method for setting a 16-bit integer attribute value (2 bytes long).

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setInt16(const std::string& name, boost::int16_t value);

        /*!
          \brief Method for retrieving a 32-bit integer attribute value (4 bytes long).

          \param i The attribute index.

          \return The 32-bit integer attribute value in the given position.
        */
        virtual boost::int32_t getInt32(int i) const = 0;

        /*!
          \brief Method for retrieving a 32-bit integer attribute value (4 bytes long).

          \param name The attribute name.

          \return The 32-bit integer attribute value with the given name.
        */
        virtual boost::int32_t getInt32(const std::string& name) const;

        /*!
          \brief Method for setting a 32-bit integer attribute value (4 bytes long).

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy.
        */
        virtual void setInt32(int i, boost::int32_t value) = 0;

        /*!
          \brief Method for setting a 32-bit integer attribute value (4 bytes long).

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setInt32(const std::string& name, boost::int32_t value);

        /*!
          \brief Method for retrieving a 64-bit integer attribute value (8 bytes long).

          \param i The attribute index.

          \return The 64-bit integer attribute value in the given position.
        */
        virtual boost::int64_t getInt64(int i) const = 0;

        /*!
          \brief Method for retrieving a 64-bit integer attribute value (8 bytes long).

          \param name The attribute name.

          \return The 64-bit integer attribute value with the given name.
        */
        virtual boost::int64_t getInt64(const std::string& name) const;

        /*!
          \brief Method for setting a 64-bit integer attribute value (8 bytes long).

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy.
        */
        virtual void setInt64(int i, boost::int64_t value) = 0;

        /*!
          \brief Method for setting a 64-bit integer attribute value (8 bytes long).

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setInt64(const std::string& name, boost::int64_t value);

        /*!
          \brief Method for retrieving a boolean attribute value.

          \param i The attribute index.

          \return The boolean attribute value in the given position.
        */
        virtual bool getBool(int i) const = 0;

        /*!
          \brief Method for retrieving a boolean attribute value.

          \param name The attribute name.

          \return The boolean attribute value with the given name.
        */
        virtual bool getBool(const std::string& name) const;

       /*!
          \brief Method for setting a boolean attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setBool(int i, bool value) = 0;

        /*!
          \brief Method for setting a boolean attribute value.

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setBool(const std::string& name, bool value);

        /*!
          \brief Method for retrieving a float attribute value.

          \param i The attribute index.

          \return The float attribute value in the given position.
        */
        virtual float getFloat(int i) const = 0;

        /*!
          \brief Method for retrieving a float attribute value.

          \param name The attribute name.

          \return The float attribute value with the given name.
        */
        virtual float getFloat(const std::string& name) const;

        /*!
          \brief Method for setting a float attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setFloat(int i, float value) = 0;

        /*!
          \brief Method for setting a float attribute value.

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setFloat(const std::string& name, float value);

        /*!
          \brief Method for retrieving a double attribute value.

          \param i The attribute index.

          \return The double attribute value in the given position.
        */
        virtual double getDouble(int i) const = 0;

        /*!
          \brief Method for retrieving a double attribute value.

          \param name The attribute name.

          \return The double attribute value with the given name.
        */
        virtual double getDouble(const std::string& name) const;

        /*!
          \brief Method for setting a double attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setDouble(int i, double value) = 0;

        /*!
          \brief Method for setting a double attribute value.

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setDouble(const std::string& name, double value);

        /*!
          \brief Method for retrieving a numeric attribute value.

          \param i The attribute index.

          \return The numeric attribute value in the given position.
        */
        virtual std::string getNumeric(int i) const = 0;

        /*!
          \brief Method for retrieving a numeric attribute value.

          \param name The attribute name.

          \return The numeric attribute value with the given name.
        */
        virtual std::string getNumeric(const std::string& name) const;

        /*!
          \brief Method for setting a numeric attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setNumeric(int i, const std::string& value) = 0;

        /*!
          \brief Method for setting a numeric attribute value.

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setNumeric(const std::string& name, const std::string& value);

        /*!
          \brief Method for retrieving a string value attribute.

          \param i The attribute index.

          \return The string attribute value in the given position.
        */
        virtual std::string getString(int i) const = 0;

        /*!
          \brief Method for retrieving a string attribute value.

          \param name The attribute name.

          \return The string attribute value with the given name.
        */
        virtual std::string getString(const std::string& name) const;

        /*!
          \brief Method for setting a string attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setString(int i, const std::string& value) = 0;

        /*!
          \brief Method for setting a string attribute value.

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setString(const std::string& name, const std::string& value);

        /*!
          \brief Method for retrieving a byte array.

          You can use this method in order to retrieve a BLOB data.

          \param i The attribute index.
          
          \return The byte array attribute.

          \note The caller of this method will take the ownership of the returned byte array.
        */
        virtual te::dt::ByteArray* getByteArray(int i) const = 0;

        /*!
          \brief Method for retrieving a byte array.

          You can use this method in order to retrieve a BLOB data.

          \param name The attribute name.
          
          \return The byte array attribute.

          \note The caller of this method will take the ownership of the returned byte array.
        */
        virtual te::dt::ByteArray* getByteArray(const std::string& name) const;

        /*!
          \brief Method for setting a byte array attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setByteArray(int i, const te::dt::ByteArray& value) = 0;

        /*!
          \brief Method for setting a byte array attribute value.

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setByteArray(const std::string& name, const te::dt::ByteArray& value);

        /*!
          \brief Method for retrieving a geometric attribute value.

          \param i The attribute index.

          \return The geometric attribute value in the given position.

          \note The caller of this method will take the ownership of the returned Geometry.
        */
        virtual te::gm::Geometry* getGeometry(int i) const = 0;

        /*!
          \brief Method for retrieving a geometric attribute value.

          \param name The attribute name.

          \return The geometric attribute value with the given name.

          \note The caller of this method will take the ownership of the returned Geometry.
        */
        virtual te::gm::Geometry* getGeometry(const std::string& name) const;

        /*!
          \brief Method for retrieving the default geometric attribute value.

          \return The default geometric attribute value.

          \note The caller of this method will take the ownership of the returned Geometry.
        */
        virtual te::gm::Geometry* getGeometry() const;

        /*!
          \brief Method for setting a geometric attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setGeometry(int i, const te::gm::Geometry& value) = 0;

        /*!
          \brief Method for setting a geometric attribute value.

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setGeometry(const std::string& name, const te::gm::Geometry& value);

        /*!
          \brief Method for retrieving a raster attribute value.

          \param i The attribute index.

          \return The raster attribute value in the given position.

          \note The caller of this method will take the ownership of the returned raster.
        */
        virtual te::rst::Raster* getRaster(int i) const = 0;

        /*!
          \brief Method for retrieving a raster attribute value.

          \param name The attribute name.

          \return The raster attribute value with the given name.

          \note The caller of this method will take the ownership of the returned raster.
        */
        virtual te::rst::Raster* getRaster(const std::string& name) const;

        /*!
          \brief Method for retrieving the default raster attribute value.

          \return The default raster attribute value.

          \note The caller of this method will take the ownership of the returned raster.

          \note For now, the first raster column will be retrieved.
        */
        virtual te::rst::Raster* getRaster() const;

        /*!
          \brief Method for setting a raster attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setRaster(int i, const te::rst::Raster& value) = 0;

        /*!
          \brief Method for setting a raster attribute value.

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setRaster(const std::string& name, const te::rst::Raster& value);

        /*!
          \brief Method for retrieving a date and time attribute value.

          \param i The attribute index.

          \return The date and time attribute value in the given position.

          \note The caller of this method will take the ownership of the returned datetime.
        */
        virtual te::dt::DateTime* getDateTime(int i) const = 0; 

        /*!
          \brief Method for retrieving a date and time attribute value.

          \param name The attribute name.

          \return The date and time attribute value with the given name.

          \note The caller of this method will take the ownership of the returned datetime.
        */
        virtual te::dt::DateTime* getDateTime(const std::string& name) const;

        /*!
          \brief Method for setting a date and time attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setDateTime(int i, const te::dt::DateTime& value) = 0; 

        /*!
          \brief Method for setting a date and time attribute value.

          \param name     The attribute name.
          \param value    The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setDateTime(const std::string& name, const te::dt::DateTime& value); 

        /*!
          \brief Method for retrieving an unidimensional array of integers (2 bytes long).

          \param i The attribute index.
          \param a The list of integers (2 bytes long).

          \todo We need a multi-dimensional array class
        */
        virtual void getArray(int i, std::vector<boost::int16_t>& values) const = 0;

        /*!
          \brief Method for retrieving an unidimensional array of integers (2 bytes long).

          \param name The attribute name.
          \param a    The list of integers (2 bytes long).

          \todo We need a multi-dimensional array class
        */
        virtual void getArray(const std::string& name, std::vector<boost::int16_t>& values) const;

        /*!
          \brief Method for retrieving an array.

          \param i The attribute index.

          \return An array. The caller will take its ownership.
        */
        virtual te::dt::Array* getArray(int /*i*/) const { return 0; }

        /*!
          \brief Method for retrieving an array.

          \param name The attribute name.

          \return An array. The caller will take its ownership.

          \return An array. The caller will take its ownership.
        */
        virtual te::dt::Array* getArray(const std::string& name) const;

        /*!
          \brief It returns the geometric attribute value in a serialized form.

          This method can avoid type conversion and memory allocation,
          if the driver is able to provide the geometry
          in a serialized way. So, it can be the faster
          method for retrieving a geometry.

          \param i The geometry index.

          \return The WKB representing the geometric attribute value.

          \note The caller of this method will not take the ownership of the returned pointer.
        */
        virtual const unsigned char* getWKB(int i) const = 0;

        /*!
          \brief It returns the geometric attribute value in a serialized form.

          This method can avoid type conversion and memory allocation,
          if the driver is able to provide the geometry
          in a serialized way. So, it can be the faster
          method for retrieving a geometry.

          \param name The geometric attribute name.

          \return The WKB representing the geometric attribute value.

          \note The caller of this method will not take the ownership of the returned pointer.
        */
        virtual const unsigned char* getWKB(const std::string& name) const;

        /*!
          \brief It returns the geometric attribute value in a serialized form.

          This method can avoid type conversion and memory allocation,
          if the driver is able to provide the geometry
          in a serialized way. So, it can be the faster
          method for retrieving a geometry.

          \return The WKB representing the geometric attribute value.

          \note The caller of this method will not take the ownership of the returned pointer.
        */
        virtual const unsigned char* getWKB() const;

        /*!
          \brief Method for retrieving an attribute value that is represented by a dataset.

          \param i The dataset index.

          \return A pointer to a dataset, if it exists, or NULL otherwise.

          \note Clients of this method will take the ownership of the returned dataset.
        */
        virtual DataSet* getDataSet(int i) = 0;

        /*!
          brief Method for retrieving an attribute value that is represented by a dataset.

          \param name The attribute name.

          \return A pointer to a dataset, if it exists, or NULL otherwise.

          \note Clients of this method will take the ownership of the returned dataset.
        */
        virtual DataSet* getDataSet(const std::string& name);

        /*!
          \brief Method for setting a dataset attribute value.

          \param i     The attribute index.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setDataSet(int i, const DataSet& value) = 0;

        /*!
          \brief Method for setting a dataset attribute value.

          \param name  The attribute name.
          \param value The attribute value.

          \note This method has effect just for datasets opened with read-write policy. 
        */
        virtual void setDataSet(const std::string& name, const DataSet& value);

        /*!
          \brief Method for retrieving any other type of data value stored in the data source.

          This method can be use for extensible datatypes.

          \param i The attribute index.

          \return A pointer to the data value.

          \note The caller of this method will take the ownership of the returned pointer.
        */
        virtual te::dt::AbstractData* getValue(int i) const;

        /*!
          \brief Method for retrieving any other type of data value stored in the data source.

          This method can be use for extensible datatypes.

          \param name The attribute name.

          \return A pointer to the data value.

          \note The caller of this method will take the ownership of the returned pointer.
         */
        virtual te::dt::AbstractData* getValue(const std::string& name) const;
        
        /*!
          \brief Sets an abstract data to the i-th data set attribute.

          This method can be used for extensible datatypes.

          \param i  The attribute index.
          \param ad The abstract data to be set in the data set.

          \note The data set will take the ownership of the given abstract data.
        */
        virtual void setValue(int i, te::dt::AbstractData* value) = 0;
        
        /*!
          \brief Sets an abstract data to a data set attribute.

          This method can be used for extensible datatypes.

          \param name The attribute name.
          \param ad   The abstract data to be set in the data set.

          \note The data set will take the ownership of the given abstract data.
        */
        virtual void setValue(const std::string& name, te::dt::AbstractData* ad);                

        /*!
          \brief Method for retrieving a data value as a string plain representation.

          \param i The attribute index.
          
          \return The attribute value in a string format.

          \note It is safe to call this method for any data type, the data source implementation will
                take care of how to convert the internal representation to a string.
        */
        virtual std::string getAsString(int i) const;

        /*!
          \brief Method for retrieving a data value as a string plain representation.

          \param name The attribute name.

          \return The attribute value in a string format.

          \note It is safe to call this method for any data type, the data source implementation will
                take care of how to convert the internal representation to a string.
        */
        virtual std::string getAsString(const std::string& name) const;

        /*!
          \brief It checks if the attribute value is NULL.

          \param i The attribute index.

          \return True if the attribute value is NULL.
        */
        virtual bool isNull(int i) const = 0;

        /*!
          \brief It checks if the attribute value is NULL.

          \param name The attribute name.

          \return True if the attribute value is NULL.
        */
        virtual bool isNull(const std::string& name) const;

        //@}
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETITEM_H


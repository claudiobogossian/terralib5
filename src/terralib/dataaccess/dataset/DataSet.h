/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/dataset/DataSet.h

  \brief A dataset is the unit of information manipulated by the data access module of TerraLib.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASET_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASET_H

// TerraLib
#include "../../common/CharEncodingConv.h"
#include "../../common/Enums.h"
#include "../../datatype/Array.h"
#include "../../datatype/ByteArray.h"
#include "../../datatype/DateTime.h"
#include "../../datatype/Enums.h"
#include "../../geometry/Enums.h"
#include "../../raster/Raster.h"
#include "../Config.h"

// STL
#include <string>

// Boost
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
// Forward declarations
  namespace dt
  {
    class AbstractData;
  }

// Forward declarations
  namespace gm
  {
    class Envelope;
    class Geometry;
  }

  namespace da
  {
// Forward declarations
    class DataSourceTransactor;

    /*!
      \class DataSet

      \brief A dataset is the unit of information manipulated by the data access module of TerraLib.

      A dataset is a container for a data collection with an internal pointer
      pointing to a specific item in the collection. There are move methods
      that can be used to set the position of this internal pointer.

      When the dataset is created, its internal pointer
      points to a sentinel location before the first item
      of the collection. So, in order to retrieve data
      from this collection one of the move methods must be called before.

      Each item of the dataset is composed by any
      number of properties. You have special methods to
      query the property information (name, data type, constraints).

      The individual value of a property of an item in the collection,
      can be retrieved by an appropriated get method. These methods
      retrieve the value by an integer index identifying the property position or
      by a string with the property name.

      When the dataset is the result of a query involving several
      datasets, you can find out the dataset that gave the original
      dataset name of a specific property.

      A dataset can be connected or disconnected. A connected dataset, after its creation through
      the data source transactor, continues to depend on the connection given by its associated
      data source. Differently, a disconnected dataset, after its creation, no more depends of the
      connection given by the data source, and it continues to live after the connection has been
      released to the data source.

      \ingroup dataaccess

      \sa DataSource, DataSourceTransactor, DataSetType

      \todo We can generalize the dataset API so that a dataset may contain other datasets;
            in this case, it will be a collection of datasets.

      \note A geometric or raster property is represented just like any other data type.

      \note Our design also allows a dataset to have multiple geometric or raster properties.
    */
    class TEDATAACCESSEXPORT DataSet : public boost::noncopyable
    {
      public:

        /*! \brief Default constructor. */
        DataSet() {}

        /*! \brief Virtual destructor. */
        virtual ~DataSet() {}

        /** @name DataSet Methods
         *  Methods for obtaining information about a given dataset.
         */
        //@{

        /*!
          \brief It returns the traverse type associated to the dataset.

          \return The traverse type associated to the dataset.
        */
        virtual te::common::TraverseType getTraverseType() const = 0;

        /*!
          \brief It returns the read and write permission associated to the dataset.

          \return The read and write permission associated to the dataset.
        */
        virtual te::common::AccessPolicy getAccessPolicy() const = 0;

        /*!
          \brief It returns the number of properties that composes an item of the dataset.

          \return The number of properties that composes an item of the dataset.
        */
        virtual std::size_t getNumProperties() const = 0;

        /*!
          \brief It returns the underlying data type of the property at position pos.

          \param i The property position of interest.

          \return The underlying data type of the property at position pos.
        */
        virtual int getPropertyDataType(std::size_t i) const = 0;

        /*!
          \brief It returns the property name at position pos.

          \param i The property name at the position of interest.

          \return The property name at position pos.
        */
        virtual std::string getPropertyName(std::size_t i) const = 0;

        /*!
          \brief It returns the property character encoding at position pos.

          \param i The property the position of interest.

          \return The property character encoding.

          \note The property at position pos must be a textual property.

          \exception Exception It throws an exception if the property at position pos is not a textual property.
        */
        virtual te::common::CharEncoding getPropertyCharEncoding(std::size_t i) const = 0;

        /*!
          \brief It returns the underlying dataset name of the property at position pos.

          \param i The property position of interest.

          \return The underlying dataset name of the property at position pos.
        */
        virtual std::string getDatasetNameOfProperty(std::size_t i) const = 0;
        //@}

        /** @name Collection Methods
         *  Methods for getting/setting atomic datasets.
         */
        //@{

        /*!
          \brief It returns true if the collection is empty.

          \return True, if the collection is empty.
        */
        virtual bool isEmpty() const = 0;

        /*!
          \brief It returns true if the dataset is connected and false if it is disconnected.
                 A dataset can be connected or disconnected. A connected dataset, after its creation through
                 the data source transactor, continues to depend on the connection given by its associated
                 data source. Differently, a disconnected dataset, after its creation, no more depends of the
                 connection given by the data source, and it continues to live after the connection has been
                 released to the data source.

          \return True, if the dataset is connected, or false if it is disconnected.
        */
        virtual bool isConnected() const = 0;

        /*!
          \brief It returns the collection size, if it is known.

          It may return std::string::npos if the size is not known,
          or it would be too costly to compute it.

          \return The size of the collection, if it is known.
        */
        virtual std::size_t size() const = 0;

        /*!
          \brief It computes the bounding rectangle for a spatial property of the dataset.

          \param i The position of a spatial property to get its bounding box.

          \pre The position i must be associated to a spatial property of the dataset.

          \exception Exception It throws an exception if something goes wrong during MBR search.

          \return The spatial property bounding rectangle, or an invalid box, if none is found.
        */
        virtual std::auto_ptr<te::gm::Envelope> getExtent(std::size_t i) = 0;

        /*!
          \brief It moves the internal pointer to the next item of the collection.

          You always has to call this method in order to move the internal pointer to the first
          item in the collection. This method can be used to traverse a dataset.

          \return True if the internal pointer is on a valid item, or false otherwise.

          \note All dataset types support this method: FORWARDONLY, BIDIRECTIONAL and RANDOM.
        */
        virtual bool moveNext() = 0;

        /*!
          \brief It moves the internal pointer to the previous item of the collection.

          \return True, if the internal pointer (cursor position) is on a valid item, or false otherwise.

          \note This method is not supported by FORWARDONLY datasets.
        */
        virtual bool movePrevious() = 0;

        /*!
          \brief It moves the internal pointer to a position before the first item in the collection.

          \return True, if it was possible to move to a sentinel position before the first item in the collection.

          \note This method is not supported by FORWARDONLY datasets.
        */
        virtual bool moveBeforeFirst() = 0;

        /*!
          \brief It moves the internal pointer to the first item in the collection.

          \return True, if it was possible to move to the first item in the collection.

          \note This method is not supported by FORWARDONLY datasets.
        */
        virtual bool moveFirst() = 0;

        /*!
          \brief It sets the dataset internal pointer to the last item in the collection.

          \return True, if it was possible to move to the last item in the collection.
        */
        virtual bool moveLast() = 0;

        /*!
          \brief It moves the dataset internal pointer to a given position.

          \param i The position the dataset internal pointer must be set up.

          \return True, if it was possible to move the dataset reading to the given position.

          \note The first item in the collection starts at address 0.
        */
        virtual bool move(std::size_t i) = 0;

        /*!
          \brief It tells if the dataset internal pointer is on the first element of the collection or not.

          \return True if the dataset internal pointer is on the first element otherwise it returns false.
        */
        virtual bool isAtBegin() const = 0;

        /*!
          \brief It tells if the dataset internal pointer is in a position before the first element of the collection or not.

          \return True, if the dataset internal pointer is in a position before the first element otherwise it returns false.
        */
        virtual bool isBeforeBegin() const = 0;

        /*!
          \brief It tells if the dataset internal pointer is on the last element of the collection.

          \return True, if the dataset internal pointer is on the last element otherwise it returns false.
        */
        virtual bool isAtEnd() const = 0;

        /*!
          \brief It tells if the dataset internal pointer is on the sentinel position after the last element of the collection or not.

          \return True, if the dataset internal pointer is on the sentinel position after the last element otherwise it returns false.
        */
        virtual bool isAfterEnd() const = 0;

        //@}

        /** @name DataSet Element Properties
         *  Methods for retrieving/setting the dataset item component values without the need to explicitly instantiate an item via getItem method.
         */
        //@{

        /*!
          \brief Method for retrieving a signed character attribute value (1 byte long).

          \param i The attribute index.

          \return The signed character attribute value (1 byte long) in the given position.
        */
        virtual char getChar(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a signed character attribute value (1 byte long).

          \param name The attribute name.

          \return The signed character attribute value (1 byte long) with the given name.
        */
        virtual char getChar(const std::string& name) const;

        /*!
          \brief Method for retrieving an unsigned character attribute value (1 byte long).

          \param i The attribute index.

          \return The unsigned character attribute value (1 byte long) in the given position.
        */
        virtual unsigned char getUChar(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving an unsigned character attribute value (1 byte long).

          \param name The attribute name.

          \return The unsigned character attribute value (1 byte long) with the given name.
        */
        virtual unsigned char getUChar(const std::string& name) const;

        /*!
          \brief Method for retrieving a 16-bit integer attribute value (2 bytes long).

          \param i The attribute index.

          \return The 16-bit integer attribute value (2 bytes long) in the given position.
        */
        virtual boost::int16_t getInt16(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a 16-bit integer attribute value (2 bytes long).

          \param name The attribute name.

          \return The 16-bit integer attribute value (2 bytes long) with the given name.
        */
        virtual boost::int16_t getInt16(const std::string& name) const;

        /*!
          \brief Method for retrieving a 32-bit integer attribute value (4 bytes long).

          \param i The attribute index.

          \return The 32-bit integer attribute value in the given position.
        */
        virtual boost::int32_t getInt32(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a 32-bit integer attribute value (4 bytes long).

          \param name The attribute name.

          \return The 32-bit integer attribute value with the given name.
        */
        virtual boost::int32_t getInt32(const std::string& name) const;

        /*!
          \brief Method for retrieving a 64-bit integer attribute value (8 bytes long).

          \param i The attribute index.

          \return The 64-bit integer attribute value in the given position.
        */
        virtual boost::int64_t getInt64(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a 64-bit integer attribute value (8 bytes long).

          \param name The attribute name.

          \return The 64-bit integer attribute value with the given name.
        */
        virtual boost::int64_t getInt64(const std::string& name) const;

        /*!
          \brief Method for retrieving a boolean attribute value.

          \param i The attribute index.

          \return The boolean attribute value in the given position.
        */
        virtual bool getBool(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a boolean attribute value.

          \param name The attribute name.

          \return The boolean attribute value with the given name.
        */
        virtual bool getBool(const std::string& name) const;

        /*!
          \brief Method for retrieving a float attribute value.

          \param i The attribute index.

          \return The float attribute value in the given position.
        */
        virtual float getFloat(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a float attribute value.

          \param name The attribute name.

          \return The float attribute value with the given name.
        */
        virtual float getFloat(const std::string& name) const;

        /*!
          \brief Method for retrieving a double attribute value.

          \param i The attribute index.

          \return The double attribute value in the given position.
        */
        virtual double getDouble(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a double attribute value.

          \param name The attribute name.

          \return The double attribute value with the given name.
        */
        virtual double getDouble(const std::string& name) const;

        /*!
          \brief Method for retrieving a numeric attribute value.

          \param i The attribute index.

          \return The numeric attribute value in the given position.
        */
        virtual std::string getNumeric(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a numeric attribute value.

          \param name The attribute name.

          \return The numeric attribute value with the given name.
        */
        virtual std::string getNumeric(const std::string& name) const;

        /*!
          \brief Method for retrieving a string value attribute.

          \param i The attribute index.

          \return The string attribute value in the given position.
        */
        virtual std::string getString(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a string attribute value.

          \param name The attribute name.

          \return The string attribute value with the given name.
        */
        virtual std::string getString(const std::string& name) const;

        /*!
          \brief Method for retrieving a byte array.

          You can use this method in order to retrieve a BLOB data.

          \param i The attribute index.
          
          \return The byte array attribute.
        */
        virtual std::auto_ptr<te::dt::ByteArray> getByteArray(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a byte array.

          You can use this method in order to retrieve a BLOB data.

          \param name The attribute name.
          
          \return The byte array attribute.
        */
        virtual std::auto_ptr<te::dt::ByteArray> getByteArray(const std::string& name) const;

        /*!
          \brief Method for retrieving a geometric attribute value.

          \param i The attribute index.

          \return The geometric attribute value in the given position.
        */
        virtual std::auto_ptr<te::gm::Geometry> getGeometry(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a geometric attribute value.

          \param name The attribute name.

          \return The geometric attribute value with the given name.
        */
        virtual std::auto_ptr<te::gm::Geometry> getGeometry(const std::string& name) const;

        /*!
          \brief Method for retrieving a raster attribute value.

          \param i The attribute index.

          \return The raster attribute value in the given position.
          

          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>the maximum number of simultaneous accesses has been reached</li>
                     </ul>          
        */
        virtual std::auto_ptr<te::rst::Raster> getRaster(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a raster attribute value.

          \param name The attribute name.

          \return The raster attribute value with the given name.
          
          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>the maximum number of simultaneous accesses has been reached</li>
                     </ul>            
        */
        virtual std::auto_ptr<te::rst::Raster> getRaster(const std::string& name) const;

        /*!
          \brief Method for retrieving a date and time attribute value.

          \param i The attribute index.

          \return The date and time attribute value in the given position.
        */
        virtual std::auto_ptr<te::dt::DateTime> getDateTime(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a date and time attribute value.

          \param name The attribute name.

          \return The date and time attribute value with the given name.
        */
        virtual std::auto_ptr<te::dt::DateTime> getDateTime(const std::string& name) const;

        /*!
          \brief Method for retrieving an array.

          \param i The attribute index.
        */
        virtual std::auto_ptr<te::dt::Array> getArray(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving an array.

          \param name The attribute name.

          \return An array. The caller will take its ownership.
        */
          virtual std::auto_ptr<te::dt::Array> getArray(const std::string& name) const;

        /*!
          \brief Method for retrieving any other type of data value stored in the data source.

          This method can be use for extensible datatypes.

          \param i The attribute index.

          \return A pointer to the data value.
        */
        virtual std::auto_ptr<te::dt::AbstractData> getValue(std::size_t i) const;

        /*!
          \brief Method for retrieving any other type of data value stored in the data source.

          This method can be use for extensible datatypes.

          \param name The attribute name.

          \return A pointer to the data value.
         */
        virtual std::auto_ptr<te::dt::AbstractData> getValue(const std::string& name) const;

        /*!
          \brief Method for retrieving a data value as a string plain representation.

          \param i         The attribute index.
          \param precision The precision in the conversion.
          
          \return The attribute value in a string format.

          \note It is safe to call this method for any data type, the data source implementation will
                take care of how to convert the internal representation to a string.
        */
        virtual std::string getAsString(std::size_t i, int precision = 0) const;

        /*!
          \brief Method for retrieving a data value as a string plain representation.

          \param name The attribute name.
          \param precision The precision in the conversion.

          \return The attribute value in a string format.

          \note It is safe to call this method for any data type, the data source implementation will
                take care of how to convert the internal representation to a string.
        */
        virtual std::string getAsString(const std::string& name, int precision = 0) const;

        /*!
          \brief It checks if the attribute value is NULL.

          \param i The attribute index.

          \return True if the attribute value is NULL.
        */
        virtual bool isNull(std::size_t i) const = 0;

        /*!
          \brief It checks if the attribute value is NULL.

          \param name The attribute name.

          \return True if the attribute value is NULL.
        */
        virtual bool isNull(const std::string& name) const;
        //@}
    };

    typedef boost::shared_ptr<DataSet> DataSetPtr;

  } // end namespace da
}   // end namespace te


#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASET_H

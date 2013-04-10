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
namespace te { namespace dt { class AbstractData; class Array; class ByteArray; class DateTime; } }
namespace te { namespace gm { class Geometry; } }
namespace te { namespace rst { class Raster; } }

namespace te
{
  namespace da
  {
// Forward declarations
    class DataSet;

    /*!
      \class DataSetItem

      \brief A single element in a data collection represented by a dataset is called DataSetItem.

      A dataset is a collection of dataset items.
      Each dataset item can be a set of primitive values or more complex values (objects and structs),
      which can be used to represent a single row of a database table,
      a single raster data, a collection of raster data or even other datasets.

      In order to know what are the components of a dataset item you can query
      its type definition.

      \sa DataSet
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
          \brief It returns a pointer to the collection where the dataset is contained.

          \return A pointer to the collection where the dataset is contained or NULL if it isn't associated to one.

          \note The caller of this method will NOT take the ownership of the returned information.
        */
        virtual DataSet* getParent() const = 0;

        /*!
          \brief It returns the number of properties that composes an item of the dataset.

          \return The number of properties that composes an item of the dataset.
        */
        virtual std::size_t getNumProperties() const = 0;

        /*!
          \brief It returns the underlying data type of the property at position pos.

          \param pos The property position of interest.

          \return The underlying data type of the property at position pos.
        */
        virtual int getPropertyDataType(std::size_t pos) const = 0;

        /*!
          \brief It returns the property name at position pos.

          \param pos The property name at the position of interest.

          \return The property name at position pos.
        */
        virtual std::string getPropertyName(std::size_t pos) const = 0;

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

          \note The caller of this method will take the ownership of the returned byte array.
        */
        virtual te::dt::ByteArray* getByteArray(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a byte array.

          You can use this method in order to retrieve a BLOB data.

          \param name The attribute name.
          
          \return The byte array attribute.

          \note The caller of this method will take the ownership of the returned byte array.
        */
        virtual te::dt::ByteArray* getByteArray(const std::string& name) const;

        /*!
          \brief Method for retrieving a geometric attribute value.

          \param i The attribute index.

          \return The geometric attribute value in the given position.

          \note The caller of this method will take the ownership of the returned Geometry.
        */
        virtual te::gm::Geometry* getGeometry(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a geometric attribute value.

          \param name The attribute name.

          \return The geometric attribute value with the given name.

          \note The caller of this method will take the ownership of the returned Geometry.
        */
        virtual te::gm::Geometry* getGeometry(const std::string& name) const;

        /*!
          \brief Method for retrieving a raster attribute value.

          \param i The attribute index.

          \return The raster attribute value in the given position.

          \note The caller of this method will take the ownership of the returned raster.
        */
        virtual te::rst::Raster* getRaster(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving a raster attribute value.

          \param name The attribute name.

          \return The raster attribute value with the given name.

          \note The caller of this method will take the ownership of the returned raster.
        */
        virtual te::rst::Raster* getRaster(const std::string& name) const;

        /*!
          \brief Method for retrieving a date and time attribute value.

          \param i The attribute index.

          \return The date and time attribute value in the given position.

          \note The caller of this method will take the ownership of the returned datetime.
        */
        virtual te::dt::DateTime* getDateTime(std::size_t i) const = 0; 

        /*!
          \brief Method for retrieving a date and time attribute value.

          \param name The attribute name.

          \return The date and time attribute value with the given name.

          \note The caller of this method will take the ownership of the returned datetime.
        */
        virtual te::dt::DateTime* getDateTime(const std::string& name) const;

        /*!
          \brief Method for retrieving an array.

          \param i The attribute index.

          \return An array. The caller will take its ownership.
        */
        virtual te::dt::Array* getArray(std::size_t i) const = 0;

        /*!
          \brief Method for retrieving an array.

          \param name The attribute name.

          \return An array. The caller will take its ownership.

          \return An array. The caller will take its ownership.
        */
        virtual te::dt::Array* getArray(const std::string& name) const;

        /*!
          \brief Method for retrieving any other type of data value stored in the data source.

          This method can be use for extensible datatypes.

          \param i The attribute index.

          \return A pointer to the data value.

          \note The caller of this method will take the ownership of the returned pointer.
        */
        virtual te::dt::AbstractData* getValue(std::size_t i) const;

        /*!
          \brief Method for retrieving any other type of data value stored in the data source.

          This method can be use for extensible datatypes.

          \param name The attribute name.

          \return A pointer to the data value.

          \note The caller of this method will take the ownership of the returned pointer.
         */
        virtual te::dt::AbstractData* getValue(const std::string& name) const;

        /*!
          \brief Method for retrieving a data value as a string plain representation.

          \param i The attribute index.
          
          \return The attribute value in a string format.

          \note It is safe to call this method for any data type, the data source implementation will
                take care of how to convert the internal representation to a string.
        */
        virtual std::string getAsString(std::size_t i) const;

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
        virtual bool isNull(std::size_t i) const = 0;

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


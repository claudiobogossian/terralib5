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
  \file terralib/dataaccess/dataset/DataSet.h

  \brief DataSet is the fundamental unit of information transferred between a data source and the data access module.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASET_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASET_H

// TerraLib
#include "../../common/Enums.h"
#include "../../datatype/AbstractData.h"
#include "../../datatype/Enums.h"
#include "../../geometry/Enums.h"
#include "../Config.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

// Forward declarations
namespace te { namespace dt { class Array; class ByteArray; class DateTime; class Property; } }
namespace te { namespace gm { class Envelope;  class Geometry; } }
namespace te { namespace rst { class Raster; } }

namespace te
{
  namespace da
  {
// Forward declarations
    class DataSetItem;
    class DataSetType;
    class DataSourceTransactor;    

    /*!
      \class DataSet

      \brief DataSet is the fundamental unit of information transferred between a data source and the data access module.

      In the data access module the fundamental unit of geospatial information
      is called dataset. It may be defined recursively, so that
      a dataset may contains other datasets. In other words,
      a dataset can be a collection of other datasets.

      As a dataset is a data collection, it has an internal pointer
      that marks the current item of the collection. The move methods (moveNext,
      moveLast, moveFirst, movePrevious, and so on) set this internal
      pointer. When the dataset is first created, its internal pointer points
      to a sentinel location before the first item of the collection. The first
      moveNext method called sets the cursor position to the first item.
      Using the move methods you can retrieve the attributes values of the elements
      in a dataset.

      A dataset may have any number of attributes whose values
      can be retrieved by invoking the appropriated methods (getInt, getDouble, ...).
      The attributes can be retrieved by integer index or by the name of the attribute.
      You can query the associated DataSetType to know the right type of each attribute,
      and then know what is the method suitable for retrieving data.

      If you use a method for retrieving the wrong type
      of the attribute in a given position the result will
      be unpredictable. For example, if the attribute type is a
      integer and you retrieve it using getString, the system
      can crash.

      Each dataset has an associated DataSetType that
      has methods for retrieving their attribute types.

      A geometric or raster property is treated just like any other attribute.
      It has a name and an index and it is retrieved using the getGeometry or getRaster method of the dataset.
      We provide another method called getWKB for retrieving the geometry in its
      raw form. Sometimes, this can be useful for speed up some operations.
      
      Our design also allows a dataset to have multiple geometric or raster attributes.

      \sa DataSetType, DataSetItem
    */
    class TEDATAACCESSEXPORT DataSet : public te::dt::AbstractData
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
          \brief It returns the transactor associated to this dataset.

          \return A transactor associated to this dataset.

          \note The caller of this method will NOT take the ownership of the returned information.
        */
        virtual DataSourceTransactor* getTransactor() const = 0;

        /*!
          \brief It loads the full description of the dataset (keys, indexes and others).

          \note Calling this method can invalidate any previous loaded DataSetType information.
        */
        virtual void loadTypeInfo() = 0;

        /*!
          \brief It returns a pointer to the collection where the dataset is contained.

          \return A pointer to the collection where the dataset is contained.

          \note The caller of this method will NOT take the ownership of the returned information.
         */
        virtual DataSet* getParent() const = 0;

        /*!
          \brief It computes the bounding rectangle for the default geometric property of the given dataset.

          \return The default geometric property bounding rectangle or NULL if none is found.

          \post The caller of this method will take the ownership of the returned box.

          \exception Exception It throws an exception if something goes wrong during MBR search.
        */
        virtual te::gm::Envelope* getExtent();

        /*!
          \brief It computes the bounding rectangle for a geometric property the given dataset.

          \param p The geometric property to get its bounding box.

          \return The geometric property bounding rectangle or NULL if none is found.

          \pre The geometric property must belong to the DataSetType of this DataSet.

          \post The caller of this method will take the ownership of the returned box.

          \exception Exception It throws an exception if something goes wrong during MBR search.
        */
        virtual te::gm::Envelope* getExtent(const te::dt::Property* p) = 0;

        //@}

        /** @name Spatial Filter Methods
         *  Methods that can be used to refine the dataset data.
         */
        //@{

        /*!
          \brief It sets a new spatial filter when fetching datasets via the moveNext() method using the dafault geometric property.

          Only datasets that geometrically satisfy the spatial relation with the geometry will be returned.

          \param g The geometry to be used as spatial filter.
          \param r The filter spatial relation.

          \note The geometry coordinates should be in the same coordinate system as the dataset.
          \note The filter will be applied to default geometry column.
          \note Any geometry or rectangular filter previously set will be released.
          \note DataSet will not take the ownership of the given geometry.
        */   
        virtual void setFilter(const te::gm::Geometry* g,
                               te::gm::SpatialRelation r = te::gm::INTERSECTS);

        /*!
          \brief It sets a new spatial filter when fetching datasets via the moveNext() method.

          Only datasets that geometrically satisfy the spatial relation with geometry will be returned.

          \param p The geometric property to be used during the filtering.
          \param g The geometry to be used as the spatial filter.
          \param r The filter spatial relation.

          \note The geometry coordinates should be in the same coordinate system as the dataset.
          \note The filter will be applied to the default geometry column.
          \note Any geometry or rectangular filter previously set will be released.
          \note DataSet will not take the ownership of the given geometry.
        */   
        virtual void setFilter(te::dt::Property* p,
                               const te::gm::Geometry* g,
                               te::gm::SpatialRelation r = te::gm::INTERSECTS) = 0;
 
        /*!
          \brief It sets a new rectangular spatial filter using the default geometric property.

          Only datasets that geometrically satisfy the spatial relation with the envelope will be returned.

          \param e The geometry to be used as the spatial filter.
          \param r The filter spatial relation.

          \note The envelope coordinates should be in the same coordinate system as the dataset.
          \note The filter will be applied to the default geometry column.
          \note Any geometry or rectangular filter previously set will be released.
          \note DataSet will not take the ownership of the given envelope.
        */
        virtual void setFilter(const te::gm::Envelope* e,
                               te::gm::SpatialRelation r = te::gm::INTERSECTS); 

        /*!
          \brief It sets a new rectangular spatial filter.

          Only datasets that geometrically satisfy the spatial relation with envelope will be returned.

          \param p The geometric property to be used during the filtering.
          \param e The geometry to be used as the spatial filter.
          \param r The filter spatial relation.

          \note The envelope coordinates should be in the same coordinate system as the dataset.
          \note The filter will be applied to the default geometry column.
          \note Any geometry or rectangular filter previously set will be released.
          \note DataSet will not take the ownership of the given envelope.
        */
        virtual void setFilter(te::dt::Property* p,
                               const te::gm::Envelope* e,
                               te::gm::SpatialRelation r = te::gm::INTERSECTS) = 0; 

        //@}

        /** @name Collection Methods
         *  Methods for getting/setting atomic datasets.
         */
        //@{

        /*!
          \brief This method will retrieve a copy of the collection element pointed by the internal pointer.

          This method can be used to get all attributes of a given element of the
          collection represented by the dataset.

          \return A dataset item with all properties of the collection element.

          \note The caller of this method will take the ownership of the returned item.
        */
        virtual DataSetItem* getItem() const = 0;

        /*!
          \brief It adds a new item to the dataset.

          \param item The item to be added to the dataset. The dataset will take the ownership of the given item.
        */
        virtual void add(DataSetItem* item) = 0;

        /*!
          \brief It returns true, if it contains no DataSets.

          \return True, if the collection is empty.
        */
        virtual bool isEmpty() const = 0;

        /*!
          \brief It returns the collection size, if it is known.

          It may return -1, if the size is not known, or it would be
          too costly to compute (such as when the datasets are streaming
          from a remote source).

          \return The size of the collection, if it is known.
        */
        virtual std::size_t size() const = 0;

        /*!
          \brief It moves the internal pointer (cursor position) to the next item of the collection.

          You always has to call this method in order to move the internal pointer to the first
          item in the collection. This method can be used to traverse a dataset.
          If you need to re-start reading from the first dataset, call reset() method.

          \return True if the internal pointer (cursor position) is on a valid item, or false otherwise.

          \note All type of datasets support this method (FORWARDONLY, BIDIRECTIONAL, RANDOM, INDEXED).
        */
        virtual bool moveNext() = 0;

        /*!
          \brief It moves the internal pointer (cursor position) to the previous dataset of the collection.

          \return True, if the internal pointer (cursor position) is on a valid item, or false otherwise.

          \note This method is not supported by FORWARDONLY datasets.
        */
        virtual bool movePrevious() = 0;
        
        /*!
          \brief It moves the dataset reading to the first item in the collection.

          \return True, if it was possible to move to the first item in the collection.

          \note This method is not supported by FORWARDONLY datasets.
        */
        virtual bool moveFirst() = 0;

        /*!
          \brief It sets the dataset reading to a sentinel position before the first item in the collection.

          \return True, if it was possible to move to a sentinel position before the first item in the collection.

          \note This method is not supported by FORWARDONLY datasets.
        */
        virtual bool moveBeforeFirst() = 0;

        /*!
          \brief It sets the dataset reading to the last item in the collection.

          \return True, if it was possible to move to the last item in the collection.
        */
        virtual bool moveLast() = 0;

        /*!
          \brief It sets the dataset reading to a sentinel position after the last item in the collection.

          \return True, if it was possible to move to a sentinel position after the last item in the collection.
        */
        virtual bool moveAfterLast() = 0;

        /*!
          \brief It moves the dataset reading to a given position.

          \param i The position the dataset reading must be set up (the first position starts at 0).

          \return True, if it was possible to move the dataset reading to the given position.
        */
        virtual bool move(std::size_t i) = 0;

        /*!
          \brief It tells if the dataset internal pointer (cursor position) is on the first element of the collection or not.

          \return True if the dataset internal pointer (cursor position) is on the first element otherwise it returns false.
        */
        virtual bool isAtBegin() const = 0;

        /*!
          \brief It tells if the dataset internal pointer (cursor position) is on the sentinel position before the first element of the collection or not.

          \return True, if the dataset internal pointer (cursor position) is on the sentinel position before the first element otherwise it returns false.
        */
        virtual bool isBeforeBegin() const = 0;

        /*!
          \brief It tells if the dataset internal pointer (cursor position) is on the last element of the collection.

          \return True, if the dataset internal pointer is on the last element otherwise it returns false.
        */
        virtual bool isAtEnd() const = 0;

        /*!
          \brief It tells if the dataset internal pointer (cursor position) is on the sentinel position after the last element of the collection or not.

          \return True, if the dataset internal pointer (cursor position) is on the sentinel position after the last element otherwise it returns false.
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
        virtual void getArray(int i, std::vector<boost::int16_t>& a) const = 0;

        /*!
          \brief Method for retrieving an unidimensional array of integers (2 bytes long).

          \param name The attribute name.
          \param a    The list of integers (2 bytes long).

          \todo We need a multi-dimensional array class
        */
        virtual void getArray(const std::string& name, std::vector<boost::int16_t>& a) const;

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
        virtual void setValue(int i, te::dt::AbstractData* ad) = 0;
        
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

          \param i         The attribute index.
          \param precision The precision in the conversion.
          
          \return The attribute value in a string format.

          \note It is safe to call this method for any data type, the data source implementation will
                take care of how to convert the internal representation to a string.
        */
        virtual std::string getAsString(int i, int precision = 0) const;

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
        virtual bool isNull(int i) const = 0;

        /*!
          \brief It checks if the attribute value is NULL.

          \param name The attribute name.

          \return True if the attribute value is NULL.
        */
        virtual bool isNull(const std::string& name) const;

        //@}

        /** @name AbstractData Re-implementation
         *  Methods re-implemneted from AbstractData.
         */
        //@{

        te::dt::AbstractData* clone() const { throw; }

        int getTypeCode() const { return te::dt::DATASET_TYPE; }

        std::string toString() const { return std::string("DataSet"); }

        //@}
    };

    typedef boost::shared_ptr<DataSet> DataSetPtr;

  } // end namespace da
}   // end namespace te


#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASET_H



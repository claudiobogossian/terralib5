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
  \file terralib/stmemory/DataSet.h

  \brief  Implementation of an in-memory data set that contains spatiotemporal observations indexed by time and space.
*/

#ifndef __TERRALIB_STMEMORY_INTERNAL_DATASET_H
#define __TERRALIB_STMEMORY_INTERNAL_DATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "../sam/rtree/Index.h"
#include "Config.h"

// STL
#include <map>

// Boost
#include <boost/shared_ptr.hpp>

namespace te { namespace da { class DataSetType; } }
namespace te { namespace dt { class DateTimePeriod; } }
namespace te { namespace mem { class DataSetItem; } }

namespace te
{
  namespace stmem
  {
    // Typedefs
    typedef boost::shared_ptr<te::mem::DataSetItem>             DateSetItemShrPtr;
    typedef std::pair<te::dt::DateTime*, DateSetItemShrPtr>     TimeAndDateSetItemPair;
    typedef std::multimap<te::dt::DateTime*, DateSetItemShrPtr, te::dt::CompareDateTime>  TimeToDataSetItemMap;
        
    /*!
      \class DataSet

      \brief  Implementation of a in-memory data set that contains spatiotemporal 
              observations indexed by time and space.

      \sa te::da::DataSet
    */
    class TESTMEMORYEXPORT DataSet : public te::da::DataSet
    {
      public:

        /*!
          \brief It constructs an empty in-memory dataset indexed by time and space.

          \param dt         the schema of the in-memory observation DataSet that will be created. 
          \param tpPropIdx  the property index of the DataSetType "dt" that contains the observation phenomenon time. 
          \param gmPropIdx  the property index of the DataSetType "dt" that contains geometries. 
       
          \note DataSet will NOT take the ownership of the given pointer. 
        */
        DataSet(const te::da::DataSetType* type, int tpPropIdx);
        
        /*!
          \brief It constructs an empty in-memory dataset indexed by time and space.

          \param dt         the schema of the in-memory observation DataSet that will be created. 
          \param tpPropIdx  the property index of the DataSetType "dt" that contains the observation phenomenon time. 
          \param gmPropIdx  the property index of the DataSetType "dt" that contains geometries. 
       
          \note DataSet will NOT take the ownership of the given pointer. 
        */
        DataSet(const te::da::DataSetType* type, int tpPropIdx, int gmPropIdx);

        /*!
          \brief It constructs an empty in-memory dataset indexed by time and space.

          \param dt              the schema of the in-memory observation DataSet that will be created. 
          \param begTimePropIdx  the property index of the DataSetType "dt" that contains the observation phenomenon beginning time.
          \param endTimePropIdx  the property index of the DataSetType "dt" that contains the observation phenomenon end time.  
          \param gmPropIdx       the property index of the DataSetType "dt" that contains geometries. 
       
          \note DataSet will NOT take the ownership of the given pointer. 
        */
        DataSet(const te::da::DataSetType* type, int begTimePropIdx, int endTimePropIdx, int gmPropIdx);

         /*!
          \brief It constructs an empty in-memory dataset indexed by time and space.

          \param pnames          The names of the properties. 
          \param ptypes          The types of the properties. 
          \param begTimePropIdx  The property index of the ptypes vector that contains the observation phenomenon beginning time.
          \param endTimePropIdx  The property index of the ptypes vector that contains the observation phenomenon end time.  
          \param gmPropIdx       The property index of the ptypes vector that contains geometries. 
        */
        DataSet(const std::vector<std::string>& pnames, const std::vector<int>& ptyes, const std::map<int, te::common::CharEncoding>& encodings, int begTimePropIdx, int endTimePropIdx, int gmPropIdx);

                
        /*!
          \brief It constructs an in-memory observation dataset from a given data set.

          It creates a new in-memory observation dataset with the items from the given dataset. 

          \param ds         the dataset which will provide the observations.
          \param tpPropIdx  the property index of the DataSet "ds" that contains the observation phenomenon time 
          \param gmPropIdx  the property index of the DataSet "ds" that contains geometries 
          \param limit  The number of items to be copied. Use 0 to copy all items.

          \pre You must assure that ds is a valid item to be read.
          \pre You must assure that ds has a valid type associated to it, that is, getType() is valid.

          \note This constructor will use the method "moveNext()" of the source dataset (ds)
                in order to read its items. It will start reading the given 
                dataset in the current position. So, the caller is responsible for
                informing the dataset in the right position to start processing it.
          
          \note DataSet will NOT take the ownership of the given ds pointer.

          \note The new dataset will not have a transactor associated to it.
        */
        DataSet(te::da::DataSet* ds, int tpPropIdx, int gmPropIdx = -1, unsigned int limit = 0);

        /*!
          \brief It constructs an in-memory observation dataset from a given data set.

          It creates a new in-memory observation dataset with the items from the given dataset. 

          \param ds         the dataset which will provide the observations.
          \param begTimePropIdx  the property index of the DataSetType "dt" that contains the observation phenomenon beginning time.
          \param endTimePropIdx  the property index of the DataSetType "dt" that contains the observation phenomenon end time.  
          \param gmPropIdx  the property index of the DataSet "ds" that contains geometries 
          \param limit  The number of items to be copied. Use 0 to copy all items.

          \pre You must assure that ds is in a valid item to be read.
          \pre You must assure that ds has a valid type associated to it, that is, getType() is valid.

          \note This constructor will use the method "moveNext()" of the source dataset (ds)
                in order to read its items. It will start reading the given 
                dataset in the current position. So, the caller is responsible for
                informing the dataset in the right position to start processing it.

          \note DataSet will NOT take the ownership of the given ds pointer.

          \note The new dataset will not have a transactor associated to it.
        */
        DataSet(te::da::DataSet* ds, int begTimePropIdx, int endTimePropIdx, int gmPropIdx, unsigned int limit);
        
        /*!
          \brief Regular copy constructor.

          It creates a new in-memory observation dataset with the items from the rhs dataset. 

          \param rhs   The dataset which will provide the items.
          \param deepCopy If true the new dataset instance will clone all dataset items from the source dataset (rhs), otherwise it will share all items.
         
          \pre You must assure that rhs is in a valid item to be read.
          \pre You must assure that rhs has a valid type associated to it, that is, getType() is valid.

          \note This constructor will copy all items from the given DataSet rhs,
                independently on its current position.                        
        */
        DataSet(const DataSet& rhs, const bool deepCopy = false);

        /*!
          \brief Assignment operator

          \note The left-side DataSet will share all observations or items of the right-side DataSet.
          \note If the caller does not want to share the items, it must use the method clone.
        */
        DataSet& operator= (const DataSet& rhs);
        
        /*! \brief Destructor. */
        ~DataSet();

        /*!
          \brief It copies up to limit items from the source dataset.

          \param src   The source dataset with the items that will be copied.
          \param limit The number of items to be copied. Use 0 to copy all items.

          \pre The internal DataSetType and ObservationSetType must be already created and valid. 
          \pre The internal DataSetType must be equal to the given DataSet src 

          \note This method will call moveNext() for the source dataset
                in order to read its items. It will start reading the given 
                dataset in the current position. So, the caller is responsible for
                informing the dataset in the right position to start processing it.
        */
        void copy(te::da::DataSet* src, unsigned int limit = 0);

        /*!
          \brief It copies up to limit items from the source dataset (src).

          \param src        The source dataset with the items that will be copied.
          \param properties The list of properties which will be considered in the copy.
          \param limit      The number of items to be copied. Use 0 to copy all items.

          \pre The properties in the property vector must come from the source dataset (src) schema. In other
               words, they must be valid pointers in the src dataset type.
          \pre The internal DataSetType must be already created, using the given properties.

          \note This method will call moveNext() for the source dataset
                in order to read its items. It will start reading the given 
                dataset in the current position. So, the caller is responsible for
                informing the dataset in the right position to start processing it.
        */
        void copy(te::da::DataSet* src, const std::vector<std::size_t>& properties, unsigned int limit = 0);
               
        /*!
          \brief It returns a reference to the internal observation set.

          \return A reference to the internal observation set.
        */
        const TimeToDataSetItemMap& getData() const;

        /*!
          \brief It returns the index of the property that contains the observed geometries. 

          \return An index of the property that conatins the observed geometries.  
        */
        int getGeomPropIdx() const;

        /*!
          \brief It returns the index of the property that contains the beginning phenomenon time. 

          \return An index of the property that conatins the beginning phenomenon time. 
        */
        int getBeginTimePropIdx() const;

        /*!
          \brief It returns the index of the property that contains the end phenomenon time. 

          \return An index of the property that conatins the end phenomenon time. 
        */
        int getEndTimePropIdx() const;
        
        /*!
          \brief It adds an existing item to the dataset.
          
          This method must be used when the user wants to share
          the same item among DataSets. After adding the item,
          it updates the internal RTree. 

          \param item An existing item coming from another DataSet.
           
          \pre The item schema must be compatible with the dataset items.
          
          \note The first element of the pair (DateTime*) will be cloned. 
          \note It will not take the ownership of the first element of the given pair.
        */
        void add(const std::pair<te::dt::DateTime*, DateSetItemShrPtr>& item);

        /*!
          \brief It adds a new item to the dataset and takes its ownership.

          \param item The item to be added to the dataset.

          \pre The item schema must be compatible with the dataset items.

          \note It will take the ownership of the item pointer.
        */
        void add(te::mem::DataSetItem* item);

        /*!
          \brief It returns the temporal extent of the observations.

          \return The temporal extent of the observations.

          \note The caller will take the ownership of the output pointer.
        */
        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent() const;

        /*!
          \brief It returns a clone of the DataSet

          \return a clone of the DataSet

          \note The caller will take the ownership of the returned pointer.
        */
        std::auto_ptr<DataSet> clone() const;

        /*!
          \brief It returns a new DataSet, based on a given spatial filter

          \param e    A given envelope
          \param r    A given spatial relation.
          
          \return     a new DataSet

          \note For while, it only works if the spatial relation is te::gm::INTERSECTS 
          \note The caller will take the ownership of the returned pointer.
          \note The returned DataSet will NOT share the intenal observations. It will clone the internal observations.
        */
        std::auto_ptr<DataSet> filter(const te::gm::Envelope* e, te::gm::SpatialRelation r) const;

        /*!
          \brief It returns a new DataSet, based on a given spatial filter

          \param g    A given geometry
          \param r    A given spatial relation.
          \return     a new DataSet

          \note For while, it only works if the spatial relation is te::gm::INTERSECTS 
          \note The caller will take the ownership of the returned pointer.
          \note The returned DataSet will NOT share the intenal observations. It will clone the internal observations.
        */
        std::auto_ptr<DataSet> filter(const te::gm::Geometry* g, te::gm::SpatialRelation r) const;

        /*!
          \brief It returns a new DataSet, based on a given temporal filter

          \param dt    A given date time
          \param tr    A given temporal relation.
          
          \return      a new DataSet

          \note The caller will take the ownership of the returned pointer.
          \note The new DataSet will share the intenal observations. 
        */
        std::auto_ptr<DataSet> filter(const te::dt::DateTime* dt, te::dt::TemporalRelation tr) const;

        /*!
          \brief It returns a new DataSet, based on a given spatial and temporal filter

          \param e     A given envelope
          \param r     A given spatial relation.
          \param dt    A given date time
          \param tr    A given temporal relation.
          
          \return     a new DataSet

          \note The caller will take the ownership of the returned pointer.
          \note The returned DataSet will NOT share the intenal observations. It will clone the internal observations.
        */
        std::auto_ptr<DataSet> filter(const te::gm::Envelope* e, te::gm::SpatialRelation r, 
                                      const te::dt::DateTime* dt, te::dt::TemporalRelation tr) const;

        /*!
          \brief It returns a new DataSet, based on a given spatial filter

          \param g     A given geometry
          \param r     A given spatial relation.
          \param dt    A given date time
          \param tr    A given temporal relation.

          \return     a new DataSet

          \note The caller will take the ownership of the returned pointer.
          \note The returned DataSet will NOT share the intenal observations. It will clone the internal observations. 
        */
        std::auto_ptr<DataSet> filter(const te::gm::Geometry* g, te::gm::SpatialRelation r, 
                                      const te::dt::DateTime* dt, te::dt::TemporalRelation tr) const;
        
         /*!
          \brief It returns the n nearest observations to a given date and time. 

          \param dt    A given date time
          \param n     The number of returned observations.

          \return     a new DataSet

          \note The caller will take the ownership of the returned pointer.
          \note The returned DataSet will share the intenal observations.  
        */
        std::auto_ptr<te::stmem::DataSet> nearestObservations(const te::dt::DateTime* time, int n) const;

                                             
        /*! \name DataSet inherited methods */
        //@{
        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t i) const;

        std::string getPropertyName(std::size_t i) const;

        te::common::CharEncoding getPropertyCharEncoding(std::size_t i) const;

        std::string getDatasetNameOfProperty(std::size_t i) const;

        bool isEmpty() const;

        bool isConnected() const;

        std::size_t size() const;

        std::auto_ptr<te::gm::Envelope> getExtent(std::size_t i);

        bool moveNext();

        bool movePrevious();

        bool moveBeforeFirst();

        bool moveFirst();

        bool moveLast();

        bool move(std::size_t i);

        bool isAtBegin() const;

        bool isBeforeBegin() const;

        bool isAtEnd() const;

        bool isAfterEnd() const;

        char getChar(std::size_t i) const;

        unsigned char getUChar(std::size_t i) const;

        boost::int16_t getInt16(std::size_t i) const;

        boost::int32_t getInt32(std::size_t i) const;

        boost::int64_t getInt64(std::size_t i) const;

        bool getBool(std::size_t i) const;

        float getFloat(std::size_t i) const;

        double getDouble(std::size_t i) const;

        std::string getNumeric(std::size_t i) const;

        std::string getString(std::size_t i) const;

        std::auto_ptr<te::dt::ByteArray> getByteArray(std::size_t i) const;

        std::auto_ptr<te::gm::Geometry> getGeometry(std::size_t i) const;

        std::auto_ptr<te::rst::Raster> getRaster(std::size_t i) const;

        std::auto_ptr<te::dt::DateTime> getDateTime(std::size_t i) const; 

        std::auto_ptr<te::dt::Array> getArray(std::size_t i) const;

        bool isNull(std::size_t i) const;

        std::auto_ptr<te::dt::AbstractData> getValue(std::size_t i) const;
        //@}

        /*! \name Methods to set values to the DataSet 

            \note These methods will take the ownership of the given pointers.
        */
        //@{
        void setChar(std::size_t i, char value);

        void setChar(const std::string& name, char value);

        void setUChar(std::size_t i, unsigned char value);

        void setUChar(const std::string& name, unsigned char value);

        void setInt16(std::size_t i, boost::int16_t value);

        void setInt16(const std::string& name, boost::int16_t value);

        void setInt32(std::size_t i, boost::int32_t value);

        void setInt32(const std::string& name, boost::int32_t value);

        void setInt64(std::size_t i, boost::int64_t value);

        void setInt64(const std::string& name, boost::int64_t value);

        void setBool(std::size_t i, bool value);

        void setBool(const std::string& name, bool value);

        void setFloat(std::size_t i, float value);

        void setFloat(const std::string& name, float value);

        void setDouble(std::size_t i, double value);

        void setDouble(const std::string& name, double value);

        void setNumeric(std::size_t i, const std::string& value);

        void setNumeric(const std::string& name, const std::string& value);

        void setString(std::size_t i, const std::string& value);

        void setString(const std::string& name, const std::string& value);

        void setByteArray(std::size_t i, te::dt::ByteArray* value);

        void setByteArray(const std::string& name, te::dt::ByteArray* value);

        void setGeometry(std::size_t i, te::gm::Geometry* value);

        void setGeometry(const std::string& name, te::gm::Geometry* value);

        void setRaster(std::size_t i, te::rst::Raster* value);

        void setRaster(const std::string& name, te::rst::Raster* value);

        void setDateTime(std::size_t i, te::dt::DateTime* value);

        void setDateTime(const std::string& name, te::dt::DateTime* value);

        void setValue(std::size_t i, te::dt::AbstractData* value);

        void setValue(const std::string& name, te::dt::AbstractData* ad);
        //@}
 
      protected:

        TimeToDataSetItemMap                                    m_items;          //!< The list of dataset items, ordered by time.
        std::auto_ptr<te::sam::rtree::Index<te::mem::DataSetItem*> >   m_RTree;   //!< A RTree index created over the default geometry property 
        TimeToDataSetItemMap::const_iterator                    m_iterator;       //!< The pointer to the current item.
        bool                                                    m_beforeFirst;    //! internal control  
        std::vector<std::string>                                m_pnames;         //!< The list of property names.
        std::vector<int>                                        m_ptypes;         //!< The list of property types.
        std::map<int, te::common::CharEncoding> m_encodings;                      //!< The list of string properties char-encoding.
        int                                                     m_begTimePropIdx; //!< The property index of the DataSetType that contains the phenomenon beginning time. 
        int                                                     m_endTimePropIdx; //!< The property index of the DataSetType that contains the phenomenon end time. 
        int                                                     m_geomPropIdx;    //!< The property index of the DataSetType that contains geometries. 
     };
  } // end namespace stmem
}   // end namespace te

#endif  // __TERRALIB_STMEMORY_INTERNAL_DATASET_H



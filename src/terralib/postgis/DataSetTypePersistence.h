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
  \file terralib/postgis/DataSetTypePersistence.h

  \brief DataSetTypePersistence implementation for PostGIS driver.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_DATASETTYPEPERSISTENCE_H
#define __TERRALIB_POSTGIS_INTERNAL_DATASETTYPEPERSISTENCE_H

// TerraLib
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "Config.h"

namespace te
{
  namespace pgis
  {
// Forward declarations
    class Transactor;

    /*!
      \class DataSetTypePersistence

      \brief DataSetTypePersistence implementation for PostGIS driver.

      \sa te::da::DataSetTypePersistence
    */
    class TEPGISEXPORT DataSetTypePersistence : public te::da::DataSetTypePersistence
    {
      public:

        /** @name Constructor/Destructor
         *  Initilizer methods.
         */
        //@{

        /*!
          \brief Constructor.

          \param t The PostGIS DataSourceTransactor from wich this persistence belongs.
        */
        DataSetTypePersistence(Transactor* t);

        /*! \brief Destructor. */
        ~DataSetTypePersistence();

        //@}

        /** @name te::da::DataSetTypePersistence Pure Virtual Methods Re-Implementation
         *  Methods reimplemented from base class.
         */
        //@{           
       
        void create(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

        void clone(const std::string& datasetName, const std::string& newDatasetName, const std::map<std::string, std::string>& options);

        void drop(te::da::DataSetType* dt);

        void drop(const std::string& datasetName);

        void rename(te::da::DataSetType* dt, const std::string& newName);

        void add(te::da::DataSetType* dt, te::dt::Property* p);

        void drop(te::dt::Property* p);

        void rename(te::dt::Property* p, const std::string& newName);

        void update(te::dt::Property* oldP, te::dt::Property* newP);

        void add(te::da::DataSetType* dt, te::da::PrimaryKey* pk);
   
        void drop(te::da::PrimaryKey* pk);

        void add(te::da::DataSetType* dt, te::da::UniqueKey* uk); 
   
        void drop(te::da::UniqueKey* uk); 

        void add(te::da::DataSetType* dt, te::da::Index* index, const std::map<std::string, std::string>& options);

        void drop(te::da::Index* index);

        void add(te::da::DataSetType* dt, te::da::ForeignKey* fk);
   
        void drop(te::da::ForeignKey* fk);
   
        void add(te::da::DataSetType* dt, te::da::CheckConstraint* cc);
   
        void drop(te::da::CheckConstraint* cc);

        void create(te::da::Sequence* sequence);

        void drop(te::da::Sequence* sequence);
        
        te::da::DataSourceTransactor* getTransactor() const;
       
        //@}
        
      protected:
        
        /** @name Specific methods for PostGIS DataSetTypePersistence 
         *  This specific methods can refresh the given DataSetType from data source updates. 
         */
        //@{           
        
        /*!
          \brief It creates a new property in PostGIS and adds it to the DataSetType.

          It will create the given property in PostGIS data source and associate
          it to the DataSetType.          

          If you want to add a property in a DataSetType with the same data
          of an existing one from another DataSetType, first of all,
          you must create a fresh copy of the property (see the method clone()
          in the PropertyType class). After that, the property will be added to the
          DataSetType in memory.

          In PostGIS, the process to create a new property can also create other objects,  
          such as constraints and indexes. So, the user can refresh the 
          given DataSetType by setting the refresh parameter as TRUE. In this case,
          all internal objects created in this process will be added to DataSetType.  

          \param dt The DataSetType where the PropertyType will be added.
          \param p  The new PropertyType to be added.
          \param refresh  Indicates if this method must refresh the DataSetType. If it is TRUE, this
                          method will refresh the DataSetType. Otherwise, it will not.  

          \pre The parameters must be valid pointers.

          \post This method can cause a cascade propagation in the data source catalog.
          
          \exception Exception It throws an exception if the PropertyType can not add to the DataSetType.
          
          \note PostGIS driver extended method.
        */ 
        void add(te::da::DataSetType* dt, te::dt::Property* p, const bool refresh);
           
        /*!
          \brief It removes a property from a PostGIS data source.

          The property must be associated to a DataSetType before calling this method.

          In order to refresh the DataSetType associated to the property, 
          the user must set the refresh parameter as TRUE. In this case, 
          every object associated to the removed property, such as, unique key, primary
          key and foreign key, will be also removed. 

          \param p The property to be removed from the DataSetType.
          \param refresh  Indicates if this method must refresh the DataSetType associated to
                          the property. If it is TRUE, this method will remove all objetcs 
                          associated to the removed property. 
                          Otherwise, it will not. 

          \pre The parameters must be valid pointers.

          \post After being removed, the property pointer will be invalidaded.
          \post This method can cause a cascade propagation in the data source catalog and in the DataSetType: changing in constraints and other stuffs.

          \exception Exception It throws an exception if the property can not be removed.
          \exception Exception It throws an exception if the property is not associated to a DataSetType.
          
          \note PostGIS driver extended method.
        */    
        void drop(te::dt::Property* p, const bool refresh);
        
        /*!
          \brief It renames the property in PostGIS.

          In order to rename a property, it must belong to a DataSetType.
          If the operation is successful, 
          the property in memory will have its name automatically changed in the dataset type.

          In order to refresh the DataSetType associated to the renamed property, 
          the user must set the refresh parameter as TRUE. In this case, 
          all objects associated to this renamed property, such as 
          check constraints, will be refreshed.
         
          \param p       The property to be renamed.
          \param newName The new property name.
          \param refresh  Indicates if this method must refresh the DataSetType associated to the property.
                          If it is TRUE, this method will refresh all objetcs
                          associated to the renamed property. Otherwise, it will not. 

          \pre The property must be a valid pointer. 

          \post This method can cause a cascade propagation in the DataSetType and associate objects: changing in constraints and other stuffs.

          \exception Exception It throws an exception if the property can not be renamed.
          \exception Exception It throws an exception if the property is not associated to a DataSetType.
          
          \note PostGIS driver extended method.
        */  
        void rename(te::dt::Property* p, const std::string& newName, const bool refresh);
        
        /*!
          \brief It updates a property in PostGIS.

          It will update an old property based on a new property information.
          Therefore, the old property must belong to a valid DataSetType and 
          its DataSetType must belong to a data source. The old property will become
          invalid and the new one will take its place in the associated DataSetType.

          In order to refresh the DataSetType associated to the updated property, 
          the user must set the refresh parameter as TRUE. In this case, 
          all objects associated to this updated property will be refreshed.
          
          \param oldP The property to be changed.
          \param newP The property with the new information.
          \param refresh  Indicates if this method must refresh the DataSetType associated to
                          the updated property. If it is TRUE, this
                          method will refresh all objetcs associated to the updated property. 
                          Otherwise, it will not.  

          \note The parameters must be valid pointers. 

          \note The newP must be not associated to another object before calling this method.

          \note If the operation was successful, the new property will be associated to the same DataSetType
                of the oldP, and the oldP pointer will be invalidated.
                
          \exception Exception It throws an exception if the DataSetType cannot be updated.
          \exception Exception It throws an exception if the old property is not associated to a DataSetType.
          \exception Exception It throws an exception if the old and new property is a simple property type.
                
          \note PostGIS driver extended method.

          \warning This method can cause a cascade propagation in the data source catalog: changing in constraints and other stuffs.
        */
        void update(te::dt::Property* oldP, te::dt::Property* newP, const bool refresh);
        
        /*!
          \brief It creates a new primary key constraint in PostGIS and adds it in the DataSetType.

          The DataSetType must be associated to a data source.
          After adding the given primary key in the data source, 
          it will be associated to the DataSetType in memory.

          In PostGIS, the process to create a primary key also creates other objects,  
          such as indexes. So, in order to refresh the given DataSetType, a user must
          set the refresh parameter as TRUE. In this case, 
          all internal objects created in this process will be added to DataSetType.  

          \param dt  The DataSetType where the primary key will be added.
          \param pk  The primary key constraint.
          \param refresh Indicates if this method must refresh the DataSetType. If it is TRUE, this
                   method will refresh the DataSetType, adding all internal objetcs
                   created as result of the new primary key creation. Otherwise, it will not.  

          \pre The parameters must be valid pointers.           
          \pre The property that will be updated to primary key must already exist in data source. 
          
          \post This method can cause a cascade propagation in the DataSetType indexes.
          
          \note PostGIS driver extended method.

          \exception Exception It throws an exception if the primary key cannot be added to the DataSetType.
        */
        void add(te::da::DataSetType* dt, te::da::PrimaryKey* pk, const bool refresh);
        
        /*!
          \brief It removes the primary key constraint from the PostGIS and the DataSetType which it is associated.

          The given primary key constraint must belong to a DataSetType and its DataSetType must be associated
          to a data souce. After removing the given primary key from the data source, 
          it will be removed from its DataSetType in memory.

          In order to refresh the DataSetType associated to the primary key, 
          the user must set the refresh parameter as TRUE. In this case, 
          every object associated to the removed primary key, such as indexes, 
          will be also removed.  

          \param pk The primary key constraint that will be removed.
          \param refresh  Indicates if this method must refresh the DataSetType associated to
                          the removed primary key. If it is TRUE, this method will remove all objetcs 
                          associated to the removed primary key, such as indexes. 
                          Otherwise, it will not. 

          \pre The primary key must be a valid pointer. 

          \post After being removed, the primary key pointer will be invalidaded.
          \post This method can cause a cascade propagation in the DataSetType indexes.
          
          \note PostGIS driver extended method.

          \exception Exception It throws an exception if the primary key cannot be removed from its DataSetType.
        */
        void drop(te::da::PrimaryKey* pk, const bool refresh);
        
        /*!
          \brief It adds a unique key constraint to the PostGIS and the DataSetType.

          The DataSetType must be associated to a data source.
          After adding the given unique key in the data source, 
          it will be associated to the DataSetType in memory.

          In PostGIS, the process to create a unique key also creates other objects,  
          such as indexes. So, in order to refresh the given DataSetType, a user must
          set the refresh parameter as TRUE. In this case, 
          all internal objects created in this process will be added to DataSetType.           

          \param dt The DataSetType where the unique key will be added.
          \param uk The unique key constraint.
          \param refresh Indicates if this method must refresh the DataSetType. If it is TRUE, this
                   method will refresh the DataSetType, adding all internal objetcs
                   created as result of the new unique key creation. Otherwise, it will not. 

          \pre The parameters must be valid pointers.

          \post This method can cause a cascade propagation in the DataSetType indexes.
          
          \note PostGIS driver extended method.

          \exception Exception It throws an exception if the unique key cannot be added to the DataSetType.
        */
        void add(te::da::DataSetType* dt, te::da::UniqueKey* uk, const bool refresh); 
   
        /*!
          \brief It removes the unique key constraint from the DataSetType which it is associated.

          The given unique key constraint must belong to a DataSetType and its DataSetType must be associated
          to a data souce. After removing the given unique key from the data source, 
          it will be removed from its DataSetType in memory.

          In order to refresh the DataSetType associated to the unique key, 
          the user must set the refresh parameter as TRUE. In this case, 
          every object associated to the removed unique key, such as indexes, 
          will be also removed.  

          \param uk The unique key constraint.
          \param refresh  Indicates if this method must refresh the DataSetType associated to
                          the removed unique key. If it is TRUE, this method will remove all objetcs 
                          associated to the removed unique key, such as indexes. 
                          Otherwise, it will not. 

          \post After being removed, the unique key pointer will be invalidaded.
          \post This method can cause a cascade propagation in the DataSetType indexes.          

          \pre The parameter must be a valid pointer. 
          
          \note PostGIS driver extended method.

          \exception Exception It throws an exception if the unique key cannot be removed from its DataSetType.
        */
        void drop(te::da::UniqueKey* uk, const bool refresh);    
        
        /*!
          \brief It returns a PostGIS transactor.
           
          \return It returns a PostGIStransactor
        */
        Transactor* getPGTransactor() const { return m_t; }
                
        //@}

      private:

        Transactor* m_t;  //!< The PostGIS DataSource transactor associated to the persistence object.
    };

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_DATASETTYPEPERSISTENCE_H



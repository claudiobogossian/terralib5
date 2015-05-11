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
 \file terralib/metadata/MetadataManager.h
 */


#ifndef __TERRALIB_METADATA_INTERNAL_METADATAMANAGER_H
#define __TERRALIB_METADATA_INTERNAL_METADATAMANAGER_H

// TerraLib
#include "../common/Singleton.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace md
  {
    class MetadataAnnotation;
    
    /*!
     \class MetadataManager
     
     \brief This singleton defines the TerraLib metadata record manager entry.
     
     \todo Persistence of the catalogue of annotations. Possibly in DBMS and/or JSON files.
     \todo Publish capabilility: the manager should be able to publish its contents to a metadata catalogue such as GeoNetwork.
     */
    class TEMDEXPORT MetadataManager: public te::common::Singleton<MetadataManager>
    {
      friend class te::common::Singleton<MetadataManager>;
      
    public:
      
      /** @name MetadataManager Accessor Method
       *  Method used to access the data stored on this manager.
       */
      //@{
      
      /*!
       \brief Inserts a new metadata annotation to the manager
       
       \param a Pointer to the new metadata annotation. Class takes pointer ownership. Do not pass null.
       
       \exception te::md::Exception If the annotation is already managed.
       */
      void insert(MetadataAnnotation* a);
      
      
      /*!
       \brief Removes the annotation from the manager and frees its resources.
       
       \param a Pointer to the annnotation to be removed. This pointer will be freed and invalidated.
       
       \exception te::md::Exception If the annotation doesn't exist.
       */
      void erase(MetadataAnnotation* a);
      
      /*!
       \brief Searches for an annotation, given its identification.
       
       \param aid The name annotation identification.
       
       \return A pointer to the annotation if found or NULL otherwise. Class maintains the ownership of returned pointer.
       */
      const MetadataAnnotation* find(const std::string& aid) const;
      
      /*!
       \brief Gives access to the set of metadata annotations.
       
       \return A const reference to the set of annotation indexed by it's id.
       */
      const std::map<std::string,MetadataAnnotation*>& getAnnotations() const;
      
      /*!
       \brief Check wheter the manager is empty.
       
       \return True if there is no annotation in the manager and false otherwise.
       */
      bool isEmpty() const;
      
      /*! 
       \brief Removes all the annotation from the manager.
       */
      void clear();
      
      //! Returns a begin iterator to the annotations managed.
      const std::map<std::string,MetadataAnnotation*>::const_iterator getBeginIterator() const;
      
      //! Returns an end iterator pointing to the annotations managed.
      const std::map<std::string,MetadataAnnotation*>::const_iterator getEndIterator() const;
      
      //@}
      
      /** @name Initializer Methods
       *  Methods related to instantiation and destruction.
       */
      //@{
      
      /*! \brief Destructor. */
      ~MetadataManager();
      
    protected:
    
      
      /*! \brief It initializes the Singleton. */
      MetadataManager();
      
      //@}
      
    private:
      
      std::map<std::string,MetadataAnnotation*> m_metadata;
      
    };
  }
}
#endif // __TERRALIB_METADATA_INTERNAL_METADATAMANAGER_H

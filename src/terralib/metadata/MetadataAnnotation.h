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
 \file terralib/metadata/MetadataAnnotation.h
 \brief 
 */

#ifndef __TERRALIB_METADATA_INTERNAL_METADATAANNOTATION_H
#define __TERRALIB_METADATA_INTERNAL_METADATAANNOTATION_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace md
  {
    /*!
     \class MetadataAnnotation
     
     \brief A class to relate a metadata annotation to a TerraLib element (or entity).
     */
    class TEMDEXPORT MetadataAnnotation 
    {
    public:
      /*!
       \brief Constructor.
       
       \param aid The annotation unique id. If empty, the constructor will provide one.
       */
      MetadataAnnotation(const std::string& aid = std::string(""));
      
      //! Destructor.
      ~MetadataAnnotation();
      
      //! Gets the annotation identifier.
      const std::string& getAnnotationtId() const;
      
      /*! 
       \brief Sets the link to the metadata.
       \param uri The link to the metadata. 
       */
      void setAnnotationURI(const std::string& uri);
      
      //! Gets the link to the metadata.
      const std::string& getAnnotationURI() const;
      
      /*!
       \brief Sets the TerrsLib element idenfification.
       \param edi The TerraLib element identification id. Do not pass empty.
       */
      void setElementId(const std::string& eid);      
      
      
      //! Gets the element being annotated identifier.
      const std::string& getElementId() const; 
      
      
      /*!
       \brief Set the element being annotated name.
       \param ename The element being annnotated name. Do not pass null.
       */
      void setElementName(const std::string& ename);
      
      /*! 
       \brief Gets the element being annotated name.
       */
      const std::string& getElementName() const;    
      
    private:
      
      std::string m_annotId;
      std::string m_annotURI;
      std::string m_elementId;
      std::string m_elementName;
    };
  }
}
#endif //__TERRALIB_METADATA_INTERNAL_METADATAANNOTATION_H

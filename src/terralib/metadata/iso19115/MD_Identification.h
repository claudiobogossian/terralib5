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
 \file terralib/metadata/iso19115/MD_Identification.h
 
 \brief Identification information contains information to uniquely identify the data.
 */

#ifndef __TERRALIB_METADATA_INTERNAL_ISO19115_MD_IDENTIFICATION_H
#define __TERRALIB_METADATA_INTERNAL_ISO19115_MD_IDENTIFICATION_H

// TerraLib
#include "../Config.h"
#include "CodeList.h"
#include "CI_ResponsibleParty.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
namespace te
{
  namespace md
  {    
    class CI_Citation;
    
    /*!
     \class MD_Identification
     
     \brief Identification information contains information to uniquely identify the data.
     
     \note It contains only the mandatory elements of core metadata.
     */
    class TEMDEXPORT MD_Identification : public boost::noncopyable
    {
    public:
      
      /*! 
       \brief Constructor.
       \param citation Pointer to the citation to the resource. Class takes ownership. Do not pass null.
       \param abstract Brief narrative summary of the resource.
       \param status Code that represents the development phase of the resource. 
      */
      MD_Identification(CI_Citation* citation, const std::string abstract, MD_ProgressCode status);
      
      //! Destructor.
      virtual ~MD_Identification();
      
      //! Returns the citation for the metadata.
      const CI_Citation* getCitation() const;
      
      //! Returns the resource abstract.
      const std::string& getAbstract() const;
      
      //! Returns the resource status code.
      te::md::MD_ProgressCode getStatus() const;
      
      /*! 
       \brief Adds a point of contact to the resource.
       \param poc Pointer to a responsible party. Class takes pointer ownership. Do not pass null.
       */
      void addPointOfContact(CI_ResponsibleParty* p);
      
      //! Returns the resource points of contact.
      const boost::ptr_vector<CI_ResponsibleParty>& getPointsOfContact() const;
      
      //! Gets a copy of this object.
      virtual MD_Identification* clone() const;
      
    protected:
      CI_Citation* m_citation;  
      std::string m_abstract; 
      te::md::MD_ProgressCode m_status;
      boost::ptr_vector<CI_ResponsibleParty> m_poc;
    };
    
  } // end namespace md
}   // end namespace te

#endif  // __TERRALIB_METADATA_INTERNAL_ISO19115_MD_IDENTIFICATION_H


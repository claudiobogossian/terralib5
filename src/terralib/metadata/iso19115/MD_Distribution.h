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
 \file terralib/metadata/iso19115/MD_Distribution.h
 
 \brief
 */

#ifndef __TERRALIB_METADATA_INTERNAL_ISO19115_MD_DISTRIBUTION_H
#define __TERRALIB_METADATA_INTERNAL_ISO19115_MD_DISTRIBUTION_H

// TerraLib
#include "../Config.h"
#include "MD_Format.h"

// Boost
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace md
  {
    //class MD_TransferOptions;
    //class MD_Distributor;
    
    /*!
     \class MD_Distribution
     
     \brief Information about the distributor of and the options for obtaining the resource.
     
     \todo include members TransferOptions and Distributor (NOT IMPLEMENTED YET)
     */
    class TEMDEXPORT MD_Distribution: public boost::noncopyable
    {
    public:
      
      MD_Distribution();
      
      ~MD_Distribution();
      
      /*!
       \brief Adds an distribution option.
       
       \param opt Pointer to a distribution option. Class takes pointer ownership. Do not pass null pointer.
       */
      void addDistrOption(MD_Format* opt);
      
      //! Gets the distribution formats
      const boost::ptr_vector<MD_Format>& getFormats() const;
      
      //! Sets an online resource
      void setURL(const std::string& url);
      
      //! Gets an online resource
      const std::string& getURL() const; 
      
    private:
      
      boost::ptr_vector<MD_Format> m_distrFormats;
      std::string m_url;
    };
    
  } // end namespace md
}   // end namespace te

#endif  // __TERRALIB_METADATA_INTERNAL_ISO19115_MD_DISTRIBUTION_H


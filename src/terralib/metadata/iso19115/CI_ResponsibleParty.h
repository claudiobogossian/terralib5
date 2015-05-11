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
 \file terralib/metadata/iso19115/CI_ResponsibleParty.h
 */

#ifndef __TERRALIB_METADATA_INTERNAL_ISO19115_CI_RESPONSIBLEPARTY_H
#define __TERRALIB_METADATA_INTERNAL_ISO19115_CI_RESPONSIBLEPARTY_H

// TerraLib
#include "../Config.h"
#include "CodeList.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace md
  {
    /*!
     \class CI_ResponsibleParty
     
     \brief Contains the identify of person(s), and/or position, and/or organization(s) associated with the resource.
     
     */
    class TEMDEXPORT CI_ResponsibleParty: public boost::noncopyable
    {
    public:
      
      /*! 
       \brief Constructor.
       \param indname The individual name. Default: empty string.
       \param orgname The organization name. Default: empty string.
       \param posname The position name. Default: empty string.
       \param code The responsible party role code. Default: provider.
       
       \note Only the role member is required, however the count of individualName + organizationName + positionName must 
       be greater than or equal to one.
       */
      CI_ResponsibleParty(const std::string& indname = std::string(""),
                          const std::string& orgname = std::string(""),
                          const std::string& posname = std::string(""),
                          CI_RoleCode code=CI_resourceProvider);  
      
      //! Destructor.
      ~CI_ResponsibleParty();
      
      /** @name Accessor methods
       *  Methods used to get or set properties.
       */
      //@{
      
      /*!
       \brief Set the individual name.
       \param name The individual name. Do not pass empty string.
       */
      void setIndividualName(const std::string& name);
      
      //! Returns the individual name.
      const std::string& getIndividualName() const;
      
      /*!
       \brief Sets the organization name.
       \param name The organization name. Do not pass empty string.
       */
      void setOrganizationName(const std::string& name);
      
      //! Returns the individual name.
      const std::string& getOrganizationName() const;
      
      /*!
       \brief Sets the position name.
       \param name The position name. Do not pass empty string.
       */
      void setPositionName(const std::string& name);
      
      //! Returns the position name.
      const std::string& getPositionName() const;
      
      //! Returns the responsible party role.
      CI_RoleCode getRoleCode() const;
      
      CI_ResponsibleParty* clone() const;
      
      //@}

    private:
      
      std::string m_individualName;    //!< Name of the responsible individual                         
      std::string m_organizationName;  //!< Name of the responsible organisation                      
      std::string m_positionName;      //!< Position of the responsible person
      CI_RoleCode m_roleCode;          //!< Function performed by the responsible party.
      
      //CI_Contact* m_contact; NOT IMPLEMENTED YET.
    };    
  } // end namespace md
}   // end namespace te

#endif  // __TERRALIB_METADATA_INTERNAL_ISO19115_CI_RESPONSIBLEPARTY_H


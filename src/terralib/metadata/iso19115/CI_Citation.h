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
 \file terralib/metadata/iso19115/CI_Citation.h
 
 \brief To support the bibliographic information to reference the resource.
 */

#ifndef __TERRALIB_METADATA_INTERNAL_ISO19115_CI_CITATION_H
#define __TERRALIB_METADATA_INTERNAL_ISO19115_CI_CITATION_H

// TerraLib
#include "../Config.h"
#include "CodeList.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp> 

namespace te
{
  namespace md
  {
    /*!
     \class CI_Citation
     
     \brief Provides a standardized method for citing a resource (dataset, feature, source, publication, etc.) 
     
     This class represents only the mandatory elements of core metadata for geographic datasets. 
     */
    class TEMDEXPORT CI_Citation : public boost::noncopyable
    {
    public: 
      
      /*! Constructor.
       \param title Name by which the cited resource is known.
       \param date Reference date for the cited resource.
       \param datetype Identification of the event used for the temporal aspects in the resource.
       */
      CI_Citation(const std::string& title, boost::gregorian::date date, CI_DateTypeCode dateype);
      
      /*! Destructor. */
      ~CI_Citation();
      
      /** @name Accessor methods
       *  Methods used to get or set properties.
       */
      //@{
    
      //! Returns the citation title.
      const std::string& getTitle() const;
      
      //! Returns the citation date.
      const boost::gregorian::date& getDate() const;
      
      //! Returns the citation date type.
      CI_DateTypeCode getDateType() const;
      
      //! Creates a new copy of this object.
      CI_Citation* clone() const;
      
      //@}
      
    private:
      
      // Mandatory elements from the core metadata for geographic objects
      
      std::string m_title;            //!< Name by which the cited resource is known.                       
      boost::gregorian::date m_date;  //!< Reference date for the cited resource.                
      CI_DateTypeCode m_dateType;     //!< Identification of the event used for the temporal aspects in the resource.
      
    };
    
  } // end namespace md
}   // end namespace te

#endif  // __TERRALIB_METADATA_INTERNAL_ISO19115_CI_CITATION_H


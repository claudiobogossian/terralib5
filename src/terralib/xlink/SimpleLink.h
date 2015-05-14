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
  \file SimpleLink.h
  
  \brief Simple link.
 */

#ifndef __TERRALIB_XLINK_INTERNAL_SIMPLELINK_H
#define __TERRALIB_XLINK_INTERNAL_SIMPLELINK_H

// TerraLib
#include "Config.h"
#include "Enumerators.h"

// STL
#include <string>

namespace te
{
  namespace xl
  { 
    /*!
      \class SimpleLink
        
      \brief Simple link.

      Attributes grouped by XLink type, as specified in the W3C 
      Proposed Recommendation (dated 2000-12-20).
     */
    class TEXLEXPORT SimpleLink
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new SimpleLink. */
        SimpleLink();

        /*! \brief Destructor. */
        ~SimpleLink();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        const std::string& getType();

        void setHref(const std::string& href);

        const std::string& getHref() const;

        void setRole(const std::string& role);

        const std::string& getRole() const;

        void setArcRole(const std::string& arcrole);

        const std::string& getArcRole() const;

        void setTitle(const std::string& title);

        const std::string& getTitle() const;

        void setShow(ShowType show);

        ShowType getShow() const;

        void setActuate(ActuateType actuate);

        ActuateType getActuate() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        SimpleLink* clone() const;

      private:

        std::string m_href;        //!< Locator attribute. (Optional)
        std::string m_role;        //!< Semantic attribute. (Optional)
        std::string m_arcrole;     //!< Semantic attribute. (Optional)
        std::string m_title;       //!< Semantic attribute. (Optional)
        ShowType m_show;           //!< Behavior attributes. Default: none. (Optional)
        ActuateType m_actuate;     //!< Behavior attributes. Default: none. (Optional)
    };

  } // end namespace xl
}   // end namespace te

#endif  // __TERRALIB_XLINK_INTERNAL_SIMPLELINK_H



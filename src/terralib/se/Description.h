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
  \file terralib/se/Description.h

  \brief A Description gives human-readable descriptive information for the object it is included within.
*/

#ifndef __TERRALIB_SE_INTERNAL_DESCRIPTION_H
#define __TERRALIB_SE_INTERNAL_DESCRIPTION_H

// TerraLib
#include "Config.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  {
    /*!
      \class Description
      
      \brief A Description gives human-readable descriptive information for the object it is included within.

      This information can be extracted and used
      for such purposes as creating informal searchable
      metadata in catalogue systems. More metadata
      fields may be added to this element in the future.

      \ingroup se

      \sa Symbolizer, CoverageStyle, Rule, FeatureTypeStyle, CoverageStyle
    */
    class TESEEXPORT Description : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Description. */
        Description();

        /*! \brief Destructor. */
        ~Description();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setTitle(const std::string& title);

        const std::string& getTitle() const;

        void setAbstract(const std::string& a);

        const std::string& getAbstract() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        Description* clone() const;

      private:

        std::string m_title;     //!< Title. (Optional)
        std::string m_abstract;  //!< Abstract. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_DESCRIPTION_H


/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/CoverageStyle.h

  \brief The CoverageStyle defines the styling that is to be applied to a subset of Coverage data.
*/

#ifndef __TERRALIB_SE_INTERNAL_COVERAGESTYLE_H
#define __TERRALIB_SE_INTERNAL_COVERAGESTYLE_H

// TerraLib
#include "Style.h"

namespace te
{
  namespace se
  {
    /*!
      \class CoverageStyle
      
      \brief The CoverageStyle defines the styling that is to be applied to a subset of Coverage data.

      \sa Style, FeatureTypeStyle
    */
    class TESEEXPORT CoverageStyle : public Style
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new CoverageStyle. */
        CoverageStyle();

        /*! \brief Destructor. */
        ~CoverageStyle();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setCoverageName(std::string* name);

        const std::string* getCoverageName() const;

        const std::string& getType() const;

        //@}

      private:

        std::string* m_coverageName;       //!< It identifies the specific coverage that the coverage style is for. (Optional)
        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_COVERAGESTYLE_H


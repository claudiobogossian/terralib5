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
  \file terralib/se/FeatureTypeStyle.h

  \brief The FeatureTypeStyle defines the styling that is to be applied to a dataset that can be viewed as a feature type.
*/

#ifndef __TERRALIB_SE_INTERNAL_FEATURETYPESTYLE_H
#define __TERRALIB_SE_INTERNAL_FEATURETYPESTYLE_H

// TerraLib
#include "Style.h"

namespace te
{
  namespace se
  {
    /*!
      \class FeatureTypeStyle
      
      \brief The FeatureTypeStyle defines the styling that is to be applied to a dataset that can be viewed as a feature type.

      \ingroup se

      \sa Style, CoverageStyle
    */
    class TESEEXPORT FeatureTypeStyle : public Style
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new FeatureTypeStyle. */
        FeatureTypeStyle();

        /*! \brief Destructor. */
        ~FeatureTypeStyle();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setFeatureTypeName(std::string* name);

        const std::string* getFeatureTypeName() const;

        const std::string& getType() const;

        /*! \brief It creates a new copy of this object. */
        Style* clone() const;

        //@}

      private:

        std::string* m_featureTypeName;    //!< It identifies the specific feature type that the style is for. (Optional)
        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_FEATURETYPESTYLE_H


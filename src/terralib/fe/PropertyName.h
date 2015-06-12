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
  \file PropertyName.h
  
  \brief This class can be used to is used to encode the name of any property of an object.  
 */

#ifndef __TERRALIB_FE_INTERNAL_PROPERTYNAME_H
#define __TERRALIB_FE_INTERNAL_PROPERTYNAME_H

// TerraLib
#include "Expression.h"

// STL
#include <string>

namespace te
{
  namespace fe
  {
    /*!
      \class PropertyName
      
      \brief This class is used to encode the name of any property of an object.

      The property name can be used in scalar or spatial
      expressions to represent the value of that property
      for a particular instance of an object.

      \ingroup fe

      \sa Expression, SortProperty, DistanceBuffer, BBOXOp, BinarySpatialOp, PropertyIsLike, PropertyIsNull

      \todo In the XML schema it is marked as mixed, so this put another requirement to this class!
     */
    class TEFEEXPORT PropertyName : public Expression
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new PropertyName.

          \name The property name.

          \note The name content can be given using XPath syntax.
         */
        PropertyName(const std::string& name);

        /*! \brief Destructor. */
        ~PropertyName();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the property name.

          \param name The property name.
         */
        void setName(const std::string& name);

        /*!
          \brief It returns the property name.

          \return The property name.
         */
        const std::string& getName() const;

        //@}

        /** @name Expression Re-implementation
         *  Methods re-implemented from Expression.
         */
        //@{

        Expression* clone() const;

        //@}

      private:

        std::string m_name;  //!< Property name. (Mandatory)
    };    

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_PROPERTYNAME_H


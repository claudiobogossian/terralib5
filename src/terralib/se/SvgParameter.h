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
  \file terralib/se/SvgParameter.h
  
  \brief A SvgParameter refers to an SVG/CSS graphical-formatting parameter.
*/

#ifndef __TERRALIB_SE_INTERNAL_SVGPARAMETER_H
#define __TERRALIB_SE_INTERNAL_SVGPARAMETER_H

// TerraLib
#include "ParameterValue.h"

namespace te
{
  namespace se
  {
    /*!
      \class SvgParameter

      \brief A SvgParameter refers to an SVG/CSS graphical-formatting parameter.

      The parameter is identified using the "name" attribute
      and the content of the element gives the SVG/CSS-coded value.

      \ingroup se

      \sa ParameterValue
    */
    class TESEEXPORT SvgParameter : public ParameterValue
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new SvgParameter.

          \param name SVG parameter name.
        */
        SvgParameter(const std::string& name);

        /*!
          \brief It initializes a new SvgParameter with the given value.

          \param name  SVG parameter name.
          \param value SVG parameter value.
        */
        SvgParameter(const std::string& name, const std::string& value);

        /*!
          \brief Copy constructor.

          \param rhs The other svg parameter.
        */
        SvgParameter(const SvgParameter& rhs);

        /*! \brief Destructor. */
        ~SvgParameter();

        //@}

        /** @name Accessors
         *  Methods for retrieving and setting properties.
         */
        //@{

        void setName(const std::string& name);

        std::string getName() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        SvgParameter* clone() const;

      private:

        std::string m_name; //!< The name attribute of the SvgParameter element. (Mandatory)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_SVGPARAMETER_H


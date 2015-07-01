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
  \file terralib/se/ParameterValue.h

  \brief The ParameterValueType uses WFS-Filter expressions to give values for SE graphic parameters.
*/

#ifndef __TERRALIB_SE_INTERNAL_PARAMETERVALUE_H
#define __TERRALIB_SE_INTERNAL_PARAMETERVALUE_H

// TerraLib
#include "Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace fe { class Expression; }

  namespace se
  {
    /*!
      \class ParameterValue
      
      \brief The "ParameterValueType" uses WFS-Filter expressions to give values for SE graphic parameters.

      A "mixed" element-content
      model is used with textual substitution for values.

      \ingroup se

      \sa SvgParameter, LineSymbolizer, GraphicStroke, Halo, MapItem, Interpolate, InterpolationPoint
    */
    class TESEEXPORT ParameterValue
    {
      public:

        /*!
          \struct Parameter

          \brief Data parameter.
        */
        struct TESEEXPORT Parameter
        {
          std::string* m_mixedData;          //!< Parameter from a mixed data content.
          te::fe::Expression* m_expression;  //!< Parameter from an expression.

          Parameter();
          Parameter(const Parameter& rhs);
          ~Parameter();
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes an empty ParameterValue. */
        ParameterValue();

        /*!
          \brief It initializes a new ParameterValue based on given literal value.
          
          \param value Literal value for the new ParamaterValue.

          \note A new Parameter with a Filter Literal Expression will be created and added to ParamaterValue.
        */
        ParameterValue(const std::string& value);

        /*!
          \brief Copy constructor.

          \param rhs The other parameter value.
        */
        ParameterValue(const ParameterValue& rhs);

        /*! \brief Virtual destructor. */
        virtual ~ParameterValue();

        //@}

        /** @name Accessors
         *  Methods for retrieving and setting properties.
         */
        //@{

        void add(Parameter* p);
        void add(const std::string& value);

        size_t getNParameters() const;
        const Parameter* getParameter(size_t i) const;

        //@}

        /*! \brief It creates a new copy of this object. */
        ParameterValue* clone() const;

      private:

        std::vector<Parameter*> m_parameters; //!< Regular text may be mixed and WFS-Filter expressions to give values for SE graphic parameters. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_PARAMETERVALUE_H

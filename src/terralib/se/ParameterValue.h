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
  \file ParameterValue.h
  
  \brief The ParameterValueType uses WFS-Filter expressions to give values for SE graphic parameters.
 */

#ifndef __TERRALIB_SE_INTERNAL_PARAMETERVALUE_H
#define __TERRALIB_SE_INTERNAL_PARAMETERVALUE_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "Config.h"
#include "Visitor.h"

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

      \sa SvgParameter, LineSymbolizer, GraphicStroke, Halo, MapItem, Interpolate, InterpolationPoint
     */
    class TESEEXPORT ParameterValue
    {
      public:

        /*!
          \struct Parameter

          \brief Data parameter.
         */
        struct Parameter
        {
          std::string* m_mixedData;          //!< Parameter from a mixed data content.
          te::fe::Expression* m_expression;  //!< Parameter from an expression.

          Parameter();
          ~Parameter();
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ParameterValue. */
        ParameterValue(); 

        /*! \brief Virtual destructor. */
        virtual ~ParameterValue();

        //@}

        /** @name Accessors
         *  Methods for retrieving and setting properties.
         */
        //@{

        void add(Parameter* p);        

        //@}

       private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other object.
         */
        ParameterValue(const ParameterValue& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other object.

          \return A reference for this.
         */
        ParameterValue& operator=(const ParameterValue& rhs);

        //@}

      private:          

        std::vector<Parameter*> m_parameters; //!< Regular text may be mixed and WFS-Filter expressions to give values for SE graphic parameters. (Optional)
    };    

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_PARAMETERVALUE_H


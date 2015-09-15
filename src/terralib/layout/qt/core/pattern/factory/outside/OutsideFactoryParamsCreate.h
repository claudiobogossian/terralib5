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
  \file OutsideFactoryParamsCreate.h
   
  \brief Parameters to create a new outside object (MVC widget Object). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_FACTORY_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_FACTORY_PARAMS_CREATE_H

// TerraLib
#include "terralib/common/AbstractParameters.h"
#include "../../../../../core/Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    /*!
    \brief Parameters to create a new outside object (MVC widget Object).  
    
    \ingroup layout

    \sa te::common::AbstractParameters
    */
    class TELAYOUTEXPORT OutsideFactoryParamsCreate : public te::common::AbstractParameters
    {
      public:
        

        /*! \brief Copy constructor. */
        OutsideFactoryParamsCreate(const OutsideFactoryParamsCreate& rhs);

        /*!
        \brief Constructor

        \param
        \param
        \param
        \param
        */
        OutsideFactoryParamsCreate();

        /*!
          \brief Destructor
        */ 
        virtual ~OutsideFactoryParamsCreate();

        te::common::AbstractParameters* clone() const;

        void reset() throw(te::common::Exception);
    };
  }
}
#endif


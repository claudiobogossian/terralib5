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
  \file ToolFactoryParamsCreate.h
   
  \brief Parameters to create a new tool. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TOOL_FACTORY_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_TOOL_FACTORY_PARAMS_CREATE_H

// TerraLib
#include "terralib/common/AbstractParameters.h"
#include "../../../../../core/Config.h"

namespace te
{
  namespace layout
  {
    class View;
    class EnumType;
    /*!
    \brief Parameters to create a new tool. 
    
    \ingroup layout

    \sa te::layout::ParamsCreate
    */
    class TELAYOUTEXPORT ToolFactoryParamsCreate : public te::common::AbstractParameters
    {
      public:
        

        /*! \brief Copy constructor. */
        ToolFactoryParamsCreate(const ToolFactoryParamsCreate& rhs);

        /*!
          \brief Constructor

          \param view
        */ 
        ToolFactoryParamsCreate(View* view, EnumType* itemType = 0);

        /*!
          \brief Destructor
        */ 
        virtual ~ToolFactoryParamsCreate();

        te::common::AbstractParameters* clone() const;

        void reset() throw(te::common::Exception);

        /*!
          \brief Returns the te::layout::View

          \return view
        */
        View*  getView();

        EnumType* getItemType();
        
      protected:

        View*       m_view; //!< View object
        EnumType*   m_itemType;
    };
  }
}
#endif


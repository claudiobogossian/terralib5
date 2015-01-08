/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file TemplateParamsCreate.h
   
  \brief Parameters to creation new template object. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEMPLATE_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_TEMPLATE_PARAMS_CREATE_H

// TerraLib
#include "../../ParamsCreate.h"
#include <string>

namespace te
{
  namespace layout
  {
    /*!
	   \brief Parameters to creation new template object. 
	  
	   \ingroup layout

	   \sa te::layout::ParamsCreate
	  */
    class TemplateParamsCreate : public ParamsCreate 
    {
      public:

        /*!
          \brief Constructor
        */ 
        TemplateParamsCreate();

        /*!
          \brief Constructor

          \param path full file path
        */
        TemplateParamsCreate(std::string path);

        /*!
          \brief Destructor
        */ 
        virtual ~TemplateParamsCreate();

        /*!
          \brief Returns full file path.

          \param full file path
        */
        virtual std::string getPath();

      protected:

        std::string m_path; //!< full file path
    };

    /*
    ** ---------------------------------------------------------------
    ** Inline methods:
    */
    inline TemplateParamsCreate::TemplateParamsCreate()
    {
    }

    inline TemplateParamsCreate::TemplateParamsCreate(std::string path) :
      m_path(path)
    {
    }

    inline TemplateParamsCreate::~TemplateParamsCreate() 
    {
    }

    inline std::string TemplateParamsCreate::getPath()
    {
      return m_path;
    }

  }
}

#endif

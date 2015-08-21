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
  \file terralib/layout/qt/core/pattern/factory/ZoomAreaToolFactory.h

  \brief This is the concrete factory for zoom area tools.
 */

#ifndef __TERRALIB_LAYOUT_INTERNAL_JSON_TEMPLATE_FACTORY_H
#define __TERRALIB_LAYOUT_INTERNAL_JSON_TEMPLATE_FACTORY_H

// TerraLib
#include "../../core/Config.h"
#include "TemplateFactory.h"
#include "TemplateFactoryParamsCreate.h"

namespace te
{
  namespace layout
  {
    class AbstractTemplate;
    /*!
      \class ZoomAreaToolFactory

      \brief This is the concrete factory for json template.

      \sa TemplateFactory, ParameterizedAbstractFactory, AbstractTemplate
    */
    class TELAYOUTEXPORT JSONTemplateFactory : public TemplateFactory
    {
      public:

        /*! It register the factory in the abstract dictionary. */
        JSONTemplateFactory();

        /*! \brief Destructor. */
        ~JSONTemplateFactory();

      protected:

        /*!
          \brief It creates a new json template.

          \return It returns json template.
        */
        AbstractTemplate* build(TemplateFactoryParamsCreate params);
    };

  } // end namespace layout
}   // end namespace te


#endif  // __TERRALIB_LAYOUT_INTERNAL_JSON_TEMPLATE_FACTORY_H


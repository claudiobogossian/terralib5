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
  \file terralib/layout/qt/core/pattern/factory/outside/ColorDialogOutsideFactory.h

  \brief This is the concrete factory for color dialog outside.
 */

#ifndef __TERRALIB_LAYOUT_INTERNAL_LEGEND_CHOICE_OUTSIDE_FACTORY_H
#define __TERRALIB_LAYOUT_INTERNAL_LEGEND_CHOICE_OUTSIDE_FACTORY_H

// TerraLib
#include "../../../../../core/Config.h"
#include "OutsideFactory.h"
#include "OutsideFactoryParamsCreate.h"

namespace te
{
  namespace layout
  {
    class AbstractOutsideView;
    /*!
      \class ColorDialogOutsideFactory

      \brief This is the concrete factory for color dialog outside.

      \sa NewOutsideFactory, ParameterizedAbstractFactory, AbstractOutsideView
    */
    class TELAYOUTEXPORT LegendChoiceOutsideFactory : public OutsideFactory
    {
      public:

        /*! It register the factory in the abstract dictionary. */
        LegendChoiceOutsideFactory();

        /*! \brief Destructor. */
        ~LegendChoiceOutsideFactory();

      protected:

        /*!
          \brief It creates a outside.

          \return It returns outside.
        */
        AbstractOutsideView* build(OutsideFactoryParamsCreate params);
    };

  } // end namespace layout
}   // end namespace te


#endif




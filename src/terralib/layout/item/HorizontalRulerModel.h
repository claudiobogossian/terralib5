/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file HorizontalRulerModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_MODEL_H

// TerraLib
#include "AbstractRulerModel.h"
#include "../core/enum/EnumMode.h"
#include "../../color/RGBAColor.h"

// STL
#include <vector>
#include <string>

namespace te
{
  namespace layout
  {
    class HorizontalRulerModel : public AbstractRulerModel
    {
      public:

        HorizontalRulerModel(PaperConfig* paperConfig);
        virtual ~HorizontalRulerModel();
        
        virtual void draw( ContextItem context );
        
        virtual void setBox(te::gm::Envelope box);
                
      protected:

        virtual void drawHorizontalRuler(te::map::Canvas* canvas, Utils* utils, double zoomFactor);

        virtual void drawRuler(te::map::Canvas* canvas, Utils* utils, double zoomFactor);
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H
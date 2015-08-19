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
  \file LegendChildModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LEGEND_MODEL_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_LEGEND_MODEL_MODEL_H

// TerraLib
#include "LegendModel.h"
#include "../../se/Rule.h"
#include "../core/Config.h"

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT LegendChildModel : public LegendModel
    {
      public:

        LegendChildModel();

        virtual ~LegendChildModel();

        virtual void draw( ContextItem context );
        
        virtual te::se::Rule* getRule();

        virtual void setRule(te::se::Rule* rule);
        
       protected:

        //virtual void visitDependent(ContextItem context);

        virtual void drawLegend(te::map::Canvas* canvas, Utils* utils);

        virtual void refreshRule();

        te::se::Rule* m_rule;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_MAPLAYOUT_MODEL_H
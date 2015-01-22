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
  \file ScaleModel.h
   
   \brief Class that represents a "Model" part of Scale MVC component.  
   Its coordinate system is the same of scene (millimeters). 
   This is also son of ItemModelObservable, so it can become observable, and son of AbstractVisitor, so it can become visitor.
   It is must visit the map, via te::layout::Visitable*, to get the scale value.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SCALE_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_SCALE_MODEL_H

// TerraLib
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/ContextItem.h"
#include "../core/pattern/derivativevisitor/AbstractVisitor.h"
#include "../../maptools/Canvas.h"
#include "../core/Utils.h"
#include "../core/Config.h"

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents a "Model" part of Scale MVC component.  
    Its coordinate system is the same of scene (millimeters). 
    This is also son of ItemModelObservable, so it can become observable, and son of AbstractVisitor, so it can become visitor.
    It is must visit the map, via te::layout::Visitable*, to get the scale value.
      	  
	    \ingroup layout

      \sa te::layout::ItemModelObservable , te::layout::AbstractVisitor
	  */
    class TELAYOUTEXPORT ScaleModel : public ItemModelObservable, public AbstractVisitor
    {
      public:

        /*!
          \brief Constructor
        */
        ScaleModel();

        /*!
          \brief Destructor
        */ 
        virtual ~ScaleModel();

        virtual void draw( ContextItem context );

        virtual Properties* getProperties() const;
        
        virtual void updateProperties(te::layout::Properties* properties);

        virtual void setScaleGapX(double x);

        virtual double getScaleGapX();

        virtual void setScaleGapY(double y);

        virtual double getScaleGapY();

       protected:

         virtual void visitDependent(ContextItem context);

         virtual void drawScale(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box);

        std::string m_mapName;
        double m_mapScale;
        double m_scaleGapX;
        double m_scaleGapY;
    };
  }
}

#endif 

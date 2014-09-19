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
  \file AbstractScene.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_SCENE_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_SCENE_H

// TerraLib
#include "../../geometry/Envelope.h"

namespace te
{
  namespace layout
  {
    class ItemObserver;
    class OutsideObserver;

    class AbstractScene
    {
      public:
        virtual ~AbstractScene(void) {};
        virtual void insertItem(ItemObserver* item) = 0;
        virtual void insertOutside(OutsideObserver* widget) = 0;
        virtual te::gm::Envelope getSceneBox() = 0;
        virtual te::gm::Envelope* getWorldBox() const = 0;
        /*
          params widthMM width of physical screen in millimeters
          params heightMM height of physical screen in millimeters
        */
        virtual void init(double screenWMM, double screenHMM, double zoomFactor = 1.0) = 0;

      protected:
        
        virtual te::gm::Envelope* calculateWindow(double wMM, double hMM) = 0;
        virtual void calculateMatrixViewScene(double zoomFactor = 1.) = 0;     
    };
  }
}

#endif
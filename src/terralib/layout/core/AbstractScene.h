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
#include "Config.h"

namespace te
{
  namespace layout
  {
    class ItemObserver;

    class TELAYOUTEXPORT AbstractScene
    {
      public:

        AbstractScene();

        virtual ~AbstractScene(void) {};

        virtual void insertItem(ItemObserver* item) = 0;
        
        virtual te::gm::Envelope getSceneBox();
        
        /*
          params widthMM width of physical screen in millimeters
          params heightMM height of physical screen in millimeters
        */
        virtual void init(double screenWMM, double screenHMM) = 0;
                
      protected:
        
        virtual void calculateMatrixViewScene() = 0;

        virtual void calculateWindow(double wMM, double hMM) = 0;
        
      protected:

        te::gm::Envelope  m_box;
    };
  }
}

#endif

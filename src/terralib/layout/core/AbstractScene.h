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
  \file AbstractScene.h
   
  \brief Abstract scene for Scene, a QGraphicsScene class, part of Graphics View Framework.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_SCENE_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_SCENE_H

// TerraLib
#include "../../geometry/Envelope.h"
#include "../qt/core/ContextObject.h"
#include "Config.h"

namespace te
{
  namespace layout
  {
		class AbstractItemView;
		class ItemObserver;

	/*!
      \class AbstractScene

      \brief Abstract scene for Scene, a QGraphicsScene class, part of Graphics View Framework.

      \ingroup layout
     */
    class TELAYOUTEXPORT AbstractScene
    {
      public:

		/*!
          \brief Constructor
        */ 
        AbstractScene();
		
		/*!
          \brief Destructor
        */ 
        virtual ~AbstractScene(void) {};

				/*!
				\brief Method that inserts a graphic object in the scene. Inverts the matrix of the object if necessary, ex.: TextItem.

				\param item graphic object
				*/
				virtual void insertItem(ItemObserver* item) = 0;
		
		/*!
          \brief Method that inserts a graphic object in the scene. Inverts the matrix of the object if necessary, ex.: TextItem.
		  
		  \param item graphic object		  
        */ 
				virtual void insertItem(AbstractItemView* item) = 0;
        
		/*!
          \brief Method that return the scene box.
		  
		  \param graphic object		  
        */
        virtual te::gm::Envelope getSceneBox();
        
		/*!
          \brief Method that starts the scene and configures. Calculates the transformation matrix of the scene and calculates the ratio of the size of the scene with the paper size.
		  
		  \param screenWMM width of physical screen in millimeters
		  \param screenHMM height of physical screen in millimeters
        */
        virtual void init(double screenWMM, double screenHMM) = 0;

        /*!
          \brief The most important common data.
          \return most important common data between te::layout::View and te::layout::Scene
        */
        virtual ContextObject getContext() = 0;
                        
      protected:
        
		/*!
          \brief Method that calculates the transformation matrix of the scene. This matrix will be set in each QGraphicsView class that watches this scene.
        */
        virtual void calculateMatrixViewScene() = 0;

		/*!
          \brief Method that calculates the ratio of the size of the scene with the paper size. This calculation is necessary so that the paper always is centered in the scene.
		  
		  \param wMM width of physical screen in millimeters
		  \param hMM height of physical screen in millimeters
        */
        virtual void calculateWindow(double wMM, double hMM) = 0;
        
      protected:

        te::gm::Envelope  m_box; //!< scene box.
    };
  }
}

#endif

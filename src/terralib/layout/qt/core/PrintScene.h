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
  \file PrintScene.h
   
  \brief Class responsible for printing the entire content or part of the scene. As the scene is upside down, it is necessary to invert the y of the painter before printing.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PRINT_SCENE_H
#define __TERRALIB_LAYOUT_INTERNAL_PRINT_SCENE_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/enum/AbstractType.h"

// Qt
#include <QObject>
#include <QRectF>

class QGraphicsScene;
class QPrinter;
class QPainter;

namespace te
{
  namespace layout
  {
    class PaperConfig;

    /*!
	  \brief Class responsible for printing the entire content or part of the scene. As the scene is upside down, it is necessary to invert the y of the painter before printing.
	  
	  \ingroup layout
	  */
    class TELAYOUTEXPORT PrintScene : public QObject
    {
      Q_OBJECT //for slots/signals
        
      public:

        PrintScene( QGraphicsScene* scene, PaperConfig* config );

        virtual ~PrintScene();

        virtual void printPreview();

        virtual bool exportToPDF();

      protected slots:

        virtual void printPaper(QPrinter* printer);

      protected:

        virtual QPrinter* createPrinter();

        virtual void renderScene( QPainter* newPainter, QPrinter* printer );

        virtual void deselectAllItems();

        QGraphicsScene* m_scene;
        PaperConfig*    m_config;
        te::layout::PrinterScene m_printState;
    };
  }
}

#endif 

/*  Copyright (C) 2013-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file Scene.h
   
  \brief 

  \ingroup layout
*/

#ifndef	__TERRALIB_LAYOUT_INTERNAL_SCENE_H 
#define __TERRALIB_LAYOUT_INTERNAL_SCENE_H

// TerraLib
#include "../../core/AbstractScene.h"
#include "../../core/Config.h"
#include "PrintScene.h"
#include "AlignItems.h"

// STL
#include <string>
#include <vector>
#include <map>

// Qt
#include <QGraphicsScene>
#include <QTransform>
#include <QColor>
#include <QPointF>

class QUndoCommand;
class QUndoStack;
class QGraphicsItemGroup;

namespace te
{
  namespace layout
  {
    class EnumType;
    class Properties;
    class VisualizationArea;

    class TELAYOUTEXPORT Scene : public QGraphicsScene, public AbstractScene
    {
      Q_OBJECT //for slots/signals

      public:

        Scene(QObject* object = (QObject*)0);

        virtual ~Scene();

        virtual void insertItem(ItemObserver* item);
                
        virtual void init(double screenWMM, double screenHMM);

        virtual QTransform sceneTransform();
        
        virtual void deleteItems();

        virtual void removeSelectedItems();

        virtual QGraphicsItemGroup* createItemGroup( const QList<QGraphicsItem *> & items );

        virtual void destroyItemGroup( QGraphicsItemGroup *group );

        virtual QGraphicsItem* createItem( const te::gm::Coord2D& coord );

        virtual void addUndoStack( QUndoCommand* command );

        virtual void setUndoStackLimit( int limit );

        virtual int getUndoStackLimit();

        virtual QUndoStack* getUndoStack();

        virtual void calculateSceneMeasures(double widthMM, double heightMM);

        virtual PrintScene* getPrintScene();

        virtual AlignItems* getAlignItems();

        virtual bool exportPropertiesToTemplate(EnumType* type, std::string fileName);

        virtual std::vector<te::layout::Properties*> importTemplateToProperties(EnumType* type, std::string fileName);
        
        virtual void reset();

        virtual bool buildTemplate(VisualizationArea* vzArea, EnumType* type, std::string fileName);

        virtual void redrawSelectionMap();

        virtual void exportItemsToImage(std::string dir);

        virtual bool	eventFilter ( QObject * watched, QEvent * event );
        
      signals:

         void addItemFinalized();
        
      protected:

        virtual void calculateMatrixViewScene();

        virtual void calculateWindow(double wMM, double hMM); 

        virtual std::vector<te::layout::Properties*> getItemsProperties();
        
    protected:

        QTransform                         m_matrix;
        QColor                             m_backgroundColor;
        QUndoStack*                        m_undoStack;
        int                                m_undoStackLimit;
        AlignItems*                        m_align;
        PrintScene*                        m_print;
        bool                               m_moveWatched;
        std::map<QGraphicsItem*, QPointF>  m_moveWatches;
    };
  }
}

#endif

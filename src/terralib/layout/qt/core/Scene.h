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

// Qt
#include <QGraphicsScene>
#include <QTransform>
#include <QGraphicsItem>
#include <QMap>

// TerraLib
#include "../../core/AbstractScene.h"
#include "../../core/enum/AbstractType.h"
#include "../../core/Config.h"
#include "../../../geometry/Coord2D.h"

// STL
#include <map>
#include <string>

class QGraphicsSceneMouseEvent;
class QGraphicsItemGroup;
class QGraphicsProxyWidget;
class QPrinter;
class QPainter;
class QLine;
class QUndoStack;
class QUndoCommand;

namespace te
{
  namespace layout
  {
    class ItemObserver;
    class Properties;
    class VisualizationArea;
    class Systematic;
    class EnumType;
    class AddCommand;
    class ChangePropertyCommand;
    class MoveCommand;
    class DeleteCommand;

    class TELAYOUTEXPORT Scene : public QGraphicsScene, public AbstractScene
    {
      Q_OBJECT //for slots/signals

      public:
        
        enum PrinterScene
        {
          PreviewScene,
          PrintScene
        };
        
        Scene(QWidget* widget = (QWidget*)0);
        ~Scene();

        virtual void insertItem(ItemObserver* item);

        virtual void insertOutside(OutsideObserver* widget);

        virtual QGraphicsProxyWidget* insertOutsideProxy(OutsideObserver* widget);
        
        virtual te::gm::Envelope getSceneBox();

        /* Redraw all items except, if viewArea false, the items than create the Visualization Area */
        virtual void redrawItems(bool viewArea = false);

        QGraphicsItemGroup*	createItemGroup ( const QList<QGraphicsItem *> & items );

        void destroyItemGroup(QGraphicsItemGroup *group);

        /*
          params widthMM width of physical screen in millimeters
          params heightMM height of physical screen in millimeters
        */
        virtual void init(double screenWMM, double screenHMM, double zoomFactor = 1.0);
        
        /* World coordinates (mm) */
        virtual te::gm::Envelope* getWorldBox() const;

        /* World coordinates (mm) */
        virtual te::gm::Envelope getPaperBox() const;
                
        virtual QTransform getMatrixViewScene();

        virtual void printPreview(bool isPdf = false);

        virtual void savePaperAsImage();

        virtual void savePaperAsPDF();
        
        virtual bool exportPropsAsJSON();

        virtual std::vector<te::layout::Properties*> importJsonAsProps();

        virtual void refresh(QGraphicsView* view = 0, double zoomFactor = 1.0);

        virtual void reset();

        virtual void buildTemplate(VisualizationArea* vzArea);

        virtual QGraphicsItem* createItem(const te::gm::Coord2D& coord );
        
        virtual void deleteItems();

        virtual void removeSelectedItems();

        virtual void setCurrentToolInSelectedMapItems(EnumType* mode);

        void setLineIntersectionHzr(QLineF* line);

        void setLineIntersectionVrt(QLineF* line);

        virtual void bringToFront();

        virtual void sendToBack();

        virtual void redrawRulers();

        virtual void refreshRulers(te::gm::Envelope newBox);

        /*! \brief Get the number of map items that intersection the coordinate */
        virtual int intersectionMap(te::gm::Coord2D coord, bool &intersection);

        virtual void setCurrentMapSystematic(Systematic* systematic, te::gm::Coord2D coord);

        virtual void createTextGridAsObject();

        virtual void createTextMapAsObject();

        virtual void alignLeft();

        virtual void alignRight();

        virtual void alignTop();

        virtual void alignBottom();

        virtual void alignCenterHorizontal();

        virtual void alignCenterVertical();

        virtual QRectF getSelectionItemsBoundingBox();

        /* Part of undo/redo framework */

        virtual void addUndoStack(QUndoCommand* command);

        virtual QUndoStack* getUndoStack();

        virtual void setUndoStackLimit(int limit);

        virtual int getUndoStackLimit();

        virtual bool	eventFilter ( QObject * watched, QEvent * event );

        virtual void redrawSelectionMap();
        
      protected slots:

        virtual void printPaper(QPrinter* printer);

      signals:

        void addItemFinalized();

      protected:

        virtual te::gm::Envelope* calculateWindow(double wMM, double hMM);

        virtual void calculateMatrixViewScene(double zoomFactor = 1.);

        virtual QPrinter* createPrinter();

        virtual void renderScene( QPainter* newPainter );

        virtual void changePrintVisibility(bool change);

        virtual std::vector<te::layout::Properties*> getItemsProperties();

        virtual void drawForeground(QPainter *painter, const QRectF &rect);

        virtual void refreshViews(QGraphicsView* view = 0);

        virtual void createDefaultTextItemFromObject(std::map<te::gm::Coord2D, std::string> map);

        virtual void changeFlagsItemForPrint();

        virtual void restoreFlagsItemForPrint();
        
      protected:

        te::gm::Envelope*  m_boxW;
        QTransform         m_matrix;
        double             m_screenWidthMM;
        double             m_screenHeightMM;
        QLineF*            m_lineIntersectHrz;
        QLineF*            m_lineIntersectVrt;
        bool               m_fixedRuler;
        PrinterScene       m_previewState;
        QUndoStack*        m_undoStack;
        int                m_undoStackLimit;
        bool               m_moveWatched;
        std::map<QGraphicsItem*, QPointF> m_moveWatches;
        QMap<QGraphicsItem*, QGraphicsItem::GraphicsItemFlags> m_itemFlags; //<! 
    };
  }
}

#endif

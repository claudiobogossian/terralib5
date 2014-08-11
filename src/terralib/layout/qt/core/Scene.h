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

// TerraLib
#include "../../core/AbstractScene.h"
#include "../../core/enum/AbstractType.h"
#include "../../core/enum/EnumMode.h"
#include "../../core/Config.h"
#include "../../../geometry/Coord2D.h"

class QGraphicsSceneMouseEvent;
class QGraphicsItemGroup;
class QGraphicsProxyWidget;
class QPrinter;
class QPainter;
class QLine;

/*!
	\def __LAYOUT_HORIZONTAL_MARGIN
	Defines the layout margin to relation with viewport
*/
#define __LAYOUT_HORIZONTAL_MARGIN 0.05


/*!
	\def __LAYOUT_VERTICAL_MARGIN
	Defines the layout margin to relation with viewport
*/
#define __LAYOUT_VERTICAL_MARGIN 0.1

namespace te
{
  namespace layout
  {
    class ItemObserver;
    class Properties;
    class VisualizationArea;
    class Systematic;

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
        virtual void init(double screenWMM, double screenHMM, double paperMMW, double paperMMH, double zoomFactor = 1.0);
        
        /* World coordinates (mm) */
        virtual te::gm::Envelope* getWorldBox() const;

        /* World coordinates (mm) */
        virtual te::gm::Envelope* getPaperBox() const;
                
        virtual QTransform getMatrixViewScene();

        virtual void printPreview(bool isPdf = false);

        virtual void savePaperAsImage();

        virtual void savePaperAsPDF();
        
        virtual bool exportPropsAsJSON();

        virtual std::vector<te::layout::Properties*> importJsonAsProps();

        virtual void refresh(QGraphicsView* view = 0, double zoomFactor = 1.0);

        virtual void reset();

        virtual void buildTemplate(VisualizationArea* vzArea);

        virtual void createItem(const te::gm::Coord2D& coord );
        
        virtual void deleteItems();

        virtual void setCurrentToolInSelectedMapItems(LayoutMode mode);

        void setLineIntersectionHzr(QLineF* line);

        void setLineIntersectionVrt(QLineF* line);

        virtual void bringToFront();

        virtual void sendToBack();

        virtual void redrawRulers();

        virtual void refreshRulers(te::gm::Envelope newBox);

        /*! \brief Get the number of map items that intersection the coordinate */
        virtual int intersectionMap(te::gm::Coord2D coord, bool &intersection);

        virtual void setCurrentMapSystematic(Systematic* systematic, te::gm::Coord2D coord);
        
      protected slots:

        virtual void printPaper(QPrinter* printer);

      signals:

        void addItemFinalized();

      protected:

        virtual void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );

        virtual te::gm::Envelope* calculateWindow(double wMM, double hMM);
        virtual void calculateMatrixViewScene(double zoomFactor = 1.);

        virtual QPrinter* createPrinter();
        void renderScene( QPainter* newPainter );

        virtual void changePrintVisibility(bool change);

        virtual std::vector<te::layout::Properties*> getItemsProperties();

        virtual void drawForeground(QPainter *painter, const QRectF &rect);

        virtual void refreshViews(QGraphicsView* view = 0);

      protected:

        te::gm::Envelope* m_boxPaperW;
        te::gm::Envelope* m_boxW;
        QTransform m_matrix;
        double m_screenWidthMM;
        double m_screenHeightMM;
        QLineF*        m_lineIntersectHrz;
        QLineF*        m_lineIntersectVrt;
        bool           m_fixedRuler;
        PrinterScene     m_previewState;
    };
  }
}

#endif
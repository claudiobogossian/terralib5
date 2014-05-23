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
#include "AbstractScene.h"
#include "AbstractType.h"
#include "EnumMode.h"

class QGraphicsSceneMouseEvent;
class QGraphicsItemGroup;
class QGraphicsProxyWidget;
class QPrinter;
class QPainter;

namespace te
{
  namespace layout
  {
    class ItemObserver;
    class Properties;
    class VisualizationArea;

    class Scene : public QGraphicsScene, public AbstractScene
    {
      Q_OBJECT //for slots/signals

      public:
        Scene(QWidget* widget = (QWidget*)0);
        ~Scene();

        virtual void insertItem(ItemObserver* item);

        virtual void insertOutside(OutsideObserver* widget);

        virtual QGraphicsProxyWidget* insertOutsideProxy(OutsideObserver* widget);
        
        virtual te::gm::Envelope getSceneBox();

        virtual void redrawItems();

        QGraphicsItemGroup*	createItemGroup ( const QList<QGraphicsItem *> & items );

        void destroyItemGroup(QGraphicsItemGroup *group);

        /*
          params widthMM width of physical screen in millimeters
          params heightMM height of physical screen in millimeters
        */
        virtual void init(double widthMM, double heightMM);

        /* World coordinates (mm) */
        virtual te::gm::Envelope* getWorldBox() const;

        /* World coordinates (mm) */
        virtual te::gm::Envelope* getPaperBox() const;
        
        virtual QTransform getMatrixViewScene();

        virtual QGraphicsItem* getMasterParentItem();

        virtual void printPreview(bool isPdf = false);

        virtual void savePaperAsImage();

        virtual void savePaperAsPDF();
        
        virtual bool exportPropsAsJSON();

        virtual std::vector<te::layout::Properties*> importJsonAsProps();

        virtual void refresh();

        void setPointIntersectionMouse(QPointF point);

        virtual void buildTemplate(VisualizationArea* vzArea);

        virtual void createItem(const te::gm::Coord2D& coord );
        
        virtual void deleteItems();

        virtual void setCurrentToolInSelectedMapItems(LayoutMode mode);

      protected slots:

        virtual void printPaper(QPrinter* printer);

      signals:

        void addItemFinalized();

      protected:

        virtual void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );

        virtual te::gm::Envelope* calculateBoxPaper();
        virtual te::gm::Envelope* calculateWindow();
        virtual void calculateMatrixViewScene();
        virtual void createMasterParentItem();

        virtual QPrinter* createPrinter();
        void renderScene( QPainter* newPainter );

        virtual void changePrintVisibility(bool change);

        virtual std::vector<te::layout::Properties*> getItemsProperties();

        virtual void drawForeground(QPainter *painter, const QRectF &rect);
        
      protected:

        te::gm::Envelope* m_boxPaperW;
        te::gm::Envelope* m_boxW;
        QTransform m_matrix;
        double m_screenWidthMM;
        double m_screenHeightMM;
        QGraphicsItem* m_masterParent;
        QPointF m_pointIntersection;
    };
  }
}

#endif
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
  \file QLayoutScene.h
   
  \brief 

  \ingroup layout
*/

#ifndef	__TERRALIB_LAYOUT_INTERNAL_QLAYOUT_SCENE_H 
#define __TERRALIB_LAYOUT_INTERNAL_QLAYOUT_SCENE_H

// Qt
#include <QGraphicsScene>
#include <QTransform>

// TerraLib
#include "LayoutScene.h"
#include "LayoutAbstractType.h"

class QGraphicsSceneMouseEvent;
class QGraphicsItemGroup;
class QGraphicsProxyWidget;
class QPrinter;
class QPainter;

namespace te
{
  namespace layout
  {
    class LayoutItemObserver;
    class LayoutProperties;

    class QLayoutScene : public QGraphicsScene, public LayoutScene
    {
      Q_OBJECT //for slots/signals

      public:
        QLayoutScene(QWidget* widget = (QWidget*)0);
        ~QLayoutScene();

        virtual void insertItem(LayoutItemObserver* item);
        virtual void insertOutside(LayoutOutsideObserver* widget);
        virtual QGraphicsProxyWidget* insertOutsideProxy(LayoutOutsideObserver* widget);
        
        virtual te::gm::Envelope getSceneBox();

        QGraphicsItemGroup*	createItemGroup ( const QList<QGraphicsItem *> & items );
        void destroyItemGroup(QGraphicsItemGroup *group);

        /*
          params widthMM width of physical screen in millimeters
          params heightMM height of physical screen in millimeters
        */
        virtual void init(double widthMM, double heightMM, double dpix, double dpiy);

        virtual te::gm::Envelope getWorldBox();
        virtual te::gm::Envelope getPaperBox();
        
        virtual QTransform getMatrixViewScene();
        virtual QTransform getMatrixSceneItem();

        virtual QGraphicsItem* getMasterParentItem();

        virtual void printPreview(bool isPdf = false);
        virtual void savePaperAsImage();
        virtual void savePaperAsPDF();

        virtual void savePropsAsJSON();

      protected slots:

        virtual void printPaper(QPrinter* printer);

      signals:

        void addItemFinalized();

      protected:
        virtual void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );

        virtual te::gm::Envelope calculateBoxPaper();
        virtual te::gm::Envelope calculateWindow();
        virtual void calculateMatrixViewScene();
        virtual void calculateMatrixSceneItem();
        virtual void createMasterParentItem();

        virtual QPrinter* createPrinter();
        void renderScene( QPainter* newPainter );

        virtual void changePrintVisibility(bool change);

        virtual std::vector<te::layout::LayoutProperties*> getItemsProperties();

      protected:

        double m_paperSizeWMM;
        double m_paperSizeHMM;
        LayoutAbstractPaperType m_paperType;
        te::gm::Envelope m_boxPaperW;
        te::gm::Envelope m_boxW;
        QTransform m_matrix;
        QTransform m_matrixItem;
        double m_logicalDpiX;
        double m_logicalDpiY;
        double m_screenWidthMM;
        double m_screenHeightMM;
        QGraphicsItem* m_masterParent;
    };
  }
}

#endif
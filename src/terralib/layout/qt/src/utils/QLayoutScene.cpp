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
  \file QLayoutScene.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include <QLayoutScene.h>
#include "LayoutItemObserver.h"
#include <QWidget>
#include "LayoutContext.h"
#include "LayoutMode.h"
#include "QLayoutItemGroup.h"
#include "ItemGroupLayoutModel.h"
#include "ItemGroupLayoutController.h"
#include "LayoutAbstractType.h"
#include "LayoutObserver.h"
#include "LayoutTemplateEditor.h"
#include "LayoutTemplate.h"

// STL
#include <iostream>
#include <fstream>

// Qt
#include <QGraphicsScene.h>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QtGui>
#include <QPixmap>
#include <QPainter>
#include "RectangleLayoutModel.h"
#include "RectangleLayoutController.h"
#include "QRectangleLayoutItem.h"

te::layout::QLayoutScene::QLayoutScene( QWidget* widget): 
  QGraphicsScene(widget),
  m_paperType(TPA4),
  m_paperSizeWMM(210.),
  m_paperSizeHMM(297.)
{

}

te::layout::QLayoutScene::~QLayoutScene()
{

}

void te::layout::QLayoutScene::init( double widthMM, double heightMM, double dpix, double dpiy )
{
  m_screenWidthMM = widthMM;
  m_screenHeightMM = heightMM;

  m_logicalDpiX = dpix;
  m_logicalDpiY = dpiy;

  m_boxW = calculateWindow();
  m_boxPaperW = calculateBoxPaper();

  calculateMatrixViewScene();
  calculateMatrixSceneItem();

  createMasterParentItem();
}

void te::layout::QLayoutScene::insertItem( LayoutItemObserver* item )
{
  QGraphicsItem* qitem = ((QGraphicsItem*)item);

  if(qitem)
  {
    int total = m_masterParent->childItems().count();

    if(m_masterParent)
      qitem->setParentItem(m_masterParent); // have a addItem call inside
    else
      this->addItem(qitem);
    
    qitem->setZValue(total);

    emit addItemFinalized();
  }
}

void te::layout::QLayoutScene::insertOutside( LayoutOutsideObserver* widget )
{
  QWidget* qWidget = ((QWidget*)widget);
  this->addWidget(qWidget);
}

QGraphicsProxyWidget* te::layout::QLayoutScene::insertOutsideProxy( LayoutOutsideObserver* widget )
{
  QWidget* qWidget = ((QWidget*)widget);
  return this->addWidget(qWidget);
}

void te::layout::QLayoutScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
  QGraphicsScene::mousePressEvent(mouseEvent);
}

te::gm::Envelope te::layout::QLayoutScene::getSceneBox()
{
  QRectF srect =	sceneRect();

  QPointF ll = srect.bottomLeft();
  QPointF ur = srect.topRight();

  te::gm::Envelope box(ll.x(), ll.y(), ur.x(), ur.y());
  return box;
}

QGraphicsItemGroup* te::layout::QLayoutScene::createItemGroup( const QList<QGraphicsItem *> & items )
{
  //The scene create a new group with important restriction
  QGraphicsItemGroup* p = QGraphicsScene::createItemGroup(items);
  //Create a new group
  ItemGroupLayoutModel* model = new ItemGroupLayoutModel();		  
  ItemGroupLayoutController* controller = new ItemGroupLayoutController(model);
  LayoutItemObserver* item = (LayoutItemObserver*)controller->getView();
  QLayoutItemGroup* group = dynamic_cast<QLayoutItemGroup*>(item);

  if(p)
  {
    group->setParentItem(p->parentItem());    
    foreach (QGraphicsItem *item, p->childItems())
      group->addToGroup(item);
    
    delete p;
  }  

  return group;
}

void te::layout::QLayoutScene::destroyItemGroup( QGraphicsItemGroup *group )
{
  group->setHandlesChildEvents(false);
  QGraphicsScene::destroyItemGroup(group);
}

te::gm::Envelope te::layout::QLayoutScene::calculateBoxPaper()
{
  double x1 = 0.;
  double y1 = 0.;
  double x2 = 0.;
  double y2 = 0.;

  te::gm::Envelope boxW = calculateWindow();

  int widthW = boxW.getWidth();
  int heightW = boxW.getHeight();

  double middleWW = widthW/2.;
  double middleHW = heightW/2.;	

  double ppW = m_paperSizeWMM/2.;
  double ppH = m_paperSizeHMM/2.;

  if(widthW > m_paperSizeWMM)
  {
    x1 = 0;
    x2 = m_paperSizeWMM;
  }
  else
  {
    x1 = middleWW - ppW;
    x2 = x1 + m_paperSizeWMM;
  }

  if(heightW > m_paperSizeHMM)
  {
    y1 = 0;
    y2 = m_paperSizeHMM;
  }
  else
  {
    y1 = middleHW - ppH;
    y2 = y1 + m_paperSizeHMM;
  }

  te::gm::Envelope box(x1, y1, x2, y2);
  return box;
}

te::gm::Envelope te::layout::QLayoutScene::calculateWindow()
{
  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;

  int w = m_screenWidthMM;
  int h = m_screenHeightMM;

  double paddingW = w - m_paperSizeWMM;
  double margin1 = paddingW / 2.;

  if(w > m_paperSizeWMM)
  {
    x1 = - margin1;
    x2 = (paddingW + m_paperSizeWMM) - margin1;
  }
  else
  {
    x1 = 0;
    x2 = w;
  }

  double paddingH = h - m_paperSizeHMM;
  double margin2 = paddingH / 2.;

  if(h > m_paperSizeHMM)
  {
    y1 = - margin2;
    y2 = (paddingW + m_paperSizeHMM) - margin2;

  }
  else
  {
    y1 = 0;
    y2 = h;
  }

  te::gm::Envelope box(x1, y1, x2, y2);
  return box;
}

void te::layout::QLayoutScene::calculateMatrixViewScene()
{
  double llx = m_boxW.getLowerLeftX();
  double lly = m_boxW.getLowerLeftY();
  double urx = m_boxW.getUpperRightX();
  double ury = m_boxW.getUpperRightY();
  
  //Window - Mundo
  double dpiX = m_logicalDpiX;
  double dpiY = m_logicalDpiY;

  //mm (inversão do y)
  m_matrix = QTransform().scale(dpiX / 25.4, -dpiY / 25.4).translate(-llx, -ury);

  //Coordenadas de mundo - mm
  setSceneRect(QRectF(QPointF(llx, lly), QPointF(urx, ury)));
}

void te::layout::QLayoutScene::calculateMatrixSceneItem()
{
  //Matrix for items ( between scene and items )
  //take translate
  double m_31 = m_matrix.inverted().m31();
  double m_32 = m_matrix.inverted().m32(); 
  //m11 and m22 : scale
  //Create matrix for items
  // shearing( m21 and m12 ) : distortion 
  // m31 and m32 : translate  
  m_matrixItem = QTransform(1, 0, 0, 0, -1, 0, m_31, m_32, 1);
}

te::gm::Envelope te::layout::QLayoutScene::getWorldBox()
{
 return m_boxW; 
}

te::gm::Envelope te::layout::QLayoutScene::getPaperBox()
{
  return m_boxPaperW;
}

QTransform te::layout::QLayoutScene::getMatrixViewScene()
{
  return m_matrix;
}

QTransform te::layout::QLayoutScene::getMatrixSceneItem()
{
  return m_matrixItem;
}

void te::layout::QLayoutScene::createMasterParentItem()
{
  double llx = m_boxW.getLowerLeftX();
  double lly = m_boxW.getLowerLeftY();

  //Background
  QRectF sceneRectBack = sceneRect();	
  m_masterParent = addRect(sceneRectBack);
  m_masterParent->setTransform(m_matrixItem.inverted()); 
  ((QGraphicsRectItem*)m_masterParent)->setBrush((QBrush(QColor(109,109,109))));	
  m_masterParent->setPos(llx, lly);
  //m_masterParent->setPos(0, 0);
}

QGraphicsItem* te::layout::QLayoutScene::getMasterParentItem()
{
  return m_masterParent;
}

void te::layout::QLayoutScene::printPreview(bool isPdf)
{
    QPrinter* printer = createPrinter();

    QPrintPreviewDialog preview(printer);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPaper(QPrinter*)));
    preview.exec();
}

void te::layout::QLayoutScene::printPaper(QPrinter* printer)
{
  //Impressão de parte da Cena
  //Não é necessário mudar a escala do View
  
  QPainter newPainter(printer);
  newPainter.setRenderHint(QPainter::Antialiasing);
  
  double dpiX = LayoutContext::getInstance()->getDpiX();
  double dpiY = LayoutContext::getInstance()->getDpiY();

  LayoutContext::getInstance()->setDpiX(printer->logicalDpiX());
  LayoutContext::getInstance()->setDpiY(printer->logicalDpiY());

  renderScene(&newPainter);

  LayoutContext::getInstance()->setDpiX(dpiX);
  LayoutContext::getInstance()->setDpiY(dpiY);
}

void te::layout::QLayoutScene::savePaperAsImage()
{
  QString fileName = QFileDialog::getSaveFileName(QApplication::desktop(), tr("Save Image File"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
  if(!fileName.isEmpty())
  {
    QImage image(fileName);
    QPainter newPainter(&image);
    newPainter.setRenderHint(QPainter::Antialiasing);

    renderScene(&newPainter);

    image.save(fileName);
  }
}

void te::layout::QLayoutScene::savePaperAsPDF()
{
  QPrinter* printer= createPrinter();
  QString fileName = QFileDialog::getSaveFileName(QApplication::desktop(), tr("Save Image File"), QDir::currentPath(), tr("PDF Files (*.pdf)"));
  if(!fileName.isEmpty())
  {
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOutputFileName(fileName); 
    printPaper(printer);
  }
}

QPrinter* te::layout::QLayoutScene::createPrinter()
{
  QPrinter* printer=new QPrinter(QPrinter::HighResolution);
  printer->setPageSize(QPrinter::A4);
  printer->setOrientation( QPrinter::Portrait );
  printer->pageRect(QPrinter::Millimeter);

  return printer;
}

void te::layout::QLayoutScene::renderScene( QPainter* newPainter )
{      
  changePrintVisibility(false);

  te::gm::Envelope paperBox =  getPaperBox();
      
  //Box Paper in the Scene (Source)
  QRectF mmSourceRect(paperBox.getLowerLeftX(), paperBox.getLowerLeftY(), 
    paperBox.getWidth(), paperBox.getHeight());

  //Paper size using the printer dpi (Target)
  QRect pxTargetRect(0, 0, newPainter->device()->width(), newPainter->device()->height());

  /* Print Paper (Scene to Printer)
  draw items with printer painter */

  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
  //Convert world to screen coordinate. Uses dpi printer.
  te::gm::Envelope paperNewBox = utils->viewportBox(paperBox);
  
  //Mirroring Y-Axis
  newPainter->translate( paperNewBox.getWidth() / 2, paperNewBox.getHeight() / 2 );
  newPainter->scale( 1, -1 );
  newPainter->translate( -(paperNewBox.getWidth() / 2), -(paperNewBox.getHeight() / 2) );

  //Without mirroring the scene is drawn upside down, 
  //since the system that it uses is Cartesian
  this->render(newPainter, pxTargetRect, mmSourceRect); 

  changePrintVisibility(true);
}

void te::layout::QLayoutScene::changePrintVisibility( bool change )
{
  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {		
      LayoutItemObserver* lItem = dynamic_cast<LayoutItemObserver*>(item);
      if(lItem)
      {
        if(!lItem->isPrintable())
        {
          item->setVisible(change);
        }
      }
    }
  }
}

void te::layout::QLayoutScene::savePropsAsJSON()
{
  QString fileName = QFileDialog::getSaveFileName(QApplication::desktop(), tr("Save File"), 
    QDir::currentPath(), tr("JSON Files (*.json)"));

  if(fileName.isEmpty())
  {
    return;
  }

  std::string j_name = fileName.toStdString();

  std::vector<te::layout::LayoutProperties*> props = getItemsProperties();
  
  if(props.empty())
    return;

  LayoutTemplateEditor editor(TPJSONTemplate, j_name);

  LayoutTemplate* jtemplate = editor.getTemplate();
  
  if(!jtemplate)
    return;
  
  bool is_export = false;

  is_export = jtemplate->exportTemplate(props);

  /*
    Object Reconstruction:
      - instance model;
      - update properties;
      - create controller;
      - draw;
  */
  std::vector<te::layout::LayoutProperties*> propsImport = jtemplate->importTemplate();
}

std::vector<te::layout::LayoutProperties*> te::layout::QLayoutScene::getItemsProperties()
{
  std::vector<te::layout::LayoutProperties*> props;

  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {		
      LayoutItemObserver* lItem = dynamic_cast<LayoutItemObserver*>(item);
      if(lItem)
      {
        if(!lItem->isPrintable())
          continue;

        props.push_back(lItem->getProperties());
      }
    }
  }

  return props;
}

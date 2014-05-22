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
  \file Scene.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Scene.h"
#include "ItemObserver.h"
#include <QWidget>
#include "Context.h"
#include "EnumMode.h"
#include "ItemGroup.h"
#include "ItemGroupModel.h"
#include "ItemGroupController.h"
#include "AbstractType.h"
#include "Observer.h"
#include "TemplateEditor.h"
#include "AbstractTemplate.h"
#include "RectangleModel.h"
#include "RectangleController.h"
#include "RectangleItem.h"
#include "PaperConfig.h"
#include "VisualizationArea.h"
#include "BuildGraphicsItem.h"

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
#include <QMessageBox>

te::layout::Scene::Scene( QWidget* widget): 
  QGraphicsScene(widget),
  m_boxW(0),
  m_boxPaperW(0),
  m_masterParent(0),
  m_pointIntersection(0,0)
{

}

te::layout::Scene::~Scene()
{
  if(m_boxW)
  {
    delete m_boxW;
    m_boxW = 0;
  }

  if(m_boxPaperW)
  {
    delete m_boxPaperW;
    m_boxPaperW = 0;
  }
}

void te::layout::Scene::init( double widthMM, double heightMM )
{
  m_screenWidthMM = widthMM;
  m_screenHeightMM = heightMM;
  
  m_boxW = calculateWindow();
  m_boxPaperW = calculateBoxPaper();

  calculateMatrixViewScene();

  createMasterParentItem();
}

void te::layout::Scene::insertItem( ItemObserver* item )
{
  QGraphicsItem* qitem = ((QGraphicsItem*)item);

  int total = 0;

  if(qitem)
  {
    if(qitem->scene() != this) 
    {
      if(m_masterParent)
      {
        total = m_masterParent->childItems().count();    
        qitem->setParentItem(m_masterParent); // have a addItem call inside
      }
      else
        this->addItem(qitem);

      qitem->setZValue(total);
    }

    ItemObserver* obs = dynamic_cast<ItemObserver*>(qitem);
    if(obs)
    {
      obs->refresh();
    }
  }

  emit addItemFinalized();
}

void te::layout::Scene::insertOutside( OutsideObserver* widget )
{
  QWidget* qWidget = ((QWidget*)widget);
  this->addWidget(qWidget);
}

QGraphicsProxyWidget* te::layout::Scene::insertOutsideProxy( OutsideObserver* widget )
{
  QWidget* qWidget = ((QWidget*)widget);
  return this->addWidget(qWidget);
}

void te::layout::Scene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
  QGraphicsScene::mousePressEvent(mouseEvent);
}

te::gm::Envelope te::layout::Scene::getSceneBox()
{
  QRectF srect =	sceneRect();

  QPointF ll = srect.bottomLeft();
  QPointF ur = srect.topRight();

  te::gm::Envelope box(ll.x(), ll.y(), ur.x(), ur.y());
  return box;
}

void te::layout::Scene::redrawItems()
{
  double factor = Context::getInstance()->getZoomFactor();

  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        lItem->redraw(factor);
      }
    }
  }
}

QGraphicsItemGroup* te::layout::Scene::createItemGroup( const QList<QGraphicsItem *> & items )
{
  //The scene create a new group with important restriction
  QGraphicsItemGroup* p = QGraphicsScene::createItemGroup(items);
  
  //Create a new group
  BuildGraphicsItem* build = Context::getInstance()->getBuildGraphicsItem();
  te::gm::Coord2D coord(0,0);
  QGraphicsItem* item = build->createItem(TypeCreateItemGroup, coord, false);

  ItemGroup* group = dynamic_cast<ItemGroup*>(item);

  if(p)
  {
    if(group)
    {   
      QGraphicsItem* parent = group->parentItem();
      group->setParentItem(p->parentItem());
      foreach (QGraphicsItem *item, p->childItems())
      {
        group->addToGroup(item);
      }

      delete p;

      group->setParentItem(parent);
    }
  }  

  return group;
}

void te::layout::Scene::destroyItemGroup( QGraphicsItemGroup *group )
{
  group->setHandlesChildEvents(false);
  QGraphicsScene::destroyItemGroup(group);
}

te::gm::Envelope* te::layout::Scene::calculateBoxPaper()
{
  PaperConfig* config =  Context::getInstance()->getPaperConfig();

  double ppSizeWMM = 0;
  double ppSizeHMM = 0;

  config->getPaperSize(ppSizeWMM, ppSizeHMM);

  double x1 = 0.;
  double y1 = 0.;
  double x2 = 0.;
  double y2 = 0.;

  te::gm::Envelope* boxW = calculateWindow();

  int widthW = boxW->getWidth();
  int heightW = boxW->getHeight();

  double middleWW = widthW/2.;
  double middleHW = heightW/2.;	

  double ppW = ppSizeWMM/2.;
  double ppH = ppSizeHMM/2.;

  if(widthW > ppSizeWMM)
  {
    x1 = 0;
    x2 = ppSizeWMM;
  }
  else
  {
    x1 = middleWW - ppW;
    x2 = x1 + ppSizeWMM;
  }

  if(heightW > ppSizeHMM)
  {
    y1 = 0;
    y2 = ppSizeHMM;
  }
  else
  {
    y1 = middleHW - ppH;
    y2 = y1 + ppSizeHMM;
  }

  te::gm::Envelope* box = new te::gm::Envelope(x1, y1, x2, y2);
  return box;
}

te::gm::Envelope* te::layout::Scene::calculateWindow()
{
  PaperConfig* config =  Context::getInstance()->getPaperConfig();

  double ppSizeWMM = 0;
  double ppSizeHMM = 0;

  config->getPaperSize(ppSizeWMM, ppSizeHMM);

  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;

  int w = m_screenWidthMM;
  int h = m_screenHeightMM;

  double paddingW = w - ppSizeWMM;
  double margin1 = paddingW / 2.;

  if(w > ppSizeWMM)
  {
    x1 = - margin1;
    x2 = (paddingW + ppSizeWMM) - margin1;
  }
  else
  {
    x1 = 0;
    x2 = w;
  }

  double paddingH = h - ppSizeHMM;
  double margin2 = paddingH / 2.;

  if(h > ppSizeHMM)
  {
    y1 = - margin2;
    y2 = (paddingW + ppSizeHMM) - margin2;

  }
  else
  {
    y1 = 0;
    y2 = h;
  }

  te::gm::Envelope* box = new te::gm::Envelope(x1, y1, x2, y2);
  return box;
}

void te::layout::Scene::calculateMatrixViewScene()
{
  double llx = m_boxW->getLowerLeftX();
  double lly = m_boxW->getLowerLeftY();
  double urx = m_boxW->getUpperRightX();
  double ury = m_boxW->getUpperRightY();
  
  double dpiX = Context::getInstance()->getDpiX();
  double dpiY = Context::getInstance()->getDpiY();

  //mm (inversão do y)
  m_matrix = QTransform().scale(dpiX / 25.4, -dpiY / 25.4).translate(-llx, -ury);

  //Coordenadas de mundo - mm
  setSceneRect(QRectF(QPointF(llx, lly), QPointF(urx, ury)));
}

te::gm::Envelope* te::layout::Scene::getWorldBox() const
{
 return m_boxW; 
}

te::gm::Envelope* te::layout::Scene::getPaperBox() const
{
  return m_boxPaperW;
}

QTransform te::layout::Scene::getMatrixViewScene()
{
  return m_matrix;
}

void te::layout::Scene::createMasterParentItem()
{
  double llx = m_boxW->getLowerLeftX();
  double lly = m_boxW->getLowerLeftY();

  //Background
  QRectF sceneRectBack = sceneRect();	
  m_masterParent = addRect(sceneRectBack);
  ((QGraphicsRectItem*)m_masterParent)->setBrush((QBrush(QColor(109,109,109))));	
  m_masterParent->setPos(0, 0);
}

QGraphicsItem* te::layout::Scene::getMasterParentItem()
{
  return m_masterParent;
}

void te::layout::Scene::printPreview(bool isPdf)
{
    QPrinter* printer = createPrinter();

    QPrintPreviewDialog preview(printer);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPaper(QPrinter*)));
    preview.exec();
}

void te::layout::Scene::printPaper(QPrinter* printer)
{
  //Impressão de parte da Cena
  //Não é necessário mudar a escala do View
  
  QPainter newPainter(printer);
  newPainter.setRenderHint(QPainter::Antialiasing);
  
  double dpiX = Context::getInstance()->getDpiX();
  double dpiY = Context::getInstance()->getDpiY();

  Context::getInstance()->setDpiX(printer->logicalDpiX());
  Context::getInstance()->setDpiY(printer->logicalDpiY());

  renderScene(&newPainter);

  Context::getInstance()->setDpiX(dpiX);
  Context::getInstance()->setDpiY(dpiY);
}

void te::layout::Scene::savePaperAsImage()
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

void te::layout::Scene::savePaperAsPDF()
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

QPrinter* te::layout::Scene::createPrinter()
{
  QPrinter* printer=new QPrinter(QPrinter::HighResolution);
  printer->setPageSize(QPrinter::A4);
  printer->setOrientation( QPrinter::Portrait );
  printer->pageRect(QPrinter::Millimeter);

  return printer;
}

void te::layout::Scene::renderScene( QPainter* newPainter )
{      
  changePrintVisibility(false);

  te::gm::Envelope* paperBox =  getPaperBox();
      
  //Box Paper in the Scene (Source)
  QRectF mmSourceRect(paperBox->getLowerLeftX(), paperBox->getLowerLeftY(), 
    paperBox->getWidth(), paperBox->getHeight());

  //Paper size using the printer dpi (Target)
  QRect pxTargetRect(0, 0, newPainter->device()->width(), newPainter->device()->height());

  /* Print Paper (Scene to Printer)
  draw items with printer painter */

  Utils* utils = Context::getInstance()->getUtils();
  //Convert world to screen coordinate. Uses dpi printer.
  te::gm::Envelope paperNewBox = utils->viewportBox(*paperBox);
  
  this->render(newPainter, pxTargetRect, mmSourceRect); 

  changePrintVisibility(true);
}

void te::layout::Scene::changePrintVisibility( bool change )
{
  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {		
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
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

bool te::layout::Scene::exportPropsAsJSON()
{
  bool is_export = false;

  QString fileName = QFileDialog::getSaveFileName(QApplication::desktop(), tr("Save File"), 
    QDir::currentPath(), tr("JSON Files (*.json)"));

  if(fileName.isEmpty())
  {
    return is_export;
  }

  std::string j_name = fileName.toStdString();

  std::vector<te::layout::Properties*> props = getItemsProperties();
  
  if(props.empty())
    return is_export;

  TemplateEditor editor(TPJSONTemplate, j_name);

  AbstractTemplate* jtemplate = editor.getTemplate();
  
  if(!jtemplate)
    return is_export;

  is_export = jtemplate->exportTemplate(props);

  QMessageBox msgBox;
  
  if(is_export)
  {
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Template exported successfully!");    
  }
  else
  {
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Error exporting template!");
  }

  msgBox.exec();

  return is_export;
}

std::vector<te::layout::Properties*> te::layout::Scene::importJsonAsProps()
{
  std::vector<te::layout::Properties*> props;

  QString fileName = QFileDialog::getOpenFileName(QApplication::desktop(), tr("Import File"), 
    QDir::currentPath(), tr("JSON Files (*.json)"));

  if(fileName.isEmpty())
  {
    return props;
  }

  std::string j_name = fileName.toStdString();  
  
  TemplateEditor editor(TPJSONTemplate, j_name);

  AbstractTemplate* jtemplate = editor.getTemplate();

  if(!jtemplate)
    return props;

  props = jtemplate->importTemplate();

  return props;
}

std::vector<te::layout::Properties*> te::layout::Scene::getItemsProperties()
{
  std::vector<te::layout::Properties*> props;

  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {		
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
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

void te::layout::Scene::refresh()
{
  clear();
  createMasterParentItem();
}

void te::layout::Scene::drawForeground( QPainter *painter, const QRectF &rect )
{
  QGraphicsScene::drawForeground(painter, rect);
}

void te::layout::Scene::setPointIntersectionMouse( QPointF point )
{
  m_pointIntersection = point;
}

void te::layout::Scene::buildTemplate(VisualizationArea* vzArea)
{
  BuildGraphicsItem* build = Context::getInstance()->getBuildGraphicsItem();

  if(!build)
    return;

  std::vector<te::layout::Properties*> props = importJsonAsProps();

  if(props.empty())
    return;

  refresh();

  std::vector<te::layout::Properties*>::iterator it;

  te::gm::Envelope* boxW = getWorldBox();
  vzArea->changeBoxArea(boxW);
  vzArea->rebuildWithoutPaper();

  for(it = props.begin() ; it != props.end() ; ++it)
  {
    te::layout::Properties* proper = (*it);

    if(!proper)
      continue;

    build->rebuildItem(proper);
  }
}

void te::layout::Scene::deleteItems()
{
  QList<QGraphicsItem*> graphicsItems = selectedItems();

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      removeItem(item);
      if(item)
      {
        delete item;
        item = 0;
      }
    }
  }
}

void te::layout::Scene::createItem( const te::gm::Coord2D& coord )
{
  BuildGraphicsItem* build = Context::getInstance()->getBuildGraphicsItem();

  if(!build)
    return;

  QGraphicsItem* item = 0;

  LayoutMode mode = Context::getInstance()->getMode();

  item = build->createItem(mode, coord);

  //If Create item
  if(item)
  {
    Context::getInstance()->setMode(TypeNone);
  }
}
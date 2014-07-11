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
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/pattern/singleton/Context.h"
#include "../item/ItemGroup.h"
#include "../../item/ItemGroupModel.h"
#include "../../item/ItemGroupController.h"
#include "../../core/enum/AbstractType.h"
#include "../../core/pattern/mvc/Observer.h"
#include "../../core/template/TemplateEditor.h"
#include "../../core/template/AbstractTemplate.h"
#include "../../item/RectangleModel.h"
#include "../../item/RectangleController.h"
#include "../item/RectangleItem.h"
#include "VisualizationArea.h"
#include "BuildGraphicsItem.h"
#include "../item/MapItem.h"
#include "ItemUtils.h"

// STL
#include <iostream>
#include <fstream>

// Qt
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QtGui>
#include <QPixmap>
#include <QPainter>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QWidget>
#include <QApplication>
#include <QDir>
#include <QPrinter>

te::layout::Scene::Scene( QWidget* widget): 
  QGraphicsScene(widget),
  m_boxW(0),
  m_boxPaperW(0),
  m_lineIntersectHrz(0),
  m_lineIntersectVrt(0),
  m_fixedRuler(true)
{
  setBackgroundBrush(QBrush(QColor(109,109,109)));

  //setBackgroundBrush(QBrush(QColor(100,100,0)));
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

void te::layout::Scene::init(double screenWMM, double screenHMM, double paperMMW, double paperMMH, double zoomFactor)
{
  m_screenWidthMM = screenWMM;
  m_screenHeightMM = screenHMM;
  
  m_boxW = calculateWindow(m_screenWidthMM, m_screenHeightMM, paperMMW, paperMMH);
  m_boxPaperW = calculateBoxPaper(m_screenWidthMM, m_screenHeightMM, paperMMW, paperMMH);

  calculateMatrixViewScene(zoomFactor);
}

void te::layout::Scene::insertItem( ItemObserver* item )
{
  QGraphicsItem* qitem = ((QGraphicsItem*)item);

  int total = 0;

  if(qitem)
  {
    if(qitem->scene() != this) 
    {
      total = this->items().count();
      //MapItem* map = dynamic_cast<MapItem*>(qitem);
      //if(map)
      //{
      //  /*
      //  As the coordinate system of the scene is in millimeters, 
      //  their children are too. But the map can not draw the mapDisplay in millimeters, 
      //  since its size is given in pixels. So the object got the 
      //  inverted matrix, to draw pixel by proper scaling between world and screen.
      //  */
      //  QTransform transf = m_matrix.inverted();
      //  double scalex = m_matrix.inverted().m11();
      //  double scaley = m_matrix.inverted().m22();
      //  transf.scale(scalex, scaley);
      //  map->setTransform(transf);
      //}
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

void te::layout::Scene::redrawItems(bool viewArea)
{  
  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        if(!viewArea)
        {
          if(lItem->isCanChangeGraphicOrder())
          {
            lItem->redraw();
          }
        }
        else
        {
          lItem->redraw();
        }        
      }
    }
  }
}

QGraphicsItemGroup* te::layout::Scene::createItemGroup( const QList<QGraphicsItem *> & items )
{
  //The scene create a new group with important restriction
  QGraphicsItemGroup* p = QGraphicsScene::createItemGroup(items);
  
  //Create a new group
  BuildGraphicsItem* build = Context::getInstance().getBuildGraphicsItem();
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

te::gm::Envelope* te::layout::Scene::calculateBoxPaper(double wMM, double hMM, double paperMMW, double paperMMH)
{
  double ppSizeWMM = paperMMW;
  double ppSizeHMM = paperMMH;
  
  double x1 = 0.;
  double y1 = 0.;
  double x2 = 0.;
  double y2 = 0.;

  te::gm::Envelope* boxW = calculateWindow(wMM, hMM, paperMMW, paperMMH);

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

te::gm::Envelope* te::layout::Scene::calculateWindow(double wMM, double hMM, double paperMMW, double paperMMH)
{
  double ppSizeWMM = paperMMW;
  double ppSizeHMM = paperMMH;
  
  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;

  int w = wMM;
  int h = hMM;

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

void te::layout::Scene::calculateMatrixViewScene(double zoomFactor)
{
  if(!m_boxW)
    return;

  double llx = m_boxW->getLowerLeftX();
  double lly = m_boxW->getLowerLeftY();
  double urx = m_boxW->getUpperRightX();
  double ury = m_boxW->getUpperRightY();
  
  double dpiX = Context::getInstance().getDpiX();
  double dpiY = Context::getInstance().getDpiY();

  /*
    The zoom is controlled by the main matrix, 
    so it's not necessary to redraw the objects 
    considering the zoom factor. When the zoom 
    factor changes, only the matrix is recalculated 
    and informed. All objects are redrawn in millimeters 
    without any change.
  */

  double newZoomFactor = (dpiX / 25.4) * zoomFactor;

  //mm (inversão do y)
  m_matrix = QTransform().scale(newZoomFactor, -newZoomFactor).translate(-llx, -ury);
  
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

void te::layout::Scene::printPreview(bool isPdf)
{
    QPrinter* printer = createPrinter();

    QPrintPreviewDialog preview(printer);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPaper(QPrinter*)));
    preview.exec();

    Context::getInstance().setMode(TypeNone);
}

void te::layout::Scene::printPaper(QPrinter* printer)
{
  //Impressão de parte da Cena
  //Não é necessário mudar a escala do View
  
  QPainter newPainter(printer);
  newPainter.setRenderHint(QPainter::Antialiasing);
  
  double dpiX = Context::getInstance().getDpiX();
  double dpiY = Context::getInstance().getDpiY();

  Context::getInstance().setDpiX(printer->logicalDpiX());
  Context::getInstance().setDpiY(printer->logicalDpiY());

  renderScene(&newPainter);

  Context::getInstance().setDpiX(dpiX);
  Context::getInstance().setDpiY(dpiY);
}

void te::layout::Scene::savePaperAsImage()
{
  QWidget* wg = (QWidget*)QApplication::desktop();
  QString fileName = QFileDialog::getSaveFileName(wg, tr("Save Image File"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
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
  QWidget* wg = (QWidget*)QApplication::desktop();
  QString fileName = QFileDialog::getSaveFileName(wg, tr("Save Image File"), QDir::currentPath(), tr("PDF Files (*.pdf)"));
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
        
  //Box Paper in the Scene (Source)
  QRectF mmSourceRect(m_boxPaperW->getLowerLeftX(), m_boxPaperW->getLowerLeftY(), 
    m_boxPaperW->getWidth(), m_boxPaperW->getHeight());

  //Paper size using the printer dpi (Target)
  QRect pxTargetRect(0, 0, newPainter->device()->width(), newPainter->device()->height());

  /* Print Paper (Scene to Printer)
  draw items with printer painter */

  Utils* utils = Context::getInstance().getUtils();
  //Convert world to screen coordinate. Uses dpi printer.
  te::gm::Envelope paperNewBox = utils->viewportBox(*m_boxPaperW);
  
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

  QWidget* wg = (QWidget*)QApplication::desktop();
  QString fileName = QFileDialog::getSaveFileName(wg, tr("Save File"), 
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

  QWidget* wg = (QWidget*)QApplication::desktop();
  QString fileName = QFileDialog::getOpenFileName(wg, tr("Import File"), 
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

void te::layout::Scene::refresh(QGraphicsView* view, double zoomFactor)
{
  if(!m_boxW)
    return;

  calculateMatrixViewScene(zoomFactor);
  refreshViews(view);

  te::gm::Envelope newBox = *m_boxW;
  if(view)
  {
    /* New box because the zoom factor change the transform(matrix) */
    QPointF ll = view->mapToScene(0,0);
    QPointF ur = view->mapToScene(view->size().width(), view->size().height());
    newBox = te::gm::Envelope(ll.x(), ll.y(), ur.x(), ur.y());
  }

  refreshRulers(newBox);
}

void te::layout::Scene::refreshRulers(te::gm::Envelope newBox)
{
  if(!m_fixedRuler)
    return;

  double llx = newBox.getLowerLeftX();
  double lly = newBox.getLowerLeftY();
  double urx = newBox.getUpperRightX();
  double ury = newBox.getUpperRightY();

  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {       
        QPointF pt(llx, lly);

        ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(lItem->getModel());

        if(!model)
          continue;

        if(model->getType() == TPHorizontalRuler)
        {
          model->setBox(te::gm::Envelope(llx, lly, urx, lly + 10));
          QPointF p = item->scenePos();
          item->setPos(pt);
        }
        if(model->getType() == TPVerticalRuler)
        {
          model->setBox(te::gm::Envelope(llx, lly, llx + 10, ury));
          QPointF p = item->scenePos();
          item->setPos(pt);
        }
      }
    }
  }
}

void te::layout::Scene::refreshViews( QGraphicsView* view /*= 0*/ )
{
  if(!m_boxW)
    return;

  double llx = m_boxW->getLowerLeftX();
  double ury = m_boxW->getUpperRightY();

  if(view)
  {
    //Transform calcula automaticamente a matriz inversa
    view->setTransform(getMatrixViewScene());
    view->setTransformationAnchor(QGraphicsView::NoAnchor);	
    view->centerOn(QPointF(llx, ury));
    view->scale(1, -1);
  }
  else
  {
    QList<QGraphicsView*> vws = views();
    foreach(QGraphicsView* v, vws)
    {
      //Transform calcula automaticamente a matriz inversa
      v->setTransform(getMatrixViewScene());
      v->setTransformationAnchor(QGraphicsView::NoAnchor);	
      v->centerOn(QPointF(llx, ury));
      v->scale(1, -1);
    }
  } 
}

void te::layout::Scene::reset()
{
  clear();
}

void te::layout::Scene::drawForeground( QPainter *painter, const QRectF &rect )
{
  QGraphicsScene::drawForeground(painter, rect);
  
  if(Context::getInstance().getLineIntersectionMouseMode() != TypeActiveLinesIntersectionMouse)
    return;

  QBrush brush = painter->brush();

  QBrush brushCopy = brush;
  brush.setColor(QColor(0,0,0,255));

  QPen pen = painter->pen();

  QPen penCopy = pen;
  penCopy.setStyle(Qt::DashDotLine);

  painter->save();
  painter->setPen(penCopy);
  painter->setBrush(brushCopy);
  painter->drawLines(m_lineIntersectHrz, 1);
  painter->drawLines(m_lineIntersectVrt, 1);
  painter->restore();  

  painter->setBrush(brush);

  update();
}

void te::layout::Scene::buildTemplate(VisualizationArea* vzArea)
{
  BuildGraphicsItem* build = Context::getInstance().getBuildGraphicsItem();

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
  BuildGraphicsItem* build = Context::getInstance().getBuildGraphicsItem();

  if(!build)
    return;

  QGraphicsItem* item = 0;

  LayoutMode mode = Context::getInstance().getMode();

  item = build->createItem(mode, coord);

  //If Create item
  if(item)
  {
    Context::getInstance().setMode(TypeNone);
  }
}

void te::layout::Scene::setCurrentToolInSelectedMapItems( LayoutMode mode )
{
  if(mode == TypeNone)
    return;

  if(!te::layout::isCurrentMapTools())
    return;

  QList<QGraphicsItem*> graphicsItems = selectedItems();

  foreach(QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::ItemObserver* lItem = dynamic_cast<te::layout::ItemObserver*>(item);
    if(!lItem)
      continue;

    te::layout::MapItem* mit = dynamic_cast<te::layout::MapItem*>(lItem);
    if(!mit)
      continue;

    mit->changeCurrentTool(mode);
  }
}

void te::layout::Scene::setLineIntersectionHzr( QLineF* line )
{
  m_lineIntersectHrz = line;
}

void te::layout::Scene::setLineIntersectionVrt( QLineF* line )
{
  m_lineIntersectVrt = line;
}

void te::layout::Scene::bringToFront()
{
  if (selectedItems().isEmpty())
    return;

  QGraphicsItem *selectedItem = selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal maxZValue = selectedItem->zValue();
  QGraphicsItem* itemMaxZValue = selectedItem;

  qreal zValue = selectedItem->zValue();
  foreach (QGraphicsItem *item, overlapItems) 
  {
    if(item)
    {
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {        
        if((item->zValue() >= zValue) && (it->isCanChangeGraphicOrder()))
        {
          maxZValue = item->zValue();
          itemMaxZValue = item;     
        }
      }
    }
  }
  selectedItem->setZValue(maxZValue);
  if(itemMaxZValue)
  {
    itemMaxZValue->setZValue(zValue);
  }
}

void te::layout::Scene::sendToBack()
{
  if (selectedItems().isEmpty())
    return;

  QGraphicsItem *selectedItem = selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal minimumZValue = selectedItem->zValue();
  QGraphicsItem* itemMinimumZValue = selectedItem;

  qreal zValue = selectedItem->zValue();
  foreach (QGraphicsItem *item, overlapItems) 
  {
    if(item)
    {
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {
        if (item->zValue() <= zValue && (it->isCanChangeGraphicOrder()))
        {
          minimumZValue = item->zValue();
          itemMinimumZValue = item;
        }
      }
    }
  }
  selectedItem->setZValue(minimumZValue);
  if(itemMinimumZValue)
  {
    itemMinimumZValue->setZValue(zValue);
  }
}

void te::layout::Scene::redrawRulers()
{
  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {       
        if(lItem->getModel()->getType() == TPHorizontalRuler ||
          lItem->getModel()->getType() == TPVerticalRuler)
        {
          lItem->redraw();
        }
      }
    }
  }
}

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
#include "../item/DefaultTextItem.h"
#include "../../core/Systematic.h"
#include "../../item/MapModel.h"
#include "../item/MapGridItem.h"
#include "../../item/MapGridModel.h"
#include "../../item/GridGeodesicModel.h"
#include "../../item/GridPlanarModel.h"
#include "../../core/enum/Enums.h"
#include "pattern/command/AddCommand.h"
#include "pattern/command/ChangePropertyCommand.h"
#include "pattern/command/MoveCommand.h"
#include "pattern/command/DeleteCommand.h"

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
#include <QUndoStack>
#include <QUndoCommand>

te::layout::Scene::Scene( QWidget* widget): 
  QGraphicsScene(widget),
  m_boxW(0),
  m_lineIntersectHrz(0),
  m_lineIntersectVrt(0),
  m_fixedRuler(true),
  m_previewState(PreviewScene),
  m_undoStack(0),
  m_undoStackLimit(6),
  m_moveWatched(false)
{
  setBackgroundBrush(QBrush(QColor(109,109,109)));

  m_undoStack = new QUndoStack(this);
}

te::layout::Scene::~Scene()
{
  if(m_boxW)
  {
    delete m_boxW;
    m_boxW = 0;
  }

  if(m_undoStack)
  {
    delete m_undoStack;
    m_undoStack = 0;
  }
}

void te::layout::Scene::init(double screenWMM, double screenHMM, double zoomFactor)
{
  m_screenWidthMM = screenWMM;
  m_screenHeightMM = screenHMM;

  m_boxW = calculateWindow(m_screenWidthMM, m_screenHeightMM); 
  
  double newZoomFactor = 1. / zoomFactor;
  if(zoomFactor < 1.)
    newZoomFactor = zoomFactor;

  double w = m_boxW->getWidth() * newZoomFactor;
  double h = m_boxW->getHeight() * newZoomFactor;

  te::gm::Coord2D center = m_boxW->getCenter();

  te::gm::Envelope* env = calculateWindow(w, h);

  double halfWidth = env->getWidth() / 2.;
  double halfHeight = env->getHeight() / 2.;

  env->m_llx = center.x - halfWidth;
  env->m_lly = center.y - halfHeight;
  env->m_urx = center.x + halfWidth;
  env->m_ury = center.y + halfHeight;

  m_boxW->m_llx = env->m_llx;
  m_boxW->m_lly = env->m_lly;
  m_boxW->m_urx = env->m_urx;
  m_boxW->m_ury = env->m_ury;
  
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

      ItemObserver* obs = dynamic_cast<ItemObserver*>(qitem);
      if(obs)
      {
        if(obs->isInvertedMatrix())
        {
          QTransform transf = m_matrix.inverted();
          double scalex = m_matrix.inverted().m11();
          double scaley = m_matrix.inverted().m22();
          transf.scale(scalex, scaley);
          qitem->setTransform(transf);
        }
      }
      this->addItem(qitem);
      qitem->setZValue(total);
      QGraphicsObject* qObj = dynamic_cast<QGraphicsObject*>(qitem);
      if(qObj)
      {
        qObj->installEventFilter(this);
      }
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

  EnumModeType* mode = Enums::getInstance().getEnumModeType();
  
  //Create a new group
  BuildGraphicsItem* build = Context::getInstance().getBuildGraphicsItem();
  te::gm::Coord2D coord(0,0);
  QGraphicsItem* item = build->createItem(mode->getModeCreateItemGroup(), coord, false);

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

      QUndoCommand* command = new AddCommand(group);
      addUndoStack(command);
    }
  }  

  return group;
}

void te::layout::Scene::destroyItemGroup( QGraphicsItemGroup *group )
{
  group->setHandlesChildEvents(false);
  QGraphicsScene::destroyItemGroup(group);
}

te::gm::Envelope* te::layout::Scene::calculateWindow(double wMM, double hMM)
{
  te::gm::Envelope* box = 0;

  te::gm::Envelope env = getPaperBox();
  if(!env.isValid())
    return box;

  double ppSizeWMM = env.getWidth();
  double ppSizeHMM = env.getHeight();
  
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
    y2 = (paddingH + ppSizeHMM) - margin2;
  }
  else
  {
    y1 = 0;
    y2 = h;
  }

  box = new te::gm::Envelope(x1, y1, x2, y2);
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

te::gm::Envelope te::layout::Scene::getPaperBox() const
{
  double w = 0;
  double h = 0;

  PaperConfig* conf = Context::getInstance().getPaperConfig();
  conf->getPaperSize(w, h);
  te::gm::Envelope env(0, 0, w, h);
  
  return env;
}

QTransform te::layout::Scene::getMatrixViewScene()
{
  return m_matrix;
}

void te::layout::Scene::printPreview(bool isPdf)
{
  EnumModeType* mode = Enums::getInstance().getEnumModeType();

  QPrinter* printer = createPrinter();

  QPrintPreviewDialog preview(printer);
  connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPaper(QPrinter*)));
  preview.exec();

  Context::getInstance().setMode(mode->getModeNone());
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
  
  PaperConfig* conf = Context::getInstance().getPaperConfig();

  if(conf->getPaperOrientantion() == Portrait)
  {
    printer->setOrientation( QPrinter::Portrait );
  }
  else
  {
    printer->setOrientation( QPrinter::Landscape );
  }
  printer->pageRect(QPrinter::Millimeter);

  return printer;
}

void te::layout::Scene::renderScene( QPainter* newPainter )
{      
  if(m_previewState == PreviewScene)
  {
    m_previewState = PrintScene;
  }
  else
  {
    m_previewState = PreviewScene;
  }

  changePrintVisibility(false);

  double w = 0;
  double h = 0;

  PaperConfig* conf = Context::getInstance().getPaperConfig();
  conf->getPaperSize(w, h);
  te::gm::Envelope env(0, 0, w, h);
        
  //Box Paper in the Scene (Source)
  QRectF mmSourceRect(env.getLowerLeftX(), env.getLowerLeftY(), 
    env.getWidth(), env.getHeight());

  //Paper size using the printer dpi (Target)
  QRect pxTargetRect(0, 0, newPainter->device()->width(), newPainter->device()->height());

  /* Print Paper (Scene to Printer)
  draw items with printer painter */

  Utils* utils = Context::getInstance().getUtils();
    
  changeFlagsItemForPrint();

  this->render(newPainter, pxTargetRect, mmSourceRect); 
  
  restoreFlagsItemForPrint();

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
        item->setSelected(false);
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
    QPointF ll = view->mapToScene(0, 0);
    QPointF ur = view->mapToScene(view->width(), view->height());
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

  QPointF pt(llx, lly);

  EnumObjectType* type = Enums::getInstance().getEnumObjectType();

  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {       
        ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(lItem->getModel());

        if(!model)
          continue;

        if(model->getType() == type->getHorizontalRuler())
        {
          te::gm::Envelope boxH(llx, lly, urx, lly + 10);
          model->setBox(boxH);
          item->setPos(pt);
        }
        if(model->getType() == type->getVerticalRuler())
        {
          te::gm::Envelope boxV(llx, lly, llx + 10, ury);
          model->setBox(boxV);
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

  EnumModeType* mode = Enums::getInstance().getEnumModeType();
  
  if(Context::getInstance().getLineIntersectionMouseMode() != mode->getModeActiveLinesIntersectionMouse())
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

void te::layout::Scene::removeSelectedItems()
{
  QList<QGraphicsItem*> graphicsItems = selectedItems();
  
  if(graphicsItems.empty())
    return;

  QUndoCommand* command = new DeleteCommand(this);
  addUndoStack(command);

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      removeItem(item);
    }
  }
}

QGraphicsItem* te::layout::Scene::createItem( const te::gm::Coord2D& coord )
{
  QGraphicsItem* item = 0;

  BuildGraphicsItem* build = Context::getInstance().getBuildGraphicsItem();

  if(!build)
    return item;

  EnumModeType* type = Enums::getInstance().getEnumModeType();
  EnumType* mode = Context::getInstance().getMode();

  item = build->createItem(mode, coord);

  if(item)
  {
    ItemObserver* obs = dynamic_cast<ItemObserver*>(item);
    if(obs)
    {
      if(obs->isCanChangeGraphicOrder())
      {
        QUndoCommand* command = new AddCommand(item);
        addUndoStack(command);
      }
    }
  }

  Context::getInstance().setMode(type->getModeNone());

  return item;
}

void te::layout::Scene::setCurrentToolInSelectedMapItems( EnumType* mode )
{
  if(!mode)
    return;

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if(mode == type->getModeNone())
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
  EnumObjectType* type = Enums::getInstance().getEnumObjectType();

  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {       
        if(lItem->getModel()->getType() == type->getHorizontalRuler() ||
          lItem->getModel()->getType() == type->getVerticalRuler())
        {
          lItem->redraw();
        }
      }
    }
  }
}

int te::layout::Scene::intersectionMap( te::gm::Coord2D coord, bool &intersection )
{
  QList<QGraphicsItem *> items = selectedItems();
  int number = 0;
  intersection = false;

  QPointF pt(coord.x, coord.y);

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      number++;
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {
        MapItem* mt = dynamic_cast<MapItem*>(it);
        if(mt)
        {
          intersection = item->contains(pt);
        }
      }
    }
  }

  return number;
}

void te::layout::Scene::setCurrentMapSystematic( Systematic* systematic, te::gm::Coord2D coord )
{
  QGraphicsItem *item = selectedItems().first();
  if(item)
  {
    ItemObserver* it = dynamic_cast<ItemObserver*>(item);
    if(it)
    {
      MapItem* mt = dynamic_cast<MapItem*>(it);
      if(mt)
      {
        MapModel* model = dynamic_cast<MapModel*>(mt->getModel());
        model->setSystematic(systematic);
        model->generateSystematic(coord);
        mt->redraw();
      }
    }
  }
}

void te::layout::Scene::createTextGridAsObject()
{
  QGraphicsItem *item = selectedItems().first();
  if(item)
  {
    ItemObserver* it = dynamic_cast<ItemObserver*>(item);
    if(it)
    {
      MapGridItem* mt = dynamic_cast<MapGridItem*>(it);
      if(mt)
      {
        MapGridModel* model = dynamic_cast<MapGridModel*>(mt->getModel());

        GridGeodesicModel* gridGeo = dynamic_cast<GridGeodesicModel*>(model->getGridGeodesic());
        if(model->getGridGeodesic()->isVisible())
        {
          model->getGridGeodesic()->setVisibleAllTexts(false);
          std::map<te::gm::Coord2D, std::string> mapGeo = gridGeo->getGridInfo();
          createDefaultTextItemFromObject(mapGeo);
        }

        GridPlanarModel* gridPlanar = dynamic_cast<GridPlanarModel*>(model->getGridPlanar());
        if(model->getGridPlanar()->isVisible())
        {
          model->getGridGeodesic()->setVisibleAllTexts(false);
          std::map<te::gm::Coord2D, std::string> mapPlanar = gridPlanar->getGridInfo();
          createDefaultTextItemFromObject(mapPlanar);
        }        
      }
    }
  }
}

void te::layout::Scene::createTextMapAsObject()
{
  QGraphicsItem *item = selectedItems().first();
  if(item)
  {
    ItemObserver* it = dynamic_cast<ItemObserver*>(item);
    if(it)
    {
      MapItem* mt = dynamic_cast<MapItem*>(it);
      if(mt)
      {
        MapModel* model = dynamic_cast<MapModel*>(mt->getModel());
        std::map<te::gm::Coord2D, std::string> map = model->getTextMapAsObjectInfo();
        createDefaultTextItemFromObject(map);
      }
    }
  }
}

void te::layout::Scene::createDefaultTextItemFromObject( std::map<te::gm::Coord2D, std::string> map )
{
  EnumModeType* mode = Enums::getInstance().getEnumModeType();
  Context::getInstance().setMode(mode->getModeCreateText());

  std::map<te::gm::Coord2D, std::string>::iterator it;

  for (it = map.begin(); it != map.end(); ++it) 
  {
    te::gm::Coord2D coord = it->first;
    std::string text = it->second;
   
    QGraphicsItem* item = 0;
    item = createItem(coord);
    if(!item)
      continue;

    DefaultTextItem* txtItem = dynamic_cast<DefaultTextItem*>(item);
    if(txtItem)
    {
      txtItem->setPlainText(text.c_str());
    }
  }

  Context::getInstance().setMode(mode->getModeNone());
}

void te::layout::Scene::alignLeft()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();
  
  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = selectedItems(); 
  double dbLeft = 0;

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx = getPaperBox();
    dbLeft = ppbx.getLowerLeftX();
    QPointF pot(dbLeft, items.first()->sceneBoundingRect().y());
    items.first()->setPos(pot);
    return;
  }

  dbLeft = sourceRect.left();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      QPointF pt(dbLeft, item->scenePos().y());
      item->setPos(pt);
    }
  }
}

void te::layout::Scene::alignRight()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = selectedItems(); 

  double dbRight = 0;
  double w = 0;

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx = getPaperBox();
    dbRight = ppbx.getUpperRightX();
    w = dbRight - items.first()->sceneBoundingRect().width();
    QPointF pot(w, items.first()->sceneBoundingRect().y());
    items.first()->setPos(pot);
    return;
  }

  dbRight = sourceRect.right();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      w = dbRight - item->sceneBoundingRect().width();
      QPointF pt(w, item->scenePos().y());
      item->setPos(pt);
    }
  }
}

void te::layout::Scene::alignTop()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = selectedItems(); 
  double dbTop = 0;

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx = getPaperBox();
    dbTop = ppbx.getLowerLeftY();
    QPointF pot(items.first()->sceneBoundingRect().x(), dbTop);
    items.first()->setPos(pot);
    return;
  }

  dbTop = sourceRect.top();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      QPointF pt(item->scenePos().x(), dbTop);
      item->setPos(pt);
    }
  }
}

void te::layout::Scene::alignBottom()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = selectedItems(); 
  double dbBottom = 0;
  double h = 0;

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx = getPaperBox();
    dbBottom = ppbx.getUpperRightY();
    h = dbBottom - items.first()->sceneBoundingRect().height();
    QPointF pot(items.first()->sceneBoundingRect().x(), h);
    items.first()->setPos(pot);
    return;
  }

  dbBottom = sourceRect.bottom();
  
  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      h = dbBottom - item->sceneBoundingRect().height();
      QPointF pt(item->scenePos().x(), h);
      item->setPos(pt);
    }
  }
}

void te::layout::Scene::alignCenterHorizontal()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = selectedItems(); 
  double dbCenterHrz = 0;
  double w = 0;

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx = getPaperBox();
    dbCenterHrz = ppbx.getCenter().x;
    w = items.first()->sceneBoundingRect().width() / 2.;
    QPointF pot(dbCenterHrz - w, items.first()->sceneBoundingRect().y());
    items.first()->setPos(pot);
    return;
  }

  dbCenterHrz = sourceRect.center().x();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      w = item->sceneBoundingRect().width() / 2.;

      QPointF pt(dbCenterHrz - w, item->scenePos().y());
      item->setPos(pt);
    }
  }
}

void te::layout::Scene::alignCenterVertical()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = selectedItems(); 
  double dbCenterVrt = 0;
  double h = 0;

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx = getPaperBox();
    dbCenterVrt = ppbx.getCenter().y;
    h = items.first()->sceneBoundingRect().height() / 2.;
    QPointF pot(items.first()->sceneBoundingRect().x(), dbCenterVrt - h);
    items.first()->setPos(pot);
    return;
  }

  dbCenterVrt = sourceRect.center().y();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      h = item->sceneBoundingRect().height() / 2.;

      QPointF pt(item->scenePos().x(), dbCenterVrt - h);
      item->setPos(pt);
    }
  }
}

QRectF te::layout::Scene::getSelectionItemsBoundingBox()
{
  QRectF sourceRect; 
  QList<QGraphicsItem *> items = selectedItems(); 
  
  foreach(QGraphicsItem *item, items) 
  { 
    sourceRect = sourceRect.united(item->sceneBoundingRect()); 
  }

  return sourceRect;
}

void te::layout::Scene::addUndoStack( QUndoCommand* command )
{
  m_undoStack->push(command);
}

void te::layout::Scene::setUndoStackLimit( int limit )
{
  m_undoStackLimit = limit;
}

int te::layout::Scene::getUndoStackLimit()
{
  return m_undoStackLimit;
}

QUndoStack* te::layout::Scene::getUndoStack()
{
  return m_undoStack;
}

bool te::layout::Scene::eventFilter( QObject * watched, QEvent * event )
{
  if(event->type() == QEvent::GraphicsSceneMousePress)
  {
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(watched);
    if(item)
    {
      QList<QGraphicsItem*> its = selectedItems();
      m_moveWatches.clear();
      foreach(QGraphicsItem *item, its) 
      {
        QPointF pt = item->scenePos();
        m_moveWatches[item] = pt;
      }
    }
  }
  
  if(event->type() == QEvent::GraphicsSceneMouseMove)
  {
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(watched);
    if(item)
    {
      m_moveWatched = true;
    }
  }

  if(event->type() == QEvent::GraphicsSceneMouseRelease)
  {
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(watched);
    if(item)
    {
      if(m_moveWatched)
      {
        QUndoCommand* command = new MoveCommand(m_moveWatches);
        addUndoStack(command);
        m_moveWatched = false;
      }
    }
  }

  return QGraphicsScene::eventFilter(watched, event);
}

void te::layout::Scene::redrawSelectionMap()
{
  QList<QGraphicsItem*> selected = selectedItems();
  foreach(QGraphicsItem *item, selected) 
  {
    if(item)
    {
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {
        MapItem* mt = dynamic_cast<MapItem*>(it);
        if(mt)
        {
          mt->redraw();
        }
      }
    }
  }
}

void te::layout::Scene::changeFlagsItemForPrint()
{
  m_itemFlags.clear();

  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {		
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        if(lItem->isInvertedMatrix())
        {
          m_itemFlags[item] = item->flags();
          item->setFlags(QGraphicsItem::ItemSendsGeometryChanges);          
        }
      }
    }
  }
}

void te::layout::Scene::restoreFlagsItemForPrint()
{
  QMapIterator<QGraphicsItem*, QGraphicsItem::GraphicsItemFlags> i(m_itemFlags);
  while (i.hasNext()) 
  {
    i.next();
    QGraphicsItem* item = i.key();
    QGraphicsItem::GraphicsItemFlags flg = i.value();
    if(item)
    {
      item->setFlags(flg);
    }
  }
}

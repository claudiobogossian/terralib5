/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file View.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "View.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/enum/Enums.h"
#include "../../../geometry/Envelope.h"
#include "VisualizationArea.h"
#include "../item/ItemGroup.h"
#include "tools/ZoomClickTool.h"
#include "HorizontalRuler.h"
#include "VerticalRuler.h"
#include "PrintScene.h"
#include "../../core/enum/EnumTemplateType.h"
#include "ItemUtils.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "pattern/factory/tool/ToolFactoryParamsCreate.h"
#include "pattern/factory/tool/ToolFactory.h"
#include "../../core/ContextObject.h"

// Qt
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainterPath>
#include <QEvent>
#include "BuildGraphicsOutside.h"

te::layout::View::View( QWidget* widget) : 
  QGraphicsView(new QGraphicsScene, widget),
  m_visualizationArea(0),
  m_currentTool(0),
  m_pageSetupOutside(0),
  m_systematicOutside(0),
  m_selectionChange(false),
  m_menuBuilder(0),
  m_width(-1),
  m_height(-1),
  m_isMoving(false),
  m_movingItemGroup(0),
  m_updateItemPos(false)
{
  setDragMode(RubberBandDrag);

  m_horizontalRuler = new HorizontalRuler;
  m_verticalRuler = new VerticalRuler;
}

te::layout::View::~View()
{
  if (m_currentTool)
  {
    viewport()->removeEventFilter(m_currentTool);
    delete m_currentTool;
    m_currentTool = 0;
  }

  for (size_t i = 0; i < m_lateRemovalVec.size(); ++i)
  {
    delete m_lateRemovalVec[i];
  }
  m_lateRemovalVec.clear();

  if(m_visualizationArea)
  {
    delete m_visualizationArea;
    m_visualizationArea = 0;
  }

  if(m_pageSetupOutside)
  {
    delete m_pageSetupOutside;
    m_pageSetupOutside = 0;
  }

  if(m_systematicOutside)
  {
    delete m_systematicOutside;
    m_systematicOutside = 0;
  }

  if(m_verticalRuler)
  {
    delete m_verticalRuler;
    m_verticalRuler = 0;
  }

  if(m_horizontalRuler)
  {
    delete m_horizontalRuler;
    m_horizontalRuler = 0;
  }
}

void te::layout::View::mousePressEvent( QMouseEvent * event )
{
  QGraphicsView::mousePressEvent(event);
  
  QPointF scenePos = mapToScene(event->pos());
  te::gm::Coord2D coord(scenePos.x(), scenePos.y());

  Scene* sc = dynamic_cast<Scene*>(scene());
  if(!sc)
    return;

  if (m_isMoving == false)
  {
    QList<QGraphicsItem*> selectedItems = sc->selectedItems();

    if (selectedItems.size() > 1)
    {
      bool isInvertedMatrix = false;
      foreach(QGraphicsItem* item, selectedItems)
      {
        AbstractItemView* observer = dynamic_cast<AbstractItemView*> (item);
        if (observer)
        {
          if (observer->isInverted() == true)
          {
            isInvertedMatrix = true;
          }
        }
      }

      if (isInvertedMatrix == true)
      {
        m_movingItemGroup = sc->createMovingItemGroup(selectedItems);
        m_isMoving = true;
      }
    }
  }
}

void te::layout::View::mouseMoveEvent( QMouseEvent * event )
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    return;
  }

  QGraphicsView::mouseMoveEvent(event);

  Scene* sc = dynamic_cast<Scene*>(scene());

  if(!sc)
    return;

  if(!scene()->selectedItems().empty())
  {
    m_updateItemPos = true;
  }
  else
  {
    m_updateItemPos = false;
  }

  QPointF pt = mapToScene(event->pos());
   
  emit changeSceneCoordMouse(pt);
}

void te::layout::View::mouseReleaseEvent( QMouseEvent * event )
{
  QGraphicsView::mouseReleaseEvent(event);

  Scene* sc = dynamic_cast<Scene*>(scene());

  if(!sc)
    return;

  if (m_isMoving == true)
  {
    QList<QGraphicsItem*> selectedItems = m_movingItemGroup->childItems();

    sc->destroyItemGroup(m_movingItemGroup);
    m_movingItemGroup = 0;
    m_isMoving = false;

    sc->clearSelection();

    foreach (QGraphicsItem* item, selectedItems)
    {
      if (item->isSelected())
      {
        item->setSelected(false);
      }
    }

    sc->selectItems(selectedItems);
  }
  
  /* The Properties only load when selection change and mouse release */
  if(!m_selectionChange && !m_updateItemPos)
    return;

  if(m_updateItemPos)
  {
  }

  if (!sc->isEditionMode()) // If scene in edition mode the reload will happen in double click event
  {
    reload();
  }
}

void te::layout::View::mouseDoubleClickEvent(QMouseEvent * event)
{
  QGraphicsView::mouseDoubleClickEvent(event);

  Scene* sc = dynamic_cast<Scene*>(scene());
  if (!sc)
    return;

  if (sc->isEditionMode()) // If scene in edition mode the reload will happen in double click event
  {
    reload();
  }
}

void te::layout::View::wheelEvent(QWheelEvent *event)
{
  ViewportUpdateMode mode = viewportUpdateMode();
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  
  if(event->modifiers() & Qt::AltModifier)
  {
    int zoom = 0;

    // Zoom in / Zoom Out
    if(event->delta() > 0) 
    {
      //Zooming In
      zoom = nextZoom();
    }
    else
    {
      zoom = previousZoom();
    }

    setZoom(zoom);
  }
  
  QGraphicsView::wheelEvent(event);

  setViewportUpdateMode(mode);
}

void te::layout::View::keyPressEvent( QKeyEvent* keyEvent )
{
  Scene* scne = dynamic_cast<Scene*>(scene());

  EnumModeType* mode = Enums::getInstance().getEnumModeType();

  if((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->key() == Qt::Key_P))
  {
    print();
  }
  else if((keyEvent->modifiers() & Qt::AltModifier) && (keyEvent->key() == Qt::Key_0))
  {
    pan();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_R))
  {
    recompose();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_A))
  {
    zoomArea();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_O))
  {
    zoomOut();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_G))
  {
    createItemGroup();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_U))
  {
    destroyItemGroup();
  }
  else if(keyEvent->key() == Qt::Key_Delete)
  {
    if(getCurrentMode() != mode->getModeTextEditorInteraction())
    {
      if (scne->isEditionMode() == false)
      {
        scne->removeSelectedItems();
      }
    }
  }
  else if(keyEvent->key() == Qt::Key_Escape)
  {
    if (m_currentTool != NULL)
    {
      m_currentTool->keyPressEvent(keyEvent);
    }
  }
  QGraphicsView::keyPressEvent(keyEvent);
}

void te::layout::View::config()
{  
  //Calculate matrix and centralizes the scene

  Scene* nscene = dynamic_cast<Scene*>(scene());

  if(!nscene)
    return;
    
  double sw = viewport()->widthMM();
  double sh = viewport()->heightMM();

  if(m_width == -1 || m_height == -1)
  {
    m_width = sw;
    m_height = sh;
  }

  nscene->init(m_width, m_height, getContext());

  QTransform mtrx = nscene->sceneTransform();

  setTransform(mtrx);

  te::gm::Envelope box = nscene->getSceneBox();
  centerOn(QPointF(box.m_llx, box.m_ury));
      
  int zoom = getDefaultZoom();
  double newScale = zoom / 100.;
  scale(newScale, newScale); //Initial zoom out

  //----------------------------------------------------------------------------------------------
  if(!m_visualizationArea)
  {
    m_visualizationArea = new VisualizationArea(nscene, box);
  }

  if(!getCurrentMode())
  {
    if(Enums::getInstance().getEnumModeType())
    {
      setCurrentMode(Enums::getInstance().getEnumModeType()->getModeNone());
    }
  }

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  connect(scene(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
  connect(scene(), SIGNAL(editionFinalized()), this, SLOT(onEditionFinalized()));
}

void te::layout::View::resizeEvent(QResizeEvent * event)
{
  QGraphicsView::resizeEvent(event);
}

void te::layout::View::onToolbarChangeMode( te::layout::EnumType* newMode )
{
  changeMode(newMode);
}

void te::layout::View::createItemGroup()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();

  if(sc)
  {
    QGraphicsItemGroup* group = sc->createItemGroup(graphicsItems);

    if(!group)
      return;
          
    ItemGroup* layoutGroup = dynamic_cast<ItemGroup*>(group);
      
    if(!layoutGroup)
      return;

    //layoutGroup->redraw();

    /*If "enabled=true", QGraphicsItemGroup will handle all the events. For example, 
    the event of mouse click on the child item won't be handled by child item.
    If "enabled=false", QGraphicsItem Group will not block the child item's event 
    and let child item handle it own event.*/
    group->setHandlesChildEvents(true);
  }
}

void te::layout::View::destroyItemGroup()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      QGraphicsItemGroup* group = dynamic_cast<QGraphicsItemGroup*>(item);
      if(group)
      {
        if(sc)
        {
          sc->destroyItemGroup(group);
        }
      }
    }
  }
}

void te::layout::View::resetDefaultConfig(bool toolLateRemoval)
{
  //Use ScrollHand Drag Mode to enable Panning
  //You do need the enable scroll bars for that to work.
  setDragMode(RubberBandDrag);

  //Whole view not interactive while in ScrollHandDrag Mode
  setInteractive(true);
  setCursor(Qt::ArrowCursor);
  setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

  for (size_t i = 0; i < m_lateRemovalVec.size(); ++i)
  {
    delete m_lateRemovalVec[i];
  }
  m_lateRemovalVec.clear();

  if(m_currentTool)
  {
    viewport()->removeEventFilter(m_currentTool);
    
    if(toolLateRemoval == true)
    {
      m_currentTool->setParent(0);
      m_lateRemovalVec.push_back(m_currentTool);
      m_currentTool = 0;
    }
    else
    {
      delete m_currentTool;
      m_currentTool = 0;
    }    
  }
}

void te::layout::View::onMainMenuChangeMode( te::layout::EnumType* newMode )
{
  changeMode(newMode);
}

void te::layout::View::changeMode( EnumType* newMode )
{
  if(newMode == getCurrentMode())
  {
    return;
  }

  setCurrentMode(newMode);

  resetDefaultConfig();

  Scene* sc = dynamic_cast<Scene*>(scene());

  if(!sc)
    return;

  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();
  ItemUtils* iUtils = Context::getInstance().getItemUtils();

  EnumType* mode = getCurrentMode();
  
  if(mode == enumMode->getModeMapPan())
  {
    iUtils->setCurrentToolInSelectedMapItems(enumMode->getModeMapPan());
  }
  else if(mode == enumMode->getModeMapZoomIn())
  {
    iUtils->setCurrentToolInSelectedMapItems(enumMode->getModeMapZoomIn());
  }
  else if(mode == enumMode->getModeMapZoomOut()) 
  {
    iUtils->setCurrentToolInSelectedMapItems(enumMode->getModeMapZoomOut());
  }

  Scene* sce = dynamic_cast<Scene*>(scene());
  if(sce)
  {
    sce->setContext(getContext());
  }

  emit changeContext();
}

void te::layout::View::hideEvent( QHideEvent * event )
{
  if(m_menuBuilder)
  {
    m_menuBuilder->closeAllWindows();
  }

  QGraphicsView::hideEvent(event);
  emit hideView();
}

void te::layout::View::closeEvent( QCloseEvent * event )
{
  if(m_menuBuilder)
  {
    m_menuBuilder->closeAllWindows();
  }

  QGraphicsView::closeEvent(event);
  emit closeView();
}

void te::layout::View::showEvent( QShowEvent * event )
{
  QGraphicsView::showEvent(event);
  emit showView();
}

void te::layout::View::showPageSetup()
{
  if(!m_pageSetupOutside)
  {
    BuildGraphicsOutside build;
    EnumObjectType* type = Enums::getInstance().getEnumObjectType();
    QWidget* outside = build.createOuside(type->getPageSetup());
    m_pageSetupOutside = dynamic_cast<PageSetupOutside*>(outside);
    connect(m_pageSetupOutside, SIGNAL(changeConfig()), this, SLOT(onChangeConfig()));
  }

  m_pageSetupOutside->load();
  m_pageSetupOutside->show();
}

void te::layout::View::onChangeConfig()
{ 
  Scene* sc = dynamic_cast<Scene*>(scene());
  if(!sc)
    return;

  QGraphicsItem* oldItem = sc->getPaperItem();
  QRectF oldBoundingRect = oldItem->boundingRect();

  QSize oldSize(oldBoundingRect.width(), oldBoundingRect.height());

  sc->deletePaperItem();

  config();

  te::gm::Envelope boxW = sc->getSceneBox();
  m_visualizationArea->changeBoxArea(boxW);

  m_visualizationArea->build();

  QGraphicsItem* newItem = sc->getPaperItem();
  QRectF newBoundingRect = newItem->boundingRect();

  QSize newSize(newBoundingRect.width(), newBoundingRect.height());
    
  sc->applyPaperProportion(oldSize, newSize);

  recompose();
}

void te::layout::View::showSystematicScale()
{
  if(!m_systematicOutside)
  {
    BuildGraphicsOutside build;
    EnumObjectType* type = Enums::getInstance().getEnumObjectType();
    QWidget* outside = build.createOuside(type->getSystematicScale());
    m_systematicOutside = dynamic_cast<SystematicScaleOutside*>(outside);
    connect(m_systematicOutside, SIGNAL(systematicApply(double,SystematicScaleType)), this, SLOT(onSystematicApply(double,SystematicScaleType)));
  }

  m_systematicOutside->show();
}

void te::layout::View::closeOutsideWindows()
{
  if(m_pageSetupOutside)
  {
    m_pageSetupOutside->close();
  }
}

void te::layout::View::onSystematicApply(double scale, SystematicScaleType type)
{
 
}

void te::layout::View::onSelectionChanged()
{
  m_selectionChange = true;

  if(m_menuBuilder)
  {
    m_menuBuilder->closeAllWindows();
  }
}

void te::layout::View::contextMenuEvent( QContextMenuEvent * event )
{
  if(event->reason() != QContextMenuEvent::Mouse)
    return;

  QPointF pt = mapToScene(event->pos());

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
  {
    return;
  }

  QGraphicsItem* hasItem = iUtils->intersectionSelectionItem(pt.x(), pt.y());
  if(!hasItem)
    return;

  if(!m_menuBuilder)
  {
    m_menuBuilder = new MenuBuilder(this);
  }

  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();

  m_menuBuilder->createMenu(graphicsItems);
  m_menuBuilder->menuExec(event->globalX(), event->globalY());
}

QImage te::layout::View::createImage()
{
  QImage ig;
  Scene* sc = dynamic_cast<Scene*>(scene());
  if(!sc)
    return ig;

  te::gm::Envelope env = sc->getSceneBox();

  QRectF rtv(0, 0, width(), height());
  QRectF rts(env.m_llx, env.m_lly, env.m_urx, env.m_ury);

  QImage img(rtv.width(), rtv.height(), QImage::Format_ARGB32);
  QPainter ptr(&img);
  ptr.setRenderHint(QPainter::Antialiasing);
  this->render(&ptr, QRect(), QRect(), Qt::IgnoreAspectRatio);

  return img;
}

QCursor te::layout::View::createCursor( std::string pathIcon )
{
  QIcon ico(QIcon::fromTheme(pathIcon.c_str()));

  //search icon size
  QList<QSize> sizes = ico.availableSizes();
  int maximum = sizes[0].width();
  for (int i = 1; i < sizes.size(); ++i)
  {
    maximum = qMax(maximum, sizes[i].width());
  }

  QSize sz(maximum, maximum);
  QPixmap pixmap = ico.pixmap(sz);

  QCursor cur(pixmap);

  return cur;
}

void te::layout::View::reload()
{
  m_selectionChange = false;
  emit reloadProperties();
}

void te::layout::View::resetView()
{
  Scene* scne = dynamic_cast<Scene*>(scene());

  if(!scne)
    return;

  QTransform mtrx = scne->sceneTransform();
  setTransform(mtrx);

  te::gm::Envelope box = scne->getSceneBox();
  QPointF pt(box.m_llx, box.m_ury);
  centerOn(pt);

  int zoom = getDefaultZoom();
  double zoomFactor = zoom / 100.;
  scale(zoomFactor, zoomFactor); //Initial zoom out
}

void te::layout::View::pan()
{
  //Use ScrollHand Drag Mode to enable Panning
  resetDefaultConfig();

  //The entire viewport is redrawn to avoid traces
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

  setInteractive(false);
  setDragMode(QGraphicsView::ScrollHandDrag); // Pan Mode
}

void te::layout::View::zoomArea()
{
  resetDefaultConfig();
   
  EnumToolType* tools = Enums::getInstance().getEnumToolType();

  std::string toolName = tools->getZoomAreaTool()->getName();
  ToolFactoryParamsCreate params(this);

  m_currentTool = te::layout::ToolFactory::make(toolName, params);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::zoomOut()
{
  resetDefaultConfig();

  // #Active ZoomClick (Out) Tool

  QCursor curOut = createCursor("layout-paper-zoom-out");
  m_currentTool = new ZoomClickTool(this, curOut);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::print()
{
  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();

  Scene* scne = dynamic_cast<Scene*>(scene());
  if(!scne)
  {
    return;
  }

  setCurrentMode(enumMode->getModePrinter());

  resetDefaultConfig();
  
  // No update Widget while print is running
  // Rulers aren't print
  disableUpdate();
  
  PrintScene printer(scne);
  printer.printPreview();

  enableUpdate();

  setCurrentMode(enumMode->getModeNone());
}

void te::layout::View::exportToPDF()
{
  Scene* scne = dynamic_cast<Scene*>(scene());

  resetDefaultConfig();

  // No update Widget while print is running
  // Rulers aren't print
  disableUpdate();

  PrintScene printer(scne);
  printer.exportToPDF();

  enableUpdate();
}

void te::layout::View::recompose()
{
  resetDefaultConfig();

  QRectF sceneRectV = sceneRect();

  fitZoom(sceneRectV);

  int zoom = getCurrentZoom();

  setZoom(zoom*0.95);
}

void te::layout::View::arrowCursor()
{
  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();
  ItemUtils* iUtils = Context::getInstance().getItemUtils();

  setCurrentMode(enumMode->getModeNone());
  resetDefaultConfig();
  std::vector<te::layout::MapItem*> list = iUtils->getMapItemList();
  if (!list.empty())
  {
    /*foreach(MapItem* mit, list)
    {
      mit->changeCurrentTool(mode);
    }*/
  }
}

void te::layout::View::newTemplate()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  sc->reset();
  m_visualizationArea->build();
}

void te::layout::View::fitZoom(const QRectF& rect)
{
  //clears the foreground pixmap
  m_foreground = QPixmap();

  double scaleOld = this->transform().m11();
  this->fitInView(rect, Qt::KeepAspectRatio);
  double scaleNew = this->transform().m11();

  double scaleFactor = scaleNew / scaleOld;

  int currentZoom = getCurrentZoom();
  int newZoom = (int)(currentZoom * scaleFactor);

  if(newZoom > 0)
  {
    setCurrentZoom(newZoom);

    Scene* sce = dynamic_cast<Scene*>(scene());
    if(sce)
    {
      sce->setContext(getContext());
    }

    emit zoomChanged(newZoom);
  }
}

void te::layout::View::setZoom(int newZoom)
{
  //clears the foreground pixmap
  m_foreground = QPixmap();

  int currentZoom = getCurrentZoom();

  if(newZoom == currentZoom)
    return;

  if(isLimitExceeded(newZoom) == true)
    return;

  double rescale = (double)newZoom / (double)currentZoom;

  if(rescale > 0)
  {
    setCurrentZoom(newZoom);

    applyScale(rescale);

    Scene* sce = dynamic_cast<Scene*>(scene());
    if(sce)
    {
      sce->setContext(getContext());
    }

    this->update();

    emit zoomChanged(newZoom);
  }
}

void te::layout::View::createLineItem()
{
  resetDefaultConfig();

  EnumToolType* tools = Enums::getInstance().getEnumToolType();

  std::string toolName = tools->getCreateLineItemTool()->getName();
  ToolFactoryParamsCreate params(this);

  m_currentTool = te::layout::ToolFactory::make(toolName, params);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::createPolygonItem()
{
  resetDefaultConfig();

  EnumToolType* tools = Enums::getInstance().getEnumToolType();

  std::string toolName = tools->getCreatePolygonItemTool()->getName();
  ToolFactoryParamsCreate params(this);

  m_currentTool = te::layout::ToolFactory::make(toolName, params);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::createItem(EnumType* itemType)
{
  resetDefaultConfig();

  EnumToolType* tools = Enums::getInstance().getEnumToolType();

  std::string toolName = tools->getCreateItemTool()->getName();
  ToolFactoryParamsCreate params(this, itemType);

  m_currentTool = te::layout::ToolFactory::make(toolName, params);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::applyScale(double newScale)
{
  if(newScale <= 0)
  {
    return;
  }

  scale(newScale, newScale);
}

void te::layout::View::drawForeground( QPainter * painter, const QRectF & rect )
{
  if ( !painter )
    return;

  QGraphicsView::drawForeground(painter, rect);
  if(!m_visibleRulers)
    return;

  QGraphicsScene* scene = this->scene();
  if(scene == 0)
  {
    return;
  }

  double scale = transform().m11();


  if (m_foreground.isNull())
  {
    m_foreground = QPixmap(painter->device()->width(), painter->device()->height());
    m_foreground.fill(Qt::transparent);
    QPainter painter2(&m_foreground);
    painter2.setTransform(painter->transform());

    m_horizontalRuler->drawRuler(this, &painter2, scale);
    m_verticalRuler->drawRuler(this, &painter2, scale);

    painter2.end();

    m_foreground = QPixmap::fromImage(m_foreground.toImage().mirrored());
  }

  QRect rectView(0, 0, this->width(), this->height());
  QPolygonF polygonScene = this->mapToScene(rectView);

  painter->drawPixmap(polygonScene.boundingRect(), m_foreground, m_foreground.rect());
}

bool te::layout::View::exportProperties( EnumType* type )
{
  bool is_export = false;

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), 
    QDir::currentPath(), tr("JSON Files (*.json)"));

  if(fileName.isEmpty())
  {
    return is_export;
  }

  std::string j_name = fileName.toStdString();

  Scene* scne = dynamic_cast<Scene*>(scene());
  if(!scne)
    return false;

  is_export = scne->exportPropertiesToTemplate(type, j_name);

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

bool te::layout::View::importTemplate( EnumType* type )
{  
  QString fileName = QFileDialog::getOpenFileName(this, tr("Import File"), 
    QDir::currentPath(), tr("JSON Files (*.json)"));

  if(fileName.isEmpty())
  {
    return false;
  }

  std::string j_name = fileName.toStdString();  

  Scene* scne = dynamic_cast<Scene*>(scene());
  if(!scne)
    return false;

  bool result = scne->buildTemplate(m_visualizationArea, type, j_name);
  if(result)
  {
    m_visualizationArea->build();
  }

  return result;
}

void te::layout::View::exportItemsToImage()
{
  Scene* scne = dynamic_cast<Scene*>(scene());
  if(!scne)
    return;

  QMessageBox msgBox;

  if(scne->selectedItems().empty())
  {
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Select at least one component!"); 
    msgBox.exec();
    return;
  }

  QFileDialog dialog(this);
  dialog.setGeometry(QRect(this->width()/4, this->height()/4, this->width()/2, this->height()/2));
  QString dir = dialog.getExistingDirectory(this, tr("Open Directory"), 
    QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if(dir.isEmpty())
    return;

  std::string dirName = dir.toStdString();

  scne->exportItemsToImage(dirName);

  msgBox.setIcon(QMessageBox::Information);
  msgBox.setText("Successfully exported images!"); 
  msgBox.exec();
}

void te::layout::View::onSelectionItem(std::string name)
{
  Scene* scne = dynamic_cast<Scene*>(scene());
  if(!scne)
    return;

  scne->selectItem(name);

  if (!scne->isEditionMode()) // If scene in edition mode the reload will happen in double click event
  {
    reload();
  }
}

void te::layout::View::refreshAllProperties()
{
  reload();
}

void te::layout::View::disableUpdate()
{
  // No update Widget while print is running
  setUpdatesEnabled(false);
  // Rulers aren't print
  m_visibleRulers = false;
}

void te::layout::View::enableUpdate()
{
  m_visibleRulers = true;
  setUpdatesEnabled(true);
}

te::layout::ContextObject te::layout::View::getContext()
{
  double dpiX = logicalDpiX();
  double dpiY = logicalDpiY();
  int zoom = getCurrentZoom();
  EnumType* mode = getCurrentMode();
  return ContextObject(zoom, dpiX, dpiY, mode);
}

void te::layout::View::onEditionFinalized()
{
  reload();
}


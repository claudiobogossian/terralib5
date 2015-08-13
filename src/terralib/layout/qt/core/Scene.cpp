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
  \file Scene.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Scene.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/enum/Enums.h"
#include "pattern/command/DeleteCommand.h"
#include "../item/ItemGroup.h"
#include "BuildGraphicsItem.h"
#include "pattern/command/AddCommand.h"
#include "../../core/AbstractBuildGraphicsItem.h"
#include "../../core/template/TemplateEditor.h"
#include "../../core/template/AbstractTemplate.h"
#include "../../core/property/Properties.h"
#include "VisualizationArea.h"
#include "../../../qt/widgets/Utils.h"
#include "../../core/Utils.h"
#include "../../../common/STLUtils.h"
#include "../item/MapItem.h"
#include "pattern/command/MoveCommand.h"
#include "../item/PaperItem.h"
#include "../../core/property/SharedProperties.h"
#include "../../core/PaperConfig.h"
#include "View.h"

// STL
#include <algorithm>

// Qt
#include <QUndoCommand>
#include <QUndoStack>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPrinter>
#include <QGraphicsTextItem>

te::layout::Scene::Scene( QObject* object): 
  QGraphicsScene(object),
  m_undoStack(0),
  m_align(0),
  m_moveWatched(false),
  m_paperConfig(0)
{
  m_backgroundColor = QColor(109,109,109);
  setBackgroundBrush(QBrush(m_backgroundColor));
  
  m_paperConfig = new PaperConfig;
  m_align = new AlignItems(this, m_paperConfig);
  
  m_undoStack = new QUndoStack(this);
}

te::layout::Scene::Scene( AlignItems* align, PaperConfig* paperConfig, QObject* object /*= 0*/ ) :
  QGraphicsScene(object),
  m_undoStack(0),
  m_align(align),
  m_moveWatched(false),
  m_paperConfig(paperConfig)
{

}

te::layout::Scene::~Scene()
{
  m_moveWatches.clear();
  
  if(m_undoStack)
  {
    delete m_undoStack;
    m_undoStack = 0;
  }

  foreach( QGraphicsItem *item, m_itemStackWithoutScene ) 
  {
    if(item->scene() != this)
    {
      delete item;
      item = 0;
    }
  }

  if(m_align)
  {
    delete m_align;
    m_align = 0;
  }
  
  if(m_paperConfig)
  {
    delete m_paperConfig;
    m_paperConfig = 0;
  }
}

void te::layout::Scene::insertItem( ItemObserver* item )
{
  if(!item)
  {
    return;
  }

  QGraphicsItem* qitem = ((QGraphicsItem*)item);
  
  insertItem(qitem);
}

void te::layout::Scene::insertItem( QGraphicsItem* item )
{
  if(!item)
  {
    return;
  }

  if(item->scene() == this)
  {
    return;
  }

  int total = 0;

  total = this->items().count();

  this->addItem(item);

  ItemObserver* obs = dynamic_cast<ItemObserver*>(item);
  if(!obs)
  {
    return;
  }

  if(obs->isInvertedMatrix())
  {
    QTransform transfItem = item->transform();
    // Check if the item had been inserted
    if(transfItem != m_matrix.inverted())
    {
      QTransform transf = m_matrix.inverted();
      item->setTransform(transf);
    }
  }

  item->setZValue(total);
  QGraphicsObject* qObj = dynamic_cast<QGraphicsObject*>(item);
  if(qObj)
  {
    qObj->installEventFilter(this);
  }

  obs->refresh(false);

  removeItemStackWithoutScene(item);

  emit addItemFinalized();
}

void te::layout::Scene::init( double screenWMM, double screenHMM )
{
  calculateSceneMeasures(screenWMM, screenHMM);
  
  if(!m_paperConfig)
  {
    m_paperConfig = new PaperConfig;
  }

  if(!m_align)
  {
    m_align = new AlignItems(this, m_paperConfig);
  }
}

void te::layout::Scene::calculateMatrixViewScene()
{
  double llx = m_box.m_llx;
  double lly = m_box.m_lly;
  double urx = m_box.m_urx;
  double ury = m_box.m_ury;

  ContextObject context = getContext();

  double dpiX = context.getDpiX();
  double dpiY = context.getDpiY();

  double factorX = (dpiX / 25.4);
  double factorY = (dpiY / 25.4);

  //inverted Y-Axis
  m_matrix = QTransform().scale(factorX, -factorY).translate(-llx, -ury);
  //World coordinate - mm
  setSceneRect(QRectF(QPointF(llx, lly), QPointF(urx, ury)));
}

void te::layout::Scene::calculateWindow( double wMM, double hMM )
{
  double ppSizeWMM;
  double ppSizeHMM;

  m_paperConfig->getPaperSize(ppSizeWMM, ppSizeHMM);

  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;

  double paddingX = 0;
  double paddingY = 0;

  //X-Axis
  double minX = std::min(ppSizeWMM, wMM);
  double maxX = std::max(ppSizeWMM, wMM);

  paddingX = (maxX - minX) / 2;
  x1 = - paddingX;
  x2 = maxX - paddingX;

  //Y-Axis
  double minY = std::min(ppSizeHMM, hMM);
  double maxY = std::max(ppSizeHMM, hMM);

  paddingY = (maxY - minY) / 2;
  y1 = - paddingY;
  y2 = maxY - paddingY;

  m_box.m_llx = x1;
  m_box.m_lly = y1;
  m_box.m_urx = x2;
  m_box.m_ury = y2;
}

QTransform te::layout::Scene::sceneTransform()
{
  return m_matrix;
}

void te::layout::Scene::deleteItems()
{
  std::vector<std::string> names;

  QList<QGraphicsItem*> graphicsItems = selectedItems();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    deleteItem(item);
  }

  if(!names.empty())
    emit deleteFinalized(names);
}

void te::layout::Scene::removeSelectedItems()
{
  std::vector<std::string> names;

  QList<QGraphicsItem*> graphicsItems = selectedItems();

  if(graphicsItems.empty())
    return;

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      ItemObserver* obs = dynamic_cast<ItemObserver*>(item);
      if(obs)
      {
        if(obs->getModel())
          names.push_back(obs->getModel()->getName());
      }
    }
  }

  QUndoCommand* command = new DeleteCommand(this);
  addUndoStack(command);

  if(!names.empty())
    emit deleteFinalized(names);
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

QGraphicsItemGroup* te::layout::Scene::createItemGroup( const QList<QGraphicsItem *> & items )
{
  this->clearSelection();

  //The scene create a new group with important restriction
  QGraphicsItemGroup* p = QGraphicsScene::createItemGroup(items);

  EnumModeType* mode = Enums::getInstance().getEnumModeType();

  //Create a new group
  AbstractBuildGraphicsItem* abstractBuild = Context::getInstance().getAbstractBuildGraphicsItem();
  BuildGraphicsItem* build = dynamic_cast<BuildGraphicsItem*>(abstractBuild);

  if(!build)
    return p;

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

te::layout::MovingItemGroup* te::layout::Scene::createMovingItemGroup( const QList<QGraphicsItem*>& items )
{
  //Create a new group
  AbstractBuildGraphicsItem* abstractBuild = Context::getInstance().getAbstractBuildGraphicsItem();
  BuildGraphicsItem* build = dynamic_cast<BuildGraphicsItem*>(abstractBuild);

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  QGraphicsItem* item = 0;
  
  item = build->createItem(enumObj->getMovingItemGroup());

  te::layout::MovingItemGroup* movingItem = dynamic_cast<MovingItemGroup*>(item);

  if (movingItem)
  {
    foreach(QGraphicsItem* i, items)
    {
      movingItem->addToGroup(i);
    }

    movingItem->setHandlesChildEvents(true);
  }

  return movingItem;
}

QGraphicsItem* te::layout::Scene::createItem( const te::gm::Coord2D& coord )
{
  QGraphicsItem* item = 0;

  AbstractBuildGraphicsItem* abstractBuild = Context::getInstance().getAbstractBuildGraphicsItem();
  BuildGraphicsItem* build = dynamic_cast<BuildGraphicsItem*>(abstractBuild);

  if(!build)
    return item;

  ContextObject context = getContext();

  EnumModeType* type = Enums::getInstance().getEnumModeType();
  EnumType* mode = context.getCurrentMode();

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

  changeViewMode(type->getModeNone());

  return item;
}

void te::layout::Scene::calculateSceneMeasures( double widthMM, double heightMM )
{
  calculateWindow(widthMM, heightMM);

  double w = m_box.getWidth();
  double h = m_box.getHeight();

  calculateWindow(w,h);

  calculateMatrixViewScene();
}

bool te::layout::Scene::exportPropertiesToTemplate( EnumType* type, std::string fileName )
{
  bool is_export = false;

  if(fileName.compare("") == 0)
  {
    return is_export;
  }
  
  std::vector<te::layout::Properties*> props = getItemsProperties();

  if(props.empty())
    return is_export;
  
  TemplateEditor editor(type, fileName);

  AbstractTemplate* jtemplate = editor.getTemplate();

  if(!jtemplate)
    return is_export;

  is_export = jtemplate->exportTemplate(props);

  return is_export;
}

std::vector<te::layout::Properties*> te::layout::Scene::importTemplateToProperties( EnumType* type, std::string fileName )
{
  std::vector<te::layout::Properties*> props;

  if(fileName.compare("") == 0)
  {
    return props;
  }
  
  TemplateEditor editor(type, fileName);

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

        if(lItem->getModel())
          props.push_back(lItem->getModel()->getProperties());
      }
    }
  }

  return props;
}

void te::layout::Scene::reset()
{
  if(!m_undoStack)
    return;

  if(m_undoStack->count() > 0)
    m_undoStack->clear();

  clear();
}

bool te::layout::Scene::buildTemplate( VisualizationArea* vzArea, EnumType* type, std::string fileName )
{
  AbstractBuildGraphicsItem* abstractBuild = Context::getInstance().getAbstractBuildGraphicsItem();
  BuildGraphicsItem* build = dynamic_cast<BuildGraphicsItem*>(abstractBuild);

  if(!build)
    return false;

  std::vector<te::layout::Properties*> props = importTemplateToProperties(type, fileName);

  if(props.empty())
    return false;

  reset();

  std::vector<te::layout::Properties*>::iterator it;

  te::gm::Envelope boxW = getSceneBox();
  vzArea->changeBoxArea(boxW);

  for(it = props.begin() ; it != props.end() ; ++it)
  {
    te::layout::Properties* proper = (*it);

    if(!proper)
      continue;

    build->rebuildItem(proper);
  }

  return true;
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

void te::layout::Scene::exportItemsToImage(std::string dir)
{
  Utils* utils = Context::getInstance().getUtils();

  QList<QGraphicsItem*> selected = selectedItems();
  foreach(QGraphicsItem *item, selected) 
  {
    if(item)
    {
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {
        QImage* img = 0;
        int w = 0;
        int h = 0;

        if(!it->getModel())
        {
          continue;
        }

        te::color::RGBAColor** rgba = it->getRGBAColorImage(w, h);
                
        if(!rgba)
          continue;
        
        //QRectF rect = item->boundingRect();               
        img = te::qt::widgets::GetImage(rgba, w, h);
        if(!img)
          continue;

        std::string dirName = dir + "/" + it->getModel()->getName() +".png";

        img->save(dirName.c_str());

        te::common::Free(rgba, h);

        if(img)
          delete img;        
      }
    }
  }
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
      if(its.empty())
      {
        QPointF pt = item->scenePos();
        m_moveWatches[item] = pt;
      }
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
      bool resultFound = false;
      std::map<QGraphicsItem*, QPointF>::iterator it;
      for(it = m_moveWatches.begin() ; it != m_moveWatches.end() ; ++it)
      {
        if(it->first == item)
        {
          resultFound = true;
        }
      }

      if(!resultFound)
      {
        m_moveWatches.clear();
        QPointF pt = item->scenePos();
        m_moveWatches[item] = pt;
      }

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

void te::layout::Scene::selectItem( std::string name )
{
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems) 
  {
    if(item)
    {
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {
        if(!it->getModel())
        {
          continue;
        }

        if(it->getModel()->getName().compare(name) == 0)
        {
          item->setSelected(true);
        }
        else
        {
          item->setSelected(false);
        }
      }
    }
  }
}

void te::layout::Scene::selectItem( QGraphicsItem* item )
{
  if (item)
  {
    item->setSelected(true);
  }
}

void te::layout::Scene::selectItems( std::vector<std::string> names )
{
  foreach(std::string name, names)
  {
    this->selectItem(name);
  }
}

void te::layout::Scene::selectItems( QList<QGraphicsItem*> items )
{
  foreach(QGraphicsItem* item, items)
  {
    this->selectItem(item);
  }
}

void te::layout::Scene::redrawItems()
{
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems) 
  {
    if(item)
    {
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {
        if(it->isPrintable())
        {
          it->redraw();
        }
      }
    }
  }
}

void te::layout::Scene::updateSelectedItemsPositions()
{
  QList<QGraphicsItem*> allItems = selectedItems();
  foreach(QGraphicsItem *item, allItems) 
  {
    if(item)
    {
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {
        it->refresh(); 
      }
    }
  }
}

void te::layout::Scene::onChangeZoom( int zoom )
{
  contextUpdated();
}

void te::layout::Scene::onChangeMode( EnumType* mode )
{
  contextUpdated();
}

bool te::layout::Scene::addItemStackWithoutScene( QGraphicsItem* item )
{
  if(!item)
  {
    return false;
  }

  if(item->scene() == this)
  {
    return false;
  }

  if(m_itemStackWithoutScene.contains(item))
  {
    return false;
  }

  m_itemStackWithoutScene.push_back(item);

  return true;
}

bool te::layout::Scene::removeItemStackWithoutScene( QGraphicsItem* item )
{
  if(!item)
  {
    return false;
  }

  return m_itemStackWithoutScene.removeOne(item);
}

void te::layout::Scene::deletePaperItem()
{
  QGraphicsItem* item = getPaperItem();
  if(!item)
  {
    return;
  }
  deleteItem(item);     
}

bool te::layout::Scene::deleteItem( QGraphicsItem *item )
{
  bool result = false;

  if (item)
  {
    removeItem(item);
    if(item)
    {
      delete item;
      item = 0;
      result = true;
    }
  }

  return result;
}

QGraphicsItem* te::layout::Scene::getPaperItem()
{
  QGraphicsItem* paper = 0;

  EnumObjectType* object = Enums::getInstance().getEnumObjectType();
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems)
  {
    if(item)
    {
      PaperItem* pItem = dynamic_cast<PaperItem*>(item);
      if(pItem)
      {
        paper = item;
        break;
      }
    }
  }

  return paper;
}

void te::layout::Scene::applyPaperProportion(QSize oldPaper, QSize newPaper)
{
  applyProportionAllItems(oldPaper, newPaper);
}

void te::layout::Scene::applyProportionAllItems( QSize oldPaper, QSize newPaper )
{
  QGraphicsItem* paper = getPaperItem(); 
  
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems)
  {
    if(item)
    {
      if(item != paper)
      {
        ItemObserver* it = dynamic_cast<ItemObserver*>(item);
        if(it)
        {
          te::gm::Envelope box = it->getModel()->getBox();
          box.m_llx = (box.m_llx * newPaper.width())/oldPaper.width();
          box.m_urx = (box.m_urx * newPaper.width())/oldPaper.width();
          box.m_lly = (box.m_lly * newPaper.height())/oldPaper.height();
          box.m_ury = (box.m_ury * newPaper.height())/oldPaper.height();

          ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(it->getModel());
          updateBoxFromProperties(box, model);
          item->setPos(box.m_llx, box.m_lly);
        }
      }
    }
  }
}

void te::layout::Scene::updateBoxFromProperties( te::gm::Envelope box, ItemModelObservable* model )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  SharedProperties* sharedProps = new SharedProperties;

  Properties* props = new Properties(model->getName(), model->getType(), model->getHashCode());
  
  double x1 = box.m_llx;
  double y1 = box.m_lly;
  double width = box.getWidth();
  double height = box.getHeight();

  Property pro_x1(model->getHashCode());
  pro_x1.setName(sharedProps->getX1());
  pro_x1.setValue(x1, dataType->getDataTypeDouble());
  pro_x1.setEditable(false);
  props->addProperty(pro_x1);

  Property pro_y1(model->getHashCode());
  pro_y1.setName(sharedProps->getY1());
  pro_y1.setValue(y1, dataType->getDataTypeDouble());
  pro_y1.setEditable(false);
  props->addProperty(pro_y1);

  Property pro_width(model->getHashCode());
  pro_width.setName(sharedProps->getWidth());
  pro_width.setValue(width, dataType->getDataTypeDouble());
  pro_width.setEditable(false);
  props->addProperty(pro_width);

  Property pro_height(model->getHashCode());
  pro_height.setName(sharedProps->getHeight());
  pro_height.setValue(height, dataType->getDataTypeDouble());
  pro_height.setEditable(false);
  props->addProperty(pro_height);

  model->updateProperties(props);

  if(sharedProps)
  {
    delete sharedProps;
    sharedProps = 0;
  }
}

te::layout::ContextObject te::layout::Scene::getContext()
{
  View* view = getView();
  if(!view)
  {
    ContextObject nullContext(0,0,0,0,0);
    return nullContext;
  }

  double dpiX = view->logicalDpiX();
  double dpiY = view->logicalDpiY();
  int zoom = view->getCurrentZoom();
  EnumType* mode = view->getCurrentMode();

  ContextObject context(zoom, dpiX, dpiY, m_paperConfig, mode);
  return context;
}

void te::layout::Scene::contextUpdated()
{
  ContextObject context = getContext();

  contextUpdated(context);
}

void te::layout::Scene::contextUpdated( ContextObject context )
{
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems)
  {
    if(item)
    {
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {
        it->contextUpdated(context);
      }
    }
  }
}

te::layout::AlignItems* te::layout::Scene::getAlignItems()
{
  return m_align;
}

void te::layout::Scene::changeViewMode( EnumType* mode )
{
  View* view = getView();
  if(!view)
  {
    return;
  }
  view->changeMode(mode);
}

te::layout::View* te::layout::Scene::getView()
{
  View* view = 0;
  QList<QGraphicsView*> viewList = views();

  QGraphicsView* gview = viewList.first();
  if(!gview)
  {
    return view;
  }

  view = dynamic_cast<View*>(gview);
  if(!view)
  {
    return view;
  }
  return view;
}

void te::layout::Scene::deselectAllItems()
{
  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      item->setSelected(false);
    }
  }
}

te::layout::PaperConfig* te::layout::Scene::getPaperConfig()
{
  return m_paperConfig;
}







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

// STL
#include <algorithm>

// Qt
#include <QUndoCommand>
#include <QUndoStack>
#include <QGraphicsItem>
#include "../item/MapItem.h"
#include "pattern/command/MoveCommand.h"

te::layout::Scene::Scene( QObject* object): 
  QGraphicsScene(object),
  m_undoStack(0),
  m_align(0),
  m_print(0),
  m_moveWatched(false)
{
  m_backgroundColor = QColor(109,109,109);
  setBackgroundBrush(QBrush(m_backgroundColor));
  
  m_undoStack = new QUndoStack(this);
}

te::layout::Scene::~Scene()
{
  if(m_undoStack)
  {
    delete m_undoStack;
    m_undoStack = 0;
  }

  if(m_align)
  {
    delete m_align;
    m_align = 0;
  }

  if(m_print)
  {
    delete m_print;
    m_print = 0;
  }
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
      this->addItem(qitem);
      if(obs)
      {
        if(obs->isInvertedMatrix())
        {
          QTransform transf = m_matrix.inverted();
          qitem->setTransform(transf);
        }
      }
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
      obs->refresh(false);
    }
  }

  emit addItemFinalized();
}

void te::layout::Scene::init( double screenWMM, double screenHMM )
{
  calculateSceneMeasures(screenWMM, screenHMM);

  PaperConfig* config = Context::getInstance().getPaperConfig();
  if(!config)
    return;

  if(!m_align)
  {
    m_align = new AlignItems(this, config);
  }

  if(!m_print)
  {
    m_print = new PrintScene(this, config);
  }
}

void te::layout::Scene::calculateMatrixViewScene()
{
  double llx = m_box.m_llx;
  double lly = m_box.m_lly;
  double urx = m_box.m_urx;
  double ury = m_box.m_ury;

  double dpiX = Context::getInstance().getDpiX();

  double factor = (dpiX / 25.4);

  //inverted Y-Axis
  m_matrix = QTransform().scale(factor, -factor).translate(-llx, -ury);
  //World coordinate - mm
  setSceneRect(QRectF(QPointF(llx, lly), QPointF(urx, ury)));
}

void te::layout::Scene::calculateWindow( double wMM, double hMM )
{
  double ppSizeWMM;
  double ppSizeHMM;

  PaperConfig* config = Context::getInstance().getPaperConfig();
  config->getPaperSize(ppSizeWMM, ppSizeHMM);

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
    if (item)
    {
      removeItem(item);
      if(item)
      {
        ItemObserver* obs = dynamic_cast<ItemObserver*>(item);
        if(obs)
        {
          names.push_back(obs->getName());
        }
        delete item;
        item = 0;
      }
    }
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
        names.push_back(obs->getName());
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

QGraphicsItem* te::layout::Scene::createItem( const te::gm::Coord2D& coord )
{
  QGraphicsItem* item = 0;

  AbstractBuildGraphicsItem* abstractBuild = Context::getInstance().getAbstractBuildGraphicsItem();
  BuildGraphicsItem* build = dynamic_cast<BuildGraphicsItem*>(abstractBuild);

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

void te::layout::Scene::calculateSceneMeasures( double widthMM, double heightMM )
{
  calculateWindow(widthMM, heightMM);

  double w = m_box.getWidth();
  double h = m_box.getHeight();

  calculateWindow(w,h);

  calculateMatrixViewScene();
}

te::layout::PrintScene* te::layout::Scene::getPrintScene()
{
  return m_print;
}

te::layout::AlignItems* te::layout::Scene::getAlignItems()
{
  return m_align;
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

        props.push_back(lItem->getProperties());
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
        te::color::RGBAColor** rgba = it->getImage();

        if(!rgba)
          continue;
        
        te::gm::Envelope box = utils->viewportBox(it->getModel()->getBox());
        img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
        std::string dirName = dir + "/" + it->getName() +".png";

        if(!img)
          continue;

        img->save(dirName.c_str());

        te::common::Free(rgba, box.getHeight());

        if(img)
          delete img;        
      }
    }
  }
}

bool te::layout::Scene::eventFilter( QObject * watched, QEvent * event )
{
  /*if(event->type() == QEvent::GraphicsSceneMousePress)
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
  }*/

  return QGraphicsScene::eventFilter(watched, event);
}

void te::layout::Scene::selectionItem( std::string name )
{
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems) 
  {
    if(item)
    {
      ItemObserver* it = dynamic_cast<ItemObserver*>(item);
      if(it)
      {
        if(it->getName().compare(name) == 0)
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

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
  \file MenuItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MenuBuilder.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../outside/GridSettingsModel.h"
#include "../../../outside/GridSettingsController.h"
#include "../../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../outside/GridSettingsOutside.h"
#include "../../../core/enum/Enums.h"
#include "../../../outside/TextGridSettingsModel.h"
#include "../../../outside/TextGridSettingsController.h"
#include "../../outside/TextGridSettingsOutside.h"
#include "../pattern/command/ChangePropertyCommand.h"
#include "../../../core/pattern/singleton/Context.h"
#include "../Scene.h"
#include "PropertiesUtils.h"
#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../core/pattern/mvc/AbstractItemModel.h"

//STL
#include <string>

// Qt
#include <QAction>
#include <QGraphicsItem>
#include <QImageReader>
#include <QString>
#include <QByteArray>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QMenu>
#include <QWidget>
#include <QPoint>
#include <QFontDialog>
#include <QFont>
#include <QColorDialog>
#include <QColor>
#include <QUndoCommand>
#include <QApplication>
#include <QRect>
#include <QDesktopWidget>

te::layout::MenuBuilder::MenuBuilder( QObject * parent ) :
  DialogPropertiesBrowser(0,0,parent),
  m_menu(0),
  m_propUtils(0)
{
  m_propUtils = new PropertiesUtils; 
  connect(this, SIGNAL(changeDlgProperty(Property)), this, SLOT(onChangeDlgProperty(Property)));
}

te::layout::MenuBuilder::~MenuBuilder()
{
  if(m_propUtils)
  {
    delete m_propUtils;
    m_propUtils = 0;
  }

  if(m_menu)
  {
    delete m_menu;
    m_menu = 0;
  }
}

void te::layout::MenuBuilder::createMenu( QList<QGraphicsItem*> items )
{
  m_graphicsItems = items;
  
  if(m_graphicsItems.empty())
    return;

  bool window = false;
  m_properties = m_propUtils->intersection(items, window);
  
  if(m_properties.getProperties().empty())
    return;

  if(m_menu)
  {
    delete m_menu;
    m_menu = 0;
  }

  m_menu = new QMenu();
  connect(m_menu, SIGNAL(triggered(QAction*)), this, SLOT(onMenuTriggered(QAction*)));

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  foreach(Property prop, m_properties.getProperties()) 
  {
    if(!prop.isMenu() || !prop.isVisible())
      continue;

    std::string label = prop.getLabel();
    if(label.compare("") == 0)
      label = prop.getName();

    QAction* action = createAction(label, prop.getName(), prop.getIcon());
    m_menu->addAction(action);
    if(prop.getType() == dataType->getDataTypeBool())
    {
      action->setCheckable(true);
      action->setChecked(prop.getValue().toBool());
    }
  }
}

QAction* te::layout::MenuBuilder::createAction( std::string text, std::string objName, std::string icon, std::string tooltip )
{
  QAction *actionMenu = new QAction(text.c_str(), this);
  actionMenu->setObjectName(objName.c_str());

  actionMenu->setIcon(QIcon::fromTheme(icon.c_str()));
  actionMenu->setToolTip(tooltip.c_str());

  return actionMenu;
}

void te::layout::MenuBuilder::onMenuTriggered( QAction* action )
{
  if(!m_menu)
    return;

  m_currentPropertyClicked = findMnuProperty(action->objectName().toStdString());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(m_currentPropertyClicked.getType() == dataType->getDataTypeColor())
  {
    onShowColorDlg();
  }
  else if(m_currentPropertyClicked.getType() == dataType->getDataTypeFont())
  {
    onShowFontDlg();
  }
  else if(m_currentPropertyClicked.getType() == dataType->getDataTypeGridSettings())
  {
    onShowGridSettingsDlg();
  }
  else if(m_currentPropertyClicked.getType() == dataType->getDataTypeImage())
  {
    onShowImageDlg();
  }
  if(m_currentPropertyClicked.getType() == dataType->getDataTypeMapChoice())
  {
    onShowMapLayerChoiceDlg();
  }
  if(m_currentPropertyClicked.getType() == dataType->getDataTypeLegendChoice())
  {
    onShowLegendChoiceDlg();
  }
  else if(m_currentPropertyClicked.getType() == dataType->getDataTypeBool())
  {
    checkedBool(action->isChecked());
  }
  else if(m_currentPropertyClicked.getType() == dataType->getDataTypeTextGridSettings())
  {
    onShowTextGridSettingsDlg();
  }
  else if(m_currentPropertyClicked.getType() == dataType->getDataTypeSVGView())
  {
    onShowViewDlg();
  }
}

void te::layout::MenuBuilder::checkedBool( bool checked )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property property = m_currentPropertyClicked;

  if(property.getType() != dataType->getDataTypeBool())
    return;

  property.setValue(checked, dataType->getDataTypeBool());
  changePropertyValue(property);
}

void te::layout::MenuBuilder::onChangeDlgProperty( Property property )
{
  changePropertyValue(property);
}

void te::layout::MenuBuilder::changePropertyValue( Property property )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() == dataType->getDataTypeNone())
    return;

  Scene* lScene = dynamic_cast<Scene*>(Context::getInstance().getScene()); 

  std::vector<QGraphicsItem*> commandItems;
  std::vector<Properties> commandOld;
  std::vector<Properties> commandNew;

  foreach(QGraphicsItem* item, m_graphicsItems) 
  {
    if (item)
    {      
      AbstractItemView* lItem = dynamic_cast<AbstractItemView*>(item);
      if(lItem)
      {
        if(!lItem->getController()->getModel())
        {
          continue;
        }

        Properties beforeProps = lItem->getController()->getModel()->getProperties();
        
        Properties props("");
        props.setObjectName(beforeProps.getObjectName());
        props.setTypeObj(beforeProps.getTypeObj());
        props.setHashCode(beforeProps.getHashCode());
        props.addProperty(property);

        lItem->getController()->getModel()->setProperties(props);

        Properties afterProps = lItem->getController()->getModel()->getProperties();
        commandItems.push_back(item);
        commandOld.push_back(beforeProps);
        commandNew.push_back(afterProps);
        
      }
    }
  }

  if(!m_graphicsItems.isEmpty())
  {
    QUndoCommand* command = new ChangePropertyCommand(commandItems, commandOld, commandNew);
    lScene->addUndoStack(command);
  }
  lScene->update();
}

te::layout::Property te::layout::MenuBuilder::findMnuProperty( te::layout::EnumType* dataType )
{
  Property prop;

  std::map<std::string, Property>::iterator it;

  foreach( Property pro, m_properties.getProperties()) 
  {
    if(pro.getType() == dataType)
    {
      prop = pro;
      break;
    }
  }

  return prop;
}

te::layout::Property te::layout::MenuBuilder::findMnuProperty( std::string name )
{
  Property prop;

  std::map<std::string, Property>::iterator it;

  foreach( Property pro, m_properties.getProperties()) 
  {
    if(pro.getName().compare(name) == 0)
    {
      prop = pro;
      break;
    }
  }

  return prop;
}

void te::layout::MenuBuilder::menuExec( int x /*= 0*/, int y /*= 0*/ )
{ 
  if(!m_menu)
    return;

  QPoint pt(x, y);
  m_menu->exec(pt);
}







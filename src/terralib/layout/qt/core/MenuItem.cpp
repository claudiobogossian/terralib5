/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
#include "MenuItem.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../outside/GridSettingsModel.h"
#include "../../outside/GridSettingsController.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../outside/GridSettingsOutside.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/enum/Enums.h"

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
#include "../../outside/TextGridSettingsModel.h"
#include "../../outside/TextGridSettingsController.h"
#include "../outside/TextGridSettingsOutside.h"

te::layout::MenuItem::MenuItem( QObject * parent ) :
	QObject(parent),
  m_menu(0), 
  m_gridSettings(0),
  m_textGridSettings(0)
{

}

te::layout::MenuItem::~MenuItem()
{
  if(m_gridSettings)
  {
    m_gridSettings->close();
    delete m_gridSettings;
    m_gridSettings = 0;
  }

  if(m_textGridSettings)
  {
    m_textGridSettings->close();
    delete m_textGridSettings;
    m_textGridSettings = 0;
  }

  if(m_menu)
  {
    delete m_menu;
    m_menu = 0;
  }
}

void te::layout::MenuItem::createMenu( QList<QGraphicsItem*> items )
{
  m_graphicsItems = items;
  
  if(m_graphicsItems.empty())
    return;

  bool window = false;
  m_properties = intersection(items, window);
  
  if(!m_properties)
    return;

  if(m_menu)
  {
    delete m_menu;
    m_menu = 0;
  }

  m_menu = new QMenu();
  connect(m_menu, SIGNAL(triggered(QAction*)), this, SLOT(onMenuTriggered(QAction*)));

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  foreach(Property prop, m_properties->getProperties()) 
  {
    if(!prop.isMenu())
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

QAction* te::layout::MenuItem::createAction( std::string text, std::string objName, std::string icon, std::string tooltip )
{
  QAction *actionMenu = new QAction(text.c_str(), this);
  actionMenu->setObjectName(objName.c_str());

  actionMenu->setIcon(QIcon::fromTheme(icon.c_str()));
  actionMenu->setToolTip(tooltip.c_str());

  return actionMenu;
}

te::layout::Properties* te::layout::MenuItem::intersection( QList<QGraphicsItem*> graphicsItems, bool& window )
{
  Properties* props = 0;

  if(graphicsItems.size() == 1)
  {
    QGraphicsItem* item = graphicsItems.first();
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        props = const_cast<Properties*>(lItem->getProperties());
        window = props->hasWindows();
      }
    }
  }
  else
  {
    props = sameProperties(graphicsItems, window);
  }

  return props;
}

te::layout::Properties* te::layout::MenuItem::sameProperties( QList<QGraphicsItem*> graphicsItems, bool& window )
{
  Properties* props = 0;
  std::vector<Properties*> propsVec = getAllProperties(graphicsItems, window);

  QGraphicsItem* firstItem = graphicsItems.first();
  ItemObserver* lItem = dynamic_cast<ItemObserver*>(firstItem);

  if(!lItem)
  {
    return props;
  }

  Properties* firstProps = const_cast<Properties*>(lItem->getProperties());
  if(!firstProps)
  {
    return props;
  }

  std::vector<Properties*>::iterator it = propsVec.begin();
  std::vector<Properties*>::iterator itend = propsVec.end();
  bool result = false;
  foreach( Property prop, firstProps->getProperties()) 
  {
    contains(itend, it, prop.getName(), result);
    if(result)
    {
      if(!props)
      {
        props = new Properties("");
      }
      props->addProperty(prop);
    }
  }  

  return props;
}

void te::layout::MenuItem::contains( std::vector<Properties*>::iterator itend, std::vector<Properties*>::iterator it, std::string name, bool& result )
{
  Property prop = (*it)->contains(name);
  if(prop.isNull())
  {
    result = false;
    return;
  }
  else
  {
    ++it;
    result = true;
    if(it != itend)
    {
      contains(itend, it, name, result);
    }
  }
}

std::vector<te::layout::Properties*> te::layout::MenuItem::getAllProperties( QList<QGraphicsItem*> graphicsItems, bool& window )
{
  Properties* props = 0;
  std::vector<Properties*> propsVec;
  bool result = true;

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        Properties* propsItem = const_cast<Properties*>(lItem->getProperties());
        if(propsItem)
        {
          propsVec.push_back(propsItem);
          if(result)
          {
            result = propsItem->hasWindows();
          }
        }
      }
    }
  }

  window = result;
  return propsVec;
}

void te::layout::MenuItem::onMenuTriggered( QAction* action )
{
  if(!m_menu)
    return;

  Property prop = findProperty(action->objectName().toStdString());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(prop.getType() == dataType->getDataTypeColor())
  {
    showColorDlg(prop);
  }
  else if(prop.getType() == dataType->getDataTypeFont())
  {
    showFontDlg(prop);
  }
  else if(prop.getType() == dataType->getDataTypeGridSettings())
  {
    showGridSettingsDlg(prop);
  }
  else if(prop.getType() == dataType->getDataTypeImage())
  {
    showImageDlg(prop);
  }
  else if(prop.getType() == dataType->getDataTypeBool())
  {
    checkedBool(prop, action->isChecked());
  }
  else if(prop.getType() == dataType->getDataTypeTextGridSettings())
  {
    showTextGridSettingsDlg(prop);
  }
}

void te::layout::MenuItem::showGridSettingsDlg(Property property)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() != dataType->getDataTypeGridSettings())
    return;

  if(!m_gridSettings)
  {
    GridSettingsModel* model = new GridSettingsModel;
    model->setOutsideProperty(property);
    GridSettingsController* controller = new GridSettingsController(model);

    Observer* obs = const_cast<Observer*>(controller->getView());
    OutsideObserver* observer = dynamic_cast<OutsideObserver*>(obs);
    m_gridSettings = dynamic_cast<GridSettingsOutside*>(observer);
    connect(m_gridSettings, SIGNAL(updateProperty()), this, SLOT(onUpdateGridSettingsProperty()));
  }

  if(m_gridSettings)
  {
    if(property.getType() == dataType->getDataTypeGridSettings())
    {
      Observable* obs = dynamic_cast<Observable*>(m_gridSettings->getModel());
      if(obs)
      {
        GridSettingsModel* modelObs = dynamic_cast<GridSettingsModel*>(obs);
        if(modelObs)
        {
          modelObs->setOutsideProperty(property);
        }

        ItemController* itCtrl = dynamic_cast<ItemController*>(m_gridSettings->getController());
        if(itCtrl)
        {
          GridSettingsController* gridController = dynamic_cast<GridSettingsController*>(itCtrl);
          if(gridController)
          {
            gridController->clearUpdate();
          }
        }
      }

      m_gridSettings->load();
      m_gridSettings->show();
    }
  }
}

void te::layout::MenuItem::onUpdateGridSettingsProperty()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_gridSettings->getController());
  if(controller)
  {
    Property prop = controller->updateProperty();

    if(prop.isNull())
      return;

    changePropertyValue(prop);
  }
}

void te::layout::MenuItem::showImageDlg(Property property)
{
  QWidget* wdg = dynamic_cast<QWidget*>(parent());

  if(!wdg)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() != dataType->getDataTypeImage())
    return;

  // Bulding the filter string
  QString filter = tr("Images") + " ( ";
  QList<QByteArray> formats = QImageReader::supportedImageFormats();
  for(int i = 0; i < formats.size() - 1; ++i)
    filter += "*." + formats[i] + " ";
  filter += ")";

  QString path = QFileDialog::getOpenFileName(wdg, tr("Select an Image File"), "", filter);
  if(path.isNull())
    return;

  // Try load image
  QImage img;
  if(!img.load(path))
  {
    QMessageBox::critical(wdg, tr("Error"), tr("The selected image cannot be loaded."));
    return;
  }
  else
  {
    property.setValue(path.toStdString(), dataType->getDataTypeImage());
    
    changePropertyValue(property);
  }
}

void te::layout::MenuItem::showFontDlg( Property property )
{
  QWidget* wdg = dynamic_cast<QWidget*>(parent());

  if(!wdg)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() != dataType->getDataTypeFont())
    return;

  bool ok = false;
  Font font;
  QFont qFont;

  font = property.getValue().toFont();
  qFont.setFamily(font.getFamily().c_str());
  qFont.setPointSize(font.getPointSize());
  qFont.setBold(font.isBold());
  qFont.setItalic(font.isItalic());
  qFont.setUnderline(font.isUnderline());
  qFont.setStrikeOut(font.isStrikeout());
  qFont.setKerning(font.isKerning());

  QFont newFont = QFontDialog::getFont(&ok, qFont, wdg, tr("Select Font"));

  if (!ok || newFont == qFont) 
    return;

  font.setFamily(newFont.family().toStdString());
  font.setPointSize(newFont.pointSize());
  font.setBold(newFont.bold());
  font.setItalic(newFont.italic());
  font.setUnderline(newFont.underline());
  font.setStrikeout(newFont.strikeOut());
  font.setKerning(newFont.kerning());
  property.setValue(font, dataType->getDataTypeFont());
    
  changePropertyValue(property);
}

void te::layout::MenuItem::showColorDlg( Property property )
{
  QWidget* wdg = dynamic_cast<QWidget*>(parent());

  if(!wdg)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() != dataType->getDataTypeColor())
    return;

  bool ok = false;
  QColor qcolor;
  te::color::RGBAColor color;

  color = property.getValue().toColor();
  qcolor.setRed(color.getRed());
  qcolor.setGreen(color.getGreen());
  qcolor.setBlue(color.getBlue());
  qcolor.setAlpha(color.getAlpha());

  QRgb oldRgba = qcolor.rgba();

  QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, wdg);

  if (!ok || newRgba == oldRgba)
    return;

  qcolor = QColor::fromRgba(newRgba);

  if(qcolor.isValid()) 
  {
    color.setColor(qcolor.red(), qcolor.green(), qcolor.blue(), qcolor.alpha());
    property.setValue(color, dataType->getDataTypeColor());

    changePropertyValue(property);
  }
}

void te::layout::MenuItem::checkedBool( Property property, bool checked )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() != dataType->getDataTypeBool())
    return;

  property.setValue(checked, dataType->getDataTypeBool());
  changePropertyValue(property);
}

void te::layout::MenuItem::showTextGridSettingsDlg( Property property )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() != dataType->getDataTypeTextGridSettings())
    return;

  if(!m_textGridSettings)
  {
    TextGridSettingsModel* model = new TextGridSettingsModel;
    model->setOutsideProperty(property);
    TextGridSettingsController* controller = new TextGridSettingsController(model);

    Observer* obs = const_cast<Observer*>(controller->getView());
    OutsideObserver* observer = dynamic_cast<OutsideObserver*>(obs);
    m_textGridSettings = dynamic_cast<TextGridSettingsOutside*>(observer);
    connect(m_textGridSettings, SIGNAL(updateProperty()), this, SLOT(onUpdateTextGridSettingsProperty()));
  }

  if(m_textGridSettings)
  {
    if(property.getType() == dataType->getDataTypeTextGridSettings())
    {
      Observable* obs = dynamic_cast<Observable*>(m_textGridSettings->getModel());
      if(obs)
      {
        TextGridSettingsModel* modelObs = dynamic_cast<TextGridSettingsModel*>(obs);
        if(modelObs)
        {
          modelObs->setOutsideProperty(property);
        }

        ItemController* itCtrl = dynamic_cast<ItemController*>(m_textGridSettings->getController());
        if(itCtrl)
        {
          TextGridSettingsController* gridController = dynamic_cast<TextGridSettingsController*>(itCtrl);
          if(gridController)
          {
            gridController->clearUpdate();
          }
        }
      }

      m_textGridSettings->load();
      m_textGridSettings->show();
    }
  }
}

void te::layout::MenuItem::onUpdateTextGridSettingsProperty()
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_textGridSettings->getController());
  if(controller)
  {
    Property prop = controller->updateProperty();

    if(prop.isNull())
      return;

    changePropertyValue(prop);
  }
}

void te::layout::MenuItem::changePropertyValue( Property property )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() == dataType->getDataTypeNone())
    return;

  foreach( QGraphicsItem* item, m_graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        Properties* props = new Properties("");
        if(props)
        {
          props->setObjectName(lItem->getProperties()->getObjectName());
          props->setTypeObj(lItem->getProperties()->getTypeObj());
          props->addProperty(property);

          lItem->updateProperties(props);
          delete props;
          props = 0;
        }       
      }
    }
  }
}

te::layout::Property te::layout::MenuItem::findProperty( te::layout::EnumType* dataType )
{
  Property prop;

  std::map<std::string, Property>::iterator it;

  foreach( Property pro, m_properties->getProperties()) 
  {
    if(pro.getType() == dataType)
    {
      prop = pro;
      break;
    }
  }

  return prop;
}

te::layout::Property te::layout::MenuItem::findProperty( std::string name )
{
  Property prop;

  std::map<std::string, Property>::iterator it;

  foreach( Property pro, m_properties->getProperties()) 
  {
    if(pro.getName().compare(name) == 0)
    {
      prop = pro;
      break;
    }
  }

  return prop;
}

void te::layout::MenuItem::menuExec( int x /*= 0*/, int y /*= 0*/ )
{ 
  if(!m_menu)
    return;

  QPoint pt(x, y);
  m_menu->exec(pt);
}

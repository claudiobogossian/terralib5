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
  \file DialogPropertiesBrowser.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DialogPropertiesBrowser.h"
#include "../../../core/property/Properties.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/Font.h"
#include "../../../core/AbstractBuildGraphicsOutside.h"
#include "../../../core/pattern/singleton/Context.h"
#include "../BuildGraphicsOutside.h"
#include "../../outside/GridSettingsOutside.h"
#include "../../../outside/GridSettingsModel.h"
#include "../ItemUtils.h"
#include "../../outside/MapLayerChoiceOutside.h"
#include "../../../outside/MapLayerChoiceModel.h"
#include "../../outside/LegendChoiceOutside.h"
#include "../../../outside/LegendChoiceModel.h"
#include "../../../core/pattern/proxy/AbstractProxyProject.h"
#include "../../item/MapItem.h"
#include "../../../item/MapModel.h"
#include "../../outside/ColorDialogOutside.h"
#include "../../../outside/ColorDialogModel.h"
#include "../../outside/SVGDialogOutside.h"
#include "../../../outside/SVGDialogModel.h"

// STL
#include <vector>

// Qt
#include <QVariant>
#include <QFont>
#include <QColor>
#include <QImageReader>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QApplication>
#include <QByteArray>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtStringPropertyManager>
#include <QtPropertyBrowser/qteditorfactory.h>
#include <QtPropertyBrowser/QtProperty>

te::layout::DialogPropertiesBrowser::DialogPropertiesBrowser(QObject* parent) :
  AbstractPropertiesBrowser(parent),
  m_strDlgManager(0),
  m_dlgEditorFactory(0)
{
  createManager();
}

te::layout::DialogPropertiesBrowser::DialogPropertiesBrowser( QtDlgEditorFactory* factory, QtStringPropertyManager* manager, QObject *parent /*= 0*/ ):
  AbstractPropertiesBrowser(parent),
  m_strDlgManager(manager),
  m_dlgEditorFactory(factory)
{
  if(m_dlgEditorFactory)
  {
    connect(m_dlgEditorFactory, SIGNAL(internalDlg(QWidget *, QtProperty *)), this, SLOT(onSetDlg(QWidget *, QtProperty *)));
  }
}

te::layout::DialogPropertiesBrowser::~DialogPropertiesBrowser()
{
  closeAllWindows();

  m_dlgProps.clear();

  if(m_strDlgManager)
  {
    delete m_strDlgManager;
    m_strDlgManager = 0;
  }

  if(m_dlgEditorFactory)
  {
    delete m_dlgEditorFactory;
    m_dlgEditorFactory = 0;
  }
}

void te::layout::DialogPropertiesBrowser::createManager()
{
  m_strDlgManager = new QtStringPropertyManager;
  
  m_dlgEditorFactory = new QtDlgEditorFactory;

  // internalDlg is called when an item of property browser tree is clicked
  connect(m_dlgEditorFactory, SIGNAL(internalDlg(QWidget *, QtProperty *)), this, SLOT(onSetDlg(QWidget *, QtProperty *)));
}

void te::layout::DialogPropertiesBrowser::onSetDlg( QWidget *parent, QtProperty * prop )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType || !parent)
  {
    return;
  }

  std::string name = prop->propertyName().toStdString();

  Property propt = findDlgProperty(name);

  m_currentPropertyClicked = propt;

  if(propt.getType() == dataType->getDataTypeNone())
    return;

  if(propt.getType() == dataType->getDataTypeGridSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowGridSettingsDlg()));
  }
  if(propt.getType() == dataType->getDataTypeImage())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowImageDlg()));
  }
  if(propt.getType() == dataType->getDataTypeTextGridSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowTextGridSettingsDlg()));
  }
  if(propt.getType() == dataType->getDataTypeMapChoice())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowMapLayerChoiceDlg()));
  }
  if(propt.getType() == dataType->getDataTypeLegendChoice())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowLegendChoiceDlg()));
  }
  if(propt.getType() == dataType->getDataTypeSVGView())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowViewDlg()));
  }
  if(propt.getType() == dataType->getDataTypeColor())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowColorDlg()));
  }
  if(propt.getType() == dataType->getDataTypeFont())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowFontDlg()));
  }
}

QtStringPropertyManager* te::layout::DialogPropertiesBrowser::getStringPropertyManager()
{
  return m_strDlgManager;
}

QtDlgEditorFactory* te::layout::DialogPropertiesBrowser::getDlgEditorFactory()
{
  return m_dlgEditorFactory;
}

bool te::layout::DialogPropertiesBrowser::changeQtPropertyValue(QtProperty* pproperty, Property property)
{
  QColor qcolor;
  QFont qfont;
  te::color::RGBAColor color;
  Font font;

  m_changeProperty = true;

  if(!pproperty)
  {
    return false;
  }
    
  m_changeProperty = false;

  return true;
}

QtProperty* te::layout::DialogPropertiesBrowser::addProperty( Property property )
{
  QtProperty* qproperty = 0;

  if(!property.isVisible())
  {
    return qproperty;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  if(!dataType)
  {
    return qproperty;
  }

  if(!m_strDlgManager)
  {
    return qproperty;
  }

  qproperty = m_strDlgManager->addProperty(tr(property.getName().c_str()));
  m_strDlgManager->setValue(qproperty, property.getValue().toString().c_str());
  addPropertyItem(qproperty, QLatin1String(property.getName().c_str()));
  /*The sub properties should not appear in this case, 
    because will be previewed in the dialog window will be opened.*/
  m_dlgProps.insert(std::pair<std::string, Property>(property.getName(),property));

  return qproperty;
}

bool te::layout::DialogPropertiesBrowser::checkDlgType( Property prop )
{
  bool result = false;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(prop.getType() == dataType->getDataTypeGridSettings()
    || prop.getType() == dataType->getDataTypeImage()
    || prop.getType() == dataType->getDataTypeTextGridSettings())
  {
    result = true;
  }

  return result;
}

te::layout::Property te::layout::DialogPropertiesBrowser::findDlgProperty( std::string name )
{
  Property prop;

  std::string propName;
  std::map<std::string, Property>::iterator it;

  for (it = m_dlgProps.begin(); it != m_dlgProps.end(); ++it) {
    propName = it->first;
    if(name.compare(propName) == 0)
    {
      prop = it->second;
      break;
    }
  }

  return prop;
}

te::layout::Property te::layout::DialogPropertiesBrowser::findDlgProperty( EnumType* dataType )
{
  Property prop;

  Property pro;
  std::map<std::string, Property>::iterator it;

  for (it = m_dlgProps.begin(); it != m_dlgProps.end(); ++it) {
    pro = it->second;
    if(pro.getType() == dataType)
    {
      prop = pro;
      break;
    }
  }

  return prop;
}

void te::layout::DialogPropertiesBrowser::changeValueQtPropertyDlg( std::string name, QVariant variant )
{
  if(!m_strDlgManager)
  {
    return;
  }

  QSet<QtProperty*> list = m_strDlgManager->properties();
  foreach( QtProperty* prop, list) 
  {
    if(prop->propertyName().toStdString().compare(name))
    {
      m_strDlgManager->setValue(prop, variant.toString());
    }
  }
}

void te::layout::DialogPropertiesBrowser::onShowGridSettingsDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if(!enumObj)
  {
    return;
  }
  
  QWidget* widget = createOutside(enumObj->getGridSettings());
  if(!widget)
  {
    return;
  }

  GridSettingsOutside* gridSettings = dynamic_cast<GridSettingsOutside*>(widget);
  if(!gridSettings)
  {
    return;
  }

  appendDialog(gridSettings);

  GridSettingsModel* model = dynamic_cast<GridSettingsModel*>(gridSettings->getModel());
  if(!model)
  {
    return;
  }

  ItemUtils* utils = Context::getInstance().getItemUtils();
  if(!utils)
  {
    return;
  }

  std::vector<te::layout::Properties*> props = utils->getGridMapProperties();

  model->setProperties(props);
  
  gridSettings->load();
  gridSettings->show(); // modeless dialog
  gridSettings->raise(); // top of the parent widget's stack
}

void te::layout::DialogPropertiesBrowser::onShowImageDlg()
{
  // Bulding the filter string
  QString filter = tr("Images") + " ( ";
  QList<QByteArray> formats = QImageReader::supportedImageFormats();
  for(int i = 0; i < formats.size() - 1; ++i)
    filter += "*." + formats[i] + " ";
  filter += ")";

  QDesktopWidget* wg = QApplication::desktop();
  QFileDialog dialog;

  //Put the dialog in the screen center
  QRect screen = wg->screenGeometry();
  dialog.move( screen.center() - dialog.rect().center() );

  QString path = dialog.getOpenFileName(wg, tr("Select an Image File"), "", filter);
  if(path.isNull())
    return;

  // Try load image
  QImage img;
  if(!img.load(path))
  {
    QMessageBox::critical(wg, tr("Error"), tr("The selected image cannot be loaded."));
    return;
  }
  else
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property prop = m_currentPropertyClicked;
    prop.setValue(path.toStdString(), dataType->getDataTypeImage());

    QVariant v(path);
    changeValueQtPropertyDlg(prop.getName(), v);

    emit changeDlgProperty(prop);
  }
}

void te::layout::DialogPropertiesBrowser::onShowTextGridSettingsDlg()
{
  
}

void te::layout::DialogPropertiesBrowser::onShowFontDlg()
{
  QWidget* wdg = dynamic_cast<QWidget*>(parent());

  if(!wdg)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property property = m_currentPropertyClicked;

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

  QFontDialog dialog;
  
  //Put the dialog in the screen center
  QRect screen = wdg->geometry();
  dialog.move( screen.center() - dialog.rect().center() );

  QFont newFont = dialog.getFont(&ok, qFont, wdg, tr("Select Font"));

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

  emit changeDlgProperty(property);
}

void te::layout::DialogPropertiesBrowser::onShowColorDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if(!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getColorDialog());
  if(!widget)
  {
    return;
  }

  ColorDialogOutside* colorDialog = dynamic_cast<ColorDialogOutside*>(widget);
  if(!colorDialog)
  {
    return;
  }

  appendDialog(colorDialog);

  ColorDialogModel* model = dynamic_cast<ColorDialogModel*>(colorDialog->getModel());
  if(!model)
  {
    return;
  }
    
  model->setColorProperty(m_currentPropertyClicked);

  colorDialog->init();
  colorDialog->show(); // modeless dialog
  colorDialog->raise(); // top of the parent widget's stack
  colorDialog->activateWindow(); // visible top-level window that has the keyboard input focus
}

void te::layout::DialogPropertiesBrowser::onShowMapLayerChoiceDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if(!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getMapLayerChoice());
  if(!widget)
  {
    return;
  }

  MapLayerChoiceOutside* layerChoice = dynamic_cast<MapLayerChoiceOutside*>(widget);
  if(!layerChoice)
  {
    return;
  }
  
  appendDialog(layerChoice);

  MapLayerChoiceModel* model = dynamic_cast<MapLayerChoiceModel*>(layerChoice->getModel());
  if(!model)
  {
    return;
  }

  AbstractProxyProject* proxy = Context::getInstance().getProxyProject();
  if(!proxy)
  {
    return;
  }

  std::list<te::map::AbstractLayerPtr> listLayers = proxy->getAllLayers();
  model->setLayers(listLayers);

  ItemUtils* utils = Context::getInstance().getItemUtils();

  std::vector<MapItem*> mapList = utils->getMapItemList(true);
  
  std::vector<Properties*> props;

  std::vector<MapItem*>::const_iterator it = mapList.begin();
  for( ; it != mapList.end() ; ++it)
  {
    MapItem* mIt = (*it);
    MapModel* mapModel = dynamic_cast<MapModel*>(mIt->getModel());
    props.push_back(mapModel->getProperties());
  }

  model->setPropertiesMaps(props);

  layerChoice->init();
  layerChoice->show();
}

void te::layout::DialogPropertiesBrowser::onShowLegendChoiceDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if(!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getGridSettings());
  if(!widget)
  {
    return;
  }

  LegendChoiceOutside* legendChoice = dynamic_cast<LegendChoiceOutside*>(widget);
  if(!legendChoice)
  {
    return;
  }

  appendDialog(legendChoice);

  LegendChoiceModel* model = dynamic_cast<LegendChoiceModel*>(legendChoice->getModel());
  if(!model)
  {
    return;
  }
  
  legendChoice->show();
}

void te::layout::DialogPropertiesBrowser::onShowViewDlg()
{
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if(!enumObj)
  {
    return;
  }

  QWidget* widget = createOutside(enumObj->getSVGDialog());
  if(!widget)
  {
    return;
  }

  SVGDialogOutside* svgOutside = dynamic_cast<SVGDialogOutside*>(widget);
  if(!svgOutside)
  {
    return;
  }

  appendDialog(svgOutside);

  SVGDialogModel* model = dynamic_cast<SVGDialogModel*>(svgOutside->getModel());
  if(!model)
  {
    return;
  }

  AbstractProxyProject* proxy = Context::getInstance().getProxyProject();
  if(!proxy)
  {
    return;
  }

  model->setPathsProperty(m_currentPropertyClicked);
  
  svgOutside->init();
  svgOutside->show();
}

te::layout::Property te::layout::DialogPropertiesBrowser::getProperty( std::string name )
{
  Property prop;
  prop.setName(name);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
  {
    return prop;
  }
  
  QVariant variant = findPropertyValue(name);

  if(variant.isNull() || !variant.isValid())
  {
    return prop;
  }

  if(prop.getType() == dataType->getDataTypeGridSettings())
  {
    prop.setValue(variant.toString().toStdString(), prop.getType());
  }
  else if(prop.getType() == dataType->getDataTypeImage())
  {
    prop.setValue(variant.toString().toStdString(), prop.getType());
  }

  return prop;
}

te::layout::EnumType* te::layout::DialogPropertiesBrowser::getLayoutType( QVariant::Type type, std::string name /*= ""*/ )
{
  Property prop;
  EnumDataType* dtType = Enums::getInstance().getEnumDataType();
  EnumType* dataType = 0;

  if(!dtType)
  {
    return 0;
  }
  
  switch(type)
  {
  case QVariant::String:
    {
      dataType = dtType->getDataTypeString();

      //Custom types: Dialog Window Type
      if(name.compare("") != 0)
      {
        prop = findDlgProperty(name);
        if(!prop.getValue().isNull())
        {
          if(prop.getType() == dtType->getDataTypeGridSettings())
          {
            dataType = dtType->getDataTypeGridSettings();
          }
          if(prop.getType() == dtType->getDataTypeImage())
          {
            dataType = dtType->getDataTypeImage();
          }
        }
      }
    }
    break;
  default:
    prop.setValue(0, dtType->getDataTypeNone());
  }

  return dataType;
}

int te::layout::DialogPropertiesBrowser::getVariantType( EnumType* dataType )
{
  int type = QVariant::Invalid;

  EnumDataType* dtType = Enums::getInstance().getEnumDataType();

  if(!dtType)
  {
    return type;
  }
  
  if(dataType == dtType->getDataTypeGridSettings())
  {
    type = QVariant::String;
  }
  else if(dataType == dtType->getDataTypeImage())
  {
    type = QVariant::String;
  }
  else
  {
    type = QVariant::Invalid;
  }

  return type;
}

bool te::layout::DialogPropertiesBrowser::updateProperty( Property property )
{
  std::string name = property.getName();
  QtProperty* qprop = findProperty(name);
  QColor qcolor;
  te::color::RGBAColor color;
  Font font;
  QFont qfont;
  
  if(!qprop)
  {
    return false;
  }  

  return changeQtPropertyValue(qprop, property);
}

std::map<std::string, te::layout::Property> te::layout::DialogPropertiesBrowser::getDlgProps()
{
  return m_dlgProps;
}

void te::layout::DialogPropertiesBrowser::closeAllWindows()
{
  if(m_dialogs.empty())
    return;

  foreach(QWidget* widget, m_dialogs)
  {
    if(m_dialogs.removeAll(widget))
    {
      widget->close();
      delete widget;
      widget = 0;
    }
  }
}

QWidget* te::layout::DialogPropertiesBrowser::createOutside( EnumType* enumType )
{
  QWidget* widget = 0;

  if(!enumType)
  {
    return widget;
  }

  AbstractBuildGraphicsOutside* abstractBuild = Context::getInstance().getAbstractBuildGraphicsOutside();
  if(!abstractBuild)
  {
    return widget;
  }

  BuildGraphicsOutside* build = dynamic_cast<BuildGraphicsOutside*>(abstractBuild);
  if(!build)
  {
    return widget;
  }

  widget = build->createOuside(enumType);
  return widget;
}

void te::layout::DialogPropertiesBrowser::updateOutside( Property prop )
{
  emit changeDlgProperty(prop);
}

void te::layout::DialogPropertiesBrowser::updateOutside( std::vector<Property> props )
{
  emit changeDlgProperty(props);
}

void te::layout::DialogPropertiesBrowser::onDestroyed( QObject* obj )
{
  if(m_dialogs.empty())
    return;

  foreach(QWidget* widget, m_dialogs)
  {
    if(widget == obj)
    {
      m_dialogs.removeAll(widget);
    }
  }
}

void te::layout::DialogPropertiesBrowser::clearAll()
{
  AbstractPropertiesBrowser::clearAll();
  closeAllWindows();
}

void te::layout::DialogPropertiesBrowser::appendDialog( QWidget* widget )
{
  m_dialogs.append(widget);

  connect(widget, SIGNAL(updateProperty(Property)), this, SLOT(updateOutside(Property)));
  connect(widget, SIGNAL(destroyed( QObject *)), this, SLOT(onDestroyed(QObject*)));

  widget->setAttribute(Qt::WA_DeleteOnClose);
}



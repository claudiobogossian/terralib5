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
  \file BuildGraphicsOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildGraphicsOutside.h"
#include "../../core/enum/Enums.h"
#include "../../outside/GridSettingsModel.h"
#include "../../outside/GridSettingsController.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../outside/GridSettingsOutside.h"
#include "../../outside/ToolbarModel.h"
#include "../../outside/ToolbarController.h"
#include "../outside/ToolbarOutside.h"
#include "../../outside/PropertiesModel.h"
#include "../../outside/PropertiesController.h"
#include "../outside/PropertiesOutside.h"
#include "../../outside/ObjectInspectorModel.h"
#include "../../outside/ObjectInspectorController.h"
#include "../outside/ObjectInspectorOutside.h"
#include "../../outside/PageSetupModel.h"
#include "../../outside/PageSetupController.h"
#include "../outside/PageSetupOutside.h"
#include "../../outside/SystematicScaleModel.h"
#include "../../outside/SystematicScaleController.h"
#include "../outside/SystematicScaleOutside.h"
#include "../../outside/EditTemplateModel.h"
#include "../../outside/EditTemplateController.h"
#include "../outside/EditTemplateOutside.h"
#include "../../outside/TextGridSettingsModel.h"
#include "../../outside/TextGridSettingsController.h"
#include "../outside/TextGridSettingsOutside.h"
#include "../outside/MapLayerChoiceOutside.h"
#include "../../outside/MapLayerChoiceModel.h"
#include "../../outside/MapLayerChoiceController.h"
#include "../../outside/ColorDialogModel.h"
#include "../../outside/ColorDialogController.h"
#include "../outside/ColorDialogOutside.h"
#include "../../outside/FontDialogModel.h"
#include "../../outside/FontDialogController.h"
#include "../outside/FontDialogOutside.h"
#include "../../outside/LegendChoiceModel.h"
#include "../../outside/LegendChoiceController.h"
#include "../outside/LegendChoiceOutside.h"

// Qt
#include <QWidget>

// STL
#include <sstream>
#include <string>  

te::layout::BuildGraphicsOutside::BuildGraphicsOutside() 
{
 
}

te::layout::BuildGraphicsOutside::~BuildGraphicsOutside()
{
 
}

QWidget* te::layout::BuildGraphicsOutside::createOuside( te::layout::EnumType* type )
{
  QWidget* widget = 0;

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  if (type == enumObj->getGridSettings())
  {
    widget = createGridSettings();
  }
  if (type == enumObj->getToolbar())
  {
    widget = createToolbar();
  }
  if (type == enumObj->getPropertiesWindow())
  {
    widget = createProperties();
  }
  if (type == enumObj->getObjectInspectorWindow())
  {
    widget = createObjectInspector();
  }
  if (type == enumObj->getPageSetup())
  {
    widget = createPageSetup();
  }
  if (type == enumObj->getSystematicScale())
  {
    widget = createSystematicScale();
  }
  if (type == enumObj->getEditTemplate())
  {
    widget = createEditTemplate();
  }
  if (type == enumObj->getTextGridSettings())
  {
    widget = createTextGridSettingss();
  }
  if (type == enumObj->getMapLayerChoice())
  {
    widget = createMapLayerChoice();
  }
  if (type == enumObj->getColorDialog())
  {
    widget = createColorDialog();
  }
  if (type == enumObj->getFontDialog())
  {
    widget = createFontDialog();
  }
  if (type == enumObj->getLegendChoice())
  {
    widget = createLegendChoice();
  }
  
  return widget;
}

QWidget* te::layout::BuildGraphicsOutside::createGridSettings()
{
  GridSettingsModel* model = new GridSettingsModel();

  GridSettingsController* controller = new GridSettingsController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  GridSettingsOutside* view = dynamic_cast<GridSettingsOutside*>(itemObs);
  
  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createToolbar()
{
  ToolbarModel* model = new ToolbarModel();

  ToolbarController* controller = new ToolbarController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  ToolbarOutside* view = dynamic_cast<ToolbarOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createProperties()
{
  PropertiesModel* model = new PropertiesModel();

  PropertiesController* controller = new PropertiesController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  PropertiesOutside* view = dynamic_cast<PropertiesOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createObjectInspector()
{
  ObjectInspectorModel* model = new ObjectInspectorModel();

  ObjectInspectorController* controller = new ObjectInspectorController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  ObjectInspectorOutside* view = dynamic_cast<ObjectInspectorOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createPageSetup()
{
  PageSetupModel* model = new PageSetupModel();

  PageSetupController* controller = new PageSetupController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  PageSetupOutside* view = dynamic_cast<PageSetupOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createSystematicScale()
{
  SystematicScaleModel* model = new SystematicScaleModel();

  SystematicScaleController* controller = new SystematicScaleController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  SystematicScaleOutside* view = dynamic_cast<SystematicScaleOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createEditTemplate()
{
  EditTemplateModel* model = new EditTemplateModel();

  EditTemplateController* controller = new EditTemplateController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  EditTemplateOutside* view = dynamic_cast<EditTemplateOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createTextGridSettingss()
{
  TextGridSettingsModel* model = new TextGridSettingsModel();

  TextGridSettingsController* controller = new TextGridSettingsController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  TextGridSettingsOutside* view = dynamic_cast<TextGridSettingsOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createMapLayerChoice()
{
  MapLayerChoiceModel* model = new MapLayerChoiceModel();

  MapLayerChoiceController* controller = new MapLayerChoiceController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  MapLayerChoiceOutside* view = dynamic_cast<MapLayerChoiceOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createColorDialog()
{
  ColorDialogModel* model = new ColorDialogModel();

  ColorDialogController* controller = new ColorDialogController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  ColorDialogOutside* view = dynamic_cast<ColorDialogOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createFontDialog()
{
  FontDialogModel* model = new FontDialogModel();

  FontDialogController* controller = new FontDialogController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  FontDialogOutside* view = dynamic_cast<FontDialogOutside*>(itemObs);

  return view;
}

QWidget* te::layout::BuildGraphicsOutside::createLegendChoice()
{
  LegendChoiceModel* model = new LegendChoiceModel();

  LegendChoiceController* controller = new LegendChoiceController(model);
  OutsideObserver* itemObs = (OutsideObserver*)controller->getView();

  LegendChoiceOutside* view = dynamic_cast<LegendChoiceOutside*>(itemObs);

  return view;
}




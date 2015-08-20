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
  \file OutsideFactory.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "OutsideFactory.h"
#include "../../../../../core/enum/AbstractType.h"
#include "../../../../../core/pattern/factory/OutsideParamsCreate.h"
#include "../../../../outside/ToolbarOutside.h"
#include "../../../../outside/PropertiesOutside.h"
#include "../../../../outside/ObjectInspectorOutside.h"
#include "../../../../outside/GridSettingsOutside.h"
#include "../../../../outside/PageSetupOutside.h"
#include "../../../../outside/SystematicScaleOutside.h"
#include "../../../../outside/EditTemplateOutside.h"
#include "../../../../outside/TextGridSettingsOutside.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../outside/MapLayerChoiceOutside.h"
#include "../../../../outside/LegendChoiceOutside.h"
#include "../../../../outside/SVGDialogOutside.h"
#include "../../../../outside/ColorDialogOutside.h"
#include "../../../../outside/FontDialogOutside.h"

te::layout::OutsideFactory::OutsideFactory()
{

}

te::layout::OutsideFactory::~OutsideFactory()
{

}

te::layout::AbstractOutsideView* te::layout::OutsideFactory::make( EnumType* type, OutsideParamsCreate params /*= LayoutItemParamsCreate()*/ )
{
  AbstractOutsideView* outside = 0;

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  if(type == enumObj->getToolbar())
  {
    ToolbarOutside* toolbar = new ToolbarOutside(params.getController());    
    outside = toolbar;
  }
  else if(type == enumObj->getPropertiesWindow())
  {
    PropertiesOutside* window = new PropertiesOutside(params.getController());    
    outside = window;
  }
  else if(type == enumObj->getObjectInspectorWindow())
  {
    ObjectInspectorOutside* inspector = new ObjectInspectorOutside(params.getController());    
    outside = inspector;
  }
  else if(type == enumObj->getGridSettings())
  {
    GridSettingsOutside* gridSettings = new GridSettingsOutside(params.getController());    
    outside = gridSettings;
  }
  else if(type == enumObj->getPageSetup())
  {
    PageSetupOutside* pageSetup = new PageSetupOutside(params.getController());    
    outside = pageSetup;
  }
  else if(type == enumObj->getSystematicScale())
  {
    SystematicScaleOutside* systematic = new SystematicScaleOutside(params.getController());    
    outside = systematic;
  }
  else if(type == enumObj->getSVGDialog())
  {
    SVGDialogOutside* svgdialog = new SVGDialogOutside(params.getController());    
    outside = svgdialog;
  }
  else if(type == enumObj->getEditTemplate())
  {
    EditTemplateOutside* editTemplate = new EditTemplateOutside(params.getController());    
    outside = editTemplate;
  }
  else if(type == enumObj->getTextGridSettings())
  {
    TextGridSettingsOutside* textGridSettings = new TextGridSettingsOutside(params.getController());    
    outside = textGridSettings;
  }
  else if(type == enumObj->getMapLayerChoice())
  {
    MapLayerChoiceOutside* mapLayerChoice = new MapLayerChoiceOutside(params.getController());    
    outside = mapLayerChoice;
  }
  else if(type == enumObj->getLegendChoice())
  {
    LegendChoiceOutside* legendChoice = new LegendChoiceOutside(params.getController());    
    outside = legendChoice;
  }
  else if(type == enumObj->getColorDialog())
  {
    ColorDialogOutside* colorDialog = new ColorDialogOutside(params.getController());    
    outside = colorDialog;
  }
  else if(type == enumObj->getFontDialog())
  {
    FontDialogOutside* fontDialog = new FontDialogOutside(params.getController());    
    outside = fontDialog;
  }

  return outside;
}

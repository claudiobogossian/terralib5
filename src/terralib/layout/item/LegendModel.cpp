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
  \file LegendModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"
#include "../core/property/SharedProperties.h"


//#include "LegendModel.h"
//#include "MapModel.h"
//#include "../core/property/Property.h"
//#include "../core/property/Properties.h"
//#include "../core/property/SharedProperties.h"
//#include "../../maptools/CanvasConfigurer.h"
//#include "../../se/Symbolizer.h"
//#include "../../se/Style.h"
//#include "../../se/Rule.h"
//#include "../../geometry/Polygon.h"
//#include "../../geometry/Geometry.h"
//#include "../../geometry/Envelope.h"
//#include "../core/enum/Enums.h"
//#include "../../maptools/GroupingItem.h"
//#include "../../maptools/Enums.h"

// STL
#include <string>
#include <sstream> 



te::layout::LegendModel::LegendModel()
  : AbstractItemModel()
  , Observer()
  /*m_mapName(""),
  m_layer(0),
  m_borderDisplacement(1),
  m_displacementBetweenSymbols(7),
  m_displacementBetweenTitleAndSymbols(7),
  m_displacementBetweenSymbolsAndText(2),
  m_symbolsize(5)*/
{
  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getLegendItem());

  std::string name = "";
  std::string mapName = "";
  Font font;
  font.setPointSize(8);
  te::color::RGBAColor fontColor(0, 0, 0, 255);
  std::list<te::map::AbstractLayerPtr> layerList;
  double borderDisplacement(1);
  double displacementBetweenSymbols(7);
  double displacementBetweenTitleAndSymbols(7);
  double displacementBetweenSymbolsAndText(2);
  double symbolsize(5);

  double width = 70.;
  double height = 50.;

  SharedProperties sharedProps;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    Property property(0);
    property.setName("legendChoice");
    property.setValue(name, dataType->getDataTypeLegendChoice());
    property.setMenu(true);
    m_properties.addProperty(property);
  }
  {
    GenericVariant gv;
    gv.setList(layerList, dataType->getDataTypeLayerList());

    Property property;
    property.setName("layers");
    property.setValue(gv, dataType->getDataTypeGenericVariant());
    property.setEditable(false);
    property.setVisible(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName(sharedProps.getMapName());
    property.setValue(mapName, dataType->getDataTypeStringList());
    Variant v;
    v.setValue(mapName, dataType->getDataTypeString());
    property.addOption(v);
  
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("font");
    property.setValue(font, dataType->getDataTypeFont());
    property.setMenu(true);
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("font_color");
    property.setValue(fontColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  //updating properties
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("show_frame");
    property.setValue(true, dataType->getDataTypeBool());
    m_properties.updateProperty(property);
  }

}

te::layout::LegendModel::~LegendModel()
{

}

void te::layout::LegendModel::update(const Subject* subject)
{
  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if(subjectModel == 0)
  {
    return;
  }

  const Property& pLayersNew = subjectModel->getProperty("layers");
  const Property& pLayersCurrent = this->getProperty("layers");

  const std::list<te::map::AbstractLayerPtr>& layersNew = pLayersNew.getValue().toGenericVariant().toLayerList();
  const std::list<te::map::AbstractLayerPtr>& layersCurrent = pLayersCurrent.getValue().toGenericVariant().toLayerList();

  if(layersNew != layersCurrent)
  {
    setProperty(pLayersNew);
  }
}


//void te::layout::LegendModel::draw( ContextItem context )
//{
//  te::map::Canvas* canvas = context.getCanvas();
//  notifyAll(context);
//}
//
//te::layout::Properties* te::layout::LegendModel::getProperties() const
//{
//  ItemModelObservable::getProperties();
//
//  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
//
//  Property pro_legendchoice(m_hashCode);
//  pro_legendchoice.setName("legendChoice");
//  pro_legendchoice.setValue(m_name, dataType->getDataTypeLegendChoice());
//  pro_legendchoice.setMenu(true);
//  m_properties->addProperty(pro_legendchoice);
//
//  Property pro_mapName(m_hashCode);
//  pro_mapName.setName(m_sharedProps->getMapName());
//  pro_mapName.setValue(m_mapName, dataType->getDataTypeStringList());
//  Variant v;
//  v.setValue(m_mapName, dataType->getDataTypeString());
//  pro_mapName.addOption(v);
//  
//  m_properties->addProperty(pro_mapName);
//
//  Property pro_font(m_hashCode);
//  pro_font.setName("Font");
//  pro_font.setValue(m_font, dataType->getDataTypeFont());
//  pro_font.setMenu(true);
//  m_properties->addProperty(pro_font);
//
//  Property pro_fontcolor(m_hashCode);
//  pro_fontcolor.setName("font_color");
//  pro_fontcolor.setValue(m_fontColor, dataType->getDataTypeColor());
//  pro_fontcolor.setMenu(true);
//  m_properties->addProperty(pro_fontcolor);
//
//  return m_properties;
//}
//
//void te::layout::LegendModel::updateProperties( te::layout::Properties* properties, bool notify )
//{
//  ItemModelObservable::updateProperties(properties, false);
//
//  Properties* vectorProps = const_cast<Properties*>(properties);
//
//  Property pro_mapName = vectorProps->getProperty(m_sharedProps->getMapName());
//
//  if(!pro_mapName.isNull())
//  {
//    m_mapName = pro_mapName.getOptionByCurrentChoice().toString();
//  }
//
//  Property pro_font = vectorProps->getProperty("Font");
//
//  if(!pro_font.isNull())
//  {
//    m_font = pro_font.getValue().toFont();
//  }
//
//  Property pro_fontColor = vectorProps->getProperty("font_color");
//
//  if(!pro_fontColor.isNull())
//  {
//    m_fontColor = pro_fontColor.getValue().toColor();
//  }
//
//  if(notify)
//  {
//    ContextItem context;
//    notifyAll(context);
//  }
//}
//
//void te::layout::LegendModel::visitDependent(ContextItem context)
//{
//  MapModel* map = dynamic_cast<MapModel*>(m_visitable);
//
//  if(map)
//  {
//    if(!map->isLoadedLayer())
//    {
//      return;
//    }
//
//    std::list<te::map::AbstractLayerPtr> layerListMap = map->getLayers();
//    std::list<te::map::AbstractLayerPtr>::iterator it;
//    it = layerListMap.begin();
//
//    te::map::AbstractLayerPtr layer = (*it);
//
//    m_layer = layer;
//    
//    draw(context);
//  }  
//}
//
//void te::layout::LegendModel::setBorderDisplacement( double value )
//{
//  m_borderDisplacement = value;
//}
//
//double te::layout::LegendModel::getBorderDisplacement()
//{
//  return m_borderDisplacement;
//}
//
//void te::layout::LegendModel::setDisplacementBetweenSymbols( double value )
//{
//  m_displacementBetweenSymbols = value;
//}
//
//double te::layout::LegendModel::getDisplacementBetweenSymbols()
//{
//  return m_displacementBetweenSymbols;
//}
//
//void te::layout::LegendModel::setDisplacementBetweenTitleAndSymbols( double value )
//{
//  m_displacementBetweenTitleAndSymbols = value;
//}
//
//double te::layout::LegendModel::getDisplacementBetweenTitleAndSymbols()
//{
//  return m_displacementBetweenTitleAndSymbols;
//}
//
//void te::layout::LegendModel::setDisplacementBetweenSymbolAndText( double value )
//{
//  m_displacementBetweenSymbolsAndText = value;
//}
//
//double te::layout::LegendModel::getDisplacementBetweenSymbolAndText()
//{
//  return m_displacementBetweenSymbolsAndText;
//}
//
//te::map::AbstractLayerPtr te::layout::LegendModel::getLayer()
//{
//  return m_layer;
//}
//
//te::layout::Font te::layout::LegendModel::getFont()
//{
//  return m_font;
//}
//
//te::color::RGBAColor te::layout::LegendModel::getFontColor()
//{
//  return m_fontColor;
//}
//
//double te::layout::LegendModel::getSymbolSize()
//{
//  return m_symbolsize;
//}
//



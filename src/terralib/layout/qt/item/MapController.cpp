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

// TerraLib
#include "MapController.h"

#include "MapItem.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "terralib/maptools/MapDisplay.h"


te::layout::MapController::MapController(AbstractItemModel* model)
  : AbstractItemController(model)
  , m_zoom(0)
  , m_ignoreExtentChangedEvent(false)
{

}

te::layout::MapController::~MapController()
{

}

void te::layout::MapController::update(const te::layout::Subject* subject)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    AbstractItemController::update(subject);
    return;
  }

  const Property& pLayers = getProperty("layers");
  const Property& pSrid = getProperty("srid");
  const Property& pWorldBox = getProperty("world_box");

  const std::list<te::map::AbstractLayerPtr>& layerList = pLayers.getValue().toGenericVariant().toLayerList();
  int srid = pSrid.getValue().toInt();
  te::gm::Envelope envelope = pWorldBox.getValue().toEnvelope();

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

  bool doRefresh = false;
  mapDisplay->setLayerList(layerList);
  mapDisplay->refresh(); //this refresh need to be done in order to correctly initialize the mapDisplay. We should review this later

  if(mapDisplay->getSRID() != srid)
  {
    mapDisplay->setSRID(srid, false);
    doRefresh = true;
  }

  if(mapDisplay->getExtent().equals(envelope) == false)
  {
    mapDisplay->setExtent(envelope, false);
    doRefresh = true;
  }

  if (doRefresh)
  {
    view->doRefresh();
  }
  

  AbstractItemController::update(subject);
}

void te::layout::MapController::addLayers(const std::list<te::map::AbstractLayerPtr>& layerList)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  GenericVariant gv;
  gv.setList(layerList, dataType->getDataTypeLayerList());

  Property property;
  property.setName("layers");
  property.setValue(gv, dataType->getDataTypeGenericVariant());

  m_model->setProperty(property);
}

void te::layout::MapController::extentChanged(const te::gm::Envelope& envelope, double scale)
{
  if(m_ignoreExtentChangedEvent == true)
  {
    return;
  }

  Properties properties = getExtentChangedProperties(envelope, scale);
  if(properties.getProperties().empty() == false)
  {
    AbstractItemController::setProperties(properties);
  }
}

void te::layout::MapController::setZoom( const int& zoom )
{
  m_zoom = zoom;
}

void te::layout::MapController::setProperty(const te::layout::Property& property)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    AbstractItemController::setProperty(property);
    return;
  }

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();
  bool extentChanged = false;

  if(property.getName() == "scale")
  {
    double newScale = property.getValue().toDouble();
    if(syncScaleToItem(newScale) == false)
    {
      return;
    }
    extentChanged = true;
  }
  else if(property.getName() == "world_box")
  {
    te::gm::Envelope newExtent = property.getValue().toEnvelope();
    if(syncExtentToItem(newExtent) == false)
    {
      return;
    }
    extentChanged = true;
  }

  if(extentChanged == true)
  {
    Properties extentChangedProperties = getExtentChangedProperties(mapDisplay->getExtent(), mapDisplay->getScale());
    AbstractItemController::setProperties(extentChangedProperties);

    view->doRefresh();
  }
  else
  {
    AbstractItemController::setProperty(property);
  }
}

void te::layout::MapController::setProperties(const te::layout::Properties& properties)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    AbstractItemController::setProperties(properties);
    return;
  }

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();
  bool extentChanged = false;

  Properties propertiesCopy;
  const std::vector<Property>& vecProperties = properties.getProperties();
  for(size_t i = 0; i < vecProperties.size(); ++i)
  {
    const Property& property = vecProperties[i];
    if(property.getName() == "scale")
    {
      double newScale = property.getValue().toDouble();
      if(syncScaleToItem(newScale) == true)
      {
        extentChanged = true;
      }
    }
    else if(property.getName() == "world_box")
    {
      te::gm::Envelope newExtent = property.getValue().toEnvelope();
      if(syncExtentToItem(newExtent) == true)
      {
        extentChanged = true;
      }
    }
    else
    {
      propertiesCopy.addProperty(property);
    }
  }

  if(extentChanged == true)
  {
    Properties extentChangedProperties = getExtentChangedProperties(mapDisplay->getExtent(), mapDisplay->getScale());
    for(size_t j = 0; j < extentChangedProperties.getProperties().size(); ++j)
    {
      propertiesCopy.addProperty(extentChangedProperties.getProperties()[j]);
    }
  }

  AbstractItemController::setProperties(propertiesCopy);

  if(extentChanged == true)
  {
    view->doRefresh();
  }
}

te::layout::Properties te::layout::MapController::getExtentChangedProperties(const te::gm::Envelope& envelope, double scale)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pWorldBox = getProperty("world_box");
  const Property& pScale = getProperty("scale");

  te::gm::Envelope currentEnvelope = pWorldBox.getValue().toEnvelope();
  double currentScale = pScale.getValue().toDouble();

  Properties properties("");
  if(envelope.equals(currentEnvelope) == false)
  {
    Property property;
    property.setName("world_box");
    property.setValue(envelope, dataType->getDataTypeEnvelope());
    properties.addProperty(property);
  }

  if(scale != currentScale)
  {
    Property property;
    property.setName("scale");
    scale = scale * (m_zoom / 100.);
    property.setValue(scale, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  return properties;
}

bool te::layout::MapController::syncScaleToItem(double scale)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    return false;
  }
  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

  scale = scale / (m_zoom / 100.); //adjusting scale to the zoom factor

  double currentScale = mapDisplay->getScale();
  if(currentScale != scale)
  {
    m_ignoreExtentChangedEvent = true;
    mapDisplay->setScale(scale);
    m_ignoreExtentChangedEvent = false;

    return true;
  }

  return false;
}

bool te::layout::MapController::syncExtentToItem(const te::gm::Envelope& extent)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    return false;
  }

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

  const te::gm::Envelope& currentExtent = mapDisplay->getExtent();

  if(extent.equals(currentExtent) == false)
  {
    te::gm::Envelope extentCopy = extent;

    m_ignoreExtentChangedEvent = true;
    mapDisplay->setExtent(extentCopy, false);
    m_ignoreExtentChangedEvent = false;

    return true;
  }

  return false;
}



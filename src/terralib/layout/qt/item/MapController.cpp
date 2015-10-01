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

#include <set>

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

void te::layout::MapController::addLayers(const std::list<te::map::AbstractLayerPtr>& layerList)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  GenericVariant gv;
  gv.setList(layerList, dataType->getDataTypeLayerList());

  Property property;
  property.setName("layers");
  property.setValue(gv, dataType->getDataTypeGenericVariant());

  setProperty(property);
}

void te::layout::MapController::extentChanged(const te::gm::Envelope& envelope, double scale, int srid)
{
  if(m_ignoreExtentChangedEvent == true)
  {
    return;
  }

  Properties properties = getExtentChangedProperties(envelope, scale, srid);
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
  bool wasSync = false;

  if(isMapDisplayProperty(property) == true)
  {
    if(syncMapDisplayProperty(property) == true)
    {
      wasSync = true;
    }
  }

  if(wasSync == true)
  {
    Properties extentChangedProperties = getExtentChangedProperties(mapDisplay->getExtent(), mapDisplay->getScale(), mapDisplay->getSRID());
    if (property.getName() == "layers")
    {
      extentChangedProperties.addProperty(property);
    }

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
  bool wasSync = false;

  Properties propertiesCopy;
  const std::vector<Property>& vecProperties = properties.getProperties();
  for(size_t i = 0; i < vecProperties.size(); ++i)
  {
    const Property& property = vecProperties[i];
    if(isMapDisplayProperty(property) == true)
    {
      if(syncMapDisplayProperty(property) == true)
      {
        wasSync = true;

        if (property.getName() == "layers")
        {
          propertiesCopy.addProperty(property);
        }

      }
    }
    else
    {
      propertiesCopy.addProperty(property);
    }
  }

  if(wasSync == true)
  {
    Properties extentChangedProperties = getExtentChangedProperties(mapDisplay->getExtent(), mapDisplay->getScale(), mapDisplay->getSRID());
    for(size_t j = 0; j < extentChangedProperties.getProperties().size(); ++j)
    {
      propertiesCopy.addProperty(extentChangedProperties.getProperties()[j]);
    }
  }

  AbstractItemController::setProperties(propertiesCopy);

  if(wasSync == true)
  {
    view->doRefresh();
  }
}

bool te::layout::MapController::isMapDisplayProperty(const te::layout::Property& property)
{
  std::set<std::string> setMapDisplayProperty;
  setMapDisplayProperty.insert("world_box");
  setMapDisplayProperty.insert("scale");
  setMapDisplayProperty.insert("srid");
  setMapDisplayProperty.insert("layers");

  if(setMapDisplayProperty.find(property.getName()) != setMapDisplayProperty.end())
  {
    return true;
  }

  return false;
}

bool te::layout::MapController::syncMapDisplayProperty(const te::layout::Property& property)
{
  if(property.getName() == "scale")
  {
    double newScale = property.getValue().toDouble();
    if(syncScaleToItem(newScale) == true)
    {
      return true;
    }
  }
  else if(property.getName() == "world_box")
  {
    te::gm::Envelope newExtent = property.getValue().toEnvelope();
    if(syncExtentToItem(newExtent) == true)
    {
      return true;
    }
  }
  else if(property.getName() == "srid")
  {
    int newSRID = property.getValue().toInt();
    if(syncSridToItem(newSRID) == true)
    {
      return true;
    }
  }
  else if(property.getName() == "layers")
  {
    const GenericVariant& gv = property.getValue().toGenericVariant();
    const std::list<te::map::AbstractLayerPtr>& newLayerList = gv.toLayerList();

    if(syncLayersToItem(newLayerList) == true)
    {
      return true;
    }
  }

  return false;
}

te::layout::Properties te::layout::MapController::getExtentChangedProperties(const te::gm::Envelope& envelope, double scale, int srid)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pWorldBox = getProperty("world_box");
  const Property& pScale = getProperty("scale");
  const Property& pSrid = getProperty("srid");

  te::gm::Envelope currentEnvelope = pWorldBox.getValue().toEnvelope();
  double currentScale = pScale.getValue().toDouble();
  int currentSrid =  pSrid.getValue().toInt();

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

  if(srid != currentSrid)
  {
    Property property;
    property.setName("srid");
    property.setValue(srid, dataType->getDataTypeInt());
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

bool te::layout::MapController::syncSridToItem(int srid)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    return false;
  }

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

  int currentSrid = mapDisplay->getSRID();

  if(srid != currentSrid)
  {
    m_ignoreExtentChangedEvent = true;
    mapDisplay->setSRID(srid, false);
    m_ignoreExtentChangedEvent = false;

    return true;
  }

  return false;
}

bool te::layout::MapController::syncLayersToItem(const std::list<te::map::AbstractLayerPtr>& layerList)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    return false;
  }

  //we read the current layer list from the model because mapDisplay does not have the getLayers function
  const Property& property = this->getProperty("layers");
  const GenericVariant& gv = property.getValue().toGenericVariant();
  const std::list<te::map::AbstractLayerPtr>& currentLayerList = gv.toLayerList();

  if(currentLayerList != layerList)
  {
    te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

   //checks the intersection between the layers list in the model and the new layer list to be set
    std::list<te::map::AbstractLayerPtr> intersectionList;

    std::list<te::map::AbstractLayerPtr>::const_iterator itCurrent = currentLayerList.begin();
    while (itCurrent != currentLayerList.end())
    {
      std::list<te::map::AbstractLayerPtr>::const_iterator itNew = layerList.begin();
      while (itNew != layerList.end())
      {
        if (itCurrent->get()->getId() == itNew->get()->getId())
        {
          intersectionList.push_back(itCurrent->get());
          break;
        }
        ++itNew;
      }
      ++itCurrent;
    }

    //If the old layer list has no intersection to the new, we need to also define the srid e the envelope
    if(intersectionList.empty() == true)
    {
      te::gm::Envelope envelope;
      int srid = -1;

      std::list<te::map::AbstractLayerPtr>::const_iterator it = layerList.begin();
      while(it != layerList.end())
      {
        te::map::AbstractLayerPtr layer = (*it);

        te::gm::Envelope currentEnvelope = layer->getExtent();

        int currentSrid = layer->getSRID();
        if(srid <= 0)
          srid = currentSrid;
        if(currentSrid != srid)
          currentEnvelope.transform(currentSrid, srid);

        envelope.Union(currentEnvelope);

        ++it;
      }

      //this refresh need to be done in order to correctly initialize the mapDisplay. We should review this later
      m_ignoreExtentChangedEvent = true;
      mapDisplay->setLayerList(layerList);
      mapDisplay->setSRID(srid, false);
      mapDisplay->refresh();
      mapDisplay->setExtent(envelope, false);
      m_ignoreExtentChangedEvent = false;
    }
    else
    {
      //If there is an intersection, we just update the list and keep all other properties enchanged
      m_ignoreExtentChangedEvent = true;
      mapDisplay->setLayerList(layerList);
      m_ignoreExtentChangedEvent = false;
    }

    return true;
  }

  return false;
}



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
  const Property& pScale = getProperty("scale");

  const std::list<te::map::AbstractLayerPtr>& layerList = pLayers.getValue().toGenericVariant().toLayerList();
  int srid = pSrid.getValue().toInt();
  te::gm::Envelope envelope = pWorldBox.getValue().toEnvelope();

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

  bool doRefresh = false;
  mapDisplay->setLayerList(layerList);
  if(mapDisplay->getSRID() != srid)
  {
    mapDisplay->setSRID(srid);
    doRefresh = true;
  }

  if(mapDisplay->getExtent().equals(envelope) == false)
  {
    mapDisplay->setExtent(envelope);
    doRefresh = true;
  }

  double scale = pScale.getValue().toDouble();
  scale = scale / (m_zoom / 100.);
  if (scale != 0 && mapDisplay->getScale() != scale)
  {
    mapDisplay->setScale(scale);
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
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pWorldBox = getProperty("world_box");
  te::gm::Envelope currentEnvelope = pWorldBox.getValue().toEnvelope();

  if(envelope.equals(currentEnvelope) == false)
  {
    Properties properties("");

    {
      Property property;
      property.setName("world_box");
      property.setValue(envelope, dataType->getDataTypeEnvelope());
      properties.addProperty(property);
    }
    {
      Property property;
      property.setName("scale");
      scale = scale * (m_zoom / 100.);
      property.setValue(scale, dataType->getDataTypeDouble());
      properties.addProperty(property);
    }
    
    m_model->setProperties(properties);
  }
}

void te::layout::MapController::setZoom( const int& zoom )
{
  m_zoom = zoom;
}

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
  \file terralib/edit/qt/Utils.cpp
   
  \brief Utility Qt functions for TerraLib Edit module.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/ObjectId.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/Utils.h"
#include "../../srs/Config.h"
#include "../Utils.h"
#include "Utils.h"

// Qt
#include <QColor>
#include <QUndoCommand>

// STL
#include <cassert>

//removing
#include "../../common/Exception.h"
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/ObjectId.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleData.h"
#include "../../edit/Feature.h"
#include "../../edit/Repository.h"
#include "../../edit/RepositoryManager.h"
#include "../../edit/qt/core/UndoStackManager.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/GeometryCollection.h"
#include "../../maptools/DataSetLayer.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../../qt/widgets/canvas/MapDisplay.h"
#include "../../qt/af/events/MapEvents.h"

//#include "../af/ApplicationController.h"
//#include "../af/events/LayerEvents.h"
//#include "../af/events/MapEvents.h"

// QT
#include <QMessageBox>

QPointF te::edit::GetPosition(QMouseEvent* e)
{
#if QT_VERSION >= 0x050000
  return e->localPos();
#else
  return e->posF();
#endif
}

te::gm::Geometry* te::edit::convertGeomType(const te::map::AbstractLayerPtr& layer, te::gm::Geometry* geom)
{
  te::gm::Geometry* geomConvert = 0;

  // Get the geometry type of layer
  std::auto_ptr<te::da::DataSetType> dt = layer->getSchema();
  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dt.get());

  switch (geomProp->getGeometryType())
  {
    case te::gm::MultiPolygonType:
    {
      te::gm::MultiPolygon* mp = 0;

      if (geom->getGeomTypeId() == te::gm::MultiPolygonType)
        mp = dynamic_cast<te::gm::MultiPolygon*>(geom);
      else
      {
        mp = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType);
        mp->add(geom);
      }

      geomConvert = mp;

      break;
    }
    case te::gm::PolygonType:
    {
      te::gm::Polygon* p = dynamic_cast<te::gm::Polygon*>(geom);

      geomConvert = p;

      break;
    }
    //others type
    //MultiLine
    //...
  }

  //projection
  if (geomConvert->getSRID() == layer->getSRID())
    return geomConvert;

  //else, need conversion...
  geomConvert->transform(layer->getSRID());

  return geomConvert;

}

void te::edit::saveGeometries(te::map::AbstractLayerPtr layer)
{
  try
  {
    te::edit::Repository* repo = te::edit::RepositoryManager::getInstance().getRepository(layer->getId());

    // Get the data souce info
    te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(layer.get()->getDataSourceId());

    // For while, use DataSetLayer to get the DataSource
    te::map::DataSetLayer* dslayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());

    te::da::DataSourcePtr dsource = te::da::GetDataSource(dslayer->getDataSourceId(), true);

    // Get the layer schema
    std::auto_ptr<te::map::LayerSchema> schema(layer->getSchema());

    // Get the property names that compose the object id
    std::vector<std::string> oidPropertyNames;
    te::da::GetOIDPropertyNames(schema.get(), oidPropertyNames);

    // Get the edited geometries
    const std::vector<te::edit::Feature*>& features = repo->getAllFeatures();

    // Build the DataSet that will be used to update
    std::map<te::edit::OperationType, te::mem::DataSet* > operationds;

    for (std::size_t i = 0; i < te::edit::NumberOfOperationTypes; i++)
      operationds[te::edit::OperationType(i)] = new te::mem::DataSet(schema.get());

    // Get the geometry property position
    std::size_t gpos = te::da::GetFirstSpatialPropertyPos(operationds[te::edit::GEOMETRY_CREATE]);

    // Get the geometry type
    std::auto_ptr<te::da::DataSetType> dt = layer->getSchema();

    std::map<te::edit::OperationType, std::set<int> > propertiesPos;

    for (std::size_t i = 0; i < features.size(); ++i) // for each edited feature
    {
      // Create the new item
      te::mem::DataSetItem* item = new te::mem::DataSetItem(operationds[te::edit::GEOMETRY_CREATE]);

      // Get the object id
      te::da::ObjectId* oid = features[i]->getId();

      const boost::ptr_vector<te::dt::AbstractData>& values = oid->getValue();

      switch (features[i]->getOperationType())
      {
        case te::edit::GEOMETRY_CREATE:
        {
          // case if postgis, take the max value of oid
          if (info.get()->getType() == "POSTGIS")
          {
            //if case is a new feature and the oid is numeric
            for (std::size_t j = 0; j < values.size(); ++j)
            {
              int pType = operationds[te::edit::GEOMETRY_CREATE]->getPropertyDataType(te::da::GetPropertyPos(operationds[te::edit::GEOMETRY_CREATE], oidPropertyNames[j]));

              if (pType != te::dt::STRING_TYPE)
              {
                std::string sql = "SELECT MAX(" + oidPropertyNames[j] + ") + " + boost::lexical_cast<std::string>(i + 1) + " AS " + oidPropertyNames[j] + " FROM " + schema.get()->getName();
                std::auto_ptr<te::da::DataSet> dsMax = dsource->query(sql);

                if (dsMax.get())
                {
                  dsMax->moveFirst();
                  item->setValue(oidPropertyNames[j], dsMax->getValue(oidPropertyNames[j]).release());
                }
              }
            }
          }

          break;
        }
        default:
          // Fill the new item
          for (std::size_t j = 0; j < values.size(); ++j)
            item->setValue(oidPropertyNames[j], values[j].clone());

          break;
      }

      // Get the edited geometry
      te::gm::Geometry* geom = features[i]->getGeometry();

      // Set the geometry type
      item->setGeometry(gpos, static_cast<te::gm::Geometry*>(geom->clone()));

      switch (features[i]->getOperationType())
      {
        case te::edit::GEOMETRY_CREATE:

          operationds[te::edit::GEOMETRY_CREATE]->add(item);
          break;

        case te::edit::GEOMETRY_UPDATE:

          propertiesPos[te::edit::GEOMETRY_UPDATE].insert(gpos);

          operationds[te::edit::GEOMETRY_UPDATE]->add(item);
          break;

        case te::edit::GEOMETRY_DELETE:

          operationds[te::edit::GEOMETRY_DELETE]->add(item);
          break;

        case te::edit::GEOMETRY_UPDATE_ATTRIBUTES:

          if (features[i]->getOperationType() == te::edit::GEOMETRY_UPDATE_ATTRIBUTES)
          {
            if (features[i]->getData().size() > 0)
            {
              for (std::map<std::size_t, te::dt::AbstractData*>::const_iterator it = features[i]->getData().begin(); it != features[i]->getData().end(); ++it)
              {
                item->setValue(it->first, it->second);
                propertiesPos[te::edit::GEOMETRY_UPDATE_ATTRIBUTES].insert(it->first);
              }
            }
          }

          operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]->add(item);
          break;

        default:
          break;
        }
    }

    std::map<te::edit::OperationType, te::da::ObjectIdSet* > currentOids;

    if (operationds[te::edit::GEOMETRY_CREATE]->size() > 0)
    {
      operationds[te::edit::GEOMETRY_CREATE]->moveBeforeFirst();

      std::map<std::string, std::string> options;

      dsource->add(dslayer->getDataSetName(), operationds[te::edit::GEOMETRY_CREATE], options, 0);
    }

    if (operationds[te::edit::GEOMETRY_UPDATE]->size() > 0)
    {
      std::vector<std::set<int> > properties;
      for (std::size_t i = 0; i < operationds[te::edit::GEOMETRY_UPDATE]->size(); i++)
        properties.push_back(propertiesPos[te::edit::GEOMETRY_UPDATE]);

      std::vector<std::size_t> oidPropertyPosition;
      for (std::size_t i = 0; i < oidPropertyNames.size(); ++i)
        oidPropertyPosition.push_back(te::da::GetPropertyPos(operationds[te::edit::GEOMETRY_UPDATE], oidPropertyNames[i]));

      currentOids[te::edit::GEOMETRY_UPDATE] = te::da::GenerateOIDSet(operationds[te::edit::GEOMETRY_UPDATE], schema.get());

      operationds[te::edit::GEOMETRY_UPDATE]->moveBeforeFirst();

      dsource->update(dslayer->getDataSetName(), operationds[te::edit::GEOMETRY_UPDATE], properties, oidPropertyPosition);
    }

    if (operationds[te::edit::GEOMETRY_DELETE]->size() > 0)
    {
      currentOids[te::edit::GEOMETRY_DELETE] = te::da::GenerateOIDSet(operationds[te::edit::GEOMETRY_DELETE], schema.get());

      operationds[te::edit::GEOMETRY_DELETE]->moveBeforeFirst();

      dsource->remove(dslayer->getDataSetName(), currentOids[te::edit::GEOMETRY_DELETE]);

    }

    if (operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]->size() > 0)
    {
      std::vector<std::set<int> > properties;
      for (std::size_t i = 0; i < operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]->size(); i++)
        properties.push_back(propertiesPos[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]);

      std::vector<std::size_t> oidPropertyPosition;
      for (std::size_t i = 0; i < oidPropertyNames.size(); ++i)
        oidPropertyPosition.push_back(te::da::GetPropertyPos(operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES], oidPropertyNames[i]));

      currentOids[te::edit::GEOMETRY_UPDATE_ATTRIBUTES] = te::da::GenerateOIDSet(operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES], schema.get());

      operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]->moveBeforeFirst();

      dsource->update(dslayer->getDataSetName(), operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES], properties, oidPropertyPosition);
    }

    te::gm::Envelope env(layer->getExtent());

    env.Union(*operationds[te::edit::GEOMETRY_CREATE]->getExtent(gpos).get());

    env.Union(*operationds[te::edit::GEOMETRY_UPDATE]->getExtent(gpos).get());

    layer->setExtent(env);

    repo->clear();

    // repaint and clear
    //te::qt::af::evt::GetMapDisplay e;
    //emit triggered(&e);

    //e.m_display->getDisplay()->refresh();

    te::edit::UndoStackManager::getInstance().getUndoStack()->clear();

    //m_layerIsStashed = false;

   // te::qt::af::evt::LayerChanged e2(layer.get());
    //emit triggered(&e2);
  }
  catch (...)//te::common::Exception& ex)
  {
    //QMessageBox::critical(0, tr("TerraLib Edit Qt Plugin"), ex.what());
    return;
  }
}
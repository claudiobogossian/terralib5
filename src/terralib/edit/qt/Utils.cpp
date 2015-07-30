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
#include "../../geometry/Coord2D.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryCollection.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/LineString.h"
#include "../../geometry/MultiPoint.h"
#include "../../geometry/Point.h"
#include "../../geometry/Polygon.h"
#include "../../geometry/Utils.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/Utils.h"
#include "../../srs/Config.h"
#include "../Utils.h"
#include "Utils.h"

//#include "../../srs/Config.h"
#include "../Feature.h"
//#include "../SnapManager.h"
#include "Utils.h"

#include "../../edit/qt/core/EditionManager.h"

// Qt
#include <QColor>

// STL
#include <cassert>

QPointF te::edit::GetPosition(QMouseEvent* e)
{
#if QT_VERSION >= 0x050000
  return e->localPos();
#else
  return e->posF();
#endif
}

te::edit::Feature* te::edit::PickFeature(te::edit::EditionManager* editionManager, const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& env, int srid)
{
  if (layer->getVisibility() != te::map::VISIBLE || !layer->isValid())
    return 0;

  te::gm::Envelope reprojectedEnvelope(env);

  if ((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (layer->getSRID() != srid))
    reprojectedEnvelope.transform(srid, layer->getSRID());

  // Try retrieves from RepositoryManager...
  Feature* f = editionManager->m_repository->getFeature(layer->getId(), env, srid);

  if (f)
    return f->clone();

  // ...else, retrieve from layer
  if (!reprojectedEnvelope.intersects(layer->getExtent()))
    return 0;

  std::auto_ptr<const te::map::LayerSchema> schema(layer->getSchema());

  if (!schema->hasGeom())
    return 0;

  std::vector<std::string> oidPropertyNames;
  te::da::GetOIDPropertyNames(schema.get(), oidPropertyNames);

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(schema.get());

  // Gets the dataset
  std::auto_ptr<te::da::DataSet> dataset = layer->getData(gp->getName(), &reprojectedEnvelope, te::gm::INTERSECTS);

  if (dataset.get() == 0)
    return 0;

  // Generates a geometry from the given extent. It will be used to refine the results
  std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&reprojectedEnvelope, layer->getSRID()));

  // The restriction point. It will be used to refine the results
  te::gm::Coord2D center = reprojectedEnvelope.getCenter();
  te::gm::Point point(center.x, center.y, layer->getSRID());

  while (dataset->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> g(dataset->getGeometry(gp->getName()));

    if (g->contains(&point) || g->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(g.get())) // Geometry found!
      return new Feature(te::da::GenerateOID(dataset.get(), oidPropertyNames), g.release());
  }

  return 0;
}

/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/edit/qt/tools/DeletePartTool.cpp

  \brief This class implements a concrete tool to delete parts of multi features (deleting polygons of a multi feature polygons for example).
*/

// TerraLib
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/MultiLineString.h"
#include "../../../geometry/MultiPoint.h"
#include "../../../geometry/MultiPolygon.h"
#include "../../../geometry/Utils.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"

#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "DeletePartTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>

// STL
#include <cassert>
#include <memory>

te::edit::DeletePartTool::DeletePartTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
  : GeometriesUpdateTool(display, layer.get(), parent)
{
  updateCursor();
}

te::edit::DeletePartTool::~DeletePartTool()
{
}

bool te::edit::DeletePartTool::mousePressEvent(QMouseEvent* e)
{
  if (e->button() != Qt::LeftButton)
    return false;

  m_mbr = buildEnvelope(e->pos());
  if (!m_mbr.isValid())
    return false;

  m_feature = PickFeature(m_layer, m_mbr, m_display->getSRID(), te::edit::GEOMETRY_DELETE);

  if (m_feature == 0)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer.")));
    return false;
  }
  else
  {
    storeFeature();
  }

  return true;
}

te::gm::Envelope te::edit::DeletePartTool::buildEnvelope(const QPointF& pos)
{
  QPointF pixelOffset(4.0, 4.0);

  QRectF rect(pos - pixelOffset, pos + pixelOffset);

  // Converts rect boundary to world coordinates
  QPointF ll(rect.left(), rect.bottom());
  QPointF ur(rect.right(), rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  te::gm::Envelope env(ll.x(), ll.y(), ur.x(), ur.y());

  return env;
}

void te::edit::DeletePartTool::storeFeature()
{
  te::edit::OperationType op = te::edit::GEOMETRY_UPDATE;

  // Generates a geometry from the given extent. It will be used to refine the results
  std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&m_mbr, m_layer->getSRID()));

  // The restriction point. It will be used to refine the results
  te::gm::Coord2D center = m_mbr.getCenter();
  te::gm::Point point(center.x, center.y, m_layer->getSRID());

  switch (m_feature->getGeometry()->getGeomTypeId())
  {
    case te::gm::MultiPolygonType:
    {
      te::gm::MultiPolygon* mp = dynamic_cast<te::gm::MultiPolygon*>(m_feature->getGeometry());

      std::size_t i = 0;
      while (i < mp->getNumGeometries() && mp->getNumGeometries() > 1)
      {
        te::gm::Polygon* p = dynamic_cast<te::gm::Polygon*>(mp->getGeometryN(i));
        if (p->contains(&point) || p->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(p))
        {
          mp->removeGeometryN(i);
          i--;
        }
        else
          i++;
      }

      m_feature->clone()->setGeometry(mp);
    }
      break;

    case te::gm::MultiLineStringType:
    {
      te::gm::MultiLineString* ml = dynamic_cast<te::gm::MultiLineString*>(m_feature->getGeometry());

      std::size_t i = 0;
      while (i < ml->getNumGeometries() && ml->getNumGeometries() > 1)
      {
        te::gm::LineString* l = dynamic_cast<te::gm::LineString*>(ml->getGeometryN(i));
        if (l->contains(&point) || l->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(l))
        {
          ml->removeGeometryN(i);
          i--;
        }
        else
          i++;
      }

      m_feature->clone()->setGeometry(ml);
    }
      break;

    case te::gm::MultiPointType:
    {
      te::gm::MultiPoint* mpt = dynamic_cast<te::gm::MultiPoint*>(m_feature->getGeometry());

      std::size_t i = 0;
      while (i < mpt->getNumGeometries() && mpt->getNumGeometries() > 1)
      {
        te::gm::Point* pt = dynamic_cast<te::gm::Point*>(mpt->getGeometryN(i));
        if (pt->contains(&point) || pt->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(pt))
        {
          mpt->removeGeometryN(i);
          i--;
        }
        else
          i++;
      }

      m_feature->clone()->setGeometry(mpt);
    }
      break;

    default:
      op = te::edit::GEOMETRY_DELETE;
      break;
  }

  RepositoryManager::getInstance().addGeometry(m_layer->getId(), m_feature->getId()->clone(), dynamic_cast<te::gm::Geometry*>(m_feature->getGeometry()->clone()), op);

  emit geometriesEdited();
}

void te::edit::DeletePartTool::updateCursor()
{
  m_display->setCursor(Qt::ArrowCursor);
}
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
\file terralib/edit/qt/tools/CreateLineTool.cpp

\brief This class implements a concrete tool to create lines.
*/

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/GeometryCollection.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/LineString.h"
#include "../../../geometry/MultiPolygon.h"
#include "../../../geometry/Point.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "MergeGeometriesTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QInputDialog>

// STL
#include <cassert>
#include <memory>


te::edit::MergeGeometriesTool::MergeGeometriesTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, const QCursor& cursor, QObject* parent)
: AbstractTool(display, parent)
,m_layer(layer),
m_feature(0)
{
  m_oidRemoved = new te::da::ObjectIdSet();

  setCursor(cursor);

  mergeGeometries();
}

te::edit::MergeGeometriesTool::~MergeGeometriesTool()
{
  delete m_oidRemoved;

  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  m_feature = 0;
}

void te::edit::MergeGeometriesTool::mergeGeometries()
{
  bool disjoint = false;
  const te::gm::Envelope* ev;
  te::gm::Geometry* g;

  const te::da::ObjectIdSet* objSet = m_layer->getSelected();

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData(objSet));

  std::auto_ptr<te::da::DataSetType> dt(m_layer->getSchema());

  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dt.get());

  std::auto_ptr<te::gm::GeometryCollection> gc(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, m_layer->getSRID()));

  while (ds->moveNext())
  {
    gc->add(static_cast<te::gm::Geometry*>(ds->getGeometry(geomProp->getName()).release()));
  }

  disjoint = spatialRelationDisjoint(gc.get());

  if (disjoint)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("All geometries selected must touch!")));
    return;
  }
  
  m_refOid = getBaseOID(objSet, tr("Allow to merge geometries selecting the geom_id that will keep the attributes."));

  if (m_refOid->getValueAsString() == "")
    return;

  ev = getRefEnvelope(ds.get(), m_refOid, geomProp);

  m_feature = PickFeature(m_layer, *ev, m_display->getSRID());

  g = gc->getGeometryN(0);

  for (std::size_t i = 1; i < gc->getNumGeometries(); i++)
  {
    g = Union(g, gc->getGeometryN(i));
  }

  switch (geomProp->getGeometryType())
  {
    case  te::gm::MultiPolygonType:
    {
      te::gm::MultiPolygon* newGeo = new te::gm::MultiPolygon(1, te::gm::MultiPolygonType);
      newGeo->setGeometryN(0, g);
      newGeo->setSRID(m_layer->getSRID());

      g = newGeo;

      break;
    }
    case  te::gm::PolygonType:
      g = dynamic_cast<te::gm::Polygon*>(g);
      break;
  }

  m_feature->setId(m_refOid);
  m_feature->setGeometry(g);

  draw();

  storeMergedFeature();
}

bool te::edit::MergeGeometriesTool::spatialRelationDisjoint(te::gm::GeometryCollection* gc)
{
  bool result = false;
  std::size_t j = 0;
  std::size_t aux = 0;

  for (std::size_t i = 0; i < gc->getNumGeometries(); i++)
  {
    te::gm::Geometry* g1 = gc->getGeometryN(i);

    aux = 0;

    for (j = 0; j < gc->getNumGeometries(); j++)
    {
      if (j != i)
      {
        if (g1->disjoint(gc->getGeometryN(j)))
          aux++;
      }
    }

    if (aux == (j-1))
    {
      result = true;
      break;
    }

  }

  return result;
}

te::da::ObjectId* te::edit::MergeGeometriesTool::getBaseOID(const te::da::ObjectIdSet* objSet, QString msg)
{
  bool ok;
  QStringList qValues;
  te::da::ObjectId* objValue;

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;

  for (it = objSet->begin(); it != objSet->end(); ++it)
    qValues.append((*it)->getValueAsString().c_str());

  QString qValue = QInputDialog::getItem(NULL, QString(tr("TerraLib Edit Qt Plugin")), QObject::tr(msg.toLatin1()), qValues, 0, false, &ok);

  if (qValue.isEmpty() || !ok)
  {
    return new te::da::ObjectId();
  }

  for (it = objSet->begin(); it != objSet->end(); ++it)
  {
    if ((*it)->getValueAsString() == qValue.toStdString())
    {
      objValue = (*it);
    }
    else
    {
      m_oidRemoved->add((*it));
    }
  }

  return objValue;
}

const te::gm::Envelope* te::edit::MergeGeometriesTool::getRefEnvelope(te::da::DataSet* ds, te::da::ObjectId* oid, te::gm::GeometryProperty* geomProp)
{
  std::string refOID;
  std::vector<std::string> oidPropertyNames;
  const te::gm::Envelope* env;
  
  te::da::GetOIDPropertyNames(m_layer->getSchema().get(), oidPropertyNames);

  ds->moveBeforeFirst();

  int colType = ds->getPropertyDataType(te::da::GetPropertyPos(ds, oidPropertyNames[0]));

  while (ds->moveNext())
  {
    if (colType == te::dt::INT16_TYPE || colType == te::dt::INT32_TYPE || colType == te::dt::INT64_TYPE || colType == te::dt::DOUBLE_TYPE)
    {
      
      refOID = boost::lexical_cast<std::string>(ds->getInt32(oidPropertyNames[0]));
    }
    else
    {
      refOID = ds->getString(oidPropertyNames[0]);
    }

    if (m_refOid->getValueAsString() == refOID)
    {
      env = (ds->getGeometry(geomProp->getName()).release())->getMBR();
    }
  }

  return env;
}

te::gm::Geometry* te::edit::MergeGeometriesTool::Union(te::gm::Geometry* g1, te::gm::Geometry* g2)
{
  return g2->Union(g1);
}

void te::edit::MergeGeometriesTool::draw()
{
  const te::gm::Envelope& env = m_display->getExtent();
  if (!env.isValid())
    return;

  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Initialize the renderer
  Renderer& renderer = Renderer::getInstance();
  renderer.begin(draft, env, m_display->getSRID());

  // Draw the layer edited geometries
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  // Draw the current geometry and the vertexes
  renderer.setPolygonStyle(Qt::red, Qt::black, 2);
  renderer.draw(m_feature->getGeometry(), true);

  renderer.end();

  m_display->repaint();
}

void te::edit::MergeGeometriesTool::storeMergedFeature()
{
  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;
  
  for (it = m_oidRemoved->begin(); it != m_oidRemoved->end(); ++it)
  {
    RepositoryManager::getInstance().addGeometry(m_layer->getId(), (*it)->clone(), dynamic_cast<te::gm::Geometry*>(m_feature->getGeometry()->clone()));
  }

  RepositoryManager::getInstance().addGeometry(m_layer->getId(), m_feature->getId()->clone(), dynamic_cast<te::gm::Geometry*>(m_feature->getGeometry()->clone()));
}

bool te::edit::MergeGeometriesTool::mousePressEvent(QMouseEvent* e)
{
  return false;
}

bool te::edit::MergeGeometriesTool::mouseMoveEvent(QMouseEvent* e)
{
  return false;
}

bool te::edit::MergeGeometriesTool::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::edit::MergeGeometriesTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  return false;
}
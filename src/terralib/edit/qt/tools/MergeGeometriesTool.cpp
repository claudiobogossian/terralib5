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
\file terralib/edit/qt/tools/MergeGeometriesTool.cpp

\brief This class implements a concrete tool to merge polygons.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/MultiPolygon.h"
#include "../../../geometry/Point.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "../core/command/UpdateCommand.h"
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
  : GeometriesUpdateTool(display, layer.get(), parent),
  m_geocollection(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, m_layer->getSRID())),
  m_updateWatches(0),
  m_oidsMerged(0)
{
  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));

  setCursor(cursor);

  mergeGeometries();
}

te::edit::MergeGeometriesTool::~MergeGeometriesTool()
{
  delete m_feature;
  delete m_geocollection;
  m_updateWatches.clear();
  delete m_oidsMerged;
}

void te::edit::MergeGeometriesTool::mergeGeometries()
{
  const te::gm::Envelope* env;
  te::gm::Geometry* mergeGeo = 0;

  const te::da::ObjectIdSet* objSet = m_layer->getSelected();

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData(objSet)) ;

  std::auto_ptr<te::da::DataSetType> dt(m_layer->getSchema()) ;

  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dt.get());

  if (m_oidsMerged == 0)
    m_oidsMerged = new te::da::ObjectIdSet();

  while (ds->moveNext())
  {
    m_geocollection->add(dynamic_cast<te::gm::Geometry*>(ds->getGeometry(geomProp->getName()).release()));
  }

  if (spatialRelationDisjoint(*m_geocollection))
  {
    m_geocollection->removeGeometryN(m_geocollection->getNumGeometries() - 1);

    QMessageBox::critical(m_display, tr("Error"), QString(tr("All geometries selected must touch!")));
    return;
  }

  getBaseOID(*objSet, tr("Allow to merge geometries selecting the geom_id that will keep the attributes."));

  if (m_chosenOid == "")
    return;

  env = getRefEnvelope(*ds, *geomProp);

  m_feature = PickFeature(m_layer, *env, m_display->getSRID(), te::edit::GEOMETRY_UPDATE);

  if (m_feature == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Merge not possible"));
    return;
  }

  //used to undo/redo
  if (m_updateWatches.size() == 0)
  {
    m_feature->setGeometry(dynamic_cast<te::gm::Geometry*>(m_geocollection->getGeometryN(0)->clone()));
    m_updateWatches.push_back(m_feature->clone());
  }

  mergeGeo = dynamic_cast<te::gm::Geometry*>(m_geocollection->getGeometryN(0)->clone());

  for (std::size_t i = 1; i < m_geocollection->getNumGeometries(); i++)
  {
    mergeGeo = Union(*mergeGeo, *m_geocollection->getGeometryN(i));
  }

  mergeGeo = convertGeomType(m_layer, mergeGeo);

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;

  for (it = m_oidsMerged->begin(); it != m_oidsMerged->end(); ++it)
  {
    if ((*it)->getValueAsString() == m_chosenOid)
    {
      m_feature->setId((*it)->clone());
      break;
    }
  }

  m_feature->setGeometry(dynamic_cast<te::gm::Geometry*>(mergeGeo->clone()));

  draw();

  storeMergedFeature();

  storeUndoCommand();

}

bool te::edit::MergeGeometriesTool::spatialRelationDisjoint(te::gm::GeometryCollection& gc)
{
  bool result = false;
  std::size_t j = 0;
  std::size_t aux = 0;

  for (std::size_t i = 0; i < gc.getNumGeometries(); i++)
  {
    aux = 0;

    te::gm::Geometry* g1 = gc.getGeometryN(i);

    for (j = 0; j < gc.getNumGeometries(); j++)
    {
      if (j != i)
      {
        if (g1->disjoint(gc.getGeometryN(j)))
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

void te::edit::MergeGeometriesTool::getBaseOID(const te::da::ObjectIdSet& objSet, QString msg)
{
  bool ok;
  QStringList qValues;

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;

  for (it = objSet.begin(); it != objSet.end(); ++it)
    qValues.append((*it)->getValueAsString().c_str());

  QString qValue = QInputDialog::getItem(NULL, QString(tr("TerraLib Edit Qt Plugin")), QObject::tr(msg.toLatin1()), qValues, 0, false, &ok);

  if (qValue.isEmpty() || !ok)
  {
    return;
  }

  for (it = objSet.begin(); it != objSet.end(); ++it)
  {
    if ((*it)->getValueAsString() == qValue.toStdString())
    {
      m_chosenOid = (*it)->clone()->getValueAsString();
    }
    
    m_oidsMerged->add((*it)->clone());
  }
}

const te::gm::Envelope* te::edit::MergeGeometriesTool::getRefEnvelope(te::da::DataSet& ds, te::gm::GeometryProperty& geomProp)
{
  std::size_t pType = 0;
  std::string convOid;
  std::vector<std::string> oidPropertyNames;
  const te::gm::Envelope* env = new te::gm::Envelope();
  
  te::da::GetOIDPropertyNames(m_layer->getSchema().get(), oidPropertyNames);

  ds.moveBeforeFirst();

  pType = ds.getPropertyDataType(te::da::GetPropertyPos(&ds, oidPropertyNames[0]));

  while (ds.moveNext())
  {
    switch (pType)
    {
      case te::dt::INT16_TYPE:
        convOid = boost::lexical_cast<std::string>(ds.getInt16(oidPropertyNames[0]));
        break;

      case te::dt::INT32_TYPE:
        convOid = boost::lexical_cast<std::string>(ds.getInt32(oidPropertyNames[0]));
        break;

      case te::dt::INT64_TYPE:
        convOid = boost::lexical_cast<std::string>(ds.getInt64(oidPropertyNames[0]));
        break;

      case te::dt::FLOAT_TYPE:
        convOid = boost::lexical_cast<std::string>(ds.getFloat(oidPropertyNames[0]));
        break;

      case te::dt::DOUBLE_TYPE:
        convOid = boost::lexical_cast<std::string>(ds.getDouble(oidPropertyNames[0]));
        break;

      case te::dt::NUMERIC_TYPE:
        convOid = boost::lexical_cast<std::string>(ds.getNumeric(oidPropertyNames[0]));
        break;

      case te::dt::STRING_TYPE:
        convOid = ds.getString(oidPropertyNames[0]);
        break;
    }
    if (m_chosenOid == convOid)
    {
      env = (ds.getGeometry(geomProp.getName()).release())->getMBR();
    }
  }

  return env;
}

te::gm::Geometry* te::edit::MergeGeometriesTool::Union(te::gm::Geometry& g1, te::gm::Geometry& g2)
{
  return g2.Union(&g1);
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
  renderer.draw(m_feature->getGeometry(), true);

  renderer.end();

  m_display->repaint();

}

void te::edit::MergeGeometriesTool::storeMergedFeature()
{
  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;

  for (it = m_oidsMerged->begin(); it != m_oidsMerged->end(); ++it)
  {
      if ((*it)->getValueAsString() == m_chosenOid)
      {
        RepositoryManager::getInstance().addFeature(m_layer->getId(), m_feature->clone());
      }
      else
      {
        RepositoryManager::getInstance().addGeometry(m_layer->getId(), (*it)->clone(), dynamic_cast<te::gm::Geometry*>(m_feature->getGeometry()->clone()), te::edit::GEOMETRY_DELETE);
      }
  }

  emit geometriesEdited();
}

void te::edit::MergeGeometriesTool::onExtentChanged()
{
  draw();
}

void te::edit::MergeGeometriesTool::storeUndoCommand()
{
  m_updateWatches.push_back(m_feature->clone());

  QUndoCommand* command = new UpdateCommand(m_updateWatches, m_display, m_layer);
  UndoStackManager::getInstance().addUndoStack(command);
}

void te::edit::MergeGeometriesTool::pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos)
{
  delete m_feature;
  m_feature = 0;

  te::gm::Envelope env = buildEnvelope(pos);

  try
  {
    m_feature = PickFeature(layer, env, m_display->getSRID(), te::edit::GEOMETRY_UPDATE);
  }
  catch (std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
  }
}

te::gm::Envelope te::edit::MergeGeometriesTool::buildEnvelope(const QPointF& pos)
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

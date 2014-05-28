/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file ChartDisplayDockWidget.cpp

  \brief A dock widget for chart display objects.
*/
 
#include "ChartDisplayDockWidget.h"

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../widgets/charts/ChartConfigurer.h"
#include "../../widgets/charts/ChartDisplayWidget.h"
#include "events/LayerEvents.h"
#include "../ApplicationController.h"

te::gm::Envelope* computeDataSetEnvelope(std::auto_ptr<te::da::DataSet> dataset, size_t propNum)
{
  te::gm::Envelope* result = new te::gm::Envelope;

  while(dataset->moveNext())
  {
    te::gm::Geometry* geom = dataset->getGeometry(propNum).release();
    assert(geom);
    result->Union(*geom->getMBR());
  }
  return result;
}


te::qt::af::ChartDisplayDockWidget::ChartDisplayDockWidget(te::qt::widgets::ChartDisplayWidget* displayWidget, QWidget* parent) :
QDockWidget(parent, Qt::Widget),
  m_displayWidget(displayWidget),
  m_layer(0)
{
  setWidget(m_displayWidget);
  m_displayWidget->setParent(this);

  setAttribute(Qt::WA_DeleteOnClose, true);

  connect (m_displayWidget, SIGNAL(selected(te::da::ObjectIdSet*, const bool&)), SLOT(selectionChanged(te::da::ObjectIdSet*, const bool&)));
}

te::qt::af::ChartDisplayDockWidget::~ChartDisplayDockWidget()
{
  delete m_displayWidget;
  emit closed(this);
}

void te::qt::af::ChartDisplayDockWidget::setLayer(te::map::AbstractLayer* layer)
{
  m_layer = layer;

  //Configuring the default selection color
  te::qt::widgets::ChartConfigurer cc(m_displayWidget);
  cc.config(m_layer);

  //Adjusting the selected objectIdSet
  m_displayWidget->highlightOIds(m_layer->getSelected());

  if(m_layer==0)
    return;

  setWindowTitle(m_layer->getTitle().c_str());
}

void te::qt::af::ChartDisplayDockWidget::setSelectionColor(QColor selColor)
{
  m_displayWidget->setSelectionColor(selColor);
}

te::map::AbstractLayer* te::qt::af::ChartDisplayDockWidget::getLayer() const
{
  return m_layer;
}

void te::qt::af::ChartDisplayDockWidget::onApplicationTriggered(te::qt::af::evt::Event* evt)
{
  switch(evt->m_id)
  {
    case te::qt::af::evt::LAYER_SELECTED_OBJECTS_CHANGED:
      {
        te::qt::af::evt::LayerSelectedObjectsChanged* ev = static_cast<te::qt::af::evt::LayerSelectedObjectsChanged*>(evt);

        if(ev->m_layer->getId() == m_layer->getId())
        {
          m_displayWidget->highlightOIds(ev->m_layer->getSelected());
        }
      }
    break;

    case te::qt::af::evt::LAYER_REMOVED:
      {
        te::qt::af::evt::LayerRemoved* ev = static_cast<te::qt::af::evt::LayerRemoved*>(evt);

        if(ev->m_layer->getId() == m_layer->getId())
        {
          this->close();
        }
      }
    break;

  }
}

void te::qt::af::ChartDisplayDockWidget::selectionChanged(te::da::ObjectIdSet* oids, const bool& add)
{
    te::da::ObjectIdSet* added = oids->clone();

  if(m_layer->getSelected())
  {
    if (add)
    {
      te::da::ObjectIdSet* removed = new te::da::ObjectIdSet();
      //Checking if the objectIds need to be emoved from the current selection
      std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator itObjSet; 
      for(itObjSet = oids->begin(); itObjSet != oids->end(); ++itObjSet)
      {
        if(m_layer->getSelected()->contains(*itObjSet))
        {
          removed->add(*itObjSet);
          added->remove(*itObjSet);
        }
      }
      m_layer->deselect(removed);
    }
    else
    {
      m_layer->clearSelected();
    }
  }
  
  std::vector<std::size_t> objIdIdx;
  te::da::GetOIDPropertyPos(m_layer->getSchema().get(), objIdIdx);

  std::vector<size_t>::iterator it;

  for(it=objIdIdx.begin(); it!=objIdIdx.end(); ++it)
    added->addProperty(m_layer->getData()->getPropertyName(*it), *it, m_layer->getData()->getPropertyDataType(*it));

  oids->clear();
  m_layer->select(added);

  //Acquiring the envelope
  if(added->size() > 0)
  {
    std::auto_ptr<te::da::DataSet> ds = m_layer->getData(added);
    ds->moveBeforeFirst();

    size_t numProp, geomPos;
    numProp =  ds->getNumProperties();

    for(size_t i=0; i<numProp; i++)
    {
      if(ds->getPropertyDataType(i) == te::dt::GEOMETRY_TYPE)
      {
        geomPos = i;
        break;
      }
    }

    te::qt::af::evt::LayerSelectedObjectsChanged e(m_layer, computeDataSetEnvelope(ds, geomPos));
    ApplicationController::getInstance().broadcast(&e);
  }
  else
  {
    te::qt::af::evt::LayerSelectedObjectsChanged e(m_layer);
    ApplicationController::getInstance().broadcast(&e);
  }
}

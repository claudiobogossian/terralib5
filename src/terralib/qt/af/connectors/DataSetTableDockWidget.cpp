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

#include "DataSetTableDockWidget.h"

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../memory/DataSet.h"
#include "../../widgets/table/DataSetTableView.h"
#include "../../widgets/utils/ScopedCursor.h"
#include "../events/LayerEvents.h"
#include "../events/TableEvents.h"
#include "../ApplicationController.h"
#include "../Utils.h"

te::qt::af::DataSetTableDockWidget::DataSetTableDockWidget(QWidget* parent)
  : QDockWidget(parent, Qt::Widget),
    m_layer(0)
{
  m_view = new te::qt::widgets::DataSetTableView(this);

  m_view->setHighlightColor(ApplicationController::getInstance().getSelectionColor());

  setWidget(m_view);

  setAttribute(Qt::WA_DeleteOnClose, true);

  connect (m_view, SIGNAL(selectOIds(te::da::ObjectIdSet*, const bool&, te::gm::Envelope*)), SLOT(selectionChanged(te::da::ObjectIdSet*, const bool&, te::gm::Envelope*)));
  connect (m_view, SIGNAL(deselectOIds(te::da::ObjectIdSet*)), SLOT(removeSelectedOIds(te::da::ObjectIdSet*)));

  // Alternate Colors
  if(te::qt::af::GetAlternateRowColorsFromSettings())
  {
    m_view->setAlternatingRowColors(true);
    m_view->setStyleSheet(te::qt::af::GetStyleSheetFromSettings());
  }
}

te::qt::af::DataSetTableDockWidget::~DataSetTableDockWidget()
{
  emit closed(this);
}

void te::qt::af::DataSetTableDockWidget::setLayer(te::map::AbstractLayer* layer)
{
  m_layer = layer;

  if(m_layer == 0)
    return;

  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  m_view->setLayer(m_layer);

  if(m_layer->getSelected() != 0)
    m_view->highlightOIds(m_layer->getSelected());

  const char* lName = m_layer->getTitle().c_str();

  setWindowTitle(lName);

  setObjectName(QString("TableView_") + lName);
}

te::map::AbstractLayer* te::qt::af::DataSetTableDockWidget::getLayer() const
{
  return m_layer;
}

void te::qt::af::DataSetTableDockWidget::onApplicationTriggered(te::qt::af::evt::Event* evt)
{
  switch(evt->m_id)
  {
    case te::qt::af::evt::LAYER_SELECTED_OBJECTS_CHANGED:
    {
      te::qt::af::evt::LayerSelectedObjectsChanged* ev = static_cast<te::qt::af::evt::LayerSelectedObjectsChanged*>(evt);

      if(ev->m_layer->getId() == m_layer->getId())
        m_view->highlightOIds(ev->m_layer->getSelected());
    }
    break;

    case te::qt::af::evt::TABLE_ALTERNATE_COLORS_CHANGED:
    {
      te::qt::af::evt::TableAlternatingColorsChanged* ev = static_cast<te::qt::af::evt::TableAlternatingColorsChanged*>(evt);
      m_view->setAlternatingRowColors(ev->m_isAlternating);
      if(ev->m_isAlternating)
      {
        m_view->setStyleSheet(te::qt::af::GetStyleSheetFromColors(ev->m_primaryColor, ev->m_secondaryColor));
      }
      else
      {
        m_view->setStyleSheet("background-color: white;");
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

void te::qt::af::DataSetTableDockWidget::selectionChanged(te::da::ObjectIdSet* oids, const bool& add, te::gm::Envelope* env)
{
  if (!add)
    m_layer->clearSelected();

  m_layer->select(oids);

  te::qt::af::evt::LayerSelectedObjectsChanged e(m_layer, env);
  ApplicationController::getInstance().broadcast(&e);
}

void te::qt::af::DataSetTableDockWidget::removeSelectedOIds(te::da::ObjectIdSet* oids)
{
  if(m_layer->getSelected() != 0)
  {
    m_layer->deselect(oids);

    te::qt::af::evt::LayerSelectedObjectsChanged e(m_layer, 0);
    ApplicationController::getInstance().broadcast(&e);
  }
}

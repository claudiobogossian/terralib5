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
  \file terralib/qt/widgets/canvas/ZoomInMapDisplayWidget.h

  \brief This file has the ZoomInMapDisplayWidget class.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Point.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../se/Fill.h"
#include "../../../se/Mark.h"
#include "../../../se/Stroke.h"
#include "../../../se/Utils.h"
#include "../../widgets/tools/AbstractTool.h"
#include "../../widgets/tools/Pan.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "ZoomInMapDisplayWidget.h"

// Qt
#include <QtGui/QGridLayout>

#define PATTERN_SIZE 14

te::qt::widgets::ZoomInMapDisplayWidget::ZoomInMapDisplayWidget(te::qt::widgets::MapDisplay* mapParent, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f), m_parentMapDisplay(mapParent), m_itsMe(false), m_isEnabled(true)
{

//build form
  QGridLayout* displayLayout = new QGridLayout(this);
  m_mapDisplay = new te::qt::widgets::MapDisplay(this->size(), this);
  displayLayout->addWidget(m_mapDisplay);
  displayLayout->setContentsMargins(0,0,0,0);

//define mark
  te::se::Stroke* strokeSel = te::se::CreateStroke("#000000", "1");
  te::se::Fill* fillSel = te::se::CreateFill("#000000", "1.0");
  m_mark = te::se::CreateMark("cross", strokeSel, fillSel);

  m_rgbaMark = te::map::MarkRendererManager::getInstance().render(m_mark, PATTERN_SIZE);

// Zoom In Display pan tool
  te::qt::widgets::Pan* zoomInPan = new te::qt::widgets::Pan(m_mapDisplay, Qt::OpenHandCursor, Qt::ClosedHandCursor, m_mapDisplay);
  m_mapDisplay->installEventFilter(zoomInPan);

// signals & slots
  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(onMapDisplayExtentChanged()));
  connect(m_parentMapDisplay, SIGNAL(extentChanged()), this, SLOT(onParentMapDisplayExtentChanged()));
}

te::qt::widgets::ZoomInMapDisplayWidget::~ZoomInMapDisplayWidget()
{
  te::common::Free(m_rgbaMark, PATTERN_SIZE);
  delete m_mark;
}

void te::qt::widgets::ZoomInMapDisplayWidget::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  te::gm::Envelope inputExt = m_layer->getExtent();

  te::gm::Envelope ext = calculateExtent(inputExt);

  m_mapDisplay->setMouseTracking(true);
  m_mapDisplay->setLayerList(list);
  m_mapDisplay->setSRID(m_layer->getSRID(), false);

  if(m_isEnabled)
  {
    m_itsMe = true;
    m_mapDisplay->setExtent(ext, true);
    m_itsMe = false;
  }
}

void te::qt::widgets::ZoomInMapDisplayWidget::drawCursorPosition(double x, double y)
{
  if(m_isEnabled == false)
    return;

  m_mapDisplay->getDraftPixmap()->fill(QColor(0, 0, 0, 0));
  const te::gm::Envelope& mapExt = m_mapDisplay->getExtent();
  te::qt::widgets::Canvas canvasInstance(m_mapDisplay->getDraftPixmap());
  canvasInstance.setWindow(mapExt.m_llx, mapExt.m_lly, mapExt.m_urx, mapExt.m_ury);

  canvasInstance.setPointColor(te::color::RGBAColor(0,0,0, TE_TRANSPARENT)); //GAMBI
  canvasInstance.setPointPattern(m_rgbaMark, PATTERN_SIZE, PATTERN_SIZE);

  te::gm::Point point(x, y);
  canvasInstance.draw(&point);

  m_mapDisplay->repaint();
}

void te::qt::widgets::ZoomInMapDisplayWidget::setEnabled(bool status)
{
  m_isEnabled = status;

  onParentMapDisplayExtentChanged();
}

void te::qt::widgets::ZoomInMapDisplayWidget::onMapDisplayExtentChanged()
{
  if(m_itsMe)
    return;

  te::gm::Coord2D center = m_mapDisplay->getExtent().getCenter();

  double w = m_parentMapDisplay->getExtent().getWidth() * 0.5;
  double h = m_parentMapDisplay->getExtent().getHeight() * 0.5;

  te::gm::Envelope auxExt;

  auxExt.m_llx = center.x - w;
  auxExt.m_lly = center.y - h;
  auxExt.m_urx = center.x + w;
  auxExt.m_ury = center.y + h;

  m_itsMe = true;
  m_parentMapDisplay->setExtent(auxExt);
  m_itsMe = false;
}

void te::qt::widgets::ZoomInMapDisplayWidget::onParentMapDisplayExtentChanged()
{
  if(m_isEnabled && m_itsMe == false)
  {
    te::gm::Envelope e = m_parentMapDisplay->getExtent();

    te::gm::Envelope ext = calculateExtent(e);

    m_itsMe = true;
    m_mapDisplay->setExtent(ext, true);
    m_itsMe = false;
  }
}

te::gm::Envelope te::qt::widgets::ZoomInMapDisplayWidget::calculateExtent(te::gm::Envelope& e)
{
  te::gm::Coord2D center = e.getCenter();

  // Bulding the zoom extent based on zoom factor value and the given point
  double w = e.getWidth() *  0.5 * 0.2;
  double h = e.getHeight() *  0.5 * 0.2;

  te::gm::Envelope ext(center.x - w, center.y - h, center.x + w, center.y + h);

  return ext;
}

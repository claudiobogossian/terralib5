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
  \file terralib/qt/widgets/canvas/ZoomInMapDisplayWidget.h

  \brief This file has the ZoomInMapDisplayWidget class.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Point.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../maptools/Utils.h"
#include "../../../se/Fill.h"
#include "../../../se/Mark.h"
#include "../../../se/Stroke.h"
#include "../../../se/Utils.h"
#include "../../widgets/tools/AbstractTool.h"
#include "../../widgets/tools/Pan.h"
#include "../canvas/Canvas.h"
#include "../canvas/MultiThreadMapDisplay.h"
#include "ZoomInMapDisplayWidget.h"

// Qt
#include <QGridLayout>

#define PATTERN_SIZE 14

te::qt::widgets::ZoomInMapDisplayWidget::ZoomInMapDisplayWidget(te::qt::widgets::MapDisplay* mapParent, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f), m_parentMapDisplay(mapParent), m_itsMe(false), m_isEnabled(true)
{

//build form
  QGridLayout* displayLayout = new QGridLayout(this);

  m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(parent->size(), this);
  m_mapDisplay->setResizePolicy(te::qt::widgets::MapDisplay::Center);

  displayLayout->addWidget(m_mapDisplay, 0,0);
  
  m_sliderZoomFactor = new QSlider(Qt::Horizontal, this);
  m_sliderZoomFactor->setRange(1, 100);
  m_sliderZoomFactor->setValue(50);
  m_sliderZoomFactor->setInvertedAppearance(true);

  displayLayout->addWidget(m_sliderZoomFactor, 1,0);

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
  connect(m_sliderZoomFactor, SIGNAL(sliderReleased()), this, SLOT(onZoomFactorChanged()));
}

te::qt::widgets::ZoomInMapDisplayWidget::~ZoomInMapDisplayWidget()
{
  te::common::Free(m_rgbaMark, PATTERN_SIZE);
  delete m_mark;
}

void te::qt::widgets::ZoomInMapDisplayWidget::setList(std::list<te::map::AbstractLayerPtr>& layerList, int srid)
{
  te::gm::Envelope e = m_parentMapDisplay->getExtent();

  te::gm::Envelope ext = calculateExtent(e);

  m_mapDisplay->setMouseTracking(true);
  m_mapDisplay->setLayerList(layerList);
  m_mapDisplay->setSRID(srid, false);

  //m_itsMe = true;
  m_mapDisplay->setExtent(ext, false);
  //m_itsMe = false;
}

te::gm::Envelope te::qt::widgets::ZoomInMapDisplayWidget::getCurrentExtent()
{
  return m_mapDisplay->getExtent();
}

void te::qt::widgets::ZoomInMapDisplayWidget::drawCursorPosition(double x, double y)
{
  if(!m_isEnabled)
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
  if(!m_isEnabled)
    return;

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
  if(!m_isEnabled)
    return;

  if(m_itsMe)
    return;

  te::gm::Envelope e = m_parentMapDisplay->getExtent();

  te::gm::Envelope ext = calculateExtent(e);

  if(!ext.isValid())
    return;

  m_itsMe = true;
  m_mapDisplay->setExtent(ext, true);
  m_itsMe = false;
}

void te::qt::widgets::ZoomInMapDisplayWidget::onZoomFactorChanged()
{
  te::gm::Envelope e = m_parentMapDisplay->getExtent();

  te::gm::Envelope ext = calculateExtent(e);

  m_itsMe = true;
  m_mapDisplay->setExtent(ext, true);
  m_itsMe = false;
}

te::gm::Envelope te::qt::widgets::ZoomInMapDisplayWidget::calculateExtent(te::gm::Envelope& e)
{
  double value = (double)m_sliderZoomFactor->value() / 100.;
  double hScale = (double)m_mapDisplay->getHeight() / (double)m_parentMapDisplay->getHeight();
  double wScale = (double)m_mapDisplay->getWidth() / (double)m_parentMapDisplay->getWidth();
  double scale = (hScale + wScale) / 2.;

  te::gm::Coord2D center = e.getCenter();

  // Bulding the zoom extent based on zoom factor value and the given point
  double w = e.getWidth() *  scale * value;
  double h = e.getHeight() *  scale * value;

  te::gm::Envelope ext(center.x - w, center.y - h, center.x + w, center.y + h);

  return ext;
}

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
  \file terralib/qt/widgets/canvas/MapDisplay.cpp

  \brief A Qt4 widget to control the display of a set of layers.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/Utils.h"
#include "../../../qt/widgets/layer/explorer/AbstractTreeItem.h"
#include "../utils/ScopedCursor.h"
#include "Canvas.h"
#include "MapDisplay.h"

// Qt
#include <QMimeData>
#include <QPaintDevice>
#include <QResizeEvent>
#include <QTimer>
#include <QGraphicsView>

te::qt::widgets::MapDisplay::MapDisplay(const QSize& size, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    te::map::MapDisplay(),
    m_displayPixmap(new QPixmap(size)),
    m_draftPixmap(new QPixmap(size)),
    m_backgroundColor(Qt::white),
    m_resizePolicy(te::qt::widgets::MapDisplay::Fixed),
    m_timer(new QTimer(this)),
    m_interval(200),
    m_isDrawing(false),
    m_scale(0)
{
  m_timer->setSingleShot(true);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(onResizeTimeout()));

  m_displayPixmap->fill(m_backgroundColor);
  m_draftPixmap->fill(Qt::transparent);

  resize(size);
  setAcceptDrops(true);
}

te::qt::widgets::MapDisplay::MapDisplay(QWidget* parent, Qt::WindowFlags f)
: QWidget(parent, f),
te::map::MapDisplay(),
m_displayPixmap(new QPixmap(size())),
m_draftPixmap(new QPixmap(size())),
m_backgroundColor(Qt::white),
m_resizePolicy(te::qt::widgets::MapDisplay::Fixed),
m_timer(new QTimer(this)),
m_interval(200),
m_isDrawing(false)
{
  m_timer->setSingleShot(true);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(onResizeTimeout()));

  m_displayPixmap->fill(m_backgroundColor);
  m_draftPixmap->fill(Qt::transparent);

  setAcceptDrops(true);
}

te::qt::widgets::MapDisplay::~MapDisplay()
{
  delete m_displayPixmap;
  delete m_draftPixmap;

  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
    delete it->second;

  m_layerCanvasMap.clear();
}

void te::qt::widgets::MapDisplay::changeData(te::map::AbstractLayerPtr al, int nsrid)
{
  // limpe todos os canvas antes usados 
  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
  {
    te::map::Canvas* c = getCanvas(it->first);
    delete c;
  }
  m_layerCanvasMap.clear();
  m_srid = nsrid;

  if(al.get() == 0)
    return;

  std::list<te::map::AbstractLayerPtr> vis, visibleLayers;
  te::map::GetVisibleLayers(al, vis);
  // remove folders
  std::list<te::map::AbstractLayerPtr>::iterator vit;
  for(vit = vis.begin(); vit != vis.end(); ++vit)
  {
    if((*vit)->getType() == "DATASETLAYER" || 
      (*vit)->getType() == "QUERYLAYER" ||
      (*vit)->getType() == "RASTERLAYER")
    {
      visibleLayers.push_front(*vit);
    }
  }

  setLayerList(visibleLayers);

  if(m_srid == TE_UNKNOWN_SRS)
  {
    // calcule SRID e extent
    te::gm::Envelope envelope;

    std::list<te::map::AbstractLayerPtr>::iterator lit;
    for(lit = visibleLayers.begin(); lit != visibleLayers.end(); ++lit)
    {
      te::gm::Envelope env = (*lit)->getExtent();

      int srid = (*lit)->getSRID();
      if(m_srid <= 0)
        m_srid = srid;
      if(srid != m_srid)
        env.transform(srid, m_srid);

      envelope.Union(env);
    }
    setExtent(envelope);
  }
}

void te::qt::widgets::MapDisplay::setExtent(te::gm::Envelope& e, bool doRefresh)
{
  te::map::MapDisplay::setExtent(e, doRefresh);

  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
  {
    te::qt::widgets::Canvas* canvas = it->second;
    canvas->calcAspectRatio(m_extent.m_llx, m_extent.m_lly, m_extent.m_urx, m_extent.m_ury, m_hAlign, m_vAlign);
    canvas->setWindow(m_extent.m_llx, m_extent.m_lly, m_extent.m_urx, m_extent.m_ury);
    canvas->clear();
    e = m_extent;
  }

  if(doRefresh)
    refresh();

  emit extentChanged();
}

void te::qt::widgets::MapDisplay::refresh()
{
  ScopedCursor cursor(Qt::WaitCursor);

  m_isDrawing = true;

  // Cleaning...
  m_displayPixmap->fill(m_backgroundColor);

  QPainter painter(m_displayPixmap);

  std::list<te::map::AbstractLayerPtr>::iterator it;

  for(it = m_layerList.begin(); it != m_layerList.end(); ++it) // for each layer
    draw(it->get(), painter);

  m_isDrawing = false;

  update();
}

unsigned int te::qt::widgets::MapDisplay::getWidth() const
{
  return static_cast<unsigned int>(width());
}

unsigned int te::qt::widgets::MapDisplay::getHeight() const
{
  return static_cast<unsigned int>(height());
}

double te::qt::widgets::MapDisplay::getWidthMM() const
{
  return static_cast<double>(widthMM());
}

double te::qt::widgets::MapDisplay::getHeightMM() const
{
  return static_cast<double>(heightMM());
}

void te::qt::widgets::MapDisplay::setSRID(const int& srid, bool doRefresh)
{
  te::map::MapDisplay::setSRID(srid, doRefresh);
  emit displaySridChanged();
}

QPixmap* te::qt::widgets::MapDisplay::getDisplayPixmap() const
{
  return m_displayPixmap;
}

QPixmap* te::qt::widgets::MapDisplay::getDraftPixmap() const
{
  return m_draftPixmap;
}

void te::qt::widgets::MapDisplay::setResizePolicy(const ResizePolicy& policy)
{
  m_resizePolicy = policy;
}

void te::qt::widgets::MapDisplay::setResizeInterval(int msec)
{
  m_interval = msec;
}

void te::qt::widgets::MapDisplay::draw(te::map::AbstractLayer* layer, QPainter& painter)
{
  // Checking the visibility...
  if(layer->getVisibility() == te::map::NOT_VISIBLE)
    return;

  // Recursive draw
  for(std::size_t i = 0; i < layer->getChildrenCount(); ++i)
    draw(boost::dynamic_pointer_cast<te::map::AbstractLayer>(layer->getChild(i)).get(), painter);
  
  // Retrieves a canvas to current layer
  te::qt::widgets::Canvas* canvas = getCanvas(layer);

  // Draw the current layer
  try
  {
    layer->draw(canvas, m_extent, m_srid);
  }
  catch(...)
  {
    return;
  }

  // Compose the result
  QPaintDevice* device = canvas->getDevice();

  if(device->devType() == QInternal::Pixmap)
    painter.drawPixmap(0, 0, *static_cast<QPixmap*>(device));
  else if(device->devType() == QInternal::Image)
    painter.drawImage(0, 0, *static_cast<QImage*>(device));
}

te::qt::widgets::Canvas* te::qt::widgets::MapDisplay::getCanvas(te::map::AbstractLayer* layer, int type)
{
  // Is there a canvas associated with the given layer?
  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it = m_layerCanvasMap.find(layer);
  if(it != m_layerCanvasMap.end())
    return it->second;

  // else, create one!
  te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(m_displayPixmap->width(), m_displayPixmap->height(), type);
  canvas->calcAspectRatio(m_extent.m_llx, m_extent.m_lly, m_extent.m_urx, m_extent.m_ury, m_hAlign, m_vAlign);
  canvas->setWindow(m_extent.m_llx, m_extent.m_lly, m_extent.m_urx, m_extent.m_ury);
  canvas->clear();

  m_layerCanvasMap[layer] = canvas;
  
  return canvas;
}

void te::qt::widgets::MapDisplay::resizeAllCanvas()
{
  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
    it->second->resize(m_displayPixmap->width(), m_displayPixmap->height());
}

void te::qt::widgets::MapDisplay::paintEvent(QPaintEvent* e)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, *m_displayPixmap);

  emit displayPaintEvent(&painter);

  painter.drawPixmap(0, 0, *m_draftPixmap);
}

void te::qt::widgets::MapDisplay::resizeEvent(QResizeEvent* e)
{
  QWidget::resizeEvent(e);

  if(m_interval == 0)
    onResizeTimeout();
  else
  {
    // Stores the old size
    if(!m_oldSize.isValid())
      e->oldSize().isValid() ? m_oldSize = e->oldSize() : m_oldSize = e->size();

    // Setups the timer controller
    m_timer->start(m_interval);
  }
}

QPointF te::qt::widgets::MapDisplay::transform(const QPointF& p)
{
  if(m_layerCanvasMap.empty())
    return QPointF();

  te::qt::widgets::Canvas* canvas = m_layerCanvasMap.begin()->second;
  if(canvas == 0)
    return QPointF();

  return canvas->getMatrix().inverted().map(p);
}

QColor te::qt::widgets::MapDisplay::getBackgroundColor()
{
  return m_backgroundColor;
}

void te::qt::widgets::MapDisplay::setBackgroundColor(const QColor& color)
{
  m_backgroundColor = color;
}

bool te::qt::widgets::MapDisplay::isDrawing() const
{
  return m_isDrawing;
}

void te::qt::widgets::MapDisplay::onResizeTimeout()
{
  // Rebulding the map display pixmaps
  delete m_displayPixmap;
  m_displayPixmap = new QPixmap(size());
  m_displayPixmap->fill(m_backgroundColor);

  delete m_draftPixmap;
  m_draftPixmap = new QPixmap(size());
  m_draftPixmap->fill(Qt::transparent);

  // Resizing all canvas
  resizeAllCanvas();

  // Adjust the extent based on resize policy
  adjustExtent(m_oldSize, size());

  // Invalidate old size
  m_oldSize = QSize();
}

void te::qt::widgets::MapDisplay::adjustExtent(const QSize& oldSize, const QSize& size)
{
  if(!m_extent.isValid())
  {
    update();
    return;
  }

  te::gm::Envelope e = m_extent;
  //if(m_resizePolicy == te::qt::widgets::MapDisplay::Fixed)
  //  setExtent(e);
  if(m_resizePolicy == te::qt::widgets::MapDisplay::Fixed)
  {
    setExtent(e);
    return;
  }

  double widthW = e.m_urx - e.m_llx;
  double heightW = e.m_ury - e.m_lly;

  double newWidthW = (size.width() * widthW) / oldSize.width();
  double newHeightW = (size.height() * heightW) / oldSize.height();

  switch(m_resizePolicy)
  {
    case te::qt::widgets::MapDisplay::Cut:
    {
      e.m_urx = e.m_llx + newWidthW;
      e.m_lly = e.m_ury - newHeightW;
    }
    break;

    case te::qt::widgets::MapDisplay::Center:
    {
      te::gm::Coord2D center = e.getCenter();
      e.m_llx = center.x - (newWidthW * 0.5);
      e.m_urx = center.x + (newWidthW * 0.5);
      e.m_lly = center.y - (newHeightW * 0.5);
      e.m_ury = center.y + (newHeightW * 0.5);
    }
    break;

    default:
    break;
  }

  setExtent(e);
}
double te::qt::widgets::MapDisplay::getScale() {

  double wPixels = this->getWidth();
  double hPixels = this->getHeight();
  double wMM = this->getWidthMM();
  double hMM = this->getHeightMM();

  te::gm::Envelope envelope = this->getExtent();
  double wdx = envelope.getUpperRightX() - envelope.getLowerLeftX();
  double wdy = envelope.getLowerLeftY() - envelope.getUpperRightY();

  double dx = wPixels / wdx, dy = hPixels / wdy, f = (dx > dy) ? dx : dy;

  double wT = wMM;

  te::common::UnitOfMeasurePtr unitPtr =
      te::srs::SpatialReferenceSystemManager::getInstance().getUnit(m_srid);

  if (unitPtr != NULL) {
    std::string unit = unitPtr->getName();
    if (unit == "METRE")
      wT = wMM / 1000.;
    else if (unit == "KILOMETRE")
      wT = wMM / 1000000.;
    else if (unit == "FOOT")
      wT = wMM / (12. * 25.4);
    else if (unit == "DEGREE")
      wT = wMM / 110000000.;

    double wp = wT / wPixels;
    m_scale = (1. / f) / wp;
  }

  return m_scale;
}
void te::qt::widgets::MapDisplay::setScale(const double& scale)
{
  return;
}

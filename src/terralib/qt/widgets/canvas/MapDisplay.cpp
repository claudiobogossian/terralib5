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
  \file MapDisplay.cpp

  \brief A widget to control the display of a set of layers.
 */

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../common/TreeItem.h"
#include "../../../geometry/Envelope.h"
#include "../../../srs/Config.h"
#include "../../../dataaccess.h"
#include "../../../maptools.h"

#include "MapDisplay.h"

// Qt
#include <QtGui/QResizeEvent>
#include <QtGui/QApplication>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QString>

te::qt::widgets::MapDisplay::MapDisplay(int w, int h, QWidget* parent, Qt::WindowFlags f)
  : te::map::MapDisplay(),
    QWidget(parent, f),
    m_displayPixmap(0),
    m_backgroundColor(Qt::white),
    m_repaint(true),
    m_resize(false),
    m_hAlign(te::map::Center),
    m_vAlign(te::map::Center),
    m_timer(0)

{
  setObjectName("MapDisplay");
  resize(w, h);
  m_displayPixmap = new QPixmap(w, h);

  m_menu = new QMenu(this);

  m_srsAction = new QAction("&Set SRID", m_menu);
  m_menu->addAction(m_srsAction);
  connect(m_srsAction, SIGNAL(triggered()), this, SLOT(setSRIDSlot()));

  m_fitAllLayersAction = new QAction("&Fit All Layers", m_menu);
  m_menu->addAction(m_fitAllLayersAction);
  connect(m_fitAllLayersAction, SIGNAL(triggered()), this, SLOT(fitAllLayersSlot()));
}

te::qt::widgets::MapDisplay::~MapDisplay()
{
  delete m_displayPixmap;
  te::common::FreeContents(m_layerCanvasMap);
  delete m_srsAction;
  delete m_fitAllLayersAction;
  delete m_menu;
}

void te::qt::widgets::MapDisplay::setAlign(const te::map::AlignType& h, const te::map::AlignType& v)
{
  te::map::MapDisplay::setAlign(h, v);
  m_hAlign = h;
  m_vAlign = v;
  setExtent(m_envelope);
  draw();
}

void te::qt::widgets::MapDisplay::setExtent(const te::gm::Envelope& e)
{
  m_envelope = e;
  te::map::MapDisplay::setExtent(e);

  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;

  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
  {
    te::qt::widgets::Canvas *c = it->second;
    c->calcAspectRatio(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury, m_hAlign, m_vAlign);
    c->setWindow(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury);
    c->clear();
  }

//  draw();
}

void te::qt::widgets::MapDisplay::draw()
{
  try
  {
    if(m_displayPixmap)
    {
      m_displayPixmap->fill(m_backgroundColor);

      if(m_layerTree) // Use the tree if it exists, otherwise use the list of layers.
      {
        std::list<te::map::AbstractLayer*> layerList;
        mountLayerList(m_layerTree, layerList);
        draw(layerList);
      }
      else if(m_layerList.empty() == false)
        draw(m_layerList);
    }
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void te::qt::widgets::MapDisplay::draw(std::list<te::map::AbstractLayer*>& layerList)
{
  try
  {
    QPainter painter(m_displayPixmap);
    std::list<te::map::AbstractLayer*>::iterator it;
 
    if(m_srid == -1 || m_extent == 0)
    {
      if(m_extent == 0)
        m_extent = new te::gm::Envelope();
      for(it = layerList.begin(); it != layerList.end(); ++it)
      {
        te::gm::Envelope env = getLayerExtent(*it);

        int srid = (*it)->getSRID();
        if(m_srid == -1)
          m_srid = srid;
        if(srid != m_srid)
          env.transform(srid, m_srid);

        m_extent->Union(env);
      }
      if(m_extent->isValid() == false)
      {
        delete m_extent;
        m_extent = 0;
        return;
      }
      m_envelope = *m_extent;
    }
    if(m_srid == -1)
      return;

    for(it = layerList.begin(); it != layerList.end(); ++it)
    {
      te::qt::widgets::Canvas *c = getCanvas(*it);
      if(c == 0)
      {
        setCanvas(*it);
        c = getCanvas(*it);
        c->calcAspectRatio(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury, m_hAlign, m_vAlign);
        c->setWindow(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury);
      }
      draw(*it);

      if(m_displayPixmap->size() == c->getPixmap()->size())
        painter.drawPixmap(0, 0, *(c->getPixmap()));
    }
    update();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void te::qt::widgets::MapDisplay::draw(te::map::AbstractLayer* al)
{
  try
  {
    setWaitCursor();
    te::map::Layer* layer = (te::map::Layer*)al;
    te::qt::widgets::Canvas* canvas = getCanvas(layer);
    canvas->clear();

    if(layer->getRenderer() == 0)
    {
      te::map::LayerRenderer* renderer = new te::map::LayerRenderer();
      layer->setRenderer(renderer);
    }
    layer->draw(canvas, *m_extent, m_srid);
    unsetWaitCursor();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void te::qt::widgets::MapDisplay::mountLayerList(te::map::AbstractLayer* al, std::list<te::map::AbstractLayer*>& layerList)
{
  try
  {
    if(al->getType() == "LAYER")
    {
      if(al->getVisibility() == te::map::VISIBLE)
        layerList.push_front(al);
    }

    te::map::AbstractLayer::iterator it;
    for(it = al->begin(); it != al->end(); ++it)
    {
      te::map::AbstractLayer* t = dynamic_cast<te::map::AbstractLayer*>(*it);
      mountLayerList(t, layerList);
    }
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

te::gm::Envelope te::qt::widgets::MapDisplay::getLayerExtent(te::map::AbstractLayer* al)
{
  try
  {
    te::map::Layer* layer = (te::map::Layer*)al;
    if(layer->getExtent() == 0)
    {
      te::da::DataSourceTransactor* transactor = layer->getDataSource()->getTransactor();
      te::da::DataSet* ds = transactor->getDataSet(layer->getId());
      te::gm::Envelope* dsEnv = ds->getExtent();
      layer->setExtent(dsEnv);
      delete ds;
      delete transactor;
    }
    return *(layer->getExtent());
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}


QPixmap* te::qt::widgets::MapDisplay::getDisplayPixmap()
{
  return m_displayPixmap;
}

void te::qt::widgets::MapDisplay::setRepaint(bool s)
{
  m_repaint = s;
}

void te::qt::widgets::MapDisplay::paintEvent(QPaintEvent* e)
{
  if(m_displayPixmap == 0)
    return;

  double w, h, nw, nh, sw, sh;
  QMatrix m;
  QPointF p1, p2;
  te::qt::widgets::Canvas *c = 0;

  if(m_resize == false)
  {
    if(m_repaint)
    {
      QPainter painter(this);
      QRect rec = e->rect();
      painter.drawPixmap(rec, *m_displayPixmap, rec);
    }
  }
  else // fazendo resize do display
  {
    if(m_layerCanvasMap.begin() != m_layerCanvasMap.end())
      c = m_layerCanvasMap.begin()->second;

    if(c == 0)
      return;

    m = c->getMatrix();

    p1 = m.map(QPointF(m_envelope.getLowerLeftX(), m_envelope.getLowerLeftY()));
    p2 = m.map(QPointF(m_envelope.getUpperRightX(), m_envelope.getUpperRightY()));
    m_resizeWRec = QRect(QPoint(p1.x(), p2.y()), QPoint(p2.x(), p1.y()));
    nw = m_resizeWRec.width();
    nh = m_resizeWRec.height();

    QPainter painter(this);
    m_resizeRec = rect();
    w = m_resizeRec.width();
    h = m_resizeRec.height();

    QPixmap pix(w, h);
    pix.fill();
    painter.drawPixmap(0, 0, pix);

    sh = h/m_resizeWRec.height();
    sw = w/m_resizeWRec.width();
    if(sw < sh)
    {
      nw *= sw;
      nh *= sw;
    }
    else if(sh < sw)
    {
      nw *= sh;
      nh *= sh;
    }

    if(m_hAlign == te::map::Center)
    {
      m_resizeRec.setLeft((w - nw) / 2);
      m_resizeRec.setRight(m_resizeRec.left() + nw);
    }
    else if(m_hAlign == te::map::Left)
      m_resizeRec.setRight(nw);
    else if(m_hAlign == te::map::Right)
      m_resizeRec.setLeft(m_resizeRec.width() - nw);

    if(m_vAlign == te::map::Center)
    {
      m_resizeRec.setTop((h - nh) / 2);
      m_resizeRec.setBottom(m_resizeRec.top() + nh);
    }
    else if(m_vAlign == te::map::Top)
      m_resizeRec.setBottom(nh);
    else if(m_vAlign == te::map::Bottom)
      m_resizeRec.setTop(m_resizeRec.height() - nh);

    painter.drawPixmap(m_resizeRec, *m_displayPixmap, m_resizeWRec);
  }
}

void te::qt::widgets::MapDisplay::resizeEvent(QResizeEvent* e)
{ 
  m_resize = true;
  QWidget::resizeEvent(e);

  if(m_timer)
  {
    m_timer->disconnect();
    delete m_timer;
  }
  m_timer = new QTimer;
  m_timer->setSingleShot(true);
  m_timer->start(500);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(drawTimerSlot()));
}

void te::qt::widgets::MapDisplay::drawTimerSlot()
{
  m_resize = false;
  if(m_displayPixmap)
    delete m_displayPixmap;
  m_displayPixmap = new QPixmap(width(), height());

  m_displayPixmap->fill(QColor(0, 0, 0, 0));
  int w = m_displayPixmap->width();
  int h = m_displayPixmap->height();

  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;

  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
  {
    te::qt::widgets::Canvas *c = it->second;
    c->resize(w, h);
    setExtent(m_envelope);
    c->calcAspectRatio(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury, m_hAlign, m_vAlign);
    c->setWindow(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury);
  }

  te::gm::Envelope env = getAllExtent();
  if(m_extent && env.contains(*m_extent))
    m_envelope = *m_extent;

  draw();
  emit sizeChanged(QSize(w, h));
}

void te::qt::widgets::MapDisplay::contextMenuEvent(QContextMenuEvent* c)
{
  m_menu->exec(c->globalPos());
}

te::qt::widgets::Canvas* te::qt::widgets::MapDisplay::getCanvas(te::map::AbstractLayer* layer)
{
  te::qt::widgets::Canvas* canvas = static_cast<te::qt::widgets::Canvas*>(te::common::GetPValue(m_layerCanvasMap, layer));
  return canvas;
}

void te::qt::widgets::MapDisplay::setCanvas(te::map::AbstractLayer* layer)
{
  if(m_layerCanvasMap.find(layer) == m_layerCanvasMap.end())
  {
    te::qt::widgets::Canvas* c = new te::qt::widgets::Canvas(m_displayPixmap->width(), m_displayPixmap->height());
    m_layerCanvasMap[layer] = c;
  }  
}

void te::qt::widgets::MapDisplay::setWaitCursor()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void te::qt::widgets::MapDisplay::unsetWaitCursor()
{
  QApplication::restoreOverrideCursor();
}

te::gm::Envelope te::qt::widgets::MapDisplay::getAllExtent()
{
  try
  {
    if(m_layerList.empty() == false)
      return getAllExtent(m_layerList);

    std::list<te::map::AbstractLayer*> layerList;
    if(m_layerTree)
      mountLayerList(m_layerTree, layerList);
    return getAllExtent(layerList);
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

te::gm::Envelope te::qt::widgets::MapDisplay::getAllExtent(std::list<te::map::AbstractLayer*>& layerList)
{
  try
  {
    std::list<te::map::AbstractLayer*>::iterator it;

    te::gm::Envelope extent;
    for(it = layerList.begin(); it != layerList.end(); ++it)
    {
      te::gm::Envelope env = getLayerExtent(*it);

      int srid = (*it)->getSRID();
      if(srid > 0)
      {
        if(m_srid == -1)
          m_srid = srid;
        if(srid != m_srid)
          env.transform(srid, m_srid);
      }

      extent.Union(env);
    }
    return extent;
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void te::qt::widgets::MapDisplay::fitAllLayersSlot()
{
  try
  {
    if(m_layerTree)
    {
      std::list<te::map::AbstractLayer*> layerList;
      mountLayerList(m_layerTree, layerList);
      fit(layerList);
    }
    else if(m_layerList.empty() == false)
      fit(m_layerList);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::information(this, tr("Error drawing..."), tr(e.what()));
    return;
  }
}

void te::qt::widgets::MapDisplay::fit(std::list<te::map::AbstractLayer*>& layerList)
{
  try
  {
    std::list<te::map::AbstractLayer*>::iterator it;

    delete m_extent;
    m_extent = new te::gm::Envelope();
    for(it = layerList.begin(); it != layerList.end(); ++it)
    {
      te::gm::Envelope env = getLayerExtent(*it);

      int srid = (*it)->getSRID();
      if(m_srid == -1)
        m_srid = srid;
      if(srid != m_srid)
      {
        env.transform(srid, m_srid);
      }

      m_extent->Union(env);
    }
    setExtent(*m_extent);
    draw();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void te::qt::widgets::MapDisplay::setSRIDSlot()
{
  //int a = getSRID();
  //QString sid;
  //sid.setNum(a);

  //QStringList items;
  ////items << "4326" << "29181" << "29183" << "29101" << "3031";

  //items.append(sid);
  //if(sid != "4326")
  //  items.append("4326");
  //if(sid != "29181")
  //  items.append("29181");
  //if(sid != "29183")
  //  items.append("29183");
  //if(sid != "29101")
  //  items.append("29101");
  //if(sid != "3031")
  //  items.append("3031");

  //bool ok;
  //QString item = QInputDialog::getItem(this, "Config SRID", "SRID:", items, 0, false, &ok);

  //if(ok && !items.isEmpty())
  //{
  //  int srid = atoi(item.toStdString().c_str());
  //  setSRID(srid);
  //  //te::map::MapDisplay::setSRID(srid);
  //  draw();
  //}

  int srid = getSRID();
  std::map<int, QString> sridMap;
  std::map<int, QString>::iterator it;

  sridMap[TE_SRS_SAD69] = "TE_SRS_SAD69";
  sridMap[TE_SRS_CORREGO_ALEGRE] = "TE_SRS_CORREGO_ALEGRE";
  sridMap[TE_SRS_WGS84] = "TE_SRS_WGS84";
  sridMap[TE_SRS_SIRGAS2000] = "TE_SRS_SIRGAS2000";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S";
  sridMap[TE_SRS_SAD69_POLYCONIC] = "TE_SRS_SAD69_POLYCONIC";
  sridMap[TE_SRS_SAD69_UTM_ZONE_18N] = "TE_SRS_SAD69_UTM_ZONE_18N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_19N] = "TE_SRS_SAD69_UTM_ZONE_19N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_20N] = "TE_SRS_SAD69_UTM_ZONE_20N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_21N] = "TE_SRS_SAD69_UTM_ZONE_21N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_22N] = "TE_SRS_SAD69_UTM_ZONE_22N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_17S] = "TE_SRS_SAD69_UTM_ZONE_17S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_18S] = "TE_SRS_SAD69_UTM_ZONE_18S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_19S] = "TE_SRS_SAD69_UTM_ZONE_19S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_20S] = "TE_SRS_SAD69_UTM_ZONE_20S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_21S] = "TE_SRS_SAD69_UTM_ZONE_21S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_22S] = "TE_SRS_SAD69_UTM_ZONE_22S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_23S] = "TE_SRS_SAD69_UTM_ZONE_23S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_24S] = "TE_SRS_SAD69_UTM_ZONE_24S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_25S] = "TE_SRS_SAD69_UTM_ZONE_25S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_17N] = "TE_SRS_SIRGAS2000_UTM_ZONE_17N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_18N] = "TE_SRS_SIRGAS2000_UTM_ZONE_18N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_19N] = "TE_SRS_SIRGAS2000_UTM_ZONE_19N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_20N] = "TE_SRS_SIRGAS2000_UTM_ZONE_20N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_21N] = "TE_SRS_SIRGAS2000_UTM_ZONE_21N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_22N] = "TE_SRS_SIRGAS2000_UTM_ZONE_22N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_17S] = "TE_SRS_SIRGAS2000_UTM_ZONE_17S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_18S] = "TE_SRS_SIRGAS2000_UTM_ZONE_18S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_19S] = "TE_SRS_SIRGAS2000_UTM_ZONE_19S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_20S] = "TE_SRS_SIRGAS2000_UTM_ZONE_20S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_21S] = "TE_SRS_SIRGAS2000_UTM_ZONE_21S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_22S] = "TE_SRS_SIRGAS2000_UTM_ZONE_22S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_23S] = "TE_SRS_SIRGAS2000_UTM_ZONE_23S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_24S] = "TE_SRS_SIRGAS2000_UTM_ZONE_24S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_25S] = "TE_SRS_SIRGAS2000_UTM_ZONE_25S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_18N] = "TE_SRS_WGS84_UTM_ZONE_18N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_19N] = "TE_SRS_WGS84_UTM_ZONE_19N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_20N] = "TE_SRS_WGS84_UTM_ZONE_20N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_21N] = "TE_SRS_WGS84_UTM_ZONE_21N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_22N] = "TE_SRS_WGS84_UTM_ZONE_22N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_17S] = "TE_SRS_WGS84_UTM_ZONE_17S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_18S] = "TE_SRS_WGS84_UTM_ZONE_18S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_19S] = "TE_SRS_WGS84_UTM_ZONE_19S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_20S] = "TE_SRS_WGS84_UTM_ZONE_20S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_21S] = "TE_SRS_WGS84_UTM_ZONE_21S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_22S] = "TE_SRS_WGS84_UTM_ZONE_22S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_23S] = "TE_SRS_WGS84_UTM_ZONE_23S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_24S] = "TE_SRS_WGS84_UTM_ZONE_24S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_25S] = "TE_SRS_WGS84_UTM_ZONE_25S";
  sridMap[TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC] = "TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC";

  QStringList items;
  for(it = sridMap.begin(); it != sridMap.end(); ++it)
  {
    if(it->first == srid)
    {
      items.append(it->second);
      break;
    }
  }

  for(it = sridMap.begin(); it != sridMap.end(); ++it)
  {
    if(it->first != srid)
      items.append(it->second);
  }

  bool ok;
  QString item = QInputDialog::getItem(this, "Config SRID", "SRID:", items, 0, false, &ok);

  if(ok && !items.isEmpty())
  {
    for(it = sridMap.begin(); it != sridMap.end(); ++it)
    {
      if(item == it->second)
      {
        srid = it->first;
        setSRID(srid);
        draw();
        break;
      }
    }
  }
}

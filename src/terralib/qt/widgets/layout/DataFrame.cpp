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
  \file terralib/qt/widgets/canvas/DataFrame.h

  \brief The data frame....
*/

// TerraLib
#include "DataFrame.h"
#include "LayoutEditor.h"
#include "GeographicGridFrame.h"
#include "UTMGridFrame.h"
#include "GraphicScaleFrame.h"

#include <terralib/geometry.h>
//#include <terralib/postgis.h>
//#include <terralib/dataaccess.h>
#include <terralib/srs/Config.h>
#include <terralib/srs/Converter.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// Qt
#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QToolTip>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/QWidget>
#include <QtGui/QFontDatabase>
#include <QtGui/QResizeEvent>
#include <QtGui/QBoxLayout>

#include <QtCore/QRect>
#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>

te::qt::widgets::DataFrame::DataFrame(const QRectF& frameRect, te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f) :
  te::qt::widgets::Frame(me, f),
  m_dataUnitToMilimeter(-1),
  m_scale(-1),
  m_UTMGridFrame(0),
  m_geoGridFrame(0),
  m_graphicScaleFrame(0),
  m_mapDisplay(0)
{
  setWindowTitle("DataFrame");
  setMouseTracking(true);

  adjustWidgetToFrameRect(frameRect);
  show();

  m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(QSize(10, 10), true, this, f);
  m_mapDisplay->setAcceptDrops(true);
  m_mapDisplay->setResizeInterval(0);
  m_mapDisplay->setMouseTracking(true);
  m_mapDisplay->installEventFilter(this);
  QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
  layout->addWidget(m_mapDisplay);
  setLayout(layout);

  m_mapDisplay->show();

  m_menu = new QMenu(this);
  m_createUTMGridAction = m_menu->addAction("Create UTM Grid");
  m_removeUTMGridAction = m_menu->addAction("Remove UTM Grid");
  m_removeUTMGridAction->setEnabled(false);
  m_createGeographicGridAction = m_menu->addAction("Create Geographic Grid");
  m_removeGeographicGridAction = m_menu->addAction("Remove Geographic Grid");
  m_removeGeographicGridAction->setEnabled(false);
  m_createGraphicScaleAction = m_menu->addAction("Create Graphic Scale");
  m_removeGraphicScaleAction = m_menu->addAction("Remove Graphic Scale");
  m_removeGraphicScaleAction->setEnabled(false);
}

te::qt::widgets::DataFrame::DataFrame(const DataFrame& rhs) :
  te::qt::widgets::Frame(rhs.m_layoutEditor, rhs.windowFlags()),
  m_dataUnitToMilimeter(rhs.m_dataUnitToMilimeter)
{
  te::qt::widgets::Frame::operator=(rhs);
  setWindowTitle("DataFrame");
  setMouseTracking(true);
  m_dataRect = rhs.m_dataRect;
  m_dataChanged = rhs.m_dataChanged;
  m_scale = rhs.m_scale;
  m_mapDisplay = 0;
  m_UTMGridFrame = 0;
  m_geoGridFrame = 0;
  m_graphicScaleFrame = 0;

  te::map::AbstractLayerPtr m_data = rhs.m_data;

  adjust();
  show();
  m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(rect().size(), true, this, rhs.windowFlags());
  m_mapDisplay->setAcceptDrops(true);
  m_mapDisplay->setAlign(rhs.m_mapDisplay->getHAlign(), rhs.m_mapDisplay->getVAlign());
  m_mapDisplay->setSRID(rhs.m_mapDisplay->getSRID());
  m_mapDisplay->setResizeInterval(0);
  m_mapDisplay->setMouseTracking(true);
  m_mapDisplay->installEventFilter(this);
  QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
  layout->addWidget(m_mapDisplay);
  setLayout(layout);

  m_mapDisplay->show();
  te::gm::Envelope env = rhs.m_mapDisplay->getExtent();
  m_mapDisplay->setExtent(env, false);

  if(rhs.m_UTMGridFrame)
  {
    m_UTMGridFrame = new te::qt::widgets::UTMGridFrame(*rhs.m_UTMGridFrame);
    m_UTMGridFrame->setDataFrame(this);
  }

  if(rhs.m_geoGridFrame)
  {
    m_geoGridFrame = new te::qt::widgets::GeographicGridFrame(*rhs.m_geoGridFrame);
    m_geoGridFrame->setDataFrame(this);
  }

  if(rhs.m_graphicScaleFrame)
  {
    m_graphicScaleFrame = new te::qt::widgets::GraphicScaleFrame(*rhs.m_graphicScaleFrame);
    m_graphicScaleFrame->setDataFrame(this);
  }
  show();

  m_menu = new QMenu(this);
  m_createUTMGridAction = m_menu->addAction("Create UTM Grid");
  m_removeUTMGridAction = m_menu->addAction("Remove UTM Grid");
  m_createGeographicGridAction = m_menu->addAction("Create Geographic Grid");
  m_removeGeographicGridAction = m_menu->addAction("Remove Geographic Grid");
  m_createGraphicScaleAction = m_menu->addAction("Create Graphic Scale");
  m_removeGraphicScaleAction = m_menu->addAction("Remove Graphic Scale");

  m_createUTMGridAction->setEnabled(rhs.m_createUTMGridAction->isEnabled());
  m_removeUTMGridAction->setEnabled(rhs.m_removeUTMGridAction->isEnabled());
  m_createGeographicGridAction->setEnabled(rhs.m_createGeographicGridAction->isEnabled());
  m_removeGeographicGridAction->setEnabled(rhs.m_removeGeographicGridAction->isEnabled());
  m_createGraphicScaleAction->setEnabled(rhs.m_createGraphicScaleAction->isEnabled());
  m_removeGraphicScaleAction->setEnabled(rhs.m_removeGraphicScaleAction->isEnabled());
}

te::qt::widgets::DataFrame& te::qt::widgets::DataFrame::operator=(const DataFrame& rhs)
{
  if(this != &rhs)
  {
    te::qt::widgets::Frame::operator=(rhs);
    setWindowTitle("DataFrame");
    setMouseTracking(true);
    m_dataUnitToMilimeter = rhs.m_dataUnitToMilimeter;
    m_dataRect = rhs.m_dataRect;
    m_dataChanged = rhs.m_dataChanged;
    m_scale = rhs.m_scale;
    m_mapDisplay = 0;
    m_UTMGridFrame = 0;
    m_geoGridFrame = 0;
    m_graphicScaleFrame = 0;

    te::map::AbstractLayerPtr m_data = rhs.m_data;

    adjust();
    show();
    m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(rect().size(), true, this, rhs.windowFlags());
    m_mapDisplay->setAcceptDrops(true);
    m_mapDisplay->setAlign(rhs.m_mapDisplay->getHAlign(), rhs.m_mapDisplay->getVAlign());
    m_mapDisplay->setSRID(rhs.m_mapDisplay->getSRID());
    m_mapDisplay->setResizeInterval(0);
    m_mapDisplay->setMouseTracking(true);
    m_mapDisplay->installEventFilter(this);
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    layout->addWidget(m_mapDisplay);
    setLayout(layout);

    m_mapDisplay->show();
    te::gm::Envelope env = rhs.m_mapDisplay->getExtent();
    m_mapDisplay->setExtent(env, false);

    if(rhs.m_UTMGridFrame)
    {
      m_UTMGridFrame = new te::qt::widgets::UTMGridFrame(*rhs.m_UTMGridFrame);
      m_UTMGridFrame->setDataFrame(this);
    }

    if(rhs.m_geoGridFrame)
    {
      m_geoGridFrame = new te::qt::widgets::GeographicGridFrame(*rhs.m_geoGridFrame);
      m_geoGridFrame->setDataFrame(this);
    }

    if(rhs.m_graphicScaleFrame)
    {
      m_graphicScaleFrame = new te::qt::widgets::GraphicScaleFrame(*rhs.m_graphicScaleFrame);
      m_graphicScaleFrame->setDataFrame(this);
    }
    show();

    m_menu = new QMenu(this);
    m_createUTMGridAction = m_menu->addAction("Create UTM Grid");
    m_removeUTMGridAction = m_menu->addAction("Remove UTM Grid");
    m_createGeographicGridAction = m_menu->addAction("Create Geographic Grid");
    m_removeGeographicGridAction = m_menu->addAction("Remove Geographic Grid");
    m_createGraphicScaleAction = m_menu->addAction("Create Graphic Scale");
    m_removeGraphicScaleAction = m_menu->addAction("Remove Graphic Scale");

    m_createUTMGridAction->setEnabled(rhs.m_createUTMGridAction->isEnabled());
    m_removeUTMGridAction->setEnabled(rhs.m_removeUTMGridAction->isEnabled());
    m_createGeographicGridAction->setEnabled(rhs.m_createGeographicGridAction->isEnabled());
    m_removeGeographicGridAction->setEnabled(rhs.m_removeGeographicGridAction->isEnabled());
    m_createGraphicScaleAction->setEnabled(rhs.m_createGraphicScaleAction->isEnabled());
    m_removeGraphicScaleAction->setEnabled(rhs.m_removeGraphicScaleAction->isEnabled());
  }
  return *this;
}

te::qt::widgets::DataFrame::~DataFrame()
{
  delete m_mapDisplay;
  delete m_UTMGridFrame;
  delete m_geoGridFrame;
  delete m_graphicScaleFrame;
}

te::qt::widgets::MultiThreadMapDisplay* te::qt::widgets::DataFrame::getMapDisplay()
{
  return m_mapDisplay;
}

void te::qt::widgets::DataFrame::setDataRect(QRectF& r)
{
  if(r != m_dataRect)
  {
    m_dataRect = r;
    te::gm::Envelope e(r.left(), r.top(), r.right(), r.bottom());
    m_mapDisplay->setExtent(e);
  }
}

QRectF te::qt::widgets::DataFrame::getDataRect()
{
  te::gm::Envelope e = m_mapDisplay->getExtent();
  m_dataRect = QRectF(e.m_llx, e.m_lly, e.getWidth(), e.getHeight());
  return m_dataRect;
}
//
//QRectF te::qt::widgets::DataFrame::getFrameRect()
//{
//  return m_frameRect;
//}

void te::qt::widgets::DataFrame::adjust()
{
  adjustWidgetToFrameRect(m_frameRect);
  if(m_UTMGridFrame)
    m_UTMGridFrame->adjust();
  if(m_geoGridFrame)
    m_geoGridFrame->adjust();
  if(m_graphicScaleFrame)
    m_graphicScaleFrame->adjust();
}

void te::qt::widgets::DataFrame::adjustWidgetToFrameRect(const QRectF& r)
{
  //if(m_frameRect == r)
  //  return;

  m_frameRect = r;
  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  QRect rec = matrix.mapRect(m_frameRect).toRect();

  QWidget::resize(rec.width()+19, rec.height()+19);
  QWidget::move(rec.left()-9, rec.top()-9);
  //QWidget::resize(rec.width()+18, rec.height()+18);
  //QWidget::move(rec.left()-8, rec.top()-7);

  double pw = rec.width();
  double ph = rec.height();
  int ipw = qRound(pw);
  int iph = qRound(ph);
  int ipw2 = qRound(pw/2.);
  int iph2 = qRound(ph/2.);
  double w = 9;
  double h = 9;

  if(pw < w*5)
    w = pw / 5.;
  if(ph < h*5)
    h = ph / 5.;

  int iw = qRound(w);
  int ih = qRound(h);
  int iw2 = qRound(w/2.);
  int ih2 = qRound(h/2.);

  m_recSel1 = QRect(0, 0, iw, ih);
  m_recSel2 = QRect(ipw2 - iw2, 0, iw, ih);
  m_recSel3 = QRect(ipw - iw, 0, iw, ih);
  m_recSel4 = QRect(ipw - iw, iph2 - ih2, iw, ih);
  m_recSel5 = QRect(ipw - iw, iph - ih, iw, ih);
  m_recSel6 = QRect(ipw2 - iw2, iph - ih, iw, ih);
  m_recSel7 = QRect(0, iph - ih, iw, ih);
  m_recSel8 = QRect(0, iph2 - ih2, iw, ih);
  //m_recSel9 = QRect(ipw2 - iw2, iph2 - ih2, iw, ih);
  m_recSel10 = QRect(0, 0, ipw, iph);
}

double te::qt::widgets::DataFrame::getScale()
{
  return m_scale;
}

void te::qt::widgets::DataFrame::setScale(double v)
{
  if(m_scale == v)
    return;

  m_scale = v;
  double toMeter = m_dataUnitToMilimeter / 1000;
  te::gm::Envelope env = m_mapDisplay->getExtent();
  double w = m_frameRect.width();
  double h = m_frameRect.height();
  double ew = env.getWidth() * toMeter;
  double eh = env.getHeight() * toMeter;
  double ecx = env.getLowerLeftX() * toMeter + ew / 2.;
  double ecy = env.getLowerLeftY() * toMeter + eh / 2.;
  double ww = w * m_scale / 1000;
  double wh = h * m_scale / 1000;
  double exi = (ecx - ww / 2.) / toMeter;
  double eyi = (ecy - wh / 2.) / toMeter;
  te::gm::Envelope env2(exi, eyi, exi + ww/toMeter, eyi + wh/toMeter);
  m_dataRect = QRectF(QPointF(exi, eyi), QPointF(exi + ww/toMeter, eyi + wh/toMeter));
  m_mapDisplay->setExtent(env2);
  if(m_graphicScaleFrame)
    m_graphicScaleFrame->setStep(0);
  adjust();
}

te::map::AbstractLayerPtr te::qt::widgets::DataFrame::getData()
{
  return m_data;
}

void te::qt::widgets::DataFrame::setData(te::map::AbstractLayerPtr d)
{
  m_data = d;
  m_mapDisplay->changeData(m_data);

  if(m_data == 0)
    return;

  te::gm::Envelope e = m_data->getExtent();
  m_dataRect = QRectF();
  QRectF r(e.getLowerLeftX(), e.getLowerLeftY(), e.getWidth(), e.getHeight());
  setDataRect(r);

  te::gm::Envelope env = m_mapDisplay->getExtent();
  findDataUnitToMilimeter(env, m_mapDisplay->getSRID());

  double w = m_frameRect.width();
  double ww = m_dataUnitToMilimeter * env.getWidth();
  m_scale = ww / w;
  setDataChanged(true);
}

void te::qt::widgets::DataFrame::setDataChanged(bool b)
{
  m_dataChanged = b;
}

bool te::qt::widgets::DataFrame::transformEnvelope(te::gm::Envelope& e, int oldsrid, int newsrid)
{
  // este codigo alem de converter ele mantem a escala inalterada.
  if(oldsrid == newsrid)
    return true;

  te::gm::Envelope oldEnv(e);

  try
  {
    double x, y, xmin, xmax, ymin, ymax;
  
    std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());

    converter->setSourceSRID(oldsrid);
    converter->setTargetSRID(newsrid);

    x = e.m_llx;
    y = e.m_lly;
    converter->convert(x, y);
    xmin = x;
    ymin = y;
    xmax = x;
    ymax = y;

    x = e.m_llx;
    y = e.m_ury;
    converter->convert(x, y);
    if(x < xmin)
      xmin = x;
    if(y < ymin)
      ymin = y;
    if(x > xmax)
      xmax = x;
    if(y > ymax)
      ymax = y;

    x = e.m_urx;
    y = e.m_ury;
    converter->convert(x, y);
    if(x < xmin)
      xmin = x;
    if(y < ymin)
      ymin = y;
    if(x > xmax)
      xmax = x;
    if(y > ymax)
      ymax = y;

    x = e.m_urx;
    y = e.m_lly;
    converter->convert(x, y);
    if(x < xmin)
      xmin = x;
    if(y < ymin)
      ymin = y;
    if(x > xmax)
      xmax = x;
    if(y > ymax)
      ymax = y;

    // faca em funcao do centro do box... mantenha o centro do box inalterado

    double cx = e.m_llx + e.getWidth() / 2.;
    double cy = e.m_lly + e.getHeight() / 2.;
    converter->convert(cx, cy);

    if(std::numeric_limits<double>::has_infinity)
    {
      if(xmin == std::numeric_limits<double>::infinity() || ymin == std::numeric_limits<double>::infinity() ||
         xmax == std::numeric_limits<double>::infinity() || ymax == std::numeric_limits<double>::infinity())
        return false;
    }

    double w = xmax - xmin;
    double h = ymax - ymin;

    // faca a altura e a largura proporcional ao antigo box... oldEnv
    double xScale = w / oldEnv.getWidth();
    double yScale = h / oldEnv.getHeight();

    if(xScale > yScale)
      h *= (xScale / yScale);
    else
      w *= (yScale / xScale);

    xmin = cx - w / 2.;
    ymin = cy - h / 2.;
    xmax = cx + w / 2.;
    ymax = cy + h / 2.;

    e.init(xmin, ymin, xmax, ymax);
    if(e.getWidth() == 0. || e.getHeight() == 0.)
      return false;

    findDataUnitToMilimeter(e, newsrid);
    return true;
  }
  catch(std::exception&)
  {
    return false;
  }
}

void te::qt::widgets::DataFrame::findDataUnitToMilimeter(const te::gm::Envelope& e, int srid)
{
  //m_dataUnitToMilimeter = 1000;
  // OBS: quero manter a escala do mapa inalterada. Lembre que quando converte,
  // o box, pode aumentar de tamanho e isto muda a escala do mapa.
  // Vamos ver como podemos fazer isto...
  // primeior vamos descobrir qual o fator de conversao para milimetros... m_dataUnitToMilimeter
  // NOTA: acho que ser feito usando a informacao de unidade da projecao, mas, como eu nao sei fazer isto.
  // Vou fazer um codigo quebra galho......
  // para isto vou converter dataRect para um projetado que deve estar em metros.
  // Vou usar #define TE_SRS_SAD69_POLYCONIC 29101 que provavelmente tem unidade em metros.

  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
  converter->setSourceSRID(srid);
  converter->setTargetSRID(29101); // Brasil polyconic em metros

  double x1 = e.m_llx;
  double y1 = e.m_lly;
  double a1 = x1;
  double b1 = y1;
  converter->convert(a1, b1);
  double x2 = e.m_urx;
  double y2 = e.m_ury;
  double a2 = x2;
  double b2 = y2;
  converter->convert(a2, b2);

  // a conversao pode causar um leve giro no box, portanto,
  // vou usar a distancia entre os pontos da diagonal do box (ll e ur)
  double dx = (x2 - x1);
  double dy = (y2 - y1);
  double da = (a2 - a1);
  double db = (b2 - b1);

  double d = sqrt(dx * dx + dy * dy);
  double dd = sqrt(da * da + db * db);
  // Calcular o fator de conversao para milimetros 
  m_dataUnitToMilimeter = 1000 * dd / d;
  // esta fator (m_dataUnitToMilimeter) eh usado da seguinte forma: 
  // multiplique a distancia do projetado, por este fator, para se obter a distancia em milimetros.
}

double te::qt::widgets::DataFrame::getDataUnitToMilimeter()
{
  return m_dataUnitToMilimeter;
}

void te::qt::widgets::DataFrame::draw()
{
  if(m_dataChanged)
    m_mapDisplay->refresh();

  if(m_UTMGridFrame)
  {
    m_UTMGridFrame->draw();
    m_UTMGridFrame->update();
  }

  if(m_geoGridFrame)
  {
    m_geoGridFrame->draw();
    m_geoGridFrame->update();
  }

  if(m_graphicScaleFrame)
  {
    m_graphicScaleFrame->draw();
    m_graphicScaleFrame->update();
  }

  setDataChanged(false);
}

void te::qt::widgets::DataFrame::showSelectionPoints()
{
  // faz tudo em coordenadas do dispositivo

  QPixmap* pixmap = m_mapDisplay->getDraftPixmap();
  pixmap->fill(Qt::transparent);

  if(m_UTMGridFrame)
    m_UTMGridFrame->copyToDraftMapDisplay();
  if(m_geoGridFrame)
    m_geoGridFrame->copyToDraftMapDisplay();
  //if(m_UTMGridFrame || m_geoGridFrame)
  //  m_mapDisplay->refresh();

  QPainter painter(pixmap);

  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::blue);
  painter.setBrush(QColor(0, 0, 255, 120));
  painter.drawRect(m_recSel1);
  painter.drawRect(m_recSel2);
  painter.drawRect(m_recSel3);
  painter.drawRect(m_recSel4);
  painter.drawRect(m_recSel5);
  painter.drawRect(m_recSel6);
  painter.drawRect(m_recSel7);
  painter.drawRect(m_recSel8);
  //painter.drawRect(m_recSel9);
  painter.setBrush(QColor(0, 0, 255, 20));
  painter.drawRect(m_recSel10);  
  painter.end();
  m_mapDisplay->repaint();
}

void te::qt::widgets::DataFrame::hideSelectionPoints()
{
  QPixmap* pixmap = m_mapDisplay->getDraftPixmap();
  pixmap->fill(Qt::transparent);

  if(m_UTMGridFrame)
    m_UTMGridFrame->copyToDraftMapDisplay();
  if(m_geoGridFrame)
    m_geoGridFrame->copyToDraftMapDisplay();
  //if(m_UTMGridFrame || m_geoGridFrame)
  //  m_mapDisplay->refresh();

  m_mapDisplay->repaint();
}
void te::qt::widgets::DataFrame::getSelectionPoint(const QPoint& p)
{
  // faz tudo em coordenadas do dispositivo

  m_selected = 0;
  if(m_recSel1.contains(p))
    m_selected = 1;
  else if(m_recSel2.contains(p))
    m_selected = 2;
  else if(m_recSel3.contains(p))
    m_selected = 3;
  else if(m_recSel4.contains(p))
    m_selected = 4;
  else if(m_recSel5.contains(p))
    m_selected = 5;
  else if(m_recSel6.contains(p))
    m_selected = 6;
  else if(m_recSel7.contains(p))
    m_selected = 7;
  else if(m_recSel8.contains(p))
    m_selected = 8;
  //else if(m_recSel9.contains(p))
  //  m_selected = 9;
  else if(m_recSel10.contains(p))
    m_selected = 10;
}

void te::qt::widgets::DataFrame::verifyConstraints()
{
  //QSize size = m_layoutEditor->getPaperSize();
  //QRectF page(0, 0, size.width(), size.height());
  //QMatrix matrix = m_layoutEditor->getMatrix();
  //QRectF r(m_auxFrameRect.left(), m_auxFrameRect.top(), m_auxFrameRect.width(), m_auxFrameRect.height());
  //r = matrix.inverted().mapRect(r);
  //                 
  //if(page.contains(r) == false)
  //{
  //  if(m_selected == 10)
  //  {
  //    if(r.top() < page.top())
  //      r = QRectF(r.left(), page.top(), r.width(), r.height());
  //    if(r.bottom() > page.bottom())
  //      r = QRectF(r.left(), page.bottom() - r.height(), r.width(), r.height());
  //    if(r.left() < page.left())
  //      r = QRectF(page.left(), r.top(), r.width(), r.height());
  //    if(r.right() > page.right())
  //      r = QRectF(page.right() - r.width(), r.top(), r.width(), r.height());
  //  }
  //  else
  //  {
  //    if(r.top() < page.top())
  //      r.setTop(page.top());
  //    if(r.bottom() > page.bottom())
  //      r.setBottom(page.bottom());
  //    if(r.left() < page.left())
  //      r.setLeft(page.left());
  //    if(r.right() > page.right())
  //      r.setRight(page.right());
  //  }
  //  m_auxFrameRect = matrix.mapRect(r).toRect();
  //}
}

QPoint te::qt::widgets::DataFrame::getCenterSelected()
{
  // faz tudo em coordenadas do dispositivo

  QPoint c;

  if(m_selected == 1)
    c = m_recSel1.center();
  else if(m_selected == 2)
    c = m_recSel2.center();
  else if(m_selected == 3)
    c = m_recSel3.center();
  else if(m_selected == 4)
    c = m_recSel4.center();
  else if(m_selected == 5)
    c = m_recSel5.center();
  else if(m_selected == 6)
    c = m_recSel6.center();
  else if(m_selected == 7)
    c = m_recSel7.center();
  else if(m_selected == 8)
    c = m_recSel8.center();
  else if(m_selected == 10)
    c = m_recSel10.center();

  return c;
}

void te::qt::widgets::DataFrame::rubberBand()
{
  // faz tudo em coordenadas do dispositivo

  QPixmap* pixmap = m_layoutEditor->getDraftPixmap();
  QPainter painter(pixmap);
  painter.translate(m_layoutEditor->getVerticalRulerWidth(), m_layoutEditor->getHorizontalRulerWidth());

  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::red);
  if(m_copyAuxFrameRect.isValid())
  {
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    painter.drawRect(m_copyAuxFrameRect);
  }
  painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
  painter.drawRect(m_auxFrameRect);

  m_copyAuxFrameRect = m_auxFrameRect;

  m_layoutEditor->update();
}

bool te::qt::widgets::DataFrame::eventFilter(QObject* obj, QEvent* e)
{
  // return true to stop the event; otherwise return false.

  int type = e->type();

  if(obj == m_mapDisplay) 
  {
    if(type == QEvent::Drop)
    {
      QDropEvent* dropEvent = (QDropEvent*)e;
      const QMimeData* mime = dropEvent->mimeData();
      QString s = mime->data("application/x-terralib;value=\"DraggedItems\"").constData();
      if(s.isEmpty() == false)
      {
        removeUTMGrid();
        removeGeographicGrid();
        removeGraphicScale(); // and remove others..............................
        m_mapDisplay->dropEvent(dropEvent);
        te::gm::Envelope env = m_mapDisplay->getExtent();
        int srid = m_mapDisplay->getSRID();
        findDataUnitToMilimeter(env, srid);
        m_dataRect = QRectF(env.getLowerLeftX(), env.getLowerLeftY(), env.getWidth(), env.getHeight());
        double w = m_frameRect.width();
        double ww = m_dataUnitToMilimeter * env.getWidth();
        m_scale = ww / w;
      }
      return true;
    }
    else if(type == QEvent::MouseButtonPress)
    {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

      if(mouseEvent->buttons() == Qt::LeftButton)
      {
        QPoint p = mouseEvent->pos();
        m_pressPoint = p;
        m_undo = false;
        m_copyAuxFrameRect = QRect();
        m_layoutEditor->raiseDraftLayoutEditor();
      }
      else if(mouseEvent->buttons() == Qt::RightButton)
      {
        QPoint gp = mouseEvent->globalPos();
        if(m_selected == 10)
        {
          QAction* action = m_menu->exec(gp);
          if(action == m_createUTMGridAction)
            createUTMGrid();
          else if(action == m_removeUTMGridAction)
            removeUTMGrid();
          else if(action == m_createGeographicGridAction)
            createGeographicGrid();
          else if(action == m_removeGeographicGridAction)
            removeGeographicGrid();
          else if(action == m_createGraphicScaleAction)
            createGraphicScale();
          else if(action == m_removeGraphicScaleAction)
            removeGraphicScale();
        }
      }
      return false;
    }
    else if (type == QEvent::MouseMove)
    {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
      QPoint p = mouseEvent->pos();
      QPoint gp = mouseEvent->globalPos();

      if(mouseEvent->buttons() == Qt::NoButton)
      {
        setCursor();

        m_dragging = false;
        getSelectionPoint(p);
        if(m_selected == 0)
          return false;
        else
        {
          showSelectionPoints();
          toolTip(gp, "Selection");
          if(m_selected == 10 && m_layoutEditor->getFrameSelected() != this)
            m_layoutEditor->setFrameSelected(this);
        }

        setCursor();
      }
      else if(mouseEvent->buttons() == Qt::LeftButton)
      {
        m_dragging = true;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
        QPoint p = mouseEvent->pos();
        QPoint d = m_pressPoint - p;
        QMatrix matrix = m_layoutEditor->getMatrix();
        m_auxFrameRect = matrix.mapRect(m_frameRect).toRect();

        // mouse drag with left buttom
        if(m_selected != 0)
        {
          if(m_undo == false)
          {
            m_layoutEditor->insertCopy2Undo(this);
            m_undo = true;
          }

          if(m_selected == 1) // resize top left
            m_auxFrameRect = QRect(m_auxFrameRect.left() - d.x(), m_auxFrameRect.top() - d.y(), m_auxFrameRect.width() + d.x(), m_auxFrameRect.height() + d.y());
          else if(m_selected == 2) // resize top
            m_auxFrameRect = QRect(m_auxFrameRect.left(), m_auxFrameRect.top() - d.y(), m_auxFrameRect.width(), m_auxFrameRect.height() + d.y());
          else if(m_selected == 3) // resize top right
            m_auxFrameRect = QRect(m_auxFrameRect.left(), m_auxFrameRect.top() - d.y(), m_auxFrameRect.width() - d.x(), m_auxFrameRect.height() + d.y());
          else if(m_selected == 4) // resize right
            m_auxFrameRect = QRect(m_auxFrameRect.left(), m_auxFrameRect.top(), m_auxFrameRect.width() - d.x(), m_auxFrameRect.height());
          else if(m_selected == 5) // resize bottom right
            m_auxFrameRect = QRect(m_auxFrameRect.left(), m_auxFrameRect.top(), m_auxFrameRect.width() - d.x(), m_auxFrameRect.height() - d.y());
          else if(m_selected == 6) // resize bottom
            m_auxFrameRect = QRect(m_auxFrameRect.left(), m_auxFrameRect.top(), m_auxFrameRect.width(), m_auxFrameRect.height() - d.y());
          else if(m_selected == 7) // resize bottom left
            m_auxFrameRect = QRect(m_auxFrameRect.left() - d.x(), m_auxFrameRect.top(), m_auxFrameRect.width() + d.x(), m_auxFrameRect.height() - d.y());
          else if(m_selected == 8) // resize left
            m_auxFrameRect = QRect(m_auxFrameRect.left() - d.x(), m_auxFrameRect.top(), m_auxFrameRect.width() + d.x(), m_auxFrameRect.height());
          else if(m_selected == 9) // rotate data frame by center
          {
          }
          else // move data frame
            m_auxFrameRect.moveCenter(m_auxFrameRect.center() - d);

          rubberBand();
          setCursor();
        }
      }
      return true;
    }
    else if(type == QEvent::MouseButtonRelease)
    {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
      if(mouseEvent->button() == Qt::LeftButton)
      {
        QPoint p = mouseEvent->pos();

        if(m_pressPoint != p && m_selected != 0)
        {
          if(m_UTMGridFrame || m_geoGridFrame)
          {
            QPixmap* pix = m_mapDisplay->getDraftPixmap();
            pix->fill(Qt::transparent);
          }
          setCursor();

          QPixmap* pixmap = m_layoutEditor->getDraftPixmap();
          pixmap->fill(Qt::transparent);
          verifyConstraints();
          QMatrix matrix = m_layoutEditor->getMatrix();
          QRectF r(m_auxFrameRect.left(), m_auxFrameRect.top(), m_auxFrameRect.width(), m_auxFrameRect.height());
          adjustWidgetToFrameRect(matrix.inverted().mapRect(r));
          m_layoutEditor->lowerDraftLayoutEditor();

          if(m_UTMGridFrame)
            m_UTMGridFrame->adjust();
          if(m_geoGridFrame)
            m_geoGridFrame->adjust();
          if(m_graphicScaleFrame)
            m_graphicScaleFrame->adjust();

          m_layoutEditor->draw();

          //if(m_selected != 10)
          //{
          //  QPoint c = getCenterSelected();
          //  m_mapDisplay->cursor().setPos(m_mapDisplay->mapToGlobal(c));
          //  QMouseEvent e1(QEvent::MouseButtonPress, c, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
          //  QApplication::sendEvent(this, &e1);
          //}
          showSelectionPoints();
        }
      }
      return true;
    }
    else if(type == QEvent::Enter)
    {
      if(QApplication::overrideCursor() || m_dragging)
        return false;

      m_layoutEditor->setFrameSelected(this);
      raise();
      m_selected = 10;
      return true;
    }
    else if(type == QEvent::Leave)
    {
      m_mapDisplay->update();
      if(m_dragging)
      {
        if(m_selected != 0)
        {
          if(m_UTMGridFrame || m_geoGridFrame)
            raise();
        }
        return false;
      }

      m_selected = 0;
      m_layoutEditor->setFrameSelected(0);
      lower();
      setCursor();
      return false;
    }
  }

  // pass the event on to the parent class
  return QWidget::eventFilter(obj, e);
}

void te::qt::widgets::DataFrame::setCursor()
{
  QCursor cursor;

  if(m_selected == 1 || m_selected == 5)
    cursor.setShape(Qt::SizeFDiagCursor);
  else if(m_selected == 2 || m_selected == 6)
    cursor.setShape(Qt::SizeVerCursor);
  else if(m_selected == 3 || m_selected == 7)
    cursor.setShape(Qt::SizeBDiagCursor);
  else if(m_selected == 4 || m_selected == 8)
    cursor.setShape(Qt::SizeHorCursor);
  else if(m_selected == 10)
    cursor.setShape(Qt::SizeAllCursor);

  if(QApplication::overrideCursor())
  {
    if(QApplication::overrideCursor()->shape() != cursor.shape())
    {
      while(QApplication::overrideCursor())
        QApplication::restoreOverrideCursor();
      if(cursor.shape() != Qt::ArrowCursor)
        QApplication::setOverrideCursor(cursor);
    }
  }
  else if(cursor.shape() != Qt::ArrowCursor)
    QApplication::setOverrideCursor(cursor);
}

void te::qt::widgets::DataFrame::toolTip(const QPoint& p, const QString& type)
{
  if(m_showToolTip == false)
  {
    QToolTip::hideText();
    return;
  }
 
  if(type == "Selection")
  {
    if(m_selected >= 1 && m_selected <= 8)
      QToolTip::showText(p, "Drag To Resize", this);
    else if(m_selected == 10)
      QToolTip::showText(p, "Drag to Move", this);
    else
      QToolTip::hideText();
  }
}

void te::qt::widgets::DataFrame::createUTMGrid()
{
  if(m_UTMGridFrame)
    delete m_UTMGridFrame;
  m_UTMGridFrame = new te::qt::widgets::UTMGridFrame(this);

  m_createUTMGridAction->setEnabled(false);
  m_removeUTMGridAction->setEnabled(true);

  draw();
}

void te::qt::widgets::DataFrame::removeUTMGrid()
{
  if(m_UTMGridFrame)
    delete m_UTMGridFrame;
  m_UTMGridFrame = 0;

  m_createUTMGridAction->setEnabled(true);
  m_removeUTMGridAction->setEnabled(false);
}

void te::qt::widgets::DataFrame::createGeographicGrid()
{
  if(m_geoGridFrame)
    delete m_geoGridFrame;
  m_geoGridFrame = new te::qt::widgets::GeographicGridFrame(this);

  m_createGeographicGridAction->setEnabled(false);
  m_removeGeographicGridAction->setEnabled(true);

  draw();
}

void te::qt::widgets::DataFrame::removeGeographicGrid()
{
  if(m_geoGridFrame)
    delete m_geoGridFrame;
  m_geoGridFrame = 0;

  m_createGeographicGridAction->setEnabled(true);
  m_removeGeographicGridAction->setEnabled(false);
}

void te::qt::widgets::DataFrame::createGraphicScale()
{
  if(m_graphicScaleFrame)
    delete m_graphicScaleFrame;

  m_graphicScaleFrame = new te::qt::widgets::GraphicScaleFrame(this);

  m_createGraphicScaleAction->setEnabled(false);
  m_removeGraphicScaleAction->setEnabled(true);

  draw();
}

void te::qt::widgets::DataFrame::removeGraphicScale()
{
  if(m_graphicScaleFrame)
    delete m_graphicScaleFrame;
  m_graphicScaleFrame = 0;

  m_createGraphicScaleAction->setEnabled(true);
  m_removeGraphicScaleAction->setEnabled(false);
}

void te::qt::widgets::DataFrame::hide()
{
  QWidget::hide();
  if(m_UTMGridFrame)
    m_UTMGridFrame->hide();
  if(m_geoGridFrame)
    m_geoGridFrame->hide();
  if(m_graphicScaleFrame)
    m_graphicScaleFrame->hide();
}

void te::qt::widgets::DataFrame::show()
{
  QWidget::show();

  if(m_UTMGridFrame)
    m_UTMGridFrame->show();
  if(m_geoGridFrame)
    m_geoGridFrame->show();
  if(m_graphicScaleFrame)
    m_graphicScaleFrame->show();
}

void te::qt::widgets::DataFrame::lower()
{
  QWidget::show();

  if(m_UTMGridFrame)
    m_UTMGridFrame->lower();
  if(m_geoGridFrame)
    m_geoGridFrame->lower();
}

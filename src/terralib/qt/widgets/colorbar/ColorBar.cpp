#include "ColorBar.h"

// TerraLib
#include "../../../color.h"
#include "../../../color/ColorBar.h"
#include "../../../color/ColorTransform.h"
#include "../utils/ColorPickerToolButton.h"
#include "../Utils.h"

// STL
#include <iostream>

// QT
#include <QAction>
#include <QColorDialog>
#include <QEvent>
#include <QMenu>
#include <QPainter>
#include <QWidgetAction>

// QWT
#include <qwt_color_map.h>
#include <qwt_interval.h>
#include <qwt_scale_engine.h>

using namespace std;

te::qt::widgets::colorbar::ColorBar::ColorBar(QWidget* parent) : QwtScaleWidget(QwtScaleDraw::BottomScale, parent),
  m_colorBar(0),
  m_currentPinPos(0),
  m_colorBarMenu(0),
  m_pinMenu(0),
  m_colorBarPicker(0),
  m_pinPicker(0),
  m_addPinAction(0),
  m_editPinAction(0),
  m_removePinAction(0)
{
  this->setColorBarEnabled(true);

  m_interval.setInterval(0., 1.);

  setHeight(20);

  m_initialXPos = -1;
  
  // sets mouse config
  setClickPrecision(0.0035);
  setMouseTracking(true);

  setScaleEngine();

  // Color pickers
  m_pinPicker = new te::qt::widgets::ColorPickerToolButton(m_pinMenu);
  m_colorBarPicker = new te::qt::widgets::ColorPickerToolButton(m_colorBarMenu);

  // Set the actions for the color bar menu
  m_colorBarMenu = new QMenu(this);
  m_addPinAction = new QWidgetAction(m_colorBarMenu);
  m_addPinAction->setDefaultWidget(m_colorBarPicker);
  m_equalStepAction = new QAction(QObject::tr("Equal Step"), m_colorBarMenu);
  m_equalStepAction->setStatusTip(QObject::tr("Equal Step"));
  connect(m_equalStepAction, SIGNAL(triggered()), this, SLOT(equalStep()));
  connect(m_colorBarPicker, SIGNAL(colorChanged(const QColor&)), this, SLOT(addPin()));
   m_colorBarMenu->addAction(m_addPinAction);
   m_colorBarMenu->addAction(m_equalStepAction);

  // Set the actions for the color bar pin menu
  m_pinMenu = new QMenu(this);
  m_editPinAction = new QWidgetAction(m_pinMenu);
  m_editPinAction->setStatusTip(QObject::tr("Edit"));
  m_editPinAction->setDefaultWidget(m_pinPicker);  
  m_removePinAction = new QAction(QObject::tr("Remove"), m_pinMenu);
  m_removePinAction->setStatusTip(QObject::tr("Remove"));  
  connect(m_pinPicker, SIGNAL(colorChanged(const QColor&)), this, SLOT(editPin()));
  connect(m_removePinAction, SIGNAL(triggered()), this, SLOT(removePin()));
  m_pinMenu->addAction(m_editPinAction);
  m_pinMenu->addAction(m_removePinAction);
}

te::qt::widgets::colorbar::ColorBar::~ColorBar()
{
  delete m_colorBar;
}

void te::qt::widgets::colorbar::ColorBar::setScaleVisible(bool flag)
{
  this->scaleDraw()->enableComponent(QwtScaleDraw::Backbone, flag);
  this->scaleDraw()->enableComponent(QwtScaleDraw::Ticks, flag);
  this->scaleDraw()->enableComponent(QwtScaleDraw::Labels, flag);
}

void te::qt::widgets::colorbar::ColorBar::setHeight(int value)
{
  m_height = value;

  this->setColorBarWidth(m_height);
}

void te::qt::widgets::colorbar::ColorBar::setInterval(double min, double max)
{
  m_interval.setMinValue(min);
  m_interval.setMaxValue(max);

  setScaleEngine();
}

void te::qt::widgets::colorbar::ColorBar::setColorBar(te::color::ColorBar* cb)
{
  delete m_colorBar;

  m_colorBar = cb;

  buildColorBar();
}

te::color::ColorBar* te::qt::widgets::colorbar::ColorBar::getColorBar()
{
  return new te::color::ColorBar(*m_colorBar);
}

void te::qt::widgets::colorbar::ColorBar::setScaleEngine()
{
  QwtLinearScaleEngine se;
  this->setScaleDiv(se.divideScale(m_interval.minValue(), m_interval.maxValue(), 8, 5));
}

void te::qt::widgets::colorbar::ColorBar::setClickPrecision(double precision)
{
  m_clickPrecision = precision;
}

void te::qt::widgets::colorbar::ColorBar::buildColorBar()
{
  if(m_colorBar && m_colorBar->getColorMap().size() >= 2)
  {

    std::map<double, te::color::RGBAColor> map  = m_colorBar->getColorMap();

    //QwtScaleWidget will delete this pointer automatically
    QwtLinearColorMap* colormap = new QwtLinearColorMap(
      QColor(map[0.].getRed(), map[0.].getGreen(), map[0.].getBlue(), map[0.].getAlpha()),  
      QColor(map[1.].getRed(), map[1.].getGreen(), map[1.].getBlue(), map[1.].getAlpha()));

    std::map<double, te::color::RGBAColor>::const_iterator it = m_colorBar->getColorMap().begin();

    while(it != m_colorBar->getColorMap().end())
    {
      double pos = it->first;

      if(pos != 0. && pos != 1.)
      {
        QColor c(it->second.getRed(), it->second.getGreen(), it->second.getBlue());

        colormap->addColorStop(pos, c);
      }

      ++it;
    }

    this->setColorMap(m_interval, colormap);

    emit colorBarChanged();
  }
}

void te::qt::widgets::colorbar::ColorBar::removePin()
{
  if(m_currentPinPos != 0 && m_currentPinPos != 1)
    m_colorBar->remove(m_currentPinPos);

  buildColorBar();
  this->repaint();
}

void te::qt::widgets::colorbar::ColorBar::editPin()
{
  QColor c = m_pinPicker->getColor();
  te::color::RGBAColor rgbaC(c.red(), c.green(), c.blue(), c.alpha());

  m_colorBar->changeColor(m_currentPinPos, rgbaC);

  buildColorBar();
  this->repaint();
}

void te::qt::widgets::colorbar::ColorBar::addPin()
{
  QColor c = m_colorBarPicker->getColor();
  te::color::RGBAColor rgbaC(c.red(), c.green(), c.blue(), c.alpha());

  m_colorBar->addColor(rgbaC, m_currentPinPos);

  m_colorBarMenu->hide();

  buildColorBar();
  this->repaint();
}

void te::qt::widgets::colorbar::ColorBar::equalStep()
{
  std::map<double, te::color::RGBAColor>::const_iterator it = m_colorBar->getColorMap().begin();

  std::map<double, double> newpos = std::map<double, double>();

  int numPins = m_colorBar->getColorMap().size();

  int increment = (int)this->width() / (numPins-1);

  int pinCount = 0;
  while(it != m_colorBar->getColorMap().end())
  {
    if(it->first != 0 && it->first != 1)
    {
      pinCount == 0 ? pinCount = 2:true;
      newpos[it->first] = convert2toolbarPos((pinCount-1)*increment);
      ++pinCount;
    }
    ++it;
  }

  std::map<double, double>::const_iterator it2 = newpos.begin();
  while(it2 != newpos.end())
  {
    m_colorBar->move(it2->first, it2->second);
    ++it2;
  }

  buildColorBar();
  this->repaint();
}

void te::qt::widgets::colorbar::ColorBar::paintEvent(QPaintEvent* e)
{
  QwtScaleWidget::paintEvent(e);

  if(!m_colorBar)
    return;

  QPainter p(this);
  p.setPen(QPen(Qt::black));

  std::map<double, te::color::RGBAColor>::const_iterator it = m_colorBar->getColorMap().begin();

  while(it != m_colorBar->getColorMap().end())
  {
    double pos = it->first * this->width();

    int r = it->second.getRed();
    int g = it->second.getGreen();
    int b = it->second.getBlue();
    int a = it->second.getAlpha();

    p.drawLine(QPoint(pos, 0 + 2), QPoint(pos, m_height + 2));
    p.setBrush(QBrush(QColor(r, g, b, a)));
    p.drawEllipse(QPoint(pos, 0 + 2), 2, 2);

    ++it;
  }
}

void te::qt::widgets::colorbar::ColorBar::mousePressEvent(QMouseEvent* e)
{
  if(!m_colorBar)
    return;

  double pos = convert2toolbarPos(e->x());
  double pinPos = getPin(e->x());
  
  if(e->button() == Qt::RightButton)
  {
    //Pin menu or colorbar menu
    if(pinPos != -1)
    {
      m_currentPinPos = pinPos;
      te::color::RGBAColor rgbaC = m_colorBar->getColorMap().at(m_currentPinPos);
      int red = rgbaC.getRed();
      int green = rgbaC.getGreen();
      int blue = rgbaC.getBlue();
      QColor c(red, green, blue);
      m_pinPicker->setColor(c);
      m_pinMenu->exec(QCursor::pos());
    }
    else
    {
      m_currentPinPos = pos;
      m_colorBarMenu->exec(QCursor::pos());
    }
  }

  //gets the initial position to move the pin
  if (e->button() == Qt::LeftButton)
    if(pinPos != -1)
      m_initialXPos = pinPos;

}

void te::qt::widgets::colorbar::ColorBar::mouseReleaseEvent(QMouseEvent* e)
{
  //finalize moving stop from colobar
  if (e->button() == Qt::LeftButton)
  {
    m_initialXPos = -1;
  }
}

void te::qt::widgets::colorbar::ColorBar::mouseMoveEvent(QMouseEvent* e)
{
  double currentPos = convert2toolbarPos(e->x());
  
  if(getPin(e->x()) != -1)
    setCursor(Qt::SplitHCursor);
  else
    setCursor(Qt::PointingHandCursor);

  if(m_initialXPos > -1)
  {
    m_colorBar->move(m_initialXPos, currentPos);

    m_initialXPos = currentPos;

    buildColorBar();
    this->repaint();
  }

}

void te::qt::widgets::colorbar::ColorBar::wheelEvent(QWheelEvent* e)
{
  double pin = getPin(e->x());

  if(pin != -1)
  {
    int hh, ss, ll, a;
    int h, s, l, d;

    te::color::RGBAColor color1 = m_colorBar->getColorMap().at(pin);
    te::color::RGBAColor color2 = m_colorBar->getColorMap().at(pin);

    te::color::ColorTransform c(color1.getRgba());
    c.getHsl(&hh, &ss, &ll, &a);

    te::color::ColorTransform qc(color2);

    d = 8;

    if(e->delta() < 0)
      d *= -1;

    qc.getHsl(&h, &s, &l, &a);
    
    l += d;

    if(l > 245)
      l = 245;
    if(l < 10)
      l = 10;

    qc.setHsl(hh, ss, l, a);

    te::color::RGBAColor cor(qc.getRed(), qc.getGreen(), qc.getBlue(), qc.getAlpha());
    m_colorBar->changeColor(pin, cor);

    buildColorBar();
    this->repaint();
  }

}

double te::qt::widgets::colorbar::ColorBar::getPin(int pos)
{
  double width = this->width(); 
  double currentPos = (double) pos / width;

  std::map<double, te::color::RGBAColor>::const_iterator it = m_colorBar->getColorMap().begin();

  bool isPin = false;

  while(it != m_colorBar->getColorMap().end())
  {
    double colorPos = it->first;

    if ( colorPos > (currentPos - m_clickPrecision) && colorPos < (currentPos + m_clickPrecision))
    {      
      currentPos = colorPos;
      isPin = true;
      break;
    }
    ++it;
  }

  if(isPin)
    return currentPos;
  else
    return -1;
}

double te::qt::widgets::colorbar::ColorBar::convert2toolbarPos(int pos)
{
  double width = this->width(); 
  double currentPos = (double) pos / width;

  return currentPos;
}
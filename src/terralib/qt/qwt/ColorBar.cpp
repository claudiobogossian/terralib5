#include "ColorBar.h"

// TerraLib
#include "../../color.h"
#include "../../color/ColorBar.h"

// QT
#include <QtGui/QPainter>
#include <QtGui/qevent.h>
#include <QtGui/QColorDialog>

// QWT
#include <qwt_color_map.h>
#include <qwt_interval.h>
#include <qwt_scale_engine.h>

te::qt::qwt::ColorBar::ColorBar(QWidget* parent) : QwtScaleWidget(QwtScaleDraw::BottomScale, parent),
  m_colorBar(0)
{
  this->setColorBarEnabled(true);

  m_interval.setInterval(0., 1.);

  setHeight(20);

  //setScaleEngine();
}

te::qt::qwt::ColorBar::~ColorBar()
{
}

void te::qt::qwt::ColorBar::setHeight(int value)
{
  m_height = value;

  this->setColorBarWidth(m_height);
}

void te::qt::qwt::ColorBar::setInterval(double min, double max)
{
  m_interval.setMinValue(min);
  m_interval.setMaxValue(max);

  //setScaleEngine();
}

void te::qt::qwt::ColorBar::setColorBar(te::color::ColorBar* cb)
{
  m_colorBar = cb;

  buildColorBar();
}

te::color::ColorBar* te::qt::qwt::ColorBar::getColorBar()
{
  return m_colorBar;
}

void te::qt::qwt::ColorBar::setScaleEngine()
{
  QwtLinearScaleEngine se;

  this->setScaleDiv(se.transformation(), se.divideScale(m_interval.minValue(), m_interval.maxValue(), 8, 5));
}

void te::qt::qwt::ColorBar::buildColorBar()
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
  }
}

void te::qt::qwt::ColorBar::paintEvent(QPaintEvent* e)
{
  QwtScaleWidget::paintEvent(e);

  if(m_colorBar)
  {
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
}

void te::qt::qwt::ColorBar::mouseDoubleClickEvent(QMouseEvent* e)
{
    QColor c = QColorDialog::getColor();

    if(c.isValid() && m_colorBar)
    {
      int x = e->x();

      double width = this->width(); 
      double pos = (double)x / width;

      te::color::RGBAColor rgbaC(c.red(), c.green(), c.blue(), c.alpha());

      m_colorBar->addColor(rgbaC, pos);

      buildColorBar();

      this->repaint();
    }
}
#include <QtGui>
#include <QPainter>

#include "RenderArea.h"

RenderArea::RenderArea(QWidget *parent)
  : QWidget(parent),
  m_pixmap(0)
{
  setObjectName("RenderArea");
}

RenderArea::~RenderArea()
{
  delete m_pixmap;
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(300, 200);
}

void RenderArea::resizeEvent(QResizeEvent* e)
{
  QWidget::resizeEvent(e);
  QRect r = rect();
  if(m_pixmap)
  {
    if(m_pixmap->width() != e->size().width() || m_pixmap->height() != e->size().height()) 
    {
      delete m_pixmap;
      m_pixmap = new QPixmap(r.width(), r.height());
    }
  }
  else
    m_pixmap = new QPixmap(r.width(), r.height());
  m_pixmap->fill(Qt::white);
}

void RenderArea::paintEvent(QPaintEvent * /* e */)
{
  if(m_pixmap)
  {
    QPainter painter(this);
    painter.drawPixmap(0, 0, *m_pixmap);
  }
}

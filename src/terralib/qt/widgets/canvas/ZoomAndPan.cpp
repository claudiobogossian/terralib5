#include "ZoomAndPan.h"
#include <zoom_in_cursor.xpm>
#include <zoom_in_cursor_mask.xpm>
#include <zoom_out_cursor.xpm>
#include <zoom_out_cursor_mask.xpm>

#include "../../../geometry/Geometry.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/Polygon.h"
#include "../../../qt/widgets/Utils.h"

#include <QtGui/QMouseEvent>

#include <time.h>


te::qt::widgets::ZoomAndPan::ZoomAndPan(QWidget* parent) : DisplayEventHandler(parent)
{
  int w = parent->width();
  int h = parent->height();
  m_pixmap = new QPixmap(w, h);
  resize(w, h);

  //m_inBitmap = new QBitmap("../../../../trunk/images/zoom_in_cursor.bmp");
  //m_inMaskBitmap = new QBitmap("../../../trunk/images/zoom_in_cursor_mask.bmp");
  //m_outBitmap = new QBitmap("../../../trunk/images/zoom_out_cursor.bmp");
  //m_outMaskBitmap = new QBitmap("../../../trunk/images/zoom_out_cursor_mask.bmp");
  QPixmap zi((const char**)zoom_in_cursor);
  QPixmap zim((const char**)zoom_in_cursor_mask);
  m_inBitmap = new QBitmap(zi);
  m_inMaskBitmap = new QBitmap(zim);
  QPixmap zo((const char**)zoom_out_cursor);
  QPixmap zom((const char**)zoom_out_cursor_mask);
  m_outBitmap = new QBitmap(zo);
  m_outMaskBitmap = new QBitmap(zom);

  m_leftButton = false;
  m_midButton = false;

  setMouseTracking(true);
  setFocusPolicy(Qt::StrongFocus);

  m_corCursor = QColor(255, 0, 0, 75);
  m_corCursorContour = QColor(255, 0, 0, 255);

  QWidget* p = parent;
  while(p)
  {
    if(p->objectName() == "MapDisplay")
      break;
    p = (QWidget*)p->parent();
  }

  connect(p, SIGNAL(sizeChanged(QSize)), this, SLOT(sizeChanged(QSize)));
}

te::qt::widgets::ZoomAndPan::~ZoomAndPan()
{
  delete m_inBitmap;
  delete m_inMaskBitmap;
  delete m_outBitmap;
  delete m_outMaskBitmap;
  delete m_pixmap;
}

void te::qt::widgets::ZoomAndPan::drawCursor()
{
  m_pixmap->fill(QColor(0, 0, 0, 0));
  if(m_rect.isValid())
  {
    QPainter painter(m_pixmap);
    QPen pen(m_corCursorContour);
    painter.setPen(pen);
    QBrush brush(m_corCursor);
    painter.setBrush(brush);
    painter.drawRect(m_rect);
  }
  update();
}

void te::qt::widgets::ZoomAndPan::keyPressEvent(QKeyEvent* e)
{
  m_key = e->key();

  if(m_key == Qt::Key_Control) // zoom in
    setCursor(QCursor(*m_inBitmap, *m_inMaskBitmap, 12, 12));
  else if(m_key == Qt::Key_Alt) // zoom out
    setCursor(QCursor(*m_outBitmap, *m_outMaskBitmap, 12, 12));
  else if(m_key == Qt::Key_Shift) // add selection
    setCursor(QCursor(Qt::ClosedHandCursor));
}

void te::qt::widgets::ZoomAndPan::keyReleaseEvent(QKeyEvent*)
{
  //if(m_key == Qt::Key_Control || m_key == Qt::Key_Alt)
  //  setCursor(QCursor(*m_inBitmap, *m_inMaskBitmap, 12, 12));
  setCursor(QCursor(Qt::ArrowCursor));

  m_key = -1;
}

void te::qt::widgets::ZoomAndPan::mousePressEvent(QMouseEvent* e)
{
  m_pressPoint = e->pos();
  if(e->button() == Qt::LeftButton)
  {
    m_leftButton = true;
    if(m_key == Qt::Key_Shift)
    {
      setCursor(QCursor(Qt::ClosedHandCursor));
    }
    else if(m_key == Qt::Key_Alt)
      setCursor(QCursor(*m_outBitmap, *m_outMaskBitmap, 12, 12));
    else if(m_key == Qt::Key_Control)
      setCursor(QCursor(*m_inBitmap, *m_inMaskBitmap, 12, 12));
  }
  else if(e->button() == Qt::MidButton)
  {
    m_midButton = true;
    setCursor(QCursor(Qt::OpenHandCursor));
  }
}

void te::qt::widgets::ZoomAndPan::mouseMoveEvent(QMouseEvent* e)
{
  if(hasFocus() == false)
    setFocus();

  m_point = e->pos();

  if(m_midButton) // pan
  {
    setCursor(QCursor(Qt::OpenHandCursor));
    m_rect = QRect();
    setRepaint(false);
    update();
    return;
  }

  if(m_leftButton)
  {
    if(e->modifiers() == Qt::AltModifier) // zoom out
    {
      setCursor(QCursor(*m_outBitmap, *m_outMaskBitmap, 12, 12));
      m_rect = QRect();
      setRepaint(true);
    }
    else if(e->modifiers() == Qt::NoModifier || // selection
      e->modifiers() == Qt::ShiftModifier || // add to selection
      e->modifiers() == Qt::ControlModifier) // zoom in
    {
      if(e->modifiers() == Qt::NoModifier)
        setCursor(QCursor(Qt::ArrowCursor));
      else if(e->modifiers() == Qt::ShiftModifier)
        setCursor(QCursor(Qt::ClosedHandCursor));
      else if(e->modifiers() == Qt::ControlModifier)
        setCursor(QCursor(*m_inBitmap, *m_inMaskBitmap, 12, 12));

      int w = abs(m_point.x() - m_pressPoint.x()) + 1;
      int h = abs(m_point.y() - m_pressPoint.y()) + 1;
      if(m_point.x() > m_pressPoint.x() && m_point.y() > m_pressPoint.y())
        m_rect.setRect(m_pressPoint.x(), m_pressPoint.y(), w, h);
      else if(m_point.x() > m_pressPoint.x() && m_point.y() < m_pressPoint.y())
        m_rect.setRect(m_pressPoint.x(), m_point.y(), w, h);
      else if(m_point.x() < m_pressPoint.x() && m_point.y() < m_pressPoint.y())
        m_rect.setRect(m_point.x(), m_point.y(), w, h);
      else
        m_rect.setRect(m_point.x(), m_pressPoint.y(), w, h);
      setRepaint(true);
      drawCursor();
    }
  }
}

void te::qt::widgets::ZoomAndPan::mouseReleaseEvent(QMouseEvent* e)
{
  setRepaint(true);
  unsetCursor();

  if(m_midButton)  
  {
    emit execPan(m_pressPoint, m_point);
    update();
  }
  else if(m_leftButton)
  {
    if(e->modifiers() == Qt::AltModifier)  
    {
      emit execZoomOut(m_pressPoint);
    }
    else if(e->modifiers() == Qt::ControlModifier)
    {
      if(m_rect.isValid() && m_rect.width() > 2 && m_rect.height() > 2)
        emit execZoomArea(m_rect);
      else
        emit execZoomIn(m_pressPoint);
      m_rect = QRect();
      drawCursor();
    }
    else if(e->modifiers() == Qt::NoModifier)
    {
      if(m_rect.isValid() == false)
        m_rect.setRect(m_pressPoint.x(), m_pressPoint.y(), 1, 1);
      emit execSelection(m_rect);
      m_rect = QRect();
      drawCursor();
    }
    else if(e->modifiers() == Qt::ShiftModifier)
    {
      if(m_rect.isValid() == false)
        m_rect.setRect(m_pressPoint.x(), m_pressPoint.y(), 1, 1);
      emit execAddSelection(m_rect);
      m_rect = QRect();
      drawCursor();
    }
  }

  m_leftButton = false;
  m_midButton = false;
}

void te::qt::widgets::ZoomAndPan::sizeChanged(QSize s)
{
  if(m_pixmap)
    delete m_pixmap;

  int w = s.width();
  int h = s.height();
  m_pixmap = new QPixmap(w, h);
  m_pixmap->fill(QColor(0, 0, 0, 0));
  resize(w, h);
}

void te::qt::widgets::ZoomAndPan::paintEvent(QPaintEvent*)
{
  if(m_midButton == true)
  {
    m_pixmap->fill(QColor(0, 0, 0, 0));
    QPixmap* pix = getDisplayPixmap();

    QPoint p = m_point - m_pressPoint;
    QPainter painter(m_pixmap);
    painter.drawPixmap(p, *pix);
    painter.end();
    painter.begin(this);
    painter.drawPixmap(0, 0, *m_pixmap);
  }
  else
  {
    QPainter painter(this);
    QRect r = m_rect;
    r.setBottomRight(r.bottomRight() + QPoint(1, 1));
    painter.drawPixmap(r, *m_pixmap, r);
  }
}
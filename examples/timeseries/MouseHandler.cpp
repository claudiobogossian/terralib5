#include "MouseHandler.h"
#include <terralib/geometry/Geometry.h>
#include <terralib/geometry/LinearRing.h>
#include <terralib/geometry/Polygon.h>
#include <terralib/qt/widgets/Utils.h>

#include <QtGui/QMouseEvent>
#include <QtCore/QDir>

#include <time.h>


MouseHandler::MouseHandler(QWidget* parent) : DisplayEventHandler(parent),
  m_mouseMode(MouseHandler::SelectionMode),
  m_panEnd(false)
{
  int w = parent->width();
  int h = parent->height();
  m_pixmap = new QPixmap(w, h);
  resize(w, h);

  QDir dir;
  if(dir.cd("images") == false)
      dir.cd("../../images");

  QString s = dir.absolutePath() + "/zoomInCursor.png";
  m_zoomInPixmap = new QPixmap(dir.absolutePath() + "/zoomInCursor.png");
  m_zoomOutPixmap = new QPixmap(dir.absolutePath() + "/zoomOutCursor.png");
  m_panPixmap = new QPixmap(dir.absolutePath() + "/panCursor.png");
  m_selectionPixmap = new QPixmap(dir.absolutePath() + "/selectionCursor.png");
  m_addSelectionPixmap = new QPixmap(dir.absolutePath() + "/addSelectionCursor.png");
  m_toggleSelectionPixmap = new QPixmap(dir.absolutePath() + "/toggleSelectionCursor.png");

  //QPixmap pix((const char**)ZoomInCursor);
  //m_zoomInBitmap = new QBitmap(pix);
  //pix = QPixmap((const char**)ZoomInMaskCursor);
  //m_zoomInMaskBitmap = new QBitmap(pix);
  //pix = QPixmap((const char**)ZoomOutCursor);
  //m_zoomOutBitmap = new QBitmap(pix);
  //pix = QPixmap((const char**)ZoomOutMaskCursor);
  //m_zoomOutMaskBitmap = new QBitmap(pix);

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

MouseHandler::~MouseHandler()
{
  delete m_zoomInPixmap;
  delete m_zoomOutPixmap;
  delete m_panPixmap;
  delete m_selectionPixmap;
  delete m_addSelectionPixmap;
  delete m_toggleSelectionPixmap;
  delete m_pixmap;
}

void MouseHandler::drawCursor()
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

MouseHandler::MouseMode MouseHandler::getMode()
{
  return m_mouseMode;
}

void MouseHandler::setMode(MouseMode m)
{
    m_mouseMode = m;
}

void MouseHandler::setCursor()
{
  QCursor* cursor;

  if(m_mouseMode == ZoomInMode) // zoom in
    cursor = new QCursor(*m_zoomInPixmap, 9, 9);
  else if(m_mouseMode == ZoomOutMode) // zoom out
    cursor = new QCursor(*m_zoomOutPixmap, 9, 9);
  else if(m_mouseMode == PanMode) // pan
    cursor = new QCursor(*m_panPixmap, 24, 24);
  else if(m_mouseMode == SelectionMode) // seletion
    cursor = new QCursor(*m_selectionPixmap, 4, 5);
  else if(m_mouseMode == AddSelectionMode) // add selection
    cursor = new QCursor(*m_addSelectionPixmap, 4, 5);
  else if(m_mouseMode == ToggleSelectionMode) // toggle selection
    cursor = new QCursor(*m_toggleSelectionPixmap, 4, 5);
  DisplayEventHandler::setCursor(*cursor);
  delete cursor;
}

void MouseHandler::mousePressEvent(QMouseEvent* e)
{
  m_pressPoint = e->pos();
  if(m_mouseMode == PanMode)
    m_panEnd = false;
}

void MouseHandler::mouseMoveEvent(QMouseEvent* e)
{
  if(hasFocus() == false)
    setFocus();

  m_point = e->pos();

  if(e->buttons() == Qt::LeftButton) //atual estado do botao
  {
    if(m_mouseMode == PanMode) // pan
    {
      m_rect = QRect();
      setRepaint(false);
      update();
    }
    else if(m_mouseMode == ZoomOutMode) // zoom out
    {
      m_rect = QRect();
      setRepaint(true);
    }
    else // draw area for zoom in or object (add/toggle) selection
    {
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

void MouseHandler::mouseReleaseEvent(QMouseEvent* e)
{
  setRepaint(true);

  if(e->button() == Qt::LeftButton) // botao que causou o evento
  {
    if(m_mouseMode == ZoomInMode) // zoom in
    {
      if(m_rect.isValid() && m_rect.width() > 2 && m_rect.height() > 2)
        emit execZoomArea(m_rect);
      else
        emit execZoomIn(m_pressPoint);
      m_rect = QRect();
      drawCursor();
    }
    else if(m_mouseMode == ZoomOutMode) // zoom out
      emit execZoomOut(m_pressPoint);
    else if(m_mouseMode == PanMode) // pan
    {
      emit execPan(m_pressPoint, m_point);
      m_panEnd = true;
      update();
    }
    else if(m_mouseMode == SelectionMode) // Selection Mode
    {
      if(m_rect.isValid() == false)
        m_rect.setRect(m_pressPoint.x(), m_pressPoint.y(), 1, 1);
      emit execSelection(m_rect);
      m_rect = QRect();
      drawCursor();
    }
    else if(m_mouseMode == AddSelectionMode) // Add Selection Mode
    {
      if(m_rect.isValid() == false)
        m_rect.setRect(m_pressPoint.x(), m_pressPoint.y(), 1, 1);
      emit execAddSelection(m_rect);
      m_rect = QRect();
      drawCursor();
    }
    else if(m_mouseMode == ToggleSelectionMode) // Toggle Selection Mode
    {
      if(m_rect.isValid() == false)
        m_rect.setRect(m_pressPoint.x(), m_pressPoint.y(), 1, 1);
      emit execToggleSelection(m_rect);
      m_rect = QRect();
      drawCursor();
    }
  }
  setCursor();
}

void MouseHandler::keyPressEvent(QKeyEvent* e)
{
  MouseMode mode = m_mouseMode;

  if(e->key() == Qt::Key_Control)
  {
    if(m_mouseMode != ZoomInMode && m_mouseMode != ZoomOutMode && m_mouseMode != PanMode)
    {
      m_mouseOldMode = m_mouseMode;
      m_mouseMode = ZoomInMode;
    }
    else
      m_mouseMode = m_mouseOldMode;
  }
  if(e->key() == Qt::Key_Alt)
  {
    if(m_mouseMode == ZoomInMode)
      setMode(ZoomOutMode);
    else if(m_mouseMode == ZoomOutMode)
      setMode(PanMode);
    else if(m_mouseMode == PanMode)
      setMode(ZoomInMode);

    if(m_mouseMode == SelectionMode)
      setMode(AddSelectionMode);
    else if(m_mouseMode == AddSelectionMode)
      setMode(ToggleSelectionMode);
    else if(m_mouseMode == ToggleSelectionMode)
      setMode(SelectionMode);
  }
  if(mode != m_mouseMode)
  {
    m_panEnd = true;
    setCursor();
  }
}

void MouseHandler::sizeChanged(QSize s)
{
  if(m_pixmap)
    delete m_pixmap;

  int w = s.width();
  int h = s.height();
  m_pixmap = new QPixmap(w, h);
  m_pixmap->fill(QColor(0, 0, 0, 0));
  resize(w, h);
}

void MouseHandler::paintEvent(QPaintEvent*)
{
  //if(m_mouseMode == PanMode && m_panEnd == false)
  //{
  //  m_pixmap->fill(QColor(0, 0, 0, 0));
  //  QPixmap* pix = getDisplayPixmap();

  //  QPoint p = m_point - m_pressPoint;
  //  QPainter painter(m_pixmap);
  //  painter.drawPixmap(p, *pix);
  //  painter.end();
  //  painter.begin(this);
  //  painter.drawPixmap(0, 0, *m_pixmap);
  //}
  //else
  //{
    //QPainter painter(this);
    //QRect r = m_rect;
    //r.setBottomRight(r.bottomRight() + QPoint(1, 1));
    //painter.drawPixmap(r, *m_pixmap, r);
    //m_panEnd = false;
  //}
  if(m_mouseMode != PanMode)
  {
    QPainter painter(this);
    QRect r = m_rect;
    r.setBottomRight(r.bottomRight() + QPoint(1, 1));
    painter.drawPixmap(r, *m_pixmap, r);
  }
  else
  {
    if(m_panEnd)
    {
      // quando termina o pan apague o MoseHandler widget (deixe ele transparente).
      // Assim veremos o conteudo do display.
      QPainter painter(this);
      m_pixmap->fill(QColor(0, 0, 0, 0));
      painter.drawPixmap(0, 0, *m_pixmap);
    }
    else
    {
      // desenhe o pan sobre o MouseHandler widget com fundo branco. Assim não vai 
      // aparecer nada do display. Somente o conteudo do MouseHandler widget será visto.
      m_pixmap->fill(); 
      QPixmap* pix = getDisplayPixmap();

      QPoint p = m_point - m_pressPoint;
      QPainter painter(m_pixmap);
      painter.drawPixmap(p, *pix);
      painter.end();
      painter.begin(this);
      painter.drawPixmap(0, 0, *m_pixmap);
    }
  }
}
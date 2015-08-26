#include "AnimationView.h"
#include "AnimationScene.h"
#include "AnimationItem.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"

//QT
#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>
#include <QtGui/QMouseEvent>
//#include <QtWidgets/QGridLayout>

te::qt::widgets::AnimationView::AnimationView(te::qt::widgets::MapDisplay* parent)
  : QGraphicsView(parent),
  m_display(parent),
  m_dragDropAccepted(false)
{
  setMouseTracking(true);
  setAcceptDrops(true);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  setResizeAnchor(QGraphicsView::NoAnchor);
  //installEventFilter(this);
}

te::qt::widgets::AnimationView::~AnimationView()
{
}

void te::qt::widgets::AnimationView::mousePressEvent(QMouseEvent* e)
{
  QCoreApplication::sendEvent(m_display, (QEvent*)e);
}

void te::qt::widgets::AnimationView::mouseMoveEvent(QMouseEvent* e)
{
  QCoreApplication::sendEvent(m_display, (QEvent*)e);
}

void te::qt::widgets::AnimationView::mouseReleaseEvent(QMouseEvent* e)
{
  QCoreApplication::sendEvent(m_display, (QEvent*)e);
}

void te::qt::widgets::AnimationView::mouseDoubleClickEvent(QMouseEvent* e)
{
  QCoreApplication::sendEvent(m_display, (QEvent*)e);
}

void te::qt::widgets::AnimationView::enterEvent(QEvent* e)
{
  QCoreApplication::sendEvent(m_display, (QEvent*)e);
}

void te::qt::widgets::AnimationView::keyPressEvent(QKeyEvent* e)
{
  QCoreApplication::sendEvent(m_display, (QEvent*)e);
}

void te::qt::widgets::AnimationView::keyReleaseEvent(QKeyEvent* e)
{
  QCoreApplication::sendEvent(m_display, (QEvent*)e);
}

void te::qt::widgets::AnimationView::leaveEvent(QEvent* e)
{
  QCoreApplication::sendEvent(m_display, (QEvent*)e);
}

void te::qt::widgets::AnimationView::resizeEvent(QResizeEvent *e)
{
  QGraphicsScene* scene = this->scene();
  if(scene)
  {
    QRectF r = scene->sceneRect();
    fitInView(r);
  }
}

void te::qt::widgets::AnimationView::dragEnterEvent(QDragEnterEvent* e)
{
  emit animationDragEnterEvent(e);
}

void te::qt::widgets::AnimationView::dragLeaveEvent(QDragLeaveEvent* e)
{
  m_dragDropAccepted = false;
  e->setAccepted(m_dragDropAccepted);
}

void te::qt::widgets::AnimationView::dragMoveEvent(QDragMoveEvent* e)
{
  e->setAccepted(m_dragDropAccepted);
}

void te::qt::widgets::AnimationView::dropEvent(QDropEvent* e)
{
  if(m_dragDropAccepted)
      emit animationDropEvent(e);
}

void te::qt::widgets::AnimationView::paintEvent(QPaintEvent* e)
{
  setMatrix();
  QGraphicsView::paintEvent(e);
}

void te::qt::widgets::AnimationView::setMatrix()
{
  int w = m_display->getDisplayPixmap()->width();
  int h = m_display->getDisplayPixmap()->height();
  te::qt::widgets::Canvas canvas(w, h);
  te::gm::Envelope e = m_display->getExtent();
  canvas.calcAspectRatio(e.m_llx, e.m_lly, e.m_urx, e.m_ury);
  canvas.setWindow(e.m_llx, e.m_lly, e.m_urx, e.m_ury);
  QMatrix matrix = canvas.getMatrix();

  QList<QGraphicsItem*> list = scene()->items();
  QList<QGraphicsItem*>::iterator it;
  for(it = list.begin(); it != list.end(); ++it)
  {
    AnimationItem* ai = (AnimationItem*)(*it);
    ai->m_matrix = matrix;
  }
  QGraphicsView::setMatrix(matrix);


  QRectF sceneRec = scene()->sceneRect(); // The scene rect is already on display projection
  if (sceneRec != sceneRect())
    updateSceneRect(sceneRec);

  // ensure the paint event
  // set bigger box to ensure paint event
  QRectF newRec(sceneRec.x() - 10 * sceneRec.width(), sceneRec.y() - 10 * sceneRec.height(), 30 * sceneRec.width(), 30 * sceneRec.height());
  QRectF dispExt(e.m_llx, e.m_lly, e.getWidth(), e.getHeight());
  QRectF srec(e.m_llx - 10 * e.getWidth(), e.m_lly - 10 * e.getHeight(), 30 * e.getWidth(), 30 * e.getHeight());
  if (dispExt.contains(sceneRec))
    fitInView(srec);
  else
    fitInView(newRec);
}

//bool te::qt::widgets::AnimationView::eventFilter(QObject* obj, QEvent* e)
//{
//  if(obj == this)
//  {
//    QCoreApplication::sendEvent(m_display, e);
//    return true;
//  }
//  return QObject::eventFilter(obj, e);;
//}

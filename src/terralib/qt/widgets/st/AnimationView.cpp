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
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
  setResizeAnchor(QGraphicsView::NoAnchor);
  setCacheMode(QGraphicsView::CacheBackground);
  setUpdatesEnabled(true);
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

  // The following code ensures continuous animation.
  // Without this, when we give great zooms, it is observed the loss of a few frames of animation.
  QRectF sceneRec = scene()->sceneRect(); // The scene rect is already on display projection
  if (sceneRec != sceneRect())
    updateSceneRect(sceneRec);

  // if the projection is different, increase the scene rect
  if (m_sameSRID == false)
  {
    QPointF c = sceneRec.center();
    sceneRec = QRectF(0, 0, sceneRec.width() * 50, sceneRec.height() * 50);
    sceneRec.moveCenter(c);
  }
  fitInView(sceneRec);
  QGraphicsView::paintEvent(e);
  update();
}

void te::qt::widgets::AnimationView::setMatrix()
{
  m_sameSRID = true;
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
    AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
    ai->m_matrix = matrix;
    if (ai->m_SRID != m_display->getSRID())
      m_sameSRID = false;
  }

  if (rect() != m_display->getDisplayPixmap()->rect())
    resize(m_display->getDisplayPixmap()->rect().size());
  if (matrix != QGraphicsView::matrix())
    QGraphicsView::setMatrix(matrix);
}

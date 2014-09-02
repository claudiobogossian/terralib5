#include "AnimationView.h"
#include "../canvas/MapDisplay.h"

//QT
#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>
#include <QtGui/QMouseEvent>

te::qt::widgets::AnimationView::AnimationView(te::qt::widgets::MapDisplay* parent)
  : QGraphicsView(parent),
  m_display(parent),
  m_dragDropAccepted(false)
{
  setMouseTracking(true);
  setAcceptDrops(true);

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


//bool te::qt::widgets::AnimationView::eventFilter(QObject* obj, QEvent* e)
//{
//  if(obj == this)
//  {
//    QCoreApplication::sendEvent(m_display, e);
//    return true;
//  }
//  return QObject::eventFilter(obj, e);;
//}

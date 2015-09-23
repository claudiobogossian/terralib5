// TerraLib
#include "AnimationItem.h"
#include "AnimationScene.h"
#include "Animation.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"

// Qt
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtCore/QVector>

te::qt::widgets::AnimationItem::AnimationItem(const QString& title, te::qt::widgets::MapDisplay* display)
  : QObject(),
  QGraphicsPixmapItem(),
  m_title(title),
  m_display(display),
  m_SRID(-1),
  m_pos(QPoint(0,0)),
  m_automaticPan(false),
  m_panFactor(.01),
  m_curTimeDuration(0),
  m_animation(0),
  m_opacity(255)
{
}

te::qt::widgets::AnimationItem::~AnimationItem()
{
  delete m_route;
}

QPoint te::qt::widgets::AnimationItem::getPosInDeviceCoordinate()
{
  QPointF p = m_pos;
  if (m_display->getSRID() != TE_UNKNOWN_SRS && m_display->getSRID() != m_SRID)
  {
    te::gm::Point point(p.x(), p.y(), m_SRID);
    point.transform(m_display->getSRID());
    p.setX(point.getX());
    p.setY(point.getY());
  }
  return m_matrix.map(p).toPoint();
}

void te::qt::widgets::AnimationItem::setDuration(const unsigned int& duration)
{
  m_duration = duration;
  m_animation->setDuration(m_duration);
  if(m_direction == QAbstractAnimation::Forward)
    m_curTimeDuration = 0;
  else
    m_curTimeDuration = m_duration;
}

void te::qt::widgets::AnimationItem::setDirection(const QAbstractAnimation::Direction& direction)
{
  m_direction = direction;
  setDuration(m_duration);
}

unsigned int te::qt::widgets::AnimationItem::getDuration()
{
  return m_duration;
}

QAbstractAnimation::Direction te::qt::widgets::AnimationItem::getDirection()
{
  return m_direction;
}

void te::qt::widgets::AnimationItem::adjustDataToAnimationTemporalExtent()
{
  te::dt::TimeInstant iTime = m_animation->m_temporalAnimationExtent.getInitialTimeInstant();
  te::dt::TimeInstant fTime = m_animation->m_temporalAnimationExtent.getFinalTimeInstant();

  size_t ini = 0;
  size_t size = m_time.count();
  size_t fim = size;
  for(int i = 0; i < (int)size; ++i)
  {
    if(m_time[i] == iTime || m_time[i] > iTime)
    {
      ini = (size_t)i;
      break;
    }
  }
  for(int i = (int)size-1; i >= 0; --i)
  {
    if (m_time[i] == fTime || m_time[i] < fTime)
    {
      fim = (size_t)i;
      break;
    }
  }
  size = 1 + fim - ini;
  size_t tfim = ini + size;

  m_animationRoute.clear();
  m_animationTime.clear();

  if(m_display->getSRID() != TE_UNKNOWN_SRS && m_display->getSRID() != m_route->getSRID())
  {
    for(size_t i = ini; i < tfim; ++i)
    {
      std::auto_ptr<te::gm::Point> p(m_route->getPointN(i));
      m_animationRoute.push_back(QPointF(p->getX(), p->getY()));
      m_animationTime.push_back(m_time[(int)i]);
    }
  }
  else
  {
    for(size_t i = ini; i < tfim; ++i)
    {
      std::auto_ptr<te::gm::Point> p(m_route->getPointN(i));
      m_animationRoute.push_back(QPointF(p->getX(), p->getY()));
      m_animationTime.push_back(m_time[(int)i]);
    }
  }
}
  
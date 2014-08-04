// TerraLib
#include "TrajectoryItem.h"
#include "AnimationScene.h"
#include "Animation.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"

// Qt
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtCore/QVector>
#include <QtCore/QFile>


te::qt::widgets::TrajectoryItem::TrajectoryItem(const QString& title, const QString& file, te::qt::widgets::MapDisplay* display)
  : te::qt::widgets::AnimationItem(title, display),
    m_forwardColor(Qt::blue),
    m_backwardColor(Qt::magenta),
    m_lineWidth(2)
{
  QFile f(file);
  if(f.exists())
    setPixmap(QPixmap(file));
  else
  {
    QPixmap pix(20, 20);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    QBrush b(Qt::red);
    painter.setBrush(b);
    QPen p(Qt::red);
    painter.setPen(p);
    painter.drawEllipse(QRect(1, 1, 18, 18));
    painter.end();
    setPixmap(pix);
  }
  setMatrix();
}

te::qt::widgets::TrajectoryItem::~TrajectoryItem()
{
}

void te::qt::widgets::TrajectoryItem::createAnimationDataInDisplayProjection()
{
  te::qt::widgets::AnimationItem::createAnimationDataInDisplayProjection();
}

void te::qt::widgets::TrajectoryItem::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
  m_posOld = m_pos;
  m_pos = pos(); // icon position

  if(m_animationRoute.empty())
    return;

  if(m_automaticPan)
  {
    te::gm::Envelope e = m_display->getExtent();
    QRectF r(QPointF(e.getLowerLeftX(), e.getLowerLeftY()), QPointF(e.getUpperRightX(), e.getUpperRightY()));
    if(r.contains(m_pos) == false)
    {
      r.moveCenter(m_pos);
      e.m_llx = r.left();
      e.m_lly = r.top();
      e.m_urx = r.right();
      e.m_ury = r.bottom();
      m_display->setExtent(e);
      return;
    }
  }
  
  unsigned int curTime = m_animation->currentTime();
  if(m_curTimeDuration == curTime)
    return;

  bool erase = false;
  if(m_animation->direction() == QAbstractAnimation::Forward)
  {
    if(curTime < m_curTimeDuration) // erase trail
      erase = true;
  }
  else
  {
    if(curTime > m_curTimeDuration) // erase trail
      erase = true;
  }

  if(erase)
  {
    if(m_erasePerfectly)
    {
      m_curTimeDuration = curTime;
      draw();
    }
    else
      this->erase(curTime);
  }
  else
    drawForward(curTime);

  m_display->update();
}

void te::qt::widgets::TrajectoryItem::drawForward(const unsigned int& curTime)
{
  if(m_animationRoute.empty())
    return;

  setMatrix();
  AnimationScene* as = (AnimationScene*)scene();
  QPixmap* scenePixmap = as->m_trajectoryPixmap;

  QPainter painter(scenePixmap);
  QPen pen(Qt::NoBrush, 2);
  QColor trailColor;
  if(m_animation->direction() == QAbstractAnimation::Forward)
    trailColor = m_forwardColor;
  else
    trailColor = m_backwardColor;

  pen.setColor(trailColor);
  painter.setPen(pen);
  painter.setBrush(Qt::NoBrush);

  int indold = m_animation->getAnimationDataIndex((double)m_curTimeDuration / (double)m_duration);
  int ind = m_animation->getAnimationDataIndex((double)curTime / (double)m_duration);
  m_curTimeDuration = curTime;

  QVector<QPointF> vec;

  if(indold == ind)
  {
    vec.push_back(m_posOld);
    vec.push_back(m_pos);
    QPolygonF polf(vec);
    QPolygon pol = m_matrix.map(polf).toPolygon();
    painter.drawPolyline(pol);
  }
  else if(m_animation->direction() == QAbstractAnimation::Forward)
  {
    while(indold <= ind)
      vec.push_back(m_animationRoute[indold++]);
    if(vec.isEmpty() == false && vec.last() != m_pos)
      vec.push_back(m_pos);
    QPolygonF polf(vec);
    QPolygon pol = m_matrix.map(polf).toPolygon();
    painter.drawPolyline(pol);
  }
  else
  {
    while(indold >= ind)
      vec.push_back(m_animationRoute[indold--]);
    if(vec.isEmpty() == false && vec.last() != m_pos)
      vec.push_back(m_pos);
    QPolygonF polf(vec);
    QPolygon pol = m_matrix.map(polf).toPolygon();
    painter.drawPolyline(pol);
  }
  painter.end();
}

void te::qt::widgets::TrajectoryItem::erase(const unsigned int& curTime)
{
  if(m_animationRoute.empty())
    return;

  setMatrix();
  AnimationScene* as = (AnimationScene*)scene();
  QPixmap* scenePixmap = as->m_trajectoryPixmap;

  QPainter painter(scenePixmap);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
  QPen pen(Qt::NoBrush, 2);
  QColor trailColor(Qt::white);
  pen.setColor(trailColor);
  painter.setPen(pen);
  painter.setBrush(Qt::NoBrush);

  int indold = m_animation->getAnimationDataIndex((double)m_curTimeDuration / (double)m_duration);
  int ind = m_animation->getAnimationDataIndex((double)curTime / (double)m_duration);
  m_curTimeDuration = curTime;

  QVector<QPointF> vec;
  if(indold == ind)
  {
    vec.push_back(m_posOld);
    vec.push_back(m_pos);
    QPolygonF polf(vec);
    QPolygon pol = m_matrix.map(polf).toPolygon();
    painter.drawPolyline(pol);
  }
  else if(m_animation->direction() == QAbstractAnimation::Backward)
  {
    vec.push_back(m_posOld);
    while(indold < ind)
      vec.push_back(m_animationRoute[indold++]);
    vec.push_back(m_pos);
    QPolygonF polf(vec);
    QPolygon pol = m_matrix.map(polf).toPolygon();
    painter.drawPolyline(pol);
  }
  else
  {
    vec.push_back(m_posOld);
    while(indold > ind)
      vec.push_back(m_animationRoute[indold--]);
    vec.push_back(m_pos);
    QPolygonF polf(vec);
    QPolygon pol = m_matrix.map(polf).toPolygon();
    painter.drawPolyline(pol);
  }
  painter.end();
}

void te::qt::widgets::TrajectoryItem::draw()
{
  if(m_animationRoute.empty())
    return;

  //setMatrix();
  AnimationScene* as = (AnimationScene*)scene();
  QPixmap* scenePixmap = as->m_trajectoryPixmap;

  QPainter painter(scenePixmap);
  QPen pen(Qt::NoBrush, 2);
  QColor trailColor;
  if(m_animation->direction() == QAbstractAnimation::Forward)
    trailColor = m_forwardColor;
  else
    trailColor = m_backwardColor;

  pen.setColor(trailColor);
  painter.setPen(pen);
  painter.setBrush(Qt::NoBrush);

  int count = m_animationRoute.size();
  int ind = m_animation->getAnimationDataIndex((double)m_curTimeDuration / (double)m_duration);

  QVector<QPointF> vec;
  if(m_animation->direction() == QAbstractAnimation::Forward)
  {
    if(ind > 0)
    {
      int i = 0;
      while(i <= ind)
        vec.push_back(m_animationRoute[i++]);
      if(vec.isEmpty() == false && vec.last() != m_pos)
        vec.push_back(m_pos);
      QPolygonF polf(vec);
      QPolygon pol = m_matrix.map(polf).toPolygon();
      painter.drawPolyline(pol);
    }
  }
  else
  {
    int i = count - 1;
    while(i >= ind)
      vec.push_back(m_animationRoute[i--]);
    if(m_curTimeDuration != m_duration)
    {
      if(vec.isEmpty() == false && vec.last() != m_pos)
        vec.push_back(m_pos);
    }
    QPolygonF polf(vec);
    QPolygon pol = m_matrix.map(polf).toPolygon();
    painter.drawPolyline(pol);
  }
  painter.end();
  m_display->update();
}

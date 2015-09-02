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
#include <QtCore/QMutex>


te::qt::widgets::TrajectoryItem::TrajectoryItem(const QString& title, const QString& id, te::qt::widgets::MapDisplay* display, const QString& file, const QSize& size)
  : te::qt::widgets::AnimationItem(title, display),
    m_iconFile(file),
    m_iconSize(size),
    m_doIconRotate(true),
    m_drawTrail(true),
    m_forwardColor(Qt::blue),
    m_backwardColor(Qt::magenta),
    m_lineWidth(2),
    m_layerId(id)
{
  if(m_iconSize.isValid() == false)
    m_iconSize = QSize(20, 20);

  if(file.isEmpty() == false)
  {
    QFile f(file);
    if(f.exists())
    {
      QPixmap p(file);
      QPixmap pp(m_iconSize);
      QPainter painter(&pp);
      painter.drawPixmap(pp.rect(), p, p.rect());
      painter.end();
      setPixmap(pp);
    }
  }

  if(pixmap().isNull())
  {
    QPixmap pix(m_iconSize);
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
}

te::qt::widgets::TrajectoryItem::~TrajectoryItem()
{
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
    double w = e.getWidth();
    double h = e.getHeight();
    QRectF r(e.getLowerLeftX(), e.getLowerLeftY(), w, h);
    double dist = 200;
    QRectF rm(e.getLowerLeftX()+w/dist, e.getLowerLeftY()+h/dist, w-2*w/dist, h-2*h/dist);
    if(rm.contains(m_pos) == false)
    {
      w -= w/20;
      h -= h/20;
      QPointF c = r.center();
      QPointF dif = m_pos - c;
      double dw = fabs(dif.x());
      double dh = fabs(dif.y());
      double mw = 0;
      double mh = 0;
      if(dw >= w/2)
        mw = (dw - w / 2) + w * m_panFactor;
      if(dh >= h/2)
        mh = (dh - h / 2) + h * m_panFactor;

      QPointF cc;
      if(dif.x() >= 0 && dif.y() >= 0)
        cc = c + QPointF(mw, mh);
      else if(dif.x() >= 0 && dif.y() < 0)
        cc = c + QPointF(mw, -mh);
      else if(dif.x() < 0 && dif.y() >= 0)
        cc = c + QPointF(-mw, mh);
      else if(dif.x() < 0 && dif.y() < 0)
        cc = c + QPointF(-mw, -mh);
      r.moveCenter(cc);

      if(r.contains(m_pos) == false)
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

  if(m_drawTrail)
  {
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
      //if(m_erasePerfectly)
      //{
      //  m_curTimeDuration = curTime;
      //  draw();
      //}
      //else
        this->erase(curTime);
    }
    else
      drawForward(curTime);
  }

  m_display->update();
}

void te::qt::widgets::TrajectoryItem::transformToDisplayProjection(QVector<QPointF>& vec)
{
  if (m_display->getSRID() != TE_UNKNOWN_SRS && m_display->getSRID() != m_SRID)
  {
    size_t size = vec.count();
    te::gm::LineString line(size, te::gm::LineStringType, m_SRID);
    for (size_t i = 0; i < size; ++i)
      line.setPoint(i, vec[(int)i].x(), vec[(int)i].y());
    line.transform(m_display->getSRID());

    vec.clear();
    for (size_t i = 0; i < size; ++i)
      vec.push_back(QPointF(line.getPointN(i)->getX(), line.getPointN(i)->getY()));
  }
}

void te::qt::widgets::TrajectoryItem::drawForward(const unsigned int& curTime)
{
  if(m_animationRoute.empty())
    return;

  int indold = m_animation->getAnimationDataIndex((double)m_curTimeDuration / (double)m_duration);
  int ind = m_animation->getAnimationDataIndex((double)curTime / (double)m_duration);
  m_curTimeDuration = curTime;
  if (ind == -1 || indold == -1)
    return;

  QVector<QPointF> vec;

  if(indold == ind)
  {
    vec.push_back(m_posOld);
    vec.push_back(m_pos);
  }
  else if(m_animation->direction() == QAbstractAnimation::Forward)
  {
    while(indold <= ind)
      vec.push_back(m_animationRoute[indold++]);
    if(vec.isEmpty() == false && vec.last() != m_pos)
      vec.push_back(m_pos);
  }
  else
  {
    while(indold >= ind)
      vec.push_back(m_animationRoute[indold--]);
    if(vec.isEmpty() == false && vec.last() != m_pos)
      vec.push_back(m_pos);
  }

  size_t size = vec.count();
  if (size > 1)
  {
    transformToDisplayProjection(vec);

    QPolygonF polf(vec);
    QPolygon pol = m_matrix.map(polf).toPolygon();

    QPen pen(Qt::NoBrush, 2);
    QColor trailColor;
    if(m_animation->direction() == QAbstractAnimation::Forward)
      trailColor = m_forwardColor;
    else
      trailColor = m_backwardColor;
    pen.setColor(trailColor);

    AnimationScene* as = (AnimationScene*)scene();
    as->m_mutex.lock();
    QPixmap* scenePixmap = as->m_trajectoryPixmap;
    QPainter painter(scenePixmap);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPolyline(pol);
    painter.end();
    as->m_mutex.unlock();
  }
}

void te::qt::widgets::TrajectoryItem::erase(const unsigned int& curTime)
{
  if(m_animationRoute.empty())
    return;

  int indold = m_animation->getAnimationDataIndex((double)m_curTimeDuration / (double)m_duration);
  int ind = m_animation->getAnimationDataIndex((double)curTime / (double)m_duration);
  m_curTimeDuration = curTime;
  if (ind == -1 || indold == -1)
    return;

  QVector<QPointF> vec;
  if(indold == ind)
  {
    vec.push_back(m_posOld);
    vec.push_back(m_pos);
  }
  else if(m_animation->direction() == QAbstractAnimation::Backward)
  {
    vec.push_back(m_posOld);
    while(indold < ind)
      vec.push_back(m_animationRoute[indold++]);
    vec.push_back(m_pos);
  }
  else
  {
    vec.push_back(m_posOld);
    while(indold > ind)
      vec.push_back(m_animationRoute[indold--]);
    vec.push_back(m_pos);
  }

  size_t size = vec.count();
if (size > 1)
{
  transformToDisplayProjection(vec);

  QPolygonF polf(vec);
  QPolygon pol = m_matrix.map(polf).toPolygon();

  QPen pen(Qt::NoBrush, 2);
  QColor trailColor(Qt::white);
  pen.setColor(trailColor);

  AnimationScene* as = (AnimationScene*)scene();
  as->m_mutex.lock();
  QPixmap* scenePixmap = as->m_trajectoryPixmap;
  QPainter painter(scenePixmap);
  painter.setPen(pen);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
  painter.setBrush(Qt::NoBrush);
  painter.drawPolyline(pol);
  painter.end();
  as->m_mutex.unlock();
}
}

void te::qt::widgets::TrajectoryItem::draw()
{
  if (m_animationRoute.empty())
    return;

  int ind = m_animation->getAnimationDataIndex((double)m_curTimeDuration / (double)m_duration);
  if (ind == -1)
    return;

  int w = m_display->getDisplayPixmap()->width();
  int h = m_display->getDisplayPixmap()->height();
  te::qt::widgets::Canvas canvas(w, h);
  te::gm::Envelope e = m_display->getExtent();
  canvas.calcAspectRatio(e.m_llx, e.m_lly, e.m_urx, e.m_ury);
  canvas.setWindow(e.m_llx, e.m_lly, e.m_urx, e.m_ury);
  m_matrix = canvas.getMatrix();

  int count = m_animationRoute.size();

  QVector<QPointF> vec;
  if (m_animation->direction() == QAbstractAnimation::Forward)
  {
    if (ind > 0)
    {
      int i = 0;
      while (i <= ind)
        vec.push_back(m_animationRoute[i++]);
      if (vec.isEmpty() == false && vec.last() != m_pos)
        vec.push_back(m_pos);
    }
  }
  else
  {
    int i = count - 1;
    while (i >= ind)
      vec.push_back(m_animationRoute[i--]);
    if (m_curTimeDuration != m_duration)
    {
      if (vec.isEmpty() == false && vec.last() != m_pos)
        vec.push_back(m_pos);
    }
  }

  size_t size = vec.count();
  if (size > 1)
  {
    transformToDisplayProjection(vec);

    QPolygonF polf(vec);
    QPolygon pol = m_matrix.map(polf).toPolygon();

    QPen pen(Qt::NoBrush, 2);
    QColor trailColor;
    if (m_animation->direction() == QAbstractAnimation::Forward)
      trailColor = m_forwardColor;
    else
      trailColor = m_backwardColor;

    pen.setColor(trailColor);

    AnimationScene* as = (AnimationScene*)scene();
    as->m_mutex.lock();
    QPixmap* scenePixmap = as->m_trajectoryPixmap;
    QPainter painter(scenePixmap);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPolyline(pol);
    painter.end();
    as->m_mutex.unlock();
  }
  m_display->update();
}

void te::qt::widgets::TrajectoryItem::drawIcon(QPainter* painter)
{
  QPoint pos = getPosInDeviceCoordinate();

  if (m_doIconRotate)
  {
    double xScale = -1.;
    double yScale = -1.;
    double angle = 0.;
    int ind = m_animation->getAnimationDataIndex((double)m_curTimeDuration / (double)m_duration);

    if (ind == -1)
      return;

    QVector<QPointF> vec;
    if (m_animation->direction() == QAbstractAnimation::Forward)
    {
      if (ind > 0)
      {
        vec.push_back(m_animationRoute[ind - 1]); // begin
        vec.push_back(m_animationRoute[ind]); // end
        transformToDisplayProjection(vec);
      }
    }
    else
    {
      if (ind < (m_animationRoute.count() - 1))
      {
        vec.push_back(m_animationRoute[ind]); // begin
        vec.push_back(m_animationRoute[ind + 1]); // end
        transformToDisplayProjection(vec);
      }
    }
    if (vec.isEmpty() == false)
    {
      double PI = 3.14159265;
      QPointF p = vec[1] - vec[0]; // end - begin
      if (p.x() > 0)
        yScale = 1.;

      if (p.x() == 0)
      {
        if (p.y() >= 0)
          angle = PI / 2;
        else
          angle = -PI / 2;
      }
      else if (p.y() == 0)
      {
        if (p.x() >= 0)
          angle = 0;
        else
          angle = -PI;
      }
      else
      {
        angle = atan(p.y() / p.x());
        if (p.x() < 0)
        {
          if (p.y() < 0)
            angle -= PI;
          else
            angle += PI;
        }
      }
      angle *= 180. / PI;
    }

    painter->save();
    painter->translate(pos);
    painter->scale(xScale, yScale);
    if (yScale == 1.)
      angle = -angle;
    painter->rotate(-angle);
    painter->translate(-pos);
  }

  QRect r = pixmap().rect();
  r.moveCenter(pos);
  QRect dr = m_display->rect();
  if (dr.intersects(r))
  {
    if (m_opacity == 255)
      painter->drawPixmap(r, pixmap());
    else
    {
      QSize size = pixmap().size();
      int width = size.width();
      int height = size.height();
      QImage ima = pixmap().toImage();

      if (ima.format() == QImage::Format_ARGB32)
      {
        for (int i = 0; i < height; ++i)
        {
          unsigned char* u = ima.scanLine(i);
          for (int j = 0; j < width; ++j)
          {
            QRgb* v = (QRgb*)(u + (j << 2));
            if (qAlpha(*v) < 50)
              *v = qRgba(255, 255, 255, 0);
            else
              *v = qRgba(qRed(*v), qGreen(*v), qBlue(*v), m_opacity);
          }
        }
        painter->drawImage(r, ima);
      }
      else
      {
        QImage img(size, QImage::Format_ARGB32);
        for (int i = 0; i < height; ++i)
        {
          unsigned char* u = ima.scanLine(i);
          unsigned char* uu = img.scanLine(i);

          for (int j = 0; j < width; ++j)
          {
            QRgb* v = (QRgb*)(u + (j << 2));
            QRgb* uv = (QRgb*)(uu + (j << 2));
            if (qAlpha(*v) < 50)
              *uv = qRgba(255, 255, 255, 0);
            else
              *uv = qRgba(qRed(*v), qGreen(*v), qBlue(*v), m_opacity);
          }
        }
        painter->drawImage(r, img);
      }
    }
  }

  if (m_doIconRotate)
    painter->restore();
}

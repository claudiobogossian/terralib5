// TerraLib
#include "PixmapItem.h"
#include "AnimationScene.h"
#include "Animation.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"

// Qt
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtCore/QVector>

te::qt::widgets::PixmapItem::PixmapItem(const QString& title, const QString& file, te::qt::widgets::MapDisplay* display)
  : te::qt::widgets::AnimationItem(title, display)
{
  setMatrix();

  QFileInfo fi(file);
  m_dir.setPath(fi.path());

  m_baseFile = fi.completeBaseName();
  m_suffix = fi.suffix();

  if(m_suffix.isEmpty())
    m_suffix = ".bin";

  for(size_t i = 0; i < 256; ++i)
    m_lut[i] = QColor(i, i, i, 255);
}

te::qt::widgets::PixmapItem::~PixmapItem()
{
}

void te::qt::widgets::PixmapItem::setImagePosition(const QPointF& p, const QRectF& dr)
{
  QString f = m_files[0];
  QString file = m_dir.path() + "/" + f;
  QImage ima(file);

  te::gm::Envelope env = m_display->getExtent();
  QRectF ar(env.m_llx, env.m_lly, env.getWidth(), env.getHeight());
  double fat = ar.width() / dr.width();

  QRectF r(0, 0, ((double)ima.rect().width()/4.)/fat, ((double)ima.rect().height()/4.)/fat);
  m_imaRect = m_matrix.inverted().mapRect(r);
  m_imaRect.moveCenter(p); 
}

void te::qt::widgets::PixmapItem::createAnimationDataInDisplayProjection()
{
  te::qt::widgets::AnimationItem::createAnimationDataInDisplayProjection();
  m_animationFiles.clear();

  te::dt::TimeInstant iTime = m_animation->m_temporalAnimationExtent.getInitialTimeInstant();
  te::dt::TimeInstant fTime = m_animation->m_temporalAnimationExtent.getFinalTimeInstant();

  size_t ini = 0;
  size_t size = m_time.size();
  size_t fim = size;
  for(size_t i = 0; i < size; ++i)
  {
    if(m_time[i] == iTime || m_time[i] > iTime)
    {
      ini = i;
      break;
    }
  }
  for(size_t i = size-1; i >= 0; --i)
  {
    if(m_time[i] == fTime || m_time[i] < fTime)
    {
      fim = i;
      break;
    }
  }
  size = fim - ini + 1;
  size_t tfim = ini + size;

  for(size_t i = ini; i < tfim; ++i)
  {
    QString f = m_files[i];
    m_animationFiles.push_back(f);
  }

  if(m_SRID != m_display->getSRID())
  {
    te::gm::Envelope env(m_imaRect.left(), m_imaRect.top(), m_imaRect.right(), m_imaRect.bottom());
    env.transform(m_SRID, m_display->getSRID());
    m_imaRect = QRectF(env.m_llx, env.m_lly, env.getWidth(), env.getHeight());
  }
}

void te::qt::widgets::PixmapItem::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
  unsigned int curTime = m_animation->currentTime();
  if(m_curTimeDuration == curTime)
    return;

  drawForward(curTime);
}

void te::qt::widgets::PixmapItem::drawForward(const unsigned int& curTime)
{
  m_pos = pos(); // image position
  m_curTimeDuration = curTime;
  calculateCurrentFile(curTime);
  m_display->update();
}

void te::qt::widgets::PixmapItem::erase(const unsigned int& curTime)
{
  drawForward(curTime);
}

void te::qt::widgets::PixmapItem::draw()
{
  calculateCurrentFile(m_curTimeDuration);
  m_display->update();
}

void te::qt::widgets::PixmapItem::calculateCurrentFile(const unsigned int& curTime)
{
  double nt = (double)curTime / (double)m_duration;
  int ind = m_animation->getAnimationDataIndex(nt);
  QString f = m_animationFiles[ind];
  m_currentImageFile = m_dir.path() + "/" + f;
  m_curTimeDuration = curTime;
}

QRect te::qt::widgets::PixmapItem::getRect()
{
  QRect r = m_matrix.mapRect(m_imaRect).toRect();
  return r;
}

void te::qt::widgets::PixmapItem::setLUT(const std::vector<std::pair<int, QColor> >& tab)
{
  size_t i = 0;
  std::vector<std::pair<int, QColor> >::const_iterator it = tab.cbegin();
  size_t v = (*it).first;
  QColor c = (*it).second;
  while(i <= 255)
  {
    while(i <= v)
      m_lut[i++] = c;

    ++it;
    if(it == tab.end())
      break;

    v = (*it).first;
    c = (*it).second;
  }

  while(i <= 255)
    m_lut[i++] = c;

}

//void te::qt::widgets::PixmapItem::reproject()
//{
//}
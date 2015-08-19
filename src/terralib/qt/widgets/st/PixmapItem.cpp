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

te::qt::widgets::PixmapItem::PixmapItem()
  : te::qt::widgets::AnimationItem("", 0)
{
}

te::qt::widgets::PixmapItem::PixmapItem(const QString& title, const QString& file, te::qt::widgets::MapDisplay* display)
  : te::qt::widgets::AnimationItem(title, display)
{
  m_dir = QDir(file);

  QFileInfo fi(file);
  m_baseFile = fi.completeBaseName();
  m_suffix = fi.suffix();

  if(m_suffix.isEmpty())
    m_suffix = ".bin";

  for(size_t i = 0; i < 256; ++i)
    m_lut[(uchar)i] = QColor((int)i, (int)i, (int)i, 255);
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

void te::qt::widgets::PixmapItem::adjustDataToAnimationTemporalExtent()
{
  te::qt::widgets::AnimationItem::adjustDataToAnimationTemporalExtent();
  m_animationFiles.clear();

  te::dt::TimeInstant iTime = m_animation->m_temporalAnimationExtent.getInitialTimeInstant();
  te::dt::TimeInstant fTime = m_animation->m_temporalAnimationExtent.getFinalTimeInstant();

  size_t ini = 0;
  size_t size = m_time.size();
  size_t fim = size;
  for(int i = 0; i < (int)size; ++i)
  {
    if(m_time[i] == iTime || m_time[i] > iTime)
    {
      ini = i;
      break;
    }
  }
  for(int i = (int)size-1; i >= 0; --i)
  {
    if(m_time[i] == fTime || m_time[i] < fTime)
    {
      fim = i;
      break;
    }
  }
  size = fim - ini + 1;
  size_t tfim = ini + size;

  for(int i = (int)ini; i < (int)tfim; ++i)
  {
    QString f = m_files[i];
    m_animationFiles.push_back(f);
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
  QRectF r = m_imaRect;
  if(m_display->getSRID() != TE_UNKNOWN_SRS && m_display->getSRID() != m_SRID)
  {
    te::gm::Envelope e(r.x(), r.y(), r.x() + r.width(), r.y() + r.height());
    e.transform(m_SRID, m_display->getSRID());
    r.setRect(e.getLowerLeftX(), e.getLowerLeftY(), e.getWidth(), e.getHeight());
  }

  return m_matrix.mapRect(r).toRect();
}

void te::qt::widgets::PixmapItem::setLUT(const std::vector<std::pair<int, QColor> >& tab)
{
  size_t i = 0;
  std::vector<std::pair<int, QColor> >::const_iterator it = tab.begin();
  size_t v = (*it).first;
  QColor c = (*it).second;
  while(i < 256)
  {
    while(i <= v)
      m_lut[(uchar)i++] = c;

    ++it;
    if(it == tab.end())
      break;

    v = (*it).first;
    c = (*it).second;
  }

  while(i < 256)
    m_lut[(uchar)i++] = c;

}

// TerraLib
#include "ImageItem.h"
#include "AnimationScene.h"
#include "Animation.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"

// Qt
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtCore/QVector>

te::qt::widgets::ImageItem::ImageItem()
  : te::qt::widgets::AnimationItem("", 0),
  m_image(0)
{
}

te::qt::widgets::ImageItem::ImageItem(const QString& title, const QString& folder, te::qt::widgets::MapDisplay* display)
  : te::qt::widgets::AnimationItem(title, display),
  m_image(0)
{
  m_dir = QDir(folder);

  QFileInfo fi(folder);
  m_baseFile = fi.completeBaseName();
  m_suffix = fi.suffix();

  if (m_suffix.isEmpty())
    m_suffix = ".bin";

  for (size_t i = 0; i < 256; ++i)
    m_lut[(uchar)i] = QColor((int)i, (int)i, (int)i, 255);
}

te::qt::widgets::ImageItem::~ImageItem()
{
}

void te::qt::widgets::ImageItem::setImagePosition(const QPointF& p, const QRectF& dr)
{
  QString f = m_files[0];
  QString file = m_dir.path() + "/" + f;
  QImage ima(file);

  te::gm::Envelope env = m_display->getExtent();
  QRectF ar(env.m_llx, env.m_lly, env.getWidth(), env.getHeight());
  double fat = ar.width() / dr.width();

  QRectF r(0, 0, ((double)ima.rect().width() / 4.) / fat, ((double)ima.rect().height() / 4.) / fat);
  m_imaRect = m_matrix.inverted().mapRect(r);
  m_imaRect.moveCenter(p);
}

bool te::qt::widgets::ImageItem::loadData()
{
  if (getCtlParameters() == false)
    return false;

  //m_animationScene->addItem(pi);
  if (m_animation)
    delete m_animation;
  m_animation = new te::qt::widgets::Animation(this, "pos");
  m_animation->m_spatialExtent = te::gm::Envelope(m_imaRect.x(), m_imaRect.y(), m_imaRect.right(), m_imaRect.bottom());
  m_animation->setEasingCurve(QEasingCurve::Linear);
  //m_parallelAnimation->addAnimation(animation);

  QString suffix = "." + m_suffix;
  QStringList nameFilter;
  nameFilter.append("*" + suffix);
  QStringList files = m_dir.entryList(nameFilter, QDir::Files, QDir::Name);
  size_t count = files.count();

  // get time extent
  te::dt::TimeInstant ti = getTime(m_dir.path() + "/" + files.first());
  te::dt::TimeInstant tf = getTime(m_dir.path() + "/" + files.last());
  m_animation->m_temporalExtent = te::dt::TimePeriod(ti, tf);

  // set data
  te::dt::TimeInstant t;
  for (size_t i = 0; i < count; ++i)
  {
    QString f = files[(int)i];
    t = getTime(f);
    m_time.push_back(t);
    m_files.push_back(f);
  }

  generateRoute();

  return true;
}

bool te::qt::widgets::ImageItem::getCtlParameters()
{
  char buf[2000];
  QString file(m_dir.path() + "/image.ctl");
  FILE* fp = fopen(file.toStdString().c_str(), "r");
  if (fp == 0)
    return false;

  fread(buf, 2000, sizeof(char), fp);
  fclose(fp);
  QString s, ss(QString(buf).simplified());

  // validation
  if (!(ss.contains("suffix ", Qt::CaseInsensitive) && ss.contains("undef", Qt::CaseInsensitive) &&
    ss.contains("srid", Qt::CaseInsensitive) && ss.contains("llx", Qt::CaseInsensitive) &&
    ss.contains("lly", Qt::CaseInsensitive) && ss.contains("urx", Qt::CaseInsensitive) &&
    ss.contains("ury", Qt::CaseInsensitive)))
    return false;

  // get suffix 
  size_t pos = ss.indexOf("suffix ", Qt::CaseInsensitive) + strlen("suffix ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" undef", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  m_suffix = s;
  ss.remove(0, (int)pos);

  // get undef
  pos = ss.indexOf("undef ", Qt::CaseInsensitive) + (int)strlen("undef ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" srid", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  m_undef = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  // get srid
  pos = ss.indexOf("srid ", Qt::CaseInsensitive) + (int)strlen("srid ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" llx", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  m_SRID = atoi(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  // get llx
  pos = ss.indexOf("llx ", Qt::CaseInsensitive) + strlen("llx ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" lly", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  double llx = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  // get lly
  pos = ss.indexOf("lly ", Qt::CaseInsensitive) + (int)strlen("lly ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" urx", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  double lly = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  // get urx
  pos = ss.indexOf("urx ", Qt::CaseInsensitive) + strlen("urx ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" ury", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  double urx = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  // get ury
  pos = ss.indexOf("ury ", Qt::CaseInsensitive) + strlen("ury ");
  ss.remove(0, (int)pos);
  double ury = atof(ss.toStdString().c_str());

  double w = urx - llx;
  double h = ury - lly;
  m_imaRect = QRectF(llx, lly, w, h);

  return true;
}

void te::qt::widgets::ImageItem::loadCurrentImage()
{
  if (m_image)
    delete m_image;
  m_image = 0;

  if (m_currentImageFile.isEmpty())
    return;

  QString file = m_currentImageFile;
  m_image = new QImage(file);
}

void te::qt::widgets::ImageItem::drawCurrentImage(QPainter* painter)
{
  if (m_image == 0)
    return;

  painter->save();

  tryDoReprojectionUsingAffineTransform(painter);
  QRect r(getRect());

  if (m_opacity == 255)
    painter->drawImage(r, *m_image);
  else
  {
    QSize size = m_image->size();
    int width = size.width();
    int height = size.height();

    if (m_image->format() == QImage::Format_ARGB32)
    {
      for (int i = 0; i < height; ++i)
      {
        unsigned char* u = m_image->scanLine(i);
        for (int j = 0; j < width; ++j)
        {
          QRgb* v = (QRgb*)(u + (j << 2));
          if (qAlpha(*v) > 50)
            *v = qRgba(qRed(*v), qGreen(*v), qBlue(*v), m_opacity);
        }
      }
      painter->drawImage(r, *m_image);
    }
    else
    {
      QImage img(size, QImage::Format_ARGB32);
      for (int i = 0; i < height; ++i)
      {
        unsigned char* u = m_image->scanLine(i);
        unsigned char* uu = img.scanLine(i);

        for (int j = 0; j < width; ++j)
        {
          QRgb* v = (QRgb*)(u + (j << 2));
          QRgb* uv = (QRgb*)(uu + (j << 2));
          if (qAlpha(*v) > 50)
            *uv = qRgba(qRed(*v), qGreen(*v), qBlue(*v), m_opacity);
        }
      }
      painter->drawImage(r, img);
    }
  }
  delete m_image;
  m_image = 0;

  painter->restore();
}

void te::qt::widgets::ImageItem::tryDoReprojectionUsingAffineTransform(QPainter* painter)
{
  // If the projection is different, try do reprojection using affine transform.
  // Note: For small areas it gives a good result, however, for larger areas the result is not good.
  if (m_display->getSRID() != TE_UNKNOWN_SRS && m_display->getSRID() != m_SRID)
  {
    // get width, height and rotation
    te::gm::LineString line(9, te::gm::LineStringType, m_SRID);
    line.setPoint(0, m_imaRect.x(), m_imaRect.y());
    line.setPoint(1, m_imaRect.x(), m_imaRect.y() + m_imaRect.height());
    line.setPoint(2, m_imaRect.x() + m_imaRect.width(), m_imaRect.y() + m_imaRect.height());
    line.setPoint(3, m_imaRect.x() + m_imaRect.width(), m_imaRect.y());
    line.setPoint(4, m_imaRect.center().x(), m_imaRect.center().y());
    line.setPoint(5, m_imaRect.x(), m_imaRect.center().y());
    line.setPoint(6, m_imaRect.x() + m_imaRect.width(), m_imaRect.center().y());
    line.setPoint(7, m_imaRect.center().x(), m_imaRect.y());
    line.setPoint(8, m_imaRect.center().x(), m_imaRect.y() + m_imaRect.height());
    line.transform(m_display->getSRID());

    // transform to device coordinate
    QPointF p0 = m_matrix.map(QPointF(line.getPointN(0)->getX(), line.getPointN(0)->getY()));
    QPointF p1 = m_matrix.map(QPointF(line.getPointN(1)->getX(), line.getPointN(1)->getY()));
    QPointF p2 = m_matrix.map(QPointF(line.getPointN(2)->getX(), line.getPointN(2)->getY()));
    QPointF p3 = m_matrix.map(QPointF(line.getPointN(3)->getX(), line.getPointN(3)->getY()));
    QPointF c = m_matrix.map(QPointF(line.getPointN(4)->getX(), line.getPointN(4)->getY()));
    QPointF p5 = m_matrix.map(QPointF(line.getPointN(5)->getX(), line.getPointN(5)->getY()));
    QPointF p6 = m_matrix.map(QPointF(line.getPointN(6)->getX(), line.getPointN(6)->getY()));
    QPointF p7 = m_matrix.map(QPointF(line.getPointN(7)->getX(), line.getPointN(7)->getY()));
    QPointF p8 = m_matrix.map(QPointF(line.getPointN(8)->getX(), line.getPointN(8)->getY()));
    QPointF ph(p8 - p7);
    QPointF ph1(p0 - p1);
    QPointF ph2(p3 - p2);
    QPointF pw(p6 - p5);
    QPointF pw1(p2 - p1);
    QPointF pw2(p3 - p0);
    QPointF prot(p6 - c);

    double w = fabs(pw.x());
    double h = fabs(ph.y());

    double PI = 3.14159265;
    double rad = 0;
    painter->translate(c.toPoint());

    if (prot.x() == 0)
    {
      if (prot.y() >= 0)
        rad = PI / 2.;
      else
        rad = -PI / 2.;
    }
    else if (prot.y() == 0)
    {
      if (prot.x() >= 0)
        rad = 0;
      else
        rad = -PI;
    }
    else
    {
      rad = atan(prot.y() / prot.x());
      if (prot.x() < 0)
      {
        if (prot.y() < 0)
          rad -= PI;
        else
          rad += PI;
      }
    }

    // set scale indirectly - calculate a new image rect
    if (rad != 0)
    {
      w /= fabs(cos(rad));
      h /= fabs(cos(PI + rad));
    }

    QRect r(0, 0, w, h);
    r.moveCenter(c.toPoint()); // move to center

    if (ph1.x() != 0 && ph2.x() != 0 && ph1.y() == ph2.y()) // make horizontal shear
    {
      double horiz = (ph1.x() + ph2.x()) / 1.35;
      painter->shear(horiz / w, 0);
    }
    else if (pw1.y() != 0 && pw2.y() != 0 && pw1.x() == pw2.x()) // make vertical shear
    {
      double vert = (pw1.y() + pw2.y()) / 1.35;
      painter->shear(0, vert / h);
    }
    else if (rad != 0) // make rotation
    {
      double degree = rad * 180. / PI;
      painter->rotate(degree);
    }
    painter->translate(-c.toPoint());
  }
}

void te::qt::widgets::ImageItem::adjustDataToAnimationTemporalExtent()
{
  te::qt::widgets::AnimationItem::adjustDataToAnimationTemporalExtent();
  m_animationFiles.clear();

  te::dt::TimeInstant iTime = m_animation->m_temporalAnimationExtent.getInitialTimeInstant();
  te::dt::TimeInstant fTime = m_animation->m_temporalAnimationExtent.getFinalTimeInstant();

  size_t ini = 0;
  size_t size = m_time.size();
  size_t fim = size;
  for (int i = 0; i < (int)size; ++i)
  {
    if (m_time[i] == iTime || m_time[i] > iTime)
    {
      ini = i;
      break;
    }
  }
  for (int i = (int)size - 1; i >= 0; --i)
  {
    if (m_time[i] == fTime || m_time[i] < fTime)
    {
      fim = i;
      break;
    }
  }
  //size = fim - ini + 1;
  size = fim - ini;
  size_t tfim = ini + size;

  size_t count = m_files.count();
  if (tfim > count)
    tfim = count;

  for (int i = (int)ini; i < (int)tfim; ++i)
  {
    QString f = m_files[i];
    m_animationFiles.push_back(f);
  }
}

void te::qt::widgets::ImageItem::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
{
  unsigned int curTime = m_animation->currentTime();
  if (m_curTimeDuration == curTime)
    return;

  drawForward(curTime);
}

void te::qt::widgets::ImageItem::drawForward(const unsigned int& curTime)
{
  m_pos = pos(); // image position
  m_curTimeDuration = curTime;
  calculateCurrentFile(curTime);
  m_display->update();
}

void te::qt::widgets::ImageItem::erase(const unsigned int& curTime)
{
  drawForward(curTime);
}

void te::qt::widgets::ImageItem::draw()
{
  calculateCurrentFile(m_curTimeDuration);
  m_display->update();
}

void te::qt::widgets::ImageItem::calculateCurrentFile(const unsigned int& curTime)
{
  double nt = (double)curTime / (double)m_duration;
  int ind = m_animation->getAnimationDataIndex(nt);
  if (ind < m_animationFiles.count())
  {
    QString f = m_animationFiles[ind];
    m_currentImageFile = m_dir.path() + "/" + f;
  }
  else
    m_currentImageFile.clear();

  m_curTimeDuration = curTime;
}

QRect te::qt::widgets::ImageItem::getRect()
{
  QRectF r = m_imaRect;
  if (m_display->getSRID() != TE_UNKNOWN_SRS && m_display->getSRID() != m_SRID)
  {
    te::gm::Envelope e(r.x(), r.y(), r.x() + r.width(), r.y() + r.height());
    e.transform(m_SRID, m_display->getSRID());
    r.setRect(e.getLowerLeftX(), e.getLowerLeftY(), e.getWidth(), e.getHeight());
  }

  return m_matrix.mapRect(r).toRect();
}

void te::qt::widgets::ImageItem::setLUT(const std::vector<std::pair<int, QColor> >& tab)
{
  size_t i = 0;
  std::vector<std::pair<int, QColor> >::const_iterator it = tab.begin();
  size_t v = (*it).first;
  QColor c = (*it).second;
  while (i < 256)
  {
    while (i <= v)
      m_lut[(uchar)i++] = c;

    ++it;
    if (it == tab.end())
      break;

    v = (*it).first;
    c = (*it).second;
  }

  while (i < 256)
    m_lut[(uchar)i++] = c;

}

void te::qt::widgets::ImageItem::generateRoute()
{
  size_t count = m_files.count();
  m_route = new te::gm::LineString(count, te::gm::LineStringType, m_SRID);

  // crie valores não repetitivos e nem muito grandes ou pequenos
  QPointF pos(m_animation->m_spatialExtent.m_llx, m_animation->m_spatialExtent.m_lly);
  double w = m_animation->m_spatialExtent.getWidth();
  double h = m_animation->m_spatialExtent.getHeight();
  int tam = 8;
  double dw = w / (double)tam;
  double dh = h / (double)tam;
  size_t i = 0;
  while (i < count)
  {
    m_route->setPoint(i, pos.x(), pos.y());
    if (i++ & tam)
      pos -= QPointF(dw, dh);
    else
      pos += QPointF(dw, dh);
  }
}
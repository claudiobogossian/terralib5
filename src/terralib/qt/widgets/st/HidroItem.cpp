// TerraLib
#include "HidroItem.h"
#include "AnimationScene.h"
#include "Animation.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"

// Qt
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtCore/QVector>

te::qt::widgets::HidroItem::HidroItem()
  : te::qt::widgets::ImageItem()
{
}

te::qt::widgets::HidroItem::HidroItem(const QString& title, const QString& folder, te::qt::widgets::MapDisplay* display)
  : te::qt::widgets::ImageItem(title, folder, display)
{
}

te::qt::widgets::HidroItem::~HidroItem()
{
}

bool te::qt::widgets::HidroItem::loadData()
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

  QStringList nameFilter;
  nameFilter.append("*.bin");
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
  setLUT();
  return true;
}

bool te::qt::widgets::HidroItem::getCtlParameters()
{
  char buf[2000];
  QString file(m_dir.path() + "/racc.ctl");
  FILE* fp = fopen(file.toStdString().c_str(), "r");
  if(fp == 0)
    return false;

  fread(buf, 2000, sizeof(char), fp);
  fclose(fp);
  QString s, ss(QString(buf).simplified());

  // validation
  if(!(ss.contains("undef ", Qt::CaseInsensitive) && ss.contains("title", Qt::CaseInsensitive) && 
    ss.contains("xdef", Qt::CaseInsensitive) && ss.contains("ydef", Qt::CaseInsensitive) && 
    ss.contains("linear", Qt::CaseInsensitive) && ss.contains("zdef", Qt::CaseInsensitive)))
    return false;

  // CHUTE SRID 4326 WGS84
  m_SRID = 4326;

  // get UNDEF value
  size_t pos = ss.indexOf("UNDEF ", Qt::CaseInsensitive) + strlen("UNDEF "); 
  ss.remove(0, (int)pos);
  pos = ss.indexOf("TITLE", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  m_undef = atoi(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  // get XDEF ulx and resX values
  pos = ss.indexOf("XDEF ", Qt::CaseInsensitive) + strlen("XDEF ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" ");
  s = ss.left((int)pos);
  m_ncols = atoi(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  pos = ss.indexOf("LINEAR ", Qt::CaseInsensitive) + strlen("LINEAR ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" ");
  s = ss.left((int)pos);
  double llx = atof(s.toStdString().c_str()) - 360.;
  ss.remove(0, (int)pos);
  pos = ss.indexOf("YDEF ", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  double resX = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  // get YDEF uly and resY values
  pos = ss.indexOf("YDEF ", Qt::CaseInsensitive) + strlen("YDEF ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" ");
  s = ss.left((int)pos);
  m_nlines = atoi(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  pos = ss.indexOf("LINEAR ", Qt::CaseInsensitive) + strlen("LINEAR ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" ");
  s = ss.left((int)pos);
  double lly = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);
  pos = ss.indexOf("ZDEF ", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  double resY = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  double w = (double)m_ncols * resX;
  double h = (double)m_nlines * resY;
  m_imaRect = QRectF(llx, lly, w, h);

  return true;
}

void te::qt::widgets::HidroItem::loadCurrentImage()
{
  if (m_image)
    delete m_image;
  m_image = 0;

  QString path = m_dir.absolutePath() + "/";
  QString file = m_currentImageFile;
  QFileInfo fi(file);
  QString baseName = fi.baseName();

  if (m_suffix == ".bin" && baseName == "racc")
  {
    size_t nchars = m_ncols * 2;
    uchar* buf = new uchar[nchars];
    FILE* fp = fopen(file.toStdString().c_str(), "rb");
    m_image = new QImage((int)m_ncols, (int)m_nlines, QImage::Format_ARGB32);
    m_image->fill(Qt::transparent);

    uchar uc[3];
    uc[2] = 0;
    for (size_t j = 0; j < m_nlines; ++j)
    {
      uchar* u = m_image->scanLine((int)j);
      fread(buf, nchars, sizeof(char), fp);
      for (size_t i = 0; i < m_ncols; i++)
      {
        uc[0] = *(buf + (i << 1));
        uc[1] = *(buf + (1 + (i << 1)));
        ushort b = *(ushort*)uc;
        if (b != m_undef)
        {
          b = (b + 5) / 10;
          QRgb* v = (QRgb*)(u + (i << 2));
          *v = qRgba(m_lut[b].red(), m_lut[b].green(), m_lut[b].blue(), 255);
        }
      }
    }
    fclose(fp);
    delete[]buf;
  }
}

te::dt::TimeInstant te::qt::widgets::HidroItem::getTime(QString fileName)
{
  QString file(fileName);
  int ind = file.indexOf("racc.") + (int)strlen("racc.");
  file.remove(0, ind);
  QString ano = "20" + file.mid(0, 2);
  QString mes = file.mid(2, 2);
  QString dia = file.mid(4, 2);
  QString hour = file.mid(7, 2);
  QString min = file.mid(9, 2);

  te::dt::Date date(ano.toInt(), mes.toInt(), dia.toInt());
  te::dt::TimeDuration dur(hour.toInt(), min.toInt(), 0);
  return te::dt::TimeInstant(date, dur);
}

void te::qt::widgets::HidroItem::setLUT()
{
  std::vector<std::pair<int, QColor> > tab;
  int i = 0;
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(2, 1, 201, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(1, 71, 254, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(3, 148, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(5, 200, 251, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(2, 254, 233, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(1, 254, 151, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(2, 254, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(99, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(200, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(237, 255, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(251, 232, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(254, 199, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(252, 179, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(254, 147, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(253, 99, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(254, 1, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(254, 0, 49, 255)));
  tab.push_back(std::pair<int, QColor>(i += 3, QColor(255, 5, 1, 255)));

  te::qt::widgets::ImageItem::setLUT(tab);
}

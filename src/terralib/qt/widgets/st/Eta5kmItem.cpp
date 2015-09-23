// TerraLib
#include "Eta5kmItem.h"
#include "AnimationScene.h"
#include "Animation.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"

// Qt
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtCore/QVector>

te::qt::widgets::Eta5kmItem::Eta5kmItem()
  : te::qt::widgets::ImageItem()
{
}

te::qt::widgets::Eta5kmItem::Eta5kmItem(const QString& title, const QString& folder, te::qt::widgets::MapDisplay* display)
  : te::qt::widgets::ImageItem(title, folder, display)
{
}

te::qt::widgets::Eta5kmItem::~Eta5kmItem()
{
}

bool te::qt::widgets::Eta5kmItem::loadData()
{
  if(getCtlParameters() == false)
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
  // the file contains 3 days of data
  tf = te::dt::TimeInstant(tf.getTimeInstant() + boost::posix_time::seconds(60 * 60 * 24 * 3 - 3600));
  m_animation->m_temporalExtent = te::dt::TimePeriod(ti, tf);

  // set data
  te::dt::TimeInstant tnext, t;
  QString sn, fname;
  for (size_t i = 0; i < count; ++i)
  {
    QString f = files[(int)i];
    QFileInfo fi(m_dir.path() + "/" + f);
    QString baseName = fi.baseName();
    t = getTime(f);

    if (i != count - 1)
      tnext = getTime(files[(int)i + 1]);
    else
      tnext = te::dt::TimeInstant(t.getTimeInstant() + boost::posix_time::seconds(60 * 60 * 24 * 3));

    size_t n = 1;
    while ((t < tnext || t == tnext) && n <= 72)
    {
      m_time.push_back(t);
      t = te::dt::TimeInstant(t.getTimeInstant() + boost::posix_time::seconds(60 * 60));
      sn.setNum(n++);
      fname = m_dir.path() + "/" + baseName + "_" + sn + "." + fi.suffix();
      m_files.push_back(fname);
    }
  }

  generateRoute();
  setLUT();

  return true;
}

bool te::qt::widgets::Eta5kmItem::getCtlParameters()
{
  char buf[300];
  QString file(m_dir.path() + "/Prec5km.ctl");
  FILE* fp = fopen(file.toStdString().c_str(), "r");
  if (fp == 0)
    return false;

  size_t n = fread(buf, sizeof(char), 300, fp);
  fclose(fp);
  buf[n] = 0;
  QString ss(QString(buf).simplified());

  // validation
  if (!(ss.contains("undef ", Qt::CaseInsensitive) && ss.contains("xdef", Qt::CaseInsensitive)
    && ss.contains("ydef", Qt::CaseInsensitive) && ss.contains("linear", Qt::CaseInsensitive)
    && ss.contains("zdef", Qt::CaseInsensitive)))
    return false;

  QString s;

  // CHUTE SRID 4326 WGS84
  m_SRID = 4326;

  // get UNDEF value
  size_t pos = ss.indexOf("undef ", Qt::CaseInsensitive) + strlen("undef ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf("xdef", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  m_undef = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  // get XDEF ulx and resX values
  pos = ss.indexOf("xdef ", Qt::CaseInsensitive) + strlen("xdef ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" ");
  s = ss.left((int)pos);
  m_ncols = atoi(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  pos = ss.indexOf("linear ", Qt::CaseInsensitive) + strlen("linear ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" ");
  s = ss.left((int)pos);
  double llx = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);
  pos = ss.indexOf("ydef ", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  double resX = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  // get YDEF uly and resY values
  pos = ss.indexOf("ydef ", Qt::CaseInsensitive) + strlen("ydef ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" ");
  s = ss.left((int)pos);
  m_nlines = atoi(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  pos = ss.indexOf("linear ", Qt::CaseInsensitive) + strlen("linear ");
  ss.remove(0, (int)pos);
  pos = ss.indexOf(" ");
  s = ss.left((int)pos);
  double lly = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);
  pos = ss.indexOf("zdef ", Qt::CaseInsensitive);
  s = ss.left((int)pos);
  double resY = atof(s.toStdString().c_str());
  ss.remove(0, (int)pos);

  double w = (double)m_ncols * resX;
  double h = (double)m_nlines * resY;
  m_imaRect = QRectF(llx, lly, w, h);

  // get static representation
  if (m_dir.exists("staticRepresentation.png"))
    m_staticRepresentation = QImage(m_dir.path() + "/staticRepresentation.png");

  return true;
}

void te::qt::widgets::Eta5kmItem::loadCurrentImage()
{
  if (m_image)
    delete m_image;
  m_image = 0;

  if (m_currentImageFile.isEmpty())
  {
    if (m_staticRepresentation.isNull())
    {
      QRect r = getRect();
      m_image = new QImage(r.size(), QImage::Format_ARGB32);
      m_image->fill(QColor(0, 0, 255, 100));
      QPainter p(m_image);
      p.setPen(QPen(QColor(255, 0, 0)));

      QFont font(p.font());
      int ps = 7;
      int w = (int)((double)m_image->width() / 1.2);
      int h = (int)((double)m_image->width() / 5.);

      font.setPointSize(ps);
      QFontMetrics fm(font);
      QRectF rec(fm.boundingRect(m_title));

      while (rec.width() < w && rec.height() < h)
      {
        ++ps;
        font.setPointSize(ps);
        QFontMetrics fm(font);
        rec = fm.boundingRect(m_title);
      }
      rec.moveCenter(m_image->rect().center());
      p.setFont(font);
      p.drawText(rec.toRect(), Qt::AlignLeft, m_title);
    }
    else
      m_image = new QImage(m_staticRepresentation);
  }
  else
  {
    QString path = m_dir.absolutePath() + "/";
    QString file = m_currentImageFile;
    QFileInfo fi(file);
    QString baseName = fi.baseName();

    if (m_suffix == ".bin" && baseName.contains("Prec5km"))
    {
      QString auxFile(file);
      size_t pos = auxFile.indexOf(baseName);
      auxFile.remove(0, (int)pos);
      pos = auxFile.indexOf("_");
      size_t pp = auxFile.indexOf(".bin");
      int offset = atoi(auxFile.mid((int)pos + 1, (int)pp - (int)pos + 1).toStdString().c_str());
      size_t fileSize = m_nlines * m_ncols * 4 + 8; // dado é float e desprepreza 4 bytes iniciais e 4 bytes finais
      offset *= (int)fileSize;
      auxFile.remove((int)pos, auxFile.length() - (int)pos);
      auxFile = path + auxFile + m_suffix;

      size_t nchars = m_ncols * 4;
      uchar* buf = new uchar[nchars];
      FILE* fp = fopen(auxFile.toStdString().c_str(), "rb");
      fseek(fp, offset, SEEK_SET);
      fseek(fp, 4, SEEK_CUR); // despreza 4 bytes da primeira linha
      m_image = new QImage((int)m_ncols, (int)m_nlines, QImage::Format_ARGB32);
      m_image->fill(Qt::transparent);

      uchar uc[5];
      uc[4] = 0;
      for (size_t j = 0; j < m_nlines; ++j)
      {
        uchar* u = m_image->scanLine((int)m_nlines - 1 - (int)j); // origem bottom left
        fread(buf, nchars, sizeof(char), fp);

        for (size_t i = 0; i < m_ncols; i++)
        {
          uc[0] = *(buf + (i << 2));
          uc[1] = *(buf + (1 + (i << 2)));
          uc[2] = *(buf + (2 + (i << 2)));
          uc[3] = *(buf + (3 + (i << 2)));
          float b = *(float*)uc;
          if (b != m_undef)
          {
            uchar a = (uchar)(b * 10000. + .5);
            QRgb* v = (QRgb*)(u + (i << 2));
            *v = qRgba(m_lut[a].red(), m_lut[a].green(), m_lut[a].blue(), 255);
          }
        }
      }
      fclose(fp);
      delete[]buf;
    }
  }
}

//void drawCurrentImage(QPainter* p)
//{
//}

te::dt::TimeInstant te::qt::widgets::Eta5kmItem::getTime(QString fileName)
{
  QString file(fileName);
  int ind = file.indexOf("Prec5km") + (int)strlen("Prec5km");
  file.remove(0, ind);
  QString ano = file.mid(0, 4);
  QString mes = file.mid(4, 2);
  QString dia = file.mid(6, 2);
  QString hour = file.mid(8, 2);

  te::dt::Date date(ano.toInt(), mes.toInt(), dia.toInt());
  te::dt::TimeDuration dur(hour.toInt() + 1, 0, 0); // tem deslocamento de 1 hora
  return te::dt::TimeInstant(date, dur);
}

void te::qt::widgets::Eta5kmItem::setLUT()
{
  std::vector<std::pair<int, QColor> > tab;
  int i = 0;
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(2, 1, 201, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(2, 18, 213, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 24, 220, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 36, 227, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 54, 240, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 71, 254, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 90, 252, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(3, 106, 251, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(3, 130, 253, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(3, 148, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(3, 160, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(4, 174, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(4, 186, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(5, 200, 251, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(5, 214, 246, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(3, 227, 241, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(3, 240, 237, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(2, 254, 233, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(2, 254, 212, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 254, 190, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 254, 170, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 254, 160, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 254, 151, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 254, 105, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 254, 72, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(1, 254, 37, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(2, 254, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(25, 254, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(56, 254, 3, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(71, 254, 3, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(99, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(125, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(150, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(175, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(200, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(209, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(218, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(229, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(237, 255, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(240, 248, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(244, 241, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(250, 237, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(251, 232, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(251, 226, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(252, 215, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(253, 208, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 199, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 194, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(253, 190, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(253, 184, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(252, 179, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(252, 172, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(253, 164, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(253, 155, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 147, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 136, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 125, 3, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 112, 3, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(253, 99, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(253, 75, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(253, 50, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(253, 25, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 1, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 1, 12, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 1, 25, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 1, 37, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 0, 49, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 2, 37, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 3, 25, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(254, 4, 12, 255)));
  tab.push_back(std::pair<int, QColor>(i += 1, QColor(255, 5, 1, 255)));

  te::qt::widgets::ImageItem::setLUT(tab);
}

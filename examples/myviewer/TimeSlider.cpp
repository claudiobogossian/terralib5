#include "TimeSlider.h"
#include "QTimerEvent"
#include "QPixmap"
#include "QPainter"
#include "QHBoxLayout"
#include "QDir.h"

#include "MyLayer.h"
#include "MyDisplay.h"

//TerraLib
#include <terralib/qt/widgets.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st/observation/Observation.h>
#include <terralib/st/observation/SpatioTemporalObservationSet.h>
#include <terralib/st/coverage/RasterCoverage.h>
#include <terralib/stloader/DataLoader.h>

#include "STExamples.h"

TimeSlider::TimeSlider(te::map::MapDisplay* md, QWidget* parent) : 
  QSlider(Qt::Horizontal, parent),
  m_mapDisplay(md),
  m_lines(false),
  m_loop(false),
  m_timerId(0),
  m_initialTime(0),
  m_finalTime(0),
  m_currentTime(0),
  m_temporalDrawingConfig(0),
  m_play(false), 
  m_stop(true),
  m_value(0)
{
  QDir dir;
  if(dir.cd(""TE_DATA_EXAMPLE_LOCALE"/data/cursorShapes") == false)
      dir.cd("../../images");

  m_playPixmap = new QPixmap(dir.absolutePath() + "/play.png");
  m_pausePixmap = new QPixmap(dir.absolutePath() + "/pause.png");

  int w = m_playPixmap->width();

  ((MyDisplay*)m_mapDisplay)->setTimeSliderIcon(m_playPixmap);

  connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueChangedSlot(int)));
  stop();
}

TimeSlider::~TimeSlider()
{
}

bool TimeSlider::loadMovingObjects(bool addTime)
{
  //como vai ter apenas 1 ativo (ou vetorial ou temporal
  // vamos simplificar o codigo...
  std::vector<te::map::AbstractLayer*>::iterator it;
  for(it = m_layers.begin(); it != m_layers.end(); ++it)
  {
    te::map::AbstractLayer* parent = (te::map::AbstractLayer*)(*it)->getParent();
    if(parent->getId() == "MovingObjects")
    {
      if(m_mObs.empty())
      {
        //carregar os moving objects: 40 e 41
        std::string XMLFileName = TE_DATA_EXAMPLE_LOCALE"/data/kml/t_40_41_metadata.xml";
        MovingObjectsFromKMLAndMetadata(m_mObs, XMLFileName);

        te::dt::TimeInstant* t = 0;
        te::dt::TimeInstant* tini = 0;
        te::dt::TimeInstant* tfim = 0;
        if(addTime)
        {
          te::dt::TimeInstant* tini = m_initialTime;
          te::dt::TimeInstant* tfim = m_finalTime;
        }

        for(int j = 0; j < (int)m_mObs.size(); j++)
        {
          te::dt::DateTimePeriod* period = m_mObs[j]->temporalExtent();
          t = (te::dt::TimeInstant*)(period->getInitialInstant());
          if(tini == 0 || t < tini)
            tini = t;
          t = (te::dt::TimeInstant*)(period->getFinalInstant());
          if(tfim == 0 || t > tfim)
            tfim = t;
        }

//std::string t1 = tini->toString();
//std::string t2 = tfim->toString();

        setInitialTime(tini);
        setFinalTime(tfim);
      }
      return true;
    }
  }

  if(m_mObs.empty())
    return false;

  for(it = m_layers.begin(); it != m_layers.end(); ++it)
  {
    te::map::AbstractLayer* parent = (te::map::AbstractLayer*)(*it)->getParent();
    if(parent->getId() == "TemporalImages")
    {
      calculateTemporalImageTimes();
      return true;
    }
  }
  return true;
}

void TimeSlider::calculateTemporalImageTimes(bool addTime)
{
// este metodo pega todos os layers existentes debaixo da pasta TemporalImages e calcula o tempo inicial e final.

}

void TimeSlider::addLayer(te::map::AbstractLayer* al)
{
  m_layers.push_back(al);
}

void TimeSlider::removeLayer(te::map::AbstractLayer* al)
{
  std::vector<te::map::AbstractLayer*>::iterator it;
  for(it =  m_layers.begin(); it != m_layers.end(); ++it)
  {
    if((*it) == al)
      m_layers.erase(it);
  }
}

void TimeSlider::removeAllLayers()
{
   m_layers.clear();
}

void TimeSlider::timerEvent(QTimerEvent* e)
{
  if(m_initialTime->getDateTimeType() != te::dt::TIME_INSTANT)
    return;

  if(m_play == false)
    return;

  m_value = this->value();
  if(m_value == 0)
  {
    ((MyDisplay*)m_mapDisplay)->clearTemporalPixmaps(m_layers);
    clearLastPointMap();
  }

  te::dt::TimeInstant* ti = (te::dt::TimeInstant*)m_initialTime;
  boost::gregorian::date initialDate(ti->getDate().getDate());
  boost::posix_time::ptime iTime(initialDate, ti->getTime().getTimeDuration());
  boost::posix_time::time_duration timedur(m_value/60, 0, 0);
  boost::posix_time::ptime time1 = iTime + timedur;
  te::dt::TimeInstant* tInitial = new te::dt::TimeInstant(time1);

  boost::posix_time::time_duration timedurinterval(0, m_minuteInterval, 0);
  boost::posix_time::ptime time2 = time1 + timedurinterval;
  te::dt::TimeInstant* tFinal = new te::dt::TimeInstant(time2);

  draw(tInitial, tFinal);
  if(m_stop)
    ((MyDisplay*)m_mapDisplay)->clearTimeLineEdit();

  int fvalue = m_value + m_minuteInterval;
  if(fvalue >= maximum())
  {
    backToInit();
    if(m_loop == false)
    {
      killTimer();
      stop();
      return;
    }
  }
  else
    setValue(fvalue);
}

void TimeSlider::valueChangedSlot(int v)
{
  if(m_initialTime->getDateTimeType() != te::dt::TIME_INSTANT)
    return;

  m_value = v;
  
  ((MyDisplay*)m_mapDisplay)->clearTemporalPixmaps(m_layers);

  te::dt::TimeInstant* ti = (te::dt::TimeInstant*)m_initialTime;
  boost::gregorian::date initialDate(ti->getDate().getDate());
  boost::posix_time::ptime iTime(initialDate, ti->getTime().getTimeDuration());
  te::dt::TimeInstant* tInitial = new te::dt::TimeInstant(iTime);

  boost::posix_time::time_duration timedur(0, v, 0);
  boost::posix_time::ptime fTime = iTime + timedur;
  te::dt::TimeInstant* tFinal = new te::dt::TimeInstant(fTime);

  clearLastPointMap();
  draw(tInitial, tFinal);
}

void TimeSlider::draw(te::dt::TimeInstant* tini, te::dt::TimeInstant* tfim)
{
  int i;
  bool rasterDrawed = false;

  te::dt::TimeInstant* tInitial = (te::dt::TimeInstant*)tini;
  te::dt::TimeInstant* tFinal = (te::dt::TimeInstant*)tfim;

  std::vector<te::gm::Geometry*> geoms;
  std::vector<te::map::AbstractLayer*>::iterator it;
  for(it = m_layers.begin(); it != m_layers.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)(*it);
    te::map::AbstractLayer* l = (te::map::AbstractLayer*)layer->getParent();
    std::string pid = l->getId();
    std::string id = layer->getId();
    if(pid == "MovingObjects")
    {
      if(m_mObs.empty())
        continue;

      for(i = 0; i < (int)m_mObs.size(); i++)
      {
        if(m_mObs[i]->getId() == id)
          break;
      }
      if(i == (int)m_mObs.size())
        return;

      te::st::SpatioTemporalObservationSet* obs = m_mObs[i]->getObservationSet();
      std::vector<te::st::Observation*> result;
      obs->during(tInitial, tFinal, result);
      if(result.empty())
        continue;

      int n = 0;
      te::gm::PointZ* p = 0;
      te::gm::LineString* line;
      if(m_lines)
      {
        if(m_lastPointMap.find(id) != m_lastPointMap.end())
        {
          line = new te::gm::LineString(result.size()+1, te::gm::LineStringType, 4291);
          line->setPoint(n++, m_lastPointMap[id]->getX(), m_lastPointMap[id]->getY());
        }
        else
          line = new te::gm::LineString(result.size(), te::gm::LineStringType, 4291);
      }

      std::vector<te::st::Observation*>::iterator ii;
      for(ii = result.begin(); ii != result.end(); ++ii)
      {
        te::gm::Geometry* point = dynamic_cast<te::gm::Geometry*>((*ii)->getObservedValue(0));
        point->setSRID(4291);
        if(m_lines == false)
          geoms.push_back(point);
        else
        {
          p = (te::gm::PointZ*)point;
          line->setPoint(n++, p->getX(), p->getY());
        }
      }
      if(n > 1 && m_lines)
      {
        geoms.push_back(line);
        if(p)
        {
          te::gm::Point* pp = new te::gm::Point(p->getX(), p->getY(), p->getSRID());
          if(m_lastPointMap.find(id) != m_lastPointMap.end())
            delete m_lastPointMap[id];
          m_lastPointMap[id] = pp;
        }
      }

      ((MyDisplay*)m_mapDisplay)->drawTemporalData(layer, geoms, m_lines);
      if(m_lines)
        te::common::FreeContents(geoms);
      geoms.clear();
    }
    else // raster
    {
      if(rasterDrawed)
        continue;
      MyLayer* layer = (MyLayer*)findImage(tini, tfim);
      if(layer)
      {
        rasterDrawed = true;
        ((MyDisplay*)m_mapDisplay)->drawTemporalData(layer, geoms, false);
      }
    }

    if(m_currentTime)
      delete m_currentTime;
    m_currentTime = new te::dt::TimeInstant(*tfim);
  }
}

te::map::AbstractLayer* TimeSlider::findImage(te::dt::TimeInstant* initial, te::dt::TimeInstant* final)
{
  //std::vector<te::map::AbstractLayer*>::iterator it;

  //te::st::Coverage* coverageOutput = 0;  
  //std::string XMLFileName = TE_DATA_EXAMPLE_LOCALE"/data/geotif/coverage_angra_metadata.xml";
  //te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
  //te::stloader::DataLoader::loadCoverage(ds, XMLFileName, &coverageOutput);
  //te::st::RasterCoverage* rc = (te::st::RasterCoverage*)coverageOutput;
  //te::rst::Raster* raster = rc->getRaster(initial);
  //std::string rname = raster->getName(); // espero que seja o nome da imagem....

  //for(it = m_layers.begin(); it != m_layers.end(); ++it)
  //{
  //  te::map::AbstractLayer* l = (te::map::AbstractLayer*)(*it)->getParent();
  //  if(l->getTitle() == "TemporalImages")
  //  {
  //    std::string name = (*it)->getId();
  //    if(name == rname)
  //      return *it;
  //  }
  //}
  //return 0;

  //teste
  int nt = 0;
  std::vector<te::map::AbstractLayer*>::iterator it;
  for(it = m_layers.begin(); it != m_layers.end(); ++it)
  {
    te::map::AbstractLayer* l = (te::map::AbstractLayer*)(*it)->getParent();
    if(l->getTitle() == "TemporalImages")
      nt++;
  }
  static int ist = 0;
  ist = ist % nt;
  return m_layers[ist++];
}

void TimeSlider::setInitialTime(te::dt::TimeInstant* dt)
{
  if(m_initialTime)
    delete m_initialTime;
  m_initialTime = dt;
  setMinimum(0);
}

void TimeSlider::setFinalTime(te::dt::TimeInstant* dt)
{
  if(m_finalTime)
    delete m_finalTime;
  m_finalTime = dt;
  if(m_finalTime->getDateTimeType() == te::dt::TIME_INSTANT)
  {
    te::dt::TimeInstant* ti = (te::dt::TimeInstant*)m_initialTime;
    boost::gregorian::date initialDate(ti->getDate().getDate());
    boost::posix_time::ptime iTime(initialDate, ti->getTime().getTimeDuration());

    te::dt::TimeInstant* tf = (te::dt::TimeInstant*)m_finalTime;
    boost::gregorian::date finalDate(tf->getDate().getDate());
    boost::posix_time::ptime fTime(finalDate, tf->getTime().getTimeDuration());

    boost::posix_time::time_duration timedur = fTime - iTime;
    int mins = timedur.hours() * 60 + timedur.minutes();
    setMaximum(mins);
  }
}

void TimeSlider::setMinuteInterval(int interval)
{
  m_minuteInterval = interval;
}

int TimeSlider::getTimerId()
{
  return m_timerId;
}

void TimeSlider::backToInit()
{
  setValue(0);
  clearLastPointMap();
}

void TimeSlider::setLines(bool b)
{
  m_lines = b;
}

void TimeSlider::setLoop(bool b)
{
  m_loop = b;
}

void TimeSlider::clearLastPointMap()
{
  if(m_lines == false)
   return;

  std::map<std::string, te::gm::Point*>::iterator it;
  for(it = m_lastPointMap.begin(); it != m_lastPointMap.end(); ++it)
    delete it->second;
  m_lastPointMap.clear();
}

std::vector<te::map::AbstractLayer*>  TimeSlider::getLayers()
{
  return m_layers;
}

void TimeSlider::startTimer(int interval)
{
  m_timerId = QSlider::startTimer(interval);
}

void TimeSlider::killTimer()
{
  if(m_timerId)
    QSlider::killTimer(m_timerId);
  m_timerId = 0;
}

void TimeSlider::clearDrawing()
{
  killTimer();
  backToInit();

  std::vector<te::map::AbstractLayer*>::iterator it;
  for(it = m_layers.begin(); it != m_layers.end(); ++it)
     ((MyDisplay*)m_mapDisplay)->clearTemporalCanvas(*it);

  ((MyDisplay*)m_mapDisplay)->clearTemporalPixmaps(m_layers);
  ((MyDisplay*)m_mapDisplay)->update();
}

void TimeSlider::configDrawing()
{
  if(m_initialTime == 0)
  {
    if(loadMovingObjects() == false)
      return;
  }
  if(m_temporalDrawingConfig == 0)
  {
    m_temporalDrawingConfig = new TemporalDrawingConfig(this);
    m_temporalDrawingConfig->setDefaultTimes(m_initialTime, m_finalTime);
  }

  if(m_temporalDrawingConfig->exec() == QDialog::Rejected)
    return;
  m_minuteInterval = 15;
  m_dateInterval = 0;

  int index = m_temporalDrawingConfig->m_intervalDateComboBox->currentIndex();
  if(index == 0)
    m_minuteInterval = 15;
  else if(index == 1)
    m_minuteInterval = 30;
  else if(index == 2)
    m_minuteInterval = 60;
  else
    m_minuteInterval = (index - 2) * 24 * 60;

  index = m_temporalDrawingConfig->m_intervalDrawingComboBox->currentIndex();
  m_drawingInterval = (index + 1) * 100;
  m_lines = m_temporalDrawingConfig->m_drawLinesCheckBox->isChecked();
  m_loop = m_temporalDrawingConfig->m_loopCheckBox->isChecked();

  QString t = m_temporalDrawingConfig->m_initialTimeLineEdit->text();
  setInitialTime(t);
  t = m_temporalDrawingConfig->m_finalTimeLineEdit->text();
  setFinalTime(t);

  killTimer();
  play();
}

void TimeSlider::setInitialTime(QString timeString)
{
  QString time = simpleTimeString2IsoString(timeString);
  if(time.isEmpty())
    return;

  te::dt::TimeInstant* t = new te::dt::TimeInstant(time.toStdString());
  setInitialTime(t);
}

void TimeSlider::setFinalTime(QString timeString)
{
  QString time = simpleTimeString2IsoString(timeString);
  if(time.isEmpty())
    return;

  te::dt::TimeInstant* t = new te::dt::TimeInstant(time.toStdString());
  setFinalTime(t);
}

QString TimeSlider::simpleTimeString2IsoString(QString timeString)
{
  QString timeIso;

  QString tt = timeString.toUpper();
  tt.remove(" ");
  std::string time = tt.toStdString();
  if(time.size() != 19)
    return timeIso;
  std::string tIso = time.substr(0, 4);

  std::string s = time.substr(5, 3);
  std::string mes;
  if(s == "JAN")
    mes = "01";
  else if(s == "FEB")
    mes = "02";
  else if(s == "MAR")
    mes = "03";
  else if(s == "APR")
    mes = "04";
  else if(s == "MAY")
    mes = "05";
  else if(s == "JUN")
    mes = "06";
  else if(s == "JUL")
    mes = "07";
  else if(s == "AUG")
    mes = "08";
  else if(s == "SEP")
    mes = "09";
  else if(s == "OCT")
    mes = "10";
  else if(s == "NOV")
    mes = "11";
  else if(s == "DEC")
    mes = "12";
  if(mes.empty())
    return timeIso;

  tIso += mes;

  tIso += time.substr(9, 2);
  tIso += "T";
  tIso += time.substr(11, 2);
  tIso += time.substr(14, 2);
  tIso += time.substr(17, 2);

  timeIso = tIso.c_str();
  return timeIso;
}

void TimeSlider::playPauseSlot()
{
  if(m_play)
    pause();
  else
    play();
}

void TimeSlider::stopSlot()
{
  stop();
  stop(); // chamar 2 vezes para apagar o time line edit;
}

void TimeSlider::play()
{
  ((MyDisplay*)m_mapDisplay)->setTimeSliderIcon(m_pausePixmap);
  m_play = true;
  int value = m_value;

  if(m_stop)
  {
    setEnabled(true);
    m_stop = false;
    if(m_temporalDrawingConfig == 0)
    {
      loadMovingObjects();
      configDrawing();
    }
  }
  killTimer();
  startTimer(m_drawingInterval);
  setValue(value);
}

void TimeSlider::pause()
{
  ((MyDisplay*)m_mapDisplay)->setTimeSliderIcon(m_playPixmap);
  m_play = false;
}

void TimeSlider::stop()
{
  m_stop = true;
  pause();
  ((MyDisplay*)m_mapDisplay)->clearTimeLineEdit();
  clearDrawing();
  setEnabled(false);
}

te::dt::TimeInstant* TimeSlider::getCurrentTime()
{
  return m_currentTime;
}

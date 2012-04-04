#include "TimeSlider.h"
#include "QTimerEvent"
#include "QPixmap"
#include "QPainter"
#include "MyLayer.h"
#include "MyDisplay.h"

//TerraLib
#include <terralib/qt/widgets.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
//#include <terralib/st/movingobject/MovingObject.h>
#include <terralib/st/observation/Observation.h>
#include <terralib/st/observation/SpatioTemporalObservationSet.h>
#include <terralib/st/coverage/RasterCoverage.h>
#include <terralib/stloader/DataLoader.h>

#include "STExamples.h"

TimeSlider::TimeSlider(QWidget* parent) : 
  QSlider(parent),
  m_lines(false),
  m_loop(false),
  m_timerId(0),
  m_initialDateTime(0),
  m_finalDateTime(0)
{
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueChangedSlot(int)));
}

TimeSlider::TimeSlider(Qt::Orientation orientation, QWidget* parent) : 
  QSlider(orientation, parent),
  m_timerId(0)
{
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueChangedSlot(int)));
}

TimeSlider::~TimeSlider()
{
}

void TimeSlider::loadMovingObjects(bool addTime)
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

        te::dt::DateTime* t = 0;
        te::dt::DateTime* tini = 0;
        te::dt::DateTime* tfim = 0;
        if(addTime)
        {
          te::dt::DateTime* tini = m_initialDateTime;
          te::dt::DateTime* tfim = m_finalDateTime;
        }

        for(int j = 0; j < (int)m_mObs.size(); j++)
        {
          te::dt::DateTimePeriod* period = m_mObs[j]->temporalExtent();
          t = period->getInitialInstant();
          if(tini == 0 || t < tini)
            tini = t;
          t = period->getFinalInstant();
          if(tfim == 0 || t > tfim)
            tfim = t;
        }

    //std::string t1 = tini->toString();
    //std::string t2 = tfim->toString();

        setInitialDateTime(tini);
        setFinalDateTime(tfim);
      }
      return;
    }
  }

  for(it = m_layers.begin(); it != m_layers.end(); ++it)
  {
    te::map::AbstractLayer* parent = (te::map::AbstractLayer*)(*it)->getParent();
    if(parent->getId() == "TemporalImages")
    {
      calculateTemporalImageTimes();
      return;
    }
  }
}

void TimeSlider::calculateTemporalImageTimes(bool addTime)
{
// este metodo pega todos os layers existentes debaixo da pasta TemporalImages e calcula o tempo inicial e final.

}

void TimeSlider::setMapDisplay(te::map::MapDisplay* display)
{
  m_mapDisplay = display;
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
  if(m_initialDateTime->getDateTimeType() != te::dt::TIME_INSTANT)
    return;

  int ivalue = this->value();
  if(ivalue == 0)
  {
    ((MyDisplay*)m_mapDisplay)->clearTemporalPixmaps(m_layers);
    clearLastPointMap();
  }

  te::dt::TimeInstant* ti = (te::dt::TimeInstant*)m_initialDateTime;
  boost::gregorian::date initialDate(ti->getDate().getDate());
  boost::posix_time::ptime iTime(initialDate, ti->getTime().getTimeDuration());
  boost::posix_time::time_duration timedur(ivalue/60, 0, 0);
  boost::posix_time::ptime time1 = iTime + timedur;
  te::dt::TimeInstant* tInitial = new te::dt::TimeInstant(time1);

  boost::posix_time::time_duration timedurinterval(0, m_minuteInterval, 0);
  boost::posix_time::ptime time2 = time1 + timedurinterval;
  te::dt::TimeInstant* tFinal = new te::dt::TimeInstant(time2);

  m_timerId = e->timerId();
  draw(tInitial, tFinal);

  int fvalue = ivalue + m_minuteInterval;
  if(fvalue >= maximum())
  {
    backToInit();
    if(m_loop == false)
    {
      killTimer();
      return;
    }
  }
  else
    setValue(fvalue);
}

void TimeSlider::valueChangedSlot(int v)
{
  if(m_timerId)
    return;
  if(m_initialDateTime->getDateTimeType() != te::dt::TIME_INSTANT)
    return;
  
  ((MyDisplay*)m_mapDisplay)->clearTemporalPixmaps(m_layers);

  te::dt::TimeInstant* ti = (te::dt::TimeInstant*)m_initialDateTime;
  boost::gregorian::date initialDate(ti->getDate().getDate());
  boost::posix_time::ptime iTime(initialDate, ti->getTime().getTimeDuration());
  te::dt::TimeInstant* tInitial = new te::dt::TimeInstant(iTime);

  boost::posix_time::time_duration timedur(0, v, 0);
  boost::posix_time::ptime fTime = iTime + timedur;
  te::dt::TimeInstant* tFinal = new te::dt::TimeInstant(fTime);

  clearLastPointMap();
  draw(tInitial, tFinal);
}

void TimeSlider::draw(te::dt::DateTime* tini, te::dt::DateTime* tfim)
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
      if(m_lines)
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
  }
}

te::map::AbstractLayer* TimeSlider::findImage(te::dt::DateTime* initial, te::dt::DateTime* final)
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

void TimeSlider::setInitialDateTime(te::dt::DateTime* dt)
{
  m_initialDateTime = dt;
  setMinimum(0);
}

void TimeSlider::setFinalDateTime(te::dt::DateTime* dt)
{
  m_finalDateTime = dt;
  if(m_finalDateTime->getDateTimeType() == te::dt::TIME_INSTANT)
  {
    te::dt::TimeInstant* ti = (te::dt::TimeInstant*)m_initialDateTime;
    boost::gregorian::date initialDate(ti->getDate().getDate());
    boost::posix_time::ptime iTime(initialDate, ti->getTime().getTimeDuration());

    te::dt::TimeInstant* tf = (te::dt::TimeInstant*)m_finalDateTime;
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

void TimeSlider::killTimer()
{
  if(m_timerId)
    QSlider::killTimer(m_timerId);
  m_timerId = 0;
}

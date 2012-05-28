// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>
#include <terralib/qt/qwt/TimeSeriesDisplay.h>

void PlotMovingObjectDistance(te::st::MovingObject* mo1, te::st::MovingObject* mo2)
{
  if(mo1 == 0)
  {
    std::cout << "The first moving object is NULL!" << std::endl;
    return;
  }

  if(mo2 == 0)
  {
    std::cout << "The second moving object is NULL!" << std::endl;
    return;
  }

  //First: associate a interpolation function to the second moving object
  te::st::AbstractInterpolator* interpolator = &(te::st::TemporalNearestNeigInterpolator::getInstance());
  
  mo2->setInterpolator(interpolator);

  //Second: calculate the distance
  te::st::TimeSeries* distance = mo1->distance(mo2);
   
  te::st::TimeSeriesIterator it = distance->begin();  

  std::vector<std::pair<te::dt::DateTime*, double> > values;
  te::dt::DateTime* date;
  while(it != distance->end())
  {
    date = it.getDateTime();
    te::dt::AbstractData* value = it.getObservedValue();
    double dv = atof(value->toString().c_str());
    std::pair<te::dt::DateTime*, double>  val = std::pair<te::dt::DateTime*, double>(date, dv);
    values.push_back(val);
    ++it;
  }

  te::qt::qwt::TimeSeries ts(date->getDateTimeType());
  std::string nameCurve = mo1->getId() + " - " + mo2->getId();
  ts.insertCurve(values, nameCurve, QColor(255, 0, 0));
  delete distance;

  std::vector<std::pair<std::string, int> > v;
  v.push_back(std::pair<std::string, int>(mo1->getId(), 0));
  v.push_back(std::pair<std::string, int>(mo2->getId(), 1));
  te::qt::qwt::TimeSeriesDisplay* tsd = new te::qt::qwt::TimeSeriesDisplay(ts, v);
  tsd->show();
  tsd->setWindowTitle("Time Series");
  tsd->setTitle("Distance Between Moving Objects");
}


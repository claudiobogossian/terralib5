#include "TimeSeries.h"

te::qt::qwt::TimeSeries::TimeSeries(int timeType):
  m_timeType(timeType)
{
  m_xMin = m_yMin = std::numeric_limits<double>::max();
  m_xMax = m_yMax = -(std::numeric_limits<double>::max());
}

te::qt::qwt::TimeSeries::~TimeSeries()
{
}

void te::qt::qwt::TimeSeries::insertCurve(const std::vector<std::pair<te::dt::DateTime*, double> >& values, const std::string& title, QColor cor, bool order)
{
  te::dt::TimeInstant* ti;
  te::dt::Date* d;
  boost::gregorian::date basedate(1400, 01, 01);
  boost::gregorian::date_duration days;
  long long int seconds, dias;
  double vx, vy;
  std::vector<std::pair<double, double> > tvalues;
  m_colorMap[title] = cor;
  m_curveMap[title] = tvalues;

  std::vector<std::pair<te::dt::DateTime*, double> >::const_iterator it;
  if(m_timeType == te::dt::TIME_INSTANT)
  {
    for(it = values.begin(); it != values.end(); ++it)
    {
      ti = (te::dt::TimeInstant*)it->first;
      days = ti->getDate().getDate() - basedate;

      seconds = ti->getTime().getTimeDuration().total_seconds();
      dias = days.days();
      vx = dias * 86400 + seconds;
      if(vx < m_xMin)
        m_xMin = vx;
      if(vx > m_xMax)
        m_xMax = vx;

      vy = it->second;
      if(vy < m_yMin)
        m_yMin = vy;
      if(vy > m_yMax)
        m_yMax = vy;

      m_curveMap[title].push_back(std::pair<double, double> (vx, vy));
    }
  }
  else if(m_timeType == te::dt::DATE)
  {
    for(it = values.begin(); it != values.end(); ++it)
    {
      d = (te::dt::Date*)it->first;
      days = d->getDate() - basedate;
      vx = days.days();
      if(vx < m_xMin)
        m_xMin = vx;
      if(vx > m_xMax)
        m_xMax = vx;

      vy = it->second;
      if(vy < m_yMin)
        m_yMin = vy;
      if(vy > m_yMax)
        m_yMax = vy;
      m_curveMap[title].push_back(std::pair<double, double> (vx, vy));
    }
  }

  if(order)
  {
     //ordernar em funcao do eixo x
    int size = m_curveMap[title].size();
    int i, j;
    double a;
    for(i = 0; i < (size-1); i++)
    {
      for(j = 0; j < (size-(i+1)); j++)
      {
        if(m_curveMap[title][j].first > m_curveMap[title][j+1].first)
        {
          a = m_curveMap[title][j].first;
          m_curveMap[title][j].first = m_curveMap[title][j+1].first;
          m_curveMap[title][j+1].first = a;
  
          a = m_curveMap[title][j].second;
          m_curveMap[title][j].second = m_curveMap[title][j+1].second;
          m_curveMap[title][j+1].second = a;
        }
      }
    } 
  }
}

int te::qt::qwt::TimeSeries::getTimeType()
{
  return m_timeType;
}


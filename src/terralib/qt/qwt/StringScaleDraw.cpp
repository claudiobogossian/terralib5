#include "StringScaleDraw.h"

//TERRALIB
#include "../../datatype.h"

te::qt::qwt::StringScaleDraw::StringScaleDraw() :
  QwtScaleDraw(),
  m_timeType(-1)
{
  //espacamento do tick para o label.
  setSpacing( 5 );
}

te::qt::qwt::StringScaleDraw::StringScaleDraw(const std::set<QString>& v) :
  QwtScaleDraw(),
  m_values(v)
{
  //espacamento do tick para o label.
  setSpacing( 5 );
}

te::qt::qwt::StringScaleDraw::StringScaleDraw(int timeType) :
  QwtScaleDraw(), 
  m_timeType(timeType)
{
  //espacamento do tick para o label.
  setSpacing( 5 );
}

te::qt::qwt::StringScaleDraw::~StringScaleDraw()
{
  m_values.clear();
}

QwtText te::qt::qwt::StringScaleDraw::label( double v ) const
{
  QString s = " ";

  if(m_values.empty() == false)
  { 
    size_t size = m_values.size();
    size_t iv = v;
    if(v - iv < 0.0000001)
    {
      if(iv < size)
      {
        std::set<QString>::const_iterator it = m_values.begin();
        while(iv > 0)
        {
          --iv;
          ++it;
        }
        s = *it;
      }
    }
  }
  else if(m_timeType == te::dt::TIME_INSTANT)
  {
    long long int ndays = v / 86400.;
  
    boost::gregorian::date_duration dd(ndays);
    boost::gregorian::date datebase(1400, 01, 01);
    boost::gregorian::date datef = datebase + dd; 
    int secs = v - ndays * 86400;
    int hours = secs / 3600;
    secs -= hours * 3600;
    int mins = secs / 60;
    secs -= mins * 60;
    boost::posix_time::time_duration td(hours, mins, secs);
    boost::posix_time::ptime pt(datef, td);
    s = boost::posix_time::to_simple_string(pt).c_str();
  }
  else if(m_timeType == te::dt::DATE)
  {
    int ndays = v;
  
    boost::gregorian::date_duration dd(ndays);
    boost::gregorian::date datebase(1400, 01, 01);
    boost::gregorian::date datef = datebase + dd; 
    s = boost::gregorian::to_simple_string(datef).c_str();
  }

  return QwtText(s);
}

void te::qt::qwt::StringScaleDraw::setTimeType(int t)
{
  m_timeType = t;
}

void te::qt::qwt::StringScaleDraw::setStringSet(const std::set<QString>& s)
{
  m_values = s;
}

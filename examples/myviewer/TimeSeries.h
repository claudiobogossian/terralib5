#ifndef __TERRALIB_QT_QWT_INTERNAL_TIME_SERIES_H_
#define __TERRALIB_QT_QWT_INTERNAL_TIME_SERIES_H_

//STL
#include<vector>
#include<map>

//TerraLib
#include "../widgets/Config.h"
#include "../../datatype.h"

//QT
#include<QColor>

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT TimeSeries
      {
        public:

          TimeSeries(int timeType);
          ~TimeSeries();

          void insertCurve(const std::vector<std::pair<te::dt::DateTime*, double> >& values, const std::string& title, QColor cor, bool order = false);
          int getTimeType();

        public:
          std::map<std::string, std::vector<std::pair<double, double> > > m_curveMap;
          std::map<std::string, QColor> m_colorMap;
          int m_timeType;
          double m_xMin;
          double m_xMax;
          double m_yMin;
          double m_yMax;
      };
    }
  }
}
#endif

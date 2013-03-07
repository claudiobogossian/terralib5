#ifndef __TERRALIB_QT_QWT_INTERNAL_HISTOGRAM_H_
#define __TERRALIB_QT_QWT_INTERNAL_HISTOGRAM_H_

//QWT
#include <qwt_plot_histogram.h>

// Boost
#include <boost/integer.hpp>

//STL
#include <map>

//TerraLib
#include "../widgets/Config.h"
#include "../../datatype.h"

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT Histogram: public QwtPlotHistogram
      {
      public:
        Histogram(const QColor &, const QString& title=QString::null);

        void setColor(const QColor &);
        void setValues(const std::map<double, int> values, int type = te::dt::STRING_TYPE, int timeType = te::dt::TIME_INSTANT, double min = 0., double max = 0.);
      };
    }
  }
}
#endif

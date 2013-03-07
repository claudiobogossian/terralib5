#ifndef __TERRALIB_QT_QWT_INTERNAL_SCATTER_H_
#define __TERRALIB_QT_QWT_INTERNAL_SCATTER_H_

//QWT
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_scale_map.h>

//STL
#include <vector>

//TerraLib
#include "../widgets/Config.h"

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT Scatter: public QwtPlotCurve
      {
      public:
        Scatter(const QString& title=QString::null);

        void setColor(const QColor&);
        QColor getColor();
        void setValues(const std::vector<std::pair<double, double> >&);

      private:
        QwtSymbol* m_symbol;
        QwtScaleMap* m_xmap;
        QwtScaleMap* m_ymap;
        double* m_xval;
        double* m_yval;
      };
    }
  }
}
#endif

#ifndef __TERRALIB_QT_QWT_INTERNAL_CURVE_H_
#define __TERRALIB_QT_QWT_INTERNAL_CURVE_H_

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
      class TEQTWIDGETSEXPORT Curve: public QwtPlotCurve
      {
      public:
        Curve(const QString& title=QString::null);

        void setColor(const QColor&);
        QColor getColor();
        void setLineColor(const QColor&);
        QColor getLineColor();
        void setValues(const std::vector<std::pair<double, double> >&, double xmin, double ymin, double xmax, double ymax);

      private:
        QwtSymbol* m_symbol;
        QwtScaleMap* m_xmap;
        QwtScaleMap* m_ymap;
        QColor m_lineColor;
        double* m_xval;
        double* m_yval;
      };
    }
  }
}
#endif

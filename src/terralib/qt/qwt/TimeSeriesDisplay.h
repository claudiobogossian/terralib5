
#ifndef __TERRALIB_QT_QWT_INTERNAL_CURVE_DISPLAY_H_
#define __TERRALIB_QT_QWT_INTERNAL_CURVE_DISPLAY_H_

#include "Plot.h"
#include "Curve.h"
#include "TimeSeries.h"

//QWT
#include <qwt_legend.h>

//QT
#include <QEvent>
#include <QMenu>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCursor>

//TerraLib
#include "../../dataaccess.h"
#include "../../maptools.h"
#include "../widgets/Config.h"

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT TimeSeriesDisplay : public Plot
      {
      Q_OBJECT

      public:

        TimeSeriesDisplay(const TimeSeries&, std::vector<std::pair<std::string, int> >, int xCol = 0, QWidget* = NULL);
        ~TimeSeriesDisplay();

        void createCurves(const TimeSeries& ts);
        void updateVisibity();
        void drawItems(QPainter*, const QRectF&, const QwtScaleMap map[axisCnt] ) const;
        void setColumns(std::vector<int>);
        std::set<int> getColumns();

      protected Q_SLOTS:
        void showTimeSeriesSlot(QwtPlotItem* , bool);
        void legendMenuSlot(QPoint&, QWidget*);
        void legendToFrontSlot();
        void legendToBackSlot();
        void legendColorSlot();

      private:
        std::vector<Curve*> m_curveVec; // para controlar a ordem de desenho dos curvess
        std::map<Curve*, bool> m_curveMap; // para controlar a visibilidade dos curvess
        std::set<int> m_columns;
        QRadioButton* m_radioButton;
        QWidget* m_legendWidget; // para informar qual curve vai para toFront ou toBack
        QMenu* m_legendMenu; // para colocar menu: toFront e toBack
      };
    }
  }
}
#endif

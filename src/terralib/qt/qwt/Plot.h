#ifndef __TERRALIB_QT_QWT_INTERNAL_PLOT_H_
#define __TERRALIB_QT_QWT_INTERNAL_PLOT_H_

#include "Legend.h"

//QWT
#include <qwt_plot.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_zoomer.h>

//QT
#include <QCursor>

// TerraLib
#include "../widgets/Config.h"
#include "../../datatype.h"
#include "../../maptools.h"
#include "StringScaleDraw.h"

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT Plot : public QwtPlot
      {
        Q_OBJECT

      public:

        enum SelectionMode
        {
          SELECT,
          TOGGLE,
          ADD,
          UNSEL,
          ZOOM,
          PAN
        };

        Plot(QString type, te::map::DataGridOperation* = NULL, QWidget * = NULL);
        ~Plot();

        void init();
        void setVerticalTitle(QString title);
        void setHorizontalTitle(QString title);
        void adjustHistogramHorizontalAxis();
        void adjustScatterLabels();
        void setTableChanged(bool);
        te::map::DataGridOperation* getDataGridOperation();

        Legend* getLegend();

      protected Q_SLOTS:
        void zoomedSlot(const QRectF&);
        void pannedSlot(int, int);

      protected:
        double m_XMIN;
        double m_XMAX;
        double m_xMin;
        double m_xMax;
        double m_YMIN;
        double m_YMAX;
        double m_yMin;
        double m_yMax;
        bool m_tableChanged;
        int m_xSize;
        int m_ySize;
        int m_xCol;
        int m_yCol;
        int m_PKPos;
        int m_maxNumberOfHorizontalLabels;
        double m_barInterval;
        int m_numberOfBars;
        int m_xType;
        int m_yType;
        int m_xTimeType;
        int m_yTimeType;
        std::set<QString> m_xStringSet;
        std::set<QString> m_yStringSet;
        te::qt::qwt::StringScaleDraw* m_xStringScaleDraw;
        te::qt::qwt::StringScaleDraw* m_yStringScaleDraw;
        te::map::DataGridOperation* m_op;
        QwtPlotZoomer* m_zoomer;
        QwtPlotPanner* m_panner;
        SelectionMode m_selectionMode;
        Legend* m_legend;
        QString m_type;

        QCursor* m_zoomCursor;
      };
    }
  }
}
#endif

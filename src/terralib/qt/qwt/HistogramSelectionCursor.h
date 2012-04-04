#ifndef __TERRALIB_QT_QWT_INTERNAL_HISTOGRAM_SELECTION_CURSOR_H_
#define __TERRALIB_QT_QWT_INTERNAL_HISTOGRAM_SELECTION_CURSOR_H_

//QWT
#include <qwt_picker.h>

//TerraLib
#include "../widgets/Config.h"

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT HistogramSeletionCursor : public QwtPicker
      {
        Q_OBJECT

      public:
        HistogramSeletionCursor(QWidget *parent);
        HistogramSeletionCursor(RubberBand, DisplayMode, QWidget*);

        virtual void drawRubberBand(QPainter*) const;
        virtual bool end(bool ok);
        virtual bool eventFilter(QObject*, QEvent*);

      Q_SIGNALS:
        void select(int x0, int x1);
        void wheelEvent(int delta);
      };
    }
  }
}
#endif

#ifndef __TERRALIB_QT_QWT_INTERNAL_SCATTER_SELECTION_CURSOR_H_
#define __TERRALIB_QT_QWT_INTERNAL_SCATTER_SELECTION_CURSOR_H_

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
      class TEQTWIDGETSEXPORT ScatterSelectionCursor : public QwtPicker
      {
        Q_OBJECT

        public:
          ScatterSelectionCursor(QWidget *parent);
          ScatterSelectionCursor(RubberBand, DisplayMode, QWidget*);

          virtual void widgetMouseReleaseEvent(QMouseEvent*);
          virtual void drawRubberBand(QPainter*) const;
          virtual bool end(bool ok);

        Q_SIGNALS:
          void select(QRect rec);

        private:
          bool m_press;
      };
    }
  }
}
#endif

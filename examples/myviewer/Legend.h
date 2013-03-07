#ifndef __TERRALIB_QT_QWT_INTERNAL_LEGEND_H_
#define __TERRALIB_QT_QWT_INTERNAL_LEGEND_H_

//QWT
#include <qwt_legend.h>

//TerraLib
#include "../widgets/Config.h"

namespace te
{
  namespace qt
  {
    namespace qwt
    {
      class TEQTWIDGETSEXPORT Legend : public QwtLegend
      {
        Q_OBJECT

      public:
        Legend(QWidget *parent=NULL);
        virtual bool eventFilter(QObject *, QEvent *);

      Q_SIGNALS:
        void legendContextMenu(QPoint& p, QWidget*);

      };
    }
  }
}
#endif

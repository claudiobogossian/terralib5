#ifndef	QLAYOUTVIEW_H
#define QLAYOUTVIEW_H

#include <QGraphicsView>
#include "LayoutView.h"
#include "LayoutAbstractType.h"
#include "../../../../geometry/Envelope.h"

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;

namespace te
{
  namespace layout
  {
    class QLayoutView : public QGraphicsView, public LayoutView
    {
      public:
        QLayoutView(QWidget* widget = (QWidget*)0);
        ~QLayoutView();

        virtual void setLayoutController(LayoutController* controller);
        virtual void config();
        virtual te::gm::Envelope* calculateBoxPaper();
        virtual te::gm::Envelope* calculateWindow();
        
        //PaintDevice
        virtual QPaintEngine* paintEngine() const;

        QString QLayoutView::fileName() const ;
        void QLayoutView::fileName(QString val);

        virtual te::gm::Envelope* getWorldBox();

      protected:
        void mousePressEvent(QMouseEvent * event);
        void wheelEvent(QWheelEvent *event);
        void scaleView(qreal scaleFactor);
        virtual void keyPressEvent(QKeyEvent* keyEvent);
        virtual void resizeEvent(QResizeEvent * event);

        //PaintDevice
        virtual int	metric ( PaintDeviceMetric metric ) const;

      protected:
        LayoutAbstractPaperType _paperType;
        double _paperSizeWMM;
        double _paperSizeHMM;
        te::gm::Envelope* _boxPaperW;
        te::gm::Envelope* _boxW;
        QTransform _matrix;
        QTransform _matrixinv;
        QTransform _matrixItem;
        double	   _diagonalScreenInchSize;
        QString	   _fileName;
    };
  }
}
#endif
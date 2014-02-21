#ifndef	QLAYOUTVIEW_H
#define QLAYOUTVIEW_H

#include <QGraphicsView>
#include "LayoutView.h"
#include "../../../../geometry/Envelope.h"
#include <QTabwidget>
#include <QDockWidget>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;
class QHideEvent;
class QCloseEvent;

namespace te
{
  namespace layout
  {
    class QPropertiesWindowOutside; 
    class QObjectInspectorWindowOutside;
    class QToolbarWindowOutside;

    class QLayoutView : public QGraphicsView, public LayoutView
    {
      Q_OBJECT //for slots/signals

      public:
        QLayoutView(QWidget* widget = (QWidget*)0);
        ~QLayoutView();

        virtual void setLayoutController(LayoutController* controller);
        virtual void config();        
        
        virtual void setDockPropertiesParent(QWidget* dockParent);
        
        virtual void changeMode();

      protected:
        void mousePressEvent(QMouseEvent * event);
        void wheelEvent(QWheelEvent *event);
        void scaleView(qreal scaleFactor);
        virtual void keyPressEvent(QKeyEvent* keyEvent);
        virtual void resizeEvent(QResizeEvent * event);
        void hideEvent ( QHideEvent * event );
        void closeEvent ( QCloseEvent * event );

        virtual void	paintEvent ( QPaintEvent * event );

        //PaintDevice
        virtual int	metric ( PaintDeviceMetric metric ) const;

      protected:
        double	   _diagonalScreenInchSize;
        QPropertiesWindowOutside* _dockProperties;
        QObjectInspectorWindowOutside* _dockInspector;
        QToolbarWindowOutside* _dockToolbar;
        QWidget*      _dockParent;
    };
  }
}
#endif
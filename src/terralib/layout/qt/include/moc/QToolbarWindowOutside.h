#ifndef QTOOLBARWINDOWOUTSIDE_H
#define QTOOLBARWINDOWOUTSIDE_H

#include <QDockWidget>
#include "LayoutOutsideObserver.h"
#include "../../../../geometry/Envelope.h"

class QGraphicsItem;
class QWidget;
class QToolBar;

namespace te
{
  namespace layout
  {
    class LayoutView;

    class QToolbarWindowOutside : public QDockWidget, public LayoutOutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    QToolbarWindowOutside(LayoutOutsideController* controller, LayoutOutsideModelObservable* o);
	    virtual ~QToolbarWindowOutside();

	    virtual void updateObserver(ContextLayoutItem context);
	    virtual void setPosition(const double& x, const double& y);
	    virtual te::gm::Coord2D getPosition();

      virtual void init(LayoutView* view);

    public slots:

      virtual void onClickPanTool(bool toggled);
      virtual void onClickZoomInTool(bool toggled);
      virtual void onClickZoomOutTool(bool toggled);

    protected:

      virtual void createToolbar();

      QToolBar*  _toolbar;
      LayoutView* _view;

    };
  }
}


#endif

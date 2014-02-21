#ifndef QDISPLAYWINDOWOUTSIDE_H
#define QDISPLAYWINDOWOUTSIDE_H

#include <QDockWidget>
#include "LayoutOutsideObserver.h"
#include "../../../../geometry/Envelope.h"

class QGraphicsItem;
class QCloseEvent;

namespace te
{
  namespace layout
  {
    class QDisplayWindowOutside : public QDockWidget, public LayoutOutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    QDisplayWindowOutside(LayoutOutsideController* controller, LayoutOutsideModelObservable* o);
	    virtual ~QDisplayWindowOutside();

	    virtual void updateObserver(ContextLayoutItem context);
	    virtual void setPosition(const double& x, const double& y);
	    virtual te::gm::Coord2D getPosition();
      void setPreviousCentralWidget(QWidget* previous);

    protected:

      virtual void	closeEvent ( QCloseEvent * event );

      QWidget* _previousCentralWidget; /* Previous central display of application */
      bool     _previousCentralWidgetVisibilite;
    };
  }
}


#endif

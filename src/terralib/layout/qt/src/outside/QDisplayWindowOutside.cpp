#include "QDisplayWindowOutside.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "QLayoutScene.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutItemObserver.h"
#include "LayoutOutsideObserver.h"
#include "LayoutOutsideController.h"

#include <QGraphicsWidget>
#include <QMainWindow>

#include "../../../../geometry/Envelope.h"

te::layout::QDisplayWindowOutside::QDisplayWindowOutside( LayoutOutsideController* controller, LayoutOutsideModelObservable* o ) :
	QDockWidget("", 0, 0),
	LayoutOutsideObserver(controller, o),
  _previousCentralWidget(0)
{  
	setVisible(false);
	setWindowTitle("Layout Display");
}

te::layout::QDisplayWindowOutside::~QDisplayWindowOutside()
{

}

void te::layout::QDisplayWindowOutside::updateObserver( ContextLayoutItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::QDisplayWindowOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::QDisplayWindowOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QDisplayWindowOutside::closeEvent( QCloseEvent * event )
{
  QDockWidget::closeEvent(event);

  widget()->close();

  QMainWindow* mw = (QMainWindow*)parentWidget();

  if(mw)
  {
    mw->removeDockWidget(this);
    if(_previousCentralWidget)
    {
      QDockWidget* dock = dynamic_cast<QDockWidget*>(_previousCentralWidget);
      mw->setCentralWidget(dock);
      dock->setVisible(_previousCentralWidgetVisibilite);
    }
  }
}

void te::layout::QDisplayWindowOutside::setPreviousCentralWidget( QWidget* previous )
{
  _previousCentralWidget = previous;
}

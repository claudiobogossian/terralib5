#include "QToolbarWindowOutside.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "QLayoutScene.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutItemObserver.h"
#include "LayoutOutsideObserver.h"
#include "LayoutOutsideController.h"

#include "../../../../geometry/Envelope.h"

#include <QToolBar>
#include <QToolButton>
#include "LayoutView.h"

te::layout::QToolbarWindowOutside::QToolbarWindowOutside( LayoutOutsideController* controller, LayoutOutsideModelObservable* o ) :
	QDockWidget("", 0, 0),
	LayoutOutsideObserver(controller, o)
{
	te::gm::Envelope box = _model->getBox();	
	setVisible(false);
	setWindowTitle("Layout - Toolbar");
  setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
}

void te::layout::QToolbarWindowOutside::init( LayoutView* view )
{
  _view = view;
  if(!_view)
    return;

  createToolbar();
  setWidget(_toolbar);
}

te::layout::QToolbarWindowOutside::~QToolbarWindowOutside()
{

}

void te::layout::QToolbarWindowOutside::updateObserver( ContextLayoutItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::QToolbarWindowOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::QToolbarWindowOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QToolbarWindowOutside::createToolbar()
{
  _toolbar = new QToolBar;

  QToolButton *button1 = new QToolButton;
  button1->setText("Pan");
  button1->setGeometry(0,0,10,10);
  button1->setCheckable(true);
  connect(button1, SIGNAL(toggled(bool)), this, SLOT(onClickPanTool(bool)));
  QToolButton *button2 = new QToolButton;
  button2->setText("Zoom In");
  button2->setGeometry(0,0,10,10);
  button2->setCheckable(true);
  connect(button2, SIGNAL(toggled(bool)), this, SLOT(onClickZoomInTool(bool)));
  QToolButton *button3 = new QToolButton;
  button3->setText("Zoom Out");
  button3->setGeometry(0,0,10,10);
  button3->setCheckable(true);
  connect(button3, SIGNAL(toggled(bool)), this, SLOT(onClickZoomOutTool(bool)));

  _toolbar->addWidget(button1);
  _toolbar->addSeparator();
  _toolbar->addWidget(button2);
  _toolbar->addSeparator();
  _toolbar->addWidget(button3);
  _toolbar->addSeparator();
}

void te::layout::QToolbarWindowOutside::onClickPanTool(bool toggled)
{
  if(toggled == false)
  {
    LayoutContext::getInstance()->setMode(TypeNone);
  }
  else
  {
     LayoutContext::getInstance()->setMode(TypePan);
  }

  if(_view)
    _view->changeMode();
}

void te::layout::QToolbarWindowOutside::onClickZoomInTool(bool toggled)
{
  if(toggled == false)
  {
    LayoutContext::getInstance()->setMode(TypeNone);
  }
  else
  {
    LayoutContext::getInstance()->setMode(TypeZoomIn);
  }

  if(_view)
    _view->changeMode();
}

void te::layout::QToolbarWindowOutside::onClickZoomOutTool( bool toggled )
{
  if(toggled == false)
  {
    LayoutContext::getInstance()->setMode(TypeNone);
  }
  else
  {
    LayoutContext::getInstance()->setMode(TypeZoomIn);
  }

  if(_view)
    _view->changeMode();
}

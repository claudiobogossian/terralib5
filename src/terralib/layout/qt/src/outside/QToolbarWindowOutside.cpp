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
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QMenuBar>

#include "LayoutView.h"

te::layout::QToolbarWindowOutside::QToolbarWindowOutside( LayoutOutsideController* controller, LayoutOutsideModelObservable* o ) :
	QDockWidget("", 0, 0),
	LayoutOutsideObserver(controller, o)
{
	te::gm::Envelope box = _model->getBox();	
	setVisible(false);
	setWindowTitle("Layout - Toolbar");
  setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
  setMinimumSize(200, 10);
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

  QToolButton *btnPan = new QToolButton;
  btnPan->setText("Pan");
  btnPan->setGeometry(0,0,10,10);
  btnPan->setCheckable(true);
  connect(btnPan, SIGNAL(toggled(bool)), this, SLOT(onClickPanTool(bool)));
  QToolButton *btnZoomIn = new QToolButton;
  btnZoomIn->setText("Zoom In");
  btnZoomIn->setGeometry(0,0,10,10);
  btnZoomIn->setCheckable(true);
  connect(btnZoomIn, SIGNAL(toggled(bool)), this, SLOT(onClickZoomInTool(bool)));
  QToolButton *btnZoomOut = new QToolButton;
  btnZoomOut->setText("Zoom Out");
  btnZoomOut->setGeometry(0,0,10,10);
  btnZoomOut->setCheckable(true);
  connect(btnZoomOut, SIGNAL(toggled(bool)), this, SLOT(onClickZoomOutTool(bool)));

  QComboBox *cmbUnitsMetrics = new QComboBox;
  cmbUnitsMetrics->insertItem(TPMillimeter, QIcon(), QString("Millimeter"));
  cmbUnitsMetrics->insertItem(TPInch, QIcon(), QString("Inch"));
  connect(cmbUnitsMetrics, SIGNAL(currentIndexChanged(int)), this, SLOT(onIndexChanged(int)));

  _toolbar->addWidget(btnPan);
  _toolbar->addSeparator();
  _toolbar->addWidget(btnZoomIn);
  _toolbar->addSeparator();
  _toolbar->addWidget(btnZoomOut);
  _toolbar->addSeparator();
  _toolbar->addWidget(cmbUnitsMetrics);
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

void te::layout::QToolbarWindowOutside::onIndexChanged(int index)
{
  LayoutContext::getInstance()->setMode(TypeUnitsMetricsChange);

  if(TPInch == index)
    LayoutContext::getInstance()->setUnitMetric(TPInch);
  if(TPMillimeter == index)
    LayoutContext::getInstance()->setUnitMetric(TPMillimeter);

  if(_view)
    _view->changeMode();
}


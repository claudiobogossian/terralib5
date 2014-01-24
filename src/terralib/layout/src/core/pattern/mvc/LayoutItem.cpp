#include "LayoutItem.h"
#include "LayoutObservable.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "LayoutItemController.h"
#include "Coordinate.h"
#include "LayoutProperties.h"

te::layout::LayoutItem::LayoutItem( LayoutItemController* controller, LayoutObservable* o ) :
  _controller(controller),
  _model(o),
  _ppi(96.)
{
  _model->addObserver(this);
}

te::layout::LayoutItem::~LayoutItem()
{
  if(_controller)
    delete (LayoutItemController*)_controller;
}

te::layout::LayoutProperties te::layout::LayoutItem::toString()
{
  return _model->toString();
}

void te::layout::LayoutItem::redraw( const double& scaleFactor /*= 1.*/ )
{
  refresh();
  _controller->redraw(scaleFactor);
}

void te::layout::LayoutItem::refresh()
{
  Coordinate coord = getPosition();
  _controller->setPosition(coord.getX(), coord.getY());
}

void te::layout::LayoutItem::setItemPosition( const double& x, const double& y )
{
  setPosition(x, y);
  refresh();
}

void te::layout::LayoutItem::setPPI( const double& ppi )
{
  _ppi = ppi;
}

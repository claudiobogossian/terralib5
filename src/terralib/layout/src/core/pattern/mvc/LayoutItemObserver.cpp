#include "LayoutItemObserver.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "LayoutProperties.h"

#include "LayoutItemController.h"
#include "LayoutItemModelObservable.h"

#include "../../../../../geometry/Coord2D.h"

te::layout::LayoutItemObserver::LayoutItemObserver(LayoutItemController* controller, LayoutItemModelObservable* o) :
  _controller(controller),
  _model(o)
{
  _model->addObserver(this);
}

te::layout::LayoutItemObserver::~LayoutItemObserver()
{
  if(_controller)
    delete (LayoutItemController*)_controller;
}

te::layout::LayoutProperties te::layout::LayoutItemObserver::toString()
{
  return _model->toString();
}

void te::layout::LayoutItemObserver::redraw( const double& scaleFactor /*= 1.*/ )
{
  refresh();
  _controller->redraw(scaleFactor);
}

void te::layout::LayoutItemObserver::refresh()
{
  te::gm::Coord2D coord = getPosition();
  _controller->setPosition(coord.x, coord.y);
}

void te::layout::LayoutItemObserver::setItemPosition( const double& x, const double& y )
{
  setPosition(x, y);
  refresh();
}

void te::layout::LayoutItemObserver::setPPI( const double& ppi )
{
  _ppi = ppi;
}

void te::layout::LayoutItemObserver::setProperties( te::layout::LayoutProperties properties )
{
  if(_model)
    _model->setProperties(properties);
}

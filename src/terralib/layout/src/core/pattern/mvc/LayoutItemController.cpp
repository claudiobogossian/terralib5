#include "LayoutItemController.h"
#include "LayoutItemModelObservable.h"
#include "LayoutItemObserver.h"
#include "ContextLayoutItem.h"
#include "../../../../../maptools/Canvas.h"
#include "LayoutContext.h"

te::layout::LayoutItemController::LayoutItemController( LayoutItemModelObservable* o )
{
  _model = o;
}

te::layout::LayoutItemController::~LayoutItemController()
{
  if(_model)
    delete _model;
}

void te::layout::LayoutItemController::redraw( const double& zoomFactor )
{
  LayoutParams params;
  ContextLayoutItem context(params, zoomFactor);

  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  canvas->clear();
  _model->draw(context);
  canvas->clear();
}

const te::layout::LayoutItemModelObservable* te::layout::LayoutItemController::getModel()
{
  return _model;
}

const te::layout::LayoutItemObserver* te::layout::LayoutItemController::getView()
{
  return _view;
}

void te::layout::LayoutItemController::setProperties( te::layout::LayoutProperties properties )
{
  if(_model)
    _model->setProperties(properties);
}

#include "LayoutItemController.h"
#include "LayoutItem.h"
#include "LayoutObservable.h"
#include "LayoutObserver.h"
#include "LayoutItemModel.h"
#include "ContextLayoutItem.h"
#include "../../../../../maptools/Canvas.h"
#include "LayoutContext.h"

te::layout::LayoutItemController::LayoutItemController( LayoutObservable* o )
{
  _model = o;
}

te::layout::LayoutItemController::~LayoutItemController()
{
  if(_model)
    delete (LayoutItem*)_model;
}

void te::layout::LayoutItemController::redraw( const double& zoomFactor )
{
  LayoutParams params;
  ContextLayoutItem context(params, zoomFactor);

  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  canvas->clear();

  LayoutItemModel* model = dynamic_cast<LayoutItemModel*>(_model);
  model->draw(context);
  canvas->clear();
}

const te::layout::LayoutObservable* te::layout::LayoutItemController::getModel()
{
  return _model;
}

const te::layout::LayoutObserver* te::layout::LayoutItemController::getView()
{
  return _view;
}

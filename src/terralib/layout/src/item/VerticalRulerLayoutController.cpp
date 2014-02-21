#include "VerticalRulerLayoutController.h"
#include "LayoutItemController.h"
#include "ContextLayoutItem.h"
#include "LayoutItemModelObservable.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"

te::layout::VerticalRulerLayoutController::VerticalRulerLayoutController( LayoutItemModelObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, _model);
  _view = (LayoutItemObserver*)factory->make(TPVerticalRuler, params);
}

te::layout::VerticalRulerLayoutController::~VerticalRulerLayoutController()
{

}

void te::layout::VerticalRulerLayoutController::setPosition( const double& x, const double& y )
{
  LayoutItemModelObservable* model = dynamic_cast<LayoutItemModelObservable*>(_model);
  model->setPosition(x, y);
}

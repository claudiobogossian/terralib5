#include "HorizontalRulerLayoutController.h"
#include "LayoutItemController.h"
#include "ContextLayoutItem.h"
#include "LayoutItemModelObservable.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"

te::layout::HorizontalRulerLayoutController::HorizontalRulerLayoutController( LayoutItemModelObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, _model);
  _view = (LayoutItemObserver*)factory->make(TPHorizontalRuler, params);
}

te::layout::HorizontalRulerLayoutController::~HorizontalRulerLayoutController()
{

}

void te::layout::HorizontalRulerLayoutController::setPosition( const double& x, const double& y )
{
  LayoutItemModelObservable* model = dynamic_cast<LayoutItemModelObservable*>(_model);
  model->setPosition(x, y);
}

#include "RectangleLayoutController.h"
#include "LayoutItemController.h"
#include "ContextLayoutItem.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"
#include "LayoutItemModelObservable.h"
#include "LayoutItemParamsCreate.h"
#include "LayoutItemObserver.h"

te::layout::RectangleLayoutController::RectangleLayoutController( LayoutItemModelObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, _model);
  _view = (LayoutItemObserver*)factory->make(TPRetangleItem, params);
}

te::layout::RectangleLayoutController::~RectangleLayoutController()
{
	
}

void te::layout::RectangleLayoutController::setPosition( const double& x, const double& y )
{
  _model->setPosition(x, y);
}

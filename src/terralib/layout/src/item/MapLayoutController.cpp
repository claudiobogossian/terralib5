#include "MapLayoutController.h"
#include "ContextLayoutItem.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"
#include "LayoutItemModelObservable.h"
#include "LayoutItemParamsCreate.h"
#include "LayoutItemObserver.h"

te::layout::MapLayoutController::MapLayoutController( LayoutItemModelObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, _model);
  _view = (LayoutItemObserver*)factory->make(TPMapItem, params);
}

te::layout::MapLayoutController::~MapLayoutController()
{
	
}

void te::layout::MapLayoutController::setPosition( const double& x, const double& y )
{
  _model->setPosition(x, y);
}

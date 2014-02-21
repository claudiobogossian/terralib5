#include "ItemGroupLayoutController.h"
#include "LayoutItemController.h"
#include "ContextLayoutItem.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"
#include "LayoutItemModelObservable.h"
#include "LayoutItemParamsCreate.h"
#include "LayoutItemObserver.h"

te::layout::ItemGroupLayoutController::ItemGroupLayoutController( LayoutItemModelObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, _model);
  _view = (LayoutItemObserver*)factory->make(TPItemGroup, params);
}

te::layout::ItemGroupLayoutController::~ItemGroupLayoutController()
{
	
}

void te::layout::ItemGroupLayoutController::setPosition( const double& x, const double& y )
{
  _model->setPosition(x, y);
}

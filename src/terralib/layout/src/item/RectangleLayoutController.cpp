#include "RectangleLayoutController.h"
#include "LayoutItemController.h"
#include "ContextLayoutItem.h"
#include "LayoutItemModel.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"
#include "LayoutObservable.h"
#include "LayoutItemParamsCreate.h"
#include "LayoutObserver.h"

te::layout::RectangleLayoutController::RectangleLayoutController( LayoutObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, _model);
  _view = (LayoutObserver*)factory->make(TPRetangleItem, params);
}

te::layout::RectangleLayoutController::~RectangleLayoutController()
{
	
}

void te::layout::RectangleLayoutController::setPosition( const double& x, const double& y )
{
  LayoutItemModel* model = dynamic_cast<LayoutItemModel*>(_model);
  model->setPosition(x, y);
}

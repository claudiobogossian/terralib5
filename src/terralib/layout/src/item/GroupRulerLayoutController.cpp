#include "GroupRulerLayoutController.h"
#include "LayoutItemController.h"
#include "ContextLayoutItem.h"
#include "LayoutItemModel.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"

te::layout::GroupRulerLayoutController::GroupRulerLayoutController( LayoutObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, _model);
  _view = (LayoutObserver*)factory->make(TPGroupRuler, params);
}

te::layout::GroupRulerLayoutController::~GroupRulerLayoutController()
{

}

void te::layout::GroupRulerLayoutController::setPosition( const double& x, const double& y )
{
  LayoutItemModel* model = dynamic_cast<LayoutItemModel*>(_model);
  model->setPosition(x, y);
}

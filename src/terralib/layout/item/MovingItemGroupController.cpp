#include "MovingItemGroupController.h"

#include "ItemGroupController.h"

te::layout::MovingItemGroupController::MovingItemGroupController( Observable* o )
  : ItemGroupController(o)
{

}

te::layout::MovingItemGroupController::MovingItemGroupController( Observable* o, EnumType* type )
  : ItemGroupController(o, type)
{

}

te::layout::MovingItemGroupController::~MovingItemGroupController()
{

}

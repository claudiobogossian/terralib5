#include "AbstractItemView.h"


#include "AbstractItemController.h"
#include "AbstractItemModel.h"

te::layout::AbstractItemView::AbstractItemView(AbstractItemController* controller, AbstractItemModel* model)
  : m_controller(controller)
{
}

te::layout::AbstractItemView::~AbstractItemView()
{
  if(m_controller != 0)
  {
    delete m_controller;
  }
}

te::layout::AbstractItemController* te::layout::AbstractItemView::getController() const
{
  return m_controller;
}


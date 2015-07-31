#include "AbstractItemView.h"

#include "AbstractItemModel.h"

te::layout::AbstractItemView::AbstractItemView(AbstractItemController* controller, AbstractItemModel* model)
  : NewObserver()
  , m_controller(controller)
  , m_model(model)
{
  if(m_model != 0)
  {
    m_model->attach(this);
  }
}

te::layout::AbstractItemView::~AbstractItemView()
{
  if(m_model != 0)
  {
    m_model->detach(this);
    delete m_model;
  }
}

te::layout::AbstractItemController* te::layout::AbstractItemView::getController() const
{
  return m_controller;
}

te::layout::AbstractItemModel* te::layout::AbstractItemView::getModel() const
{
  return m_model;
}


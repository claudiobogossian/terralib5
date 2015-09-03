#include "AbstractItemView.h"


#include "AbstractItemController.h"
#include "AbstractItemModel.h"

te::layout::AbstractItemView::AbstractItemView(AbstractItemController* controller, bool invertedMatrix)
  : m_controller(controller)
  , m_invertedMatrix(invertedMatrix)
  , m_isEditionMode(false)
{
  
}

te::layout::AbstractItemView::~AbstractItemView()
{
  if(m_controller != 0)
  {
    delete m_controller;
    m_controller = 0;
  }
}

te::layout::AbstractItemController* te::layout::AbstractItemView::getController() const
{
  return m_controller;
}

void te::layout::AbstractItemView::setController(AbstractItemController* controller)
{
  if (m_controller != 0)
  {
    delete m_controller;
    m_controller = 0;
  }
  m_controller = controller;
}

bool te::layout::AbstractItemView::isInverted()
{
  return m_invertedMatrix;
}

void te::layout::AbstractItemView::setEditionMode(bool editionMode)
{
  if(m_isEditionMode == editionMode)
  {
    return;
  }

  m_isEditionMode = editionMode;
  if (m_isEditionMode)
  {
    enterEditionMode();
  }
  else
  {
    leaveEditionMode();
  }
}

bool te::layout::AbstractItemView::isEditionMode()
{
  return m_isEditionMode;
}

void te::layout::AbstractItemView::enterEditionMode()
{
  //do nothing
}

void te::layout::AbstractItemView::leaveEditionMode()
{
  //do nothing
}





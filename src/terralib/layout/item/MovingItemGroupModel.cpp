#include "MovingItemGroupModel.h"

#include "../core/enum/Enums.h"

te::layout::MovingItemGroupModel::MovingItemGroupModel()
{
  //m_type = Enums::getInstance().getEnumObjectType()->getMovingItemGroup();
  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getMovingItemGroup());

  //m_box = te::gm::Envelope(0., 0., 10., 10.);
}

te::layout::MovingItemGroupModel::~MovingItemGroupModel()
{

}


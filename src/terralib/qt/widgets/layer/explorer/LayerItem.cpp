#include "LayerItem.h"

#include "LegendItem.h"

te::qt::widgets::LayerItem::LayerItem(te::map::AbstractLayerPtr layer) :
  TreeItem("LAYER"),
  m_layer(layer)
{
  addChild(new LegendItem("default style", (te::se::FeatureTypeStyle*)m_layer->getStyle()));
}

te::qt::widgets::LayerItem::~LayerItem()
{
}

std::string te::qt::widgets::LayerItem::getAsString() const
{
  return m_layer->getTitle();
}

te::qt::widgets::VISIBLE te::qt::widgets::LayerItem::isVisible() const
{
  return (m_layer->getVisibility() == te::map::VISIBLE) ? TOTALLY : NONE;
}

void te::qt::widgets::LayerItem::setVisible(const VISIBLE& visible, const bool& updateAncestors, const bool&)
{
  m_layer->setVisibility((visible == TOTALLY) ? te::map::VISIBLE : te::map::NOT_VISIBLE);

  if(updateAncestors)
    m_parent->setVisible(visible, true);
}

te::map::AbstractLayerPtr te::qt::widgets::LayerItem::getLayer() const
{
  return m_layer.get();
}

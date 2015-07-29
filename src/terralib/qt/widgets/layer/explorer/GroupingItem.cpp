#include "GroupingItem.h"

// TerraLib
#include "../../../../color/RGBAColor.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../maptools/Grouping.h"
#include "../../../../maptools/GroupingItem.h"

#include "LegendItem.h"

// Qt
#include <QObject>

void AddSliceItems(te::qt::widgets::GroupingItem* item, const te::map::Grouping* g)
{
  const std::vector<te::map::GroupingItem*> items = g->getGroupingItems();

  for(std::vector<te::map::GroupingItem*>::const_iterator it = items.begin(); it != items.end(); ++it)
    item->addChild(new te::qt::widgets::LegendItem((*it)->getTitle(), (*it)->getSymbolizers()));
}

te::qt::widgets::GroupingItem::GroupingItem(te::map::Grouping* g) :
  TreeItem("GROUPING"),
  m_grouping(g)
{
  m_label = (QObject::tr("Classification by") + " " + m_grouping->getPropertyName().c_str()).toStdString();

  AddSliceItems(this, m_grouping);
}

te::qt::widgets::GroupingItem::~GroupingItem()
{
}

std::string te::qt::widgets::GroupingItem::getAsString() const
{
  return m_label;
}

te::qt::widgets::VISIBLE te::qt::widgets::GroupingItem::isVisible() const
{
  return m_grouping->isVisible() ? TOTALLY : NONE;
}

void te::qt::widgets::GroupingItem::setVisible(const VISIBLE& visible, const bool&, const bool&)
{
  m_grouping->setVisibility(visible == TOTALLY ? true : false);
}

Qt::ItemFlags te::qt::widgets::GroupingItem::flags()
{
  return TreeItem::flags() | Qt::ItemIsUserCheckable;
}

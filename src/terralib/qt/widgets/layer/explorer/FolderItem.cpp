#include "FolderItem.h"

te::qt::widgets::VISIBLE GetVisibility(const te::qt::widgets::TreeItem* item)
{
  size_t c = item->getChildrenCount("");
  size_t nv = 0;

  if(c == 0)
    return te::qt::widgets::NONE;

  for(size_t i = 0; i < c; i++)
  {
    if(item->getChild(i)->isVisible() == te::qt::widgets::NONE)
      nv++;
  }

  if(nv == c)
    return te::qt::widgets::NONE;

  if(nv == 0)
    return te::qt::widgets::TOTALLY;

  return te::qt::widgets::PARTIALLY;
}

te::qt::widgets::FolderItem::FolderItem(const std::string& label):
  TreeItem("FOLDER"),
  m_label(label),
  m_v(NONE)
{

}

te::qt::widgets::FolderItem::~FolderItem()
{
}

std::string te::qt::widgets::FolderItem::getAsString() const
{
  return m_label;
}

te::qt::widgets::VISIBLE te::qt::widgets::FolderItem::isVisible() const
{
  return GetVisibility(this);
}

void te::qt::widgets::FolderItem::setVisible(const VISIBLE& visible, const bool& updateAncestors, const bool& updateDescendents)
{
  if(m_parent == 0)
    return;

  if(updateDescendents)
  {
    unsigned int c = (unsigned int)getChildrenCount("");

    for(unsigned int i = 0; i < c; i++)
      getChild(i)->setVisible(visible, false, true);
  }

  m_v = GetVisibility(this);

  if(updateAncestors)
    m_parent->setVisible(m_v, true);
}

Qt::ItemFlags te::qt::widgets::FolderItem::flags()
{
  return TreeItem::flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable;
}

void te::qt::widgets::FolderItem::setTitle(const std::string& title)
{
  m_label = title;
}

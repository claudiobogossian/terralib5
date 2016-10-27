#include "WMSItemDelegate.h"

//TerraLib
#include "../../../../qt/widgets/layer/explorer/LayerItem.h"

te::ws::ogc::wms::qt::WMSItemDelegate::WMSItemDelegate(QStyledItemDelegate *decorated, QObject *parent)
  : te::common::Decorator<QStyledItemDelegate>(decorated, false)
{
  setParent(parent);

  m_icon = QIcon::fromTheme("datasource-wms");
}

te::ws::ogc::wms::qt::WMSItemDelegate::~WMSItemDelegate()
{

}

void te::ws::ogc::wms::qt::WMSItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(index.internalPointer());

  if(item->getType() == "LAYER")
  {
    te::qt::widgets::LayerItem* li = (te::qt::widgets::LayerItem*)item;

    if(li->getLayer()->isValid() && li->getLayer()->getType() == "OGCWMSLAYER")
    {
      QStyleOptionViewItem opt = option;
      opt.decorationSize = QSize(20, 20);
      opt.features |= QStyleOptionViewItem::HasDecoration;

      opt.icon = m_icon;

      QStyledItemDelegate::paint(painter, opt, index);

      return;
    }
  }

  if(m_decorated != 0)
    m_decorated->paint(painter, option, index);
  else
    QStyledItemDelegate::paint(painter, option, index);
}

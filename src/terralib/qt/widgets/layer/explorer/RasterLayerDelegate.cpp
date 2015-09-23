#include "RasterLayerDelegate.h"

#include "LayerItem.h"

// Terralib 
#include "../../../../maptools/AbstractLayer.h"

// Qt
#include <QFont>
#include <QPainter>

te::qt::widgets::RasterLayerDelegate::RasterLayerDelegate(QStyledItemDelegate* decorated, QObject* parent):
te::common::Decorator<QStyledItemDelegate>(decorated)
{
  setParent(parent);
}

te::qt::widgets::RasterLayerDelegate::~RasterLayerDelegate()
{
}

void te::qt::widgets::RasterLayerDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

  if(item->getType() == "LAYER")
  {
    te::map::AbstractLayerPtr l = ((LayerItem*)item)->getLayer();

    if(l->getSchema()->hasRaster())
    {
      QStyleOptionViewItem opt = option;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

      opt.icon = QIcon(QIcon::fromTheme("file-raster"));

#endif

      opt.decorationSize = QSize(20, 20);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

      opt.features |= QStyleOptionViewItem::HasDecoration;

#endif

      opt.font.setItalic(true);
      opt.palette.setColor(QPalette::Text, Qt::red);

      QStyledItemDelegate::paint(painter, opt, index);

      return;
    }
  }

  if(m_decorated != 0)
    m_decorated->paint(painter, option, index);
}

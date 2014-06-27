#include "CentralizedCheckBoxDelegate.h"

#include <QApplication>
#include <QMouseEvent>

te::qt::widgets::CentralizedCheckBoxDelegate::CentralizedCheckBoxDelegate(QObject* parent):
QStyledItemDelegate(parent)
{
}

te::qt::widgets::CentralizedCheckBoxDelegate::~CentralizedCheckBoxDelegate()
{
}

void te::qt::widgets::CentralizedCheckBoxDelegate::paint (QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  QStyleOptionViewItemV4 viewItemOption(option);

  if (index.column() == 0) 
  {
    const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
    QRect newRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
    QSize(option.decorationSize.width() + 5,option.decorationSize.height()),
    QRect(option.rect.x() + textMargin, option.rect.y(),
    option.rect.width() - (2 * textMargin), option.rect.height()));
    viewItemOption.rect = newRect;
  }
  
  QStyledItemDelegate::paint(painter, viewItemOption, index);
}

bool te::qt::widgets::CentralizedCheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
  Q_ASSERT(event);
  Q_ASSERT(model);
  Qt::ItemFlags flags = model->flags(index);
  if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
    return false;

  QVariant value = index.data(Qt::CheckStateRole);
  if (!value.isValid())
    return false;

  // make sure that we have the right event type
  if (event->type() == QEvent::MouseButtonRelease) 
  {
    const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
    QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
    option.decorationSize,
    QRect(option.rect.x() + (2 * textMargin), option.rect.y(),
    option.rect.width() - (2 * textMargin),
    option.rect.height()));

    if (!checkRect.contains(static_cast<QMouseEvent*>(event)->pos()))
      return false;
  }
  else if (event->type() == QEvent::KeyPress) 
  {
    if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space&& static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)
      return false;
  } 
  else 
  {
    return false;
  }

  Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked ? Qt::Unchecked : Qt::Checked);

  return model->setData(index, state, Qt::CheckStateRole);
}

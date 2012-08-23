#include "CheckBoxCentralizer.h"

#include <QtGui>
//#include <QEvent>

CheckBoxCentralizer::CheckBoxCentralizer(QObject* parent) :
QItemDelegate(parent)
{
}

void CheckBoxCentralizer::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
      // Draw a check box for the status column
  if(m_chkColumns.find(index.column()) != m_chkColumns.end())
  {
    QStyleOptionViewItem opt(option);
    QRect crect = opt.rect;
    crect.moveTo((opt.rect.width()+opt.rect.x())/2, opt.rect.y());
    drawCheck(painter, opt, crect, Qt::Unchecked);
  }
  else
    QItemDelegate::paint(painter, option, index);
}

bool CheckBoxCentralizer::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
  Q_ASSERT(event);
  Q_ASSERT(model);

  // make sure that the item is checkable
  Qt::ItemFlags flags = model->flags(index);
  if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
    return false;
  // make sure that we have a check state
  QVariant value = index.data(Qt::CheckStateRole);
  if (!value.isValid())
    return false;
  // make sure that we have the right event type
  if (event->type() == QEvent::MouseButtonRelease) {
    const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
    QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
      option.decorationSize,
      QRect(option.rect.x() + (2 * textMargin), option.rect.y(),
      option.rect.width() - (2 * textMargin),
      option.rect.height()));
    if (!checkRect.contains(static_cast<QMouseEvent*>(event)->pos()))
      return false;
  } else if (event->type() == QEvent::KeyPress) {
    if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space&& static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)
      return false;
  } else {
    return false;
  }
  Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked
    ? Qt::Unchecked : Qt::Checked);
  return model->setData(index, state, Qt::CheckStateRole);
}

void CheckBoxCentralizer::setCheckBoxColumns(const std::set<int>& colsPos)
{
  m_chkColumns = colsPos;
}

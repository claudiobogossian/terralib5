#include "DataSetTableVerticalHeader.h"
#include "DataSetTableView.h"

// Qt
#include <QMouseEvent>

te::qt::widgets::DataSetTableVerticalHeader::DataSetTableVerticalHeader(DataSetTableView* view) :
QHeaderView(Qt::Vertical, view),
  m_initRow(-1),
  m_view(view)
{
}

te::qt::widgets::DataSetTableVerticalHeader::~DataSetTableVerticalHeader()
{
}

void te::qt::widgets::DataSetTableVerticalHeader::mousePressEvent(QMouseEvent* evt)
{
  if(evt->button() == Qt::LeftButton)
  {
    int row = m_view->rowAt(evt->pos().y());

    if(evt->modifiers() & Qt::ShiftModifier)
      emit selectedRows(m_initRow, row);
    else
    {
      m_initRow = row;

      bool add = evt->modifiers() & Qt::ControlModifier;

      emit selectedRow(row, add);
    }
  }

  QHeaderView::mousePressEvent(evt);
}
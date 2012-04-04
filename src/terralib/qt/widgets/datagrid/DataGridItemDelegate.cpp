/*  Copyright (C) 2001-2011 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file DataGridItemDelegate.cpp

  \brief It is used to paint the rows and columns selected.
 */

// TerraLib
#include "../../../maptools/DataGridOperation.h"
#include "DataGridItemDelegate.h"

te::qt::widgets::DataGridItemDelegate::DataGridItemDelegate(QObject* parent)
  : m_dataGridOp(0), QItemDelegate(parent)
{
}

te::qt::widgets::DataGridItemDelegate::~DataGridItemDelegate()
{
}

void te::qt::widgets::DataGridItemDelegate::setDataGridOperation(te::map::DataGridOperation* dataGridOp)
{
  m_dataGridOp = dataGridOp;
}

void te::qt::widgets::DataGridItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if(m_dataGridOp == 0)
  {
    QItemDelegate::paint(painter, option, index);
    return;
  }

  const te::color::RGBAColor& pColor = m_dataGridOp->getPointedColor();
  QColor pointedColor = QColor(pColor.getRed(), pColor.getGreen(), pColor.getBlue());

  const te::color::RGBAColor& qColor = m_dataGridOp->getQueriedColor();
  QColor queriedColor = QColor(qColor.getRed(), qColor.getGreen(), qColor.getBlue());

  const te::color::RGBAColor& pqColor = m_dataGridOp->getPointedAndQueriedColor();
  QColor pointedAndQueriedColor = QColor(pqColor.getRed(), pqColor.getGreen(), pqColor.getBlue());

  int row = index.row();
  int column = index.column();

  QPalette pal = option.palette;
  QStyleOptionViewItem  viewOption(option);

  te::map::DataGridOperation::SelectionStatus visualRowStatus = m_dataGridOp->getVisualRowStatus(row);
  bool visualColumnIsSelected = m_dataGridOp->getVisualColumnStatus(column);

  if(visualColumnIsSelected == true)
  {
    viewOption.palette.setColor(QPalette::Highlight, QColor(0xFF0000A0));
    viewOption.palette.setColor(QPalette::HighlightedText, Qt::white);
  }
  else if(visualRowStatus == te::map::DataGridOperation::POINTED)
  {
    viewOption.palette.setColor(QPalette::Highlight, pointedColor);
    viewOption.palette.setColor(QPalette::HighlightedText, Qt::black);
  }
  else if(visualRowStatus == te::map::DataGridOperation::QUERIED)
  {
    viewOption.palette.setColor(QPalette::Highlight, queriedColor);
    viewOption.palette.setColor(QPalette::HighlightedText, Qt::black);
  }
  else if(visualRowStatus == te::map::DataGridOperation::POINTED_AND_QUERIED)
  {
    viewOption.palette.setColor(QPalette::Highlight, pointedAndQueriedColor);
    viewOption.palette.setColor(QPalette::HighlightedText, Qt::black);
  }
  else
  {
    viewOption.palette.setColor(QPalette::Highlight, Qt::white);
    viewOption.palette.setColor(QPalette::HighlightedText, Qt::black);
  }

  QItemDelegate::paint(painter, viewOption, index);
}

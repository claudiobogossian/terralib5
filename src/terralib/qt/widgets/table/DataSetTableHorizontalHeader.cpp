/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.
  
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


#include "DataSetTableHorizontalHeader.h"

#include "DataSetTableView.h"

// Qt
#include <QtGui/QMouseEvent>

te::qt::widgets::DataSetTableHorizontalHeader::DataSetTableHorizontalHeader(DataSetTableView *view) :
QHeaderView(Qt::Horizontal, view),
  m_view(view)
{
}


void te::qt::widgets::DataSetTableHorizontalHeader::mouseReleaseEvent(QMouseEvent *e)
{
  if(e->button() == Qt::LeftButton)
  {
    int c = m_view->columnAt(e->pos().x());
    QItemSelectionModel* sel = m_view->selectionModel();

    if(c > 0)
    {
      QItemSelectionModel::SelectionFlag selF = (sel->isColumnSelected(c, QModelIndex())) ? QItemSelectionModel::Deselect : QItemSelectionModel::Select;

      if(m_view->selectionMode() == QAbstractItemView::SingleSelection)
        sel->clear();


      QModelIndex tl = m_view->model()->index(0, c);
      QModelIndex br = m_view->model()->index(model()->rowCount()-1, c);

      QItemSelection newSel(tl, br);

      sel->select(newSel, selF);

      return;
    }
    else
      sel->clear();
  }

  QHeaderView::mouseReleaseEvent(e);
}

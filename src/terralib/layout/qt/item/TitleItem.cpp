/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file TitleItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TitleItem.h"

te::layout::TitleItem::TitleItem(AbstractItemController* controller) 
  : TextItem(controller)
{
}

te::layout::TitleItem::~TitleItem()
{
}

/*
void te::layout::TitleItem::refreshDocument()
{
  if(!m_model)
    return;

  TitleModel* model = dynamic_cast<TitleModel*>(m_model);

  if(!model)
    return;

  QImage img = createImage();
  QPointF pp = scenePos();
  te::gm::Envelope box(pp.x(), pp.y(), pp.x() + img.widthMM(), pp.y() + img.heightMM());

  model->setBox(box);
  
  // Update Model

  // Header
  QTextTableCell cellOne = m_table->cellAt(0, 0);
  QTextCursor cellCursorOne = cellOne.firstCursorPosition();
  std::string txtOne = cellCursorOne.block().text().toStdString();
  model->setTitle(txtOne);

  // Table
  QTextTableCell cellTwo = m_table->cellAt(1, 0);
  QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();
  std::string txtTwo = cellCursorTwo.block().text().toStdString();
  model->setText(txtTwo);
}
*/

void te::layout::TitleItem::updateGeometry( int position, int charsRemoved, int charsAdded )
{
  //we override this function just to avoid the view to call textChanged function from the controller
}

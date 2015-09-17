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
  \file TextGridItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextGridItem.h"
#include "../../core/AbstractScene.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/TextGridModel.h"
#include "../../core/pattern/singleton/Context.h"

// Qt
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTableCell>

te::layout::TextGridItem::TextGridItem(AbstractItemController* controller) 
  : TitleItem(controller)
{
  init();
}

te::layout::TextGridItem::~TextGridItem()
{
  
}

void te::layout::TextGridItem::init()
{
  updateDocument();

  //resetEdit();
}

void te::layout::TextGridItem::refreshDocument()
{
  /*
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
  int countRows = m_table->rows();
  int countColumns = m_table->columns();

  //Table Headers (Hrz)
  for(int i = 1 ; i < countRows ; ++i)
  {
    QTextTableCell cellOne = m_table->cellAt(i, 0);
    QTextCursor cellCursorOne = cellOne.firstCursorPosition();
    std::string txtOne = cellCursorOne.block().text().toStdString();
    //model->setTitle(txtOne);
  }

  //Table Headers (Vrt)
  for(int j = 0 ; j < countColumns ; ++j)
  {
    QTextTableCell cellTwo = m_table->cellAt(0, j);
    QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();
    std::string txtTwo = cellCursorTwo.block().text().toStdString();
    //model->setText(txtTwo);
  }

  //Table
  for (int i = 1 ; i < countRows ; ++i) 
  {
    for(int j = 1 ; j < countColumns ; ++j)
    {
      QTextTableCell cellThree = m_table->cellAt(i, j);
      QTextCursor cellCursorThree = cellThree.firstCursorPosition();
      std::string txtThree = cellCursorThree.block().text().toStdString();
      //model->setText(txtThree);
    }
  }
  */
}

void te::layout::TextGridItem::updateDocument()
{
  /*
  document()->clear();

  QTextDocument* doc = document();

  QTextCursor cursor(doc);
  cursor.movePosition(QTextCursor::Start);

  QBrush blackBrush(Qt::SolidPattern);

  QTextTableFormat tableFormat;
  tableFormat.setAlignment(Qt::AlignLeft);
  tableFormat.setBorderBrush(blackBrush);
  tableFormat.setBorder(0.5);

  int columns = model->getNumberColumns();
  int rows = model->getNumberRows();
 
  tableFormat.setCellSpacing(3);  
  tableFormat.setCellPadding(3); 
  tableFormat.setHeaderRowCount(1);
  
  m_table = cursor.insertTable(rows, columns, tableFormat);
    
  int countRows = m_table->rows();
  int countColumns = m_table->columns();
    
  //Table Headers (Hrz)
  for(int i = 1 ; i < countRows ; ++i)
  {
    QTextTableCell cellOne = m_table->cellAt(i, 0);
    QTextCharFormat fmtOne = cellOne.format(); 
    fmtOne.setBackground(Qt::gray);
    fmtOne.setFontWeight(QFont::Bold);
    cellOne.setFormat(fmtOne);
  }

  //Table Headers (Vrt)
  for(int j = 0 ; j < countColumns ; ++j)
  {
    QTextTableCell cellTwo = m_table->cellAt(0, j);
    QTextCharFormat fmtTwo = cellTwo.format(); 
    fmtTwo.setBackground(Qt::gray); 
    fmtTwo.setFontWeight(QFont::Bold);
    cellTwo.setFormat(fmtTwo);
    
  }
  */
}

/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
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

te::layout::TextGridItem::TextGridItem( ItemController* controller, Observable* o ) :
  TitleItem(controller, o)
{

}

te::layout::TextGridItem::~TextGridItem()
{
  
}

void te::layout::TextGridItem::updateObserver( ContextItem context )
{
  if(!m_model)
    return;

  te::color::RGBAColor** rgba = context.getPixmap();

  if(!rgba)
    return;

  Utils* utils = context.getUtils();

  if(!utils)
    return;

  DefaultTextModel* model = dynamic_cast<DefaultTextModel*>(m_model);
  if(model)
  {
    Font font = model->getFont();
    QFont qfont;
    qfont.setFamily(font.getFamily().c_str());
    qfont.setPointSize(font.getPointSize());
    qfont.setBold(font.isBold());
    qfont.setItalic(font.isItalic());
    qfont.setUnderline(font.isUnderline());
    qfont.setStrikeOut(font.isStrikeout());
    qfont.setKerning(font.isKerning());
    setFont(qfont);
  }

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

  if(!box.isValid())
    return;

  QPixmap pixmap;
  QImage* img = 0;

  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmap = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;

  setPixmap(pixmap);
  update();
}

void te::layout::TextGridItem::init()
{
  TextGridModel* model = dynamic_cast<TextGridModel*>(m_model);

  if(!model)
    return;

  QTextDocument* doc = new QTextDocument(this);
  setDocument(doc);
  
  QTextCursor cursor(doc);
  cursor.movePosition(QTextCursor::Start);
  
  QBrush blackBrush(Qt::SolidPattern);

  QTextTableFormat tableFormat;
  tableFormat.setAlignment(Qt::AlignLeft);
  tableFormat.setBorderBrush(blackBrush);
  tableFormat.setBorder(0.5);
  
  Utils* utils = Context::getInstance().getUtils();

  int cellSpacing = utils->mm2pixel(model->getSpacing());  
  tableFormat.setCellSpacing(cellSpacing); 

  int cellPadding = utils->mm2pixel(model->getPadding()); 
  tableFormat.setCellPadding(cellPadding); 

  tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));

   QVector<QTextLength> constraints;
  constraints << QTextLength(QTextLength::PercentageLength, 100)
  << QTextLength(QTextLength::PercentageLength, 100)
  << QTextLength(QTextLength::PercentageLength, 100)
  << QTextLength(QTextLength::PercentageLength, 100)
  << QTextLength(QTextLength::PercentageLength, 100)
  << QTextLength(QTextLength::PercentageLength, 100);
  tableFormat.setColumnWidthConstraints(constraints);

  tableFormat.setHeaderRowCount(1);

  int columns = model->getNumberColumns();
  int rows = model->getNumberRows();
  m_table = cursor.insertTable(rows, columns, tableFormat);
  
  int countRows = m_table->rows();
  int countColumns = m_table->columns();

  QTextCharFormat format = cursor.charFormat();
  format.setFontPointSize(12);

  QTextCharFormat boldFormat = format;
  boldFormat.setFontWeight(QFont::Bold);
 
  //Table Headers (Hrz)
  for(int i = 1 ; i < countRows ; ++i)
  {
    QTextTableCell cellOne = m_table->cellAt(i, 0);
    QTextCharFormat fmtOne = cellOne.format(); 
    fmtOne.setBackground(Qt::gray);
    cellOne.setFormat(fmtOne);
    QTextCursor cellCursorOne = cellOne.firstCursorPosition();
    cellCursorOne.insertText("Header", boldFormat);
  }

  //Table Headers (Vrt)
  for(int j = 0 ; j < countColumns ; ++j)
  {
    QTextTableCell cellTwo = m_table->cellAt(0, j);
    QTextCharFormat fmtTwo = cellTwo.format(); 
    fmtTwo.setBackground(Qt::gray); 
    cellTwo.setFormat(fmtTwo);
    QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();
    cellCursorTwo.insertText("Header", boldFormat);
  }

  //Table
  for (int i = 1 ; i < countRows ; ++i) 
  {
    for(int j = 1 ; j < countColumns ; ++j)
    {
      QTextTableCell cellThree = m_table->cellAt(i, j);
      QTextCursor cellCursorThree = cellThree.firstCursorPosition();
      cellCursorThree.insertText("Text", format);
    }
  }

  DefaultTextItem::init();
  adjustSizeMM();
}

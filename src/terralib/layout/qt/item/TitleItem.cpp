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
  \file TitleItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TitleItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../core/Font.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../item/TitleModel.h"
#include "../../core/Utils.h"

// Qt
#include <QTextDocument>
#include <QFont>
#include <QTextCursor>
#include <QTextTableCell>
#include <QTextTableFormat>
#include <QTextLength>
#include <QTextCharFormat>

te::layout::TitleItem::TitleItem( ItemController* controller, Observable* o ) :
  TextItem(controller, o)
{
  
}

te::layout::TitleItem::~TitleItem()
{
  if(m_table)
  {
    delete m_table;
    m_table = 0;
  }
}

//void te::layout::TitleItem::init()
//{
//  TitleModel* model = dynamic_cast<TitleModel*>(m_model);
//
//  if(!model)
//    return;
//
//  QTextDocument* doc = m_document;
//    
//  QTextCursor cursor(doc);
//  cursor.movePosition(QTextCursor::Start);
//  
//  QBrush blackBrush(Qt::SolidPattern);
//
//  QTextTableFormat tableFormat;
//  tableFormat.setAlignment(Qt::AlignLeft);
//  tableFormat.setBorderBrush(blackBrush);
//
//  tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
//  tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));
//
//  QVector<QTextLength> constraints;
//  constraints << QTextLength(QTextLength::PercentageLength, 100);
//  tableFormat.setColumnWidthConstraints(constraints);
//
//  tableFormat.setHeaderRowCount(1);
//
//  m_table = cursor.insertTable(2, 1, tableFormat);
//  
//  int countRows = m_table->rows();
//  int countColumns = m_table->columns();
//
//  QTextCharFormat format = cursor.charFormat();
//  format.setFontPointSize(12);
//
//  QTextCharFormat boldFormat = format;
//  boldFormat.setFontWeight(QFont::Bold);
//
//  // Header
//  QTextTableCell cellOne = m_table->cellAt(0, 0);
//  QTextCharFormat fmtOne = cellOne.format(); 
//  fmtOne.setBackground(Qt::blue); 
//  cellOne.setFormat(fmtOne);
//  QTextCursor cellCursorOne = cellOne.firstCursorPosition();
//  std::string title = model->getTitle();
//  cellCursorOne.insertText(title.c_str(), boldFormat);
//  
//  //Table
//  QTextTableCell cellTwo = m_table->cellAt(1, 0);
//  QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();
//  std::string txt = model->getText();
//  cellCursorTwo.insertText(txt.c_str(), format);
//
//  refreshTable();
//
//  DefaultTextItem::init();
//  adjustSizeMM();
//}
//
//void te::layout::TitleItem::refreshTable()
//{
//  TitleModel* model = dynamic_cast<TitleModel*>(m_model);
//
//  if(!model)
//    return;
//
//  Font font = model->getFont();
//  QFont qfont;
//  qfont.setFamily(font.getFamily().c_str());
//  qfont.setPointSize(font.getPointSize());
//  qfont.setBold(font.isBold());
//  qfont.setItalic(font.isItalic());
//  qfont.setUnderline(font.isUnderline());
//  qfont.setStrikeOut(font.isStrikeout());
//  qfont.setKerning(font.isKerning());
//  setFont(qfont);
//
//  QTextCursor cursor(document());
//  cursor.movePosition(QTextCursor::Start);
//  
//  double spacing = model->getSpacing();
//  double padding = model->getPadding();
//
//  int numRows = model->getNumberRows();
//  int numColumns = model->getNumberColumns();
//
//  te::color::RGBAColor backColor = model->getTableColor();
//  QColor qBackColor(backColor.getRed(), backColor.getGreen(),
//    backColor.getBlue(), backColor.getAlpha());
//
//  te::color::RGBAColor borderColor = model->getBorderGridColor();
//  QColor qBorderColor(borderColor.getRed(), borderColor.getGreen(),
//    borderColor.getBlue(), borderColor.getAlpha());
//
//  double borderWidth = model->getBorderWidth();
//
//  QBrush tableBrush(qBackColor);
//  tableBrush.setStyle(Qt::SolidPattern);
//
//  QTextTableFormat tableFormat;
//  if(m_table)
//  {
//    tableFormat = m_table->format();
//  }
//  tableFormat.setAlignment(Qt::AlignLeft);
//  tableFormat.setBorderBrush(tableBrush);
//  tableFormat.setBorder(borderWidth);
//
//  Utils* utils = Context::getInstance().getUtils();
//
//  int cellSpacing = utils->mm2pixel(spacing);  
//  tableFormat.setCellSpacing(cellSpacing); 
//
//  int cellPadding = utils->mm2pixel(padding); 
//  tableFormat.setCellPadding(cellPadding); 
//
//  tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
//  tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));
//
//  QVector<QTextLength> constraints;
//
//  for(int c = 0 ; c < numColumns ; ++c)
//  {
//    constraints << QTextLength(QTextLength::PercentageLength, 100);    
//  }
//
//  tableFormat.setColumnWidthConstraints(constraints);
//
//  tableFormat.setHeaderRowCount(1);
//
//  if(m_table)
//  {
//    m_table->setFormat(tableFormat);
//    int countRows = m_table->rows();
//    int countColumns = m_table->columns();
//
//    if(countRows != numRows || countColumns != numColumns)
//    {
//      m_table->resize(numRows, numColumns);
//    }
//  }
//  else
//  {
//    m_table = cursor.insertTable(numRows, numColumns, tableFormat);
//  }
//
//  refreshText();
//}
//
//void te::layout::TitleItem::refreshText()
//{
//  TitleModel* model = dynamic_cast<TitleModel*>(m_model);
//
//  if(!model)
//    return;
//
//  int numRows = model->getNumberRows();
//  int numColumns = model->getNumberColumns();
//
//  QTextCursor cursor(document());
//  cursor.movePosition(QTextCursor::Start);
//
//  QTextCharFormat format = cursor.charFormat();
//  format.setFontPointSize(12);
//
//  QTextCharFormat boldFormat = format;
//  boldFormat.setFontWeight(QFont::Bold);
//
//  te::color::RGBAColor headerVtrColor = model->getHeaderVerticalColor();
//  QColor qHeaderVtrColor(headerVtrColor.getRed(), headerVtrColor.getGreen(),
//    headerVtrColor.getBlue(), headerVtrColor.getAlpha());
//
//  te::color::RGBAColor headerHrzColor = model->getHeaderHorizontalColor();
//  QColor qHeaderHrzColor(headerHrzColor.getRed(), headerHrzColor.getGreen(),
//    headerHrzColor.getBlue(), headerHrzColor.getAlpha());
//
//  te::color::RGBAColor borderColor = model->getBorderGridColor();
//  QColor qBorderColor(borderColor.getRed(), borderColor.getGreen(),
//    borderColor.getBlue(), borderColor.getAlpha());
//
//  te::color::RGBAColor evenRowColor = model->getEvenRow();
//  QColor qEvenRowColor(evenRowColor.getRed(), evenRowColor.getGreen(),
//    evenRowColor.getBlue(), evenRowColor.getAlpha());
//
//  te::color::RGBAColor oddRowColor = model->getOddRow();
//  QColor qOddRowColor(oddRowColor.getRed(), oddRowColor.getGreen(),
//    oddRowColor.getBlue(), oddRowColor.getAlpha());
//  
//  // Header
//  QBrush headerHrz(qHeaderHrzColor);
//  QTextTableCell cellOne = m_table->cellAt(0, 0);
//  QTextCharFormat fmtOne = cellOne.format(); 
//  fmtOne.setBackground(headerHrz); 
//  cellOne.setFormat(fmtOne);
//  QTextCursor cellCursorOne = cellOne.firstCursorPosition();
//  std::string title = model->getTitle();
//  cellCursorOne.insertText(title.c_str(), boldFormat);
//  
//  //Table
//  QBrush headerVrt(qHeaderVtrColor);
//  QTextTableCell cellTwo = m_table->cellAt(1, 0);
//  QTextCharFormat fmtTwo = cellTwo.format(); 
//  fmtTwo.setBackground(headerVrt); 
//  cellTwo.setFormat(fmtTwo);
//  QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();
//  std::string txt = model->getText();
//  cellCursorTwo.insertText(txt.c_str(), format);
//}

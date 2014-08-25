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
#include "../../item/DefaultTextModel.h"
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
  DefaultTextItem(controller, o)
{
  setTextInteractionFlags(Qt::TextEditorInteraction);
}

te::layout::TitleItem::~TitleItem()
{
  if(m_table)
  {
    delete m_table;
    m_table = 0;
  }
}

void te::layout::TitleItem::updateObserver( ContextItem context )
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

void te::layout::TitleItem::init()
{
  TitleModel* model = dynamic_cast<TitleModel*>(m_model);

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

  tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));

  QVector<QTextLength> constraints;
  constraints << QTextLength(QTextLength::PercentageLength, 100);
  tableFormat.setColumnWidthConstraints(constraints);

  tableFormat.setHeaderRowCount(1);

  m_table = cursor.insertTable(2, 1, tableFormat);
  
  int countRows = m_table->rows();
  int countColumns = m_table->columns();

  QTextCharFormat format = cursor.charFormat();
  format.setFontPointSize(12);

  QTextCharFormat boldFormat = format;
  boldFormat.setFontWeight(QFont::Bold);

  // Header
  QTextTableCell cellOne = m_table->cellAt(0, 0);
  QTextCharFormat fmtOne = cellOne.format(); 
  fmtOne.setBackground(Qt::blue); 
  cellOne.setFormat(fmtOne);
  QTextCursor cellCursorOne = cellOne.firstCursorPosition();
  std::string title = model->getTitle();
  cellCursorOne.insertText(title.c_str(), boldFormat);
  
  //Table
  QTextTableCell cellTwo = m_table->cellAt(1, 0);
  QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();
  std::string txt = model->getText();
  cellCursorTwo.insertText(txt.c_str(), format);

  DefaultTextItem::init();
  adjustSizeMM();
}

void te::layout::TitleItem::refreshText()
{
  TitleModel* model = dynamic_cast<TitleModel*>(m_model);

  if(!model)
    return;

  // Header
  QTextTableCell cellOne = m_table->cellAt(0, 0);
  QTextCursor cellCursorOne = cellOne.firstCursorPosition();
    
  QString st = cellCursorOne.block().text();
  int rs = cellOne.rowSpan();
  
  //.
  //.
  //.

  //Table
  QTextTableCell cellTwo = m_table->cellAt(1, 0);
  QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();

}

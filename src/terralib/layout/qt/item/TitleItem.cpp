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
#include <QTextList>

te::layout::TitleItem::TitleItem( ItemController* controller, Observable* o ) :
  TextItem(controller, o)
{
  init();
}

te::layout::TitleItem::~TitleItem()
{
  if(m_table)
  {
    delete m_table;
    m_table = 0;
  }
}

void te::layout::TitleItem::init()
{
  TitleModel* model = dynamic_cast<TitleModel*>(m_model);

  if(!model)
    return;

  updateDocument();

  resetEdit();
}

void te::layout::TitleItem::updateObserver( ContextItem context )
{
  if(!m_model)
    return;

  if(!document())
    return;
 
  updateDocument();

  refreshDocument();

  refresh();

  update();
}

void te::layout::TitleItem::updateDocument()
{
  TitleModel* model = dynamic_cast<TitleModel*>(m_model);

  if(!model)
    return;

  document()->clear();

  QTextDocument* doc = document();

  QTextCursor cursor(doc);
  cursor.movePosition(QTextCursor::Start);

  QBrush blackBrush(Qt::SolidPattern);

  QTextTableFormat tableFormat;
  tableFormat.setAlignment(Qt::AlignLeft);

  QVector<QTextLength> constraints;

  for(int c = 0 ; c < 2 ; ++c)
  {
    constraints << QTextLength(QTextLength::PercentageLength, 100);    
  }

  tableFormat.setColumnWidthConstraints(constraints);
  
  tableFormat.setHeaderRowCount(1);

  m_table = cursor.insertTable(2, 1, tableFormat);
  
  QTextCharFormat format = cursor.charFormat();
  format.setFontPointSize(model->getFont().getPointSize());
      
  te::color::RGBAColor headerVtrColor = model->getHeaderVerticalColor();
  QColor qHeaderVtrColor(headerVtrColor.getRed(), headerVtrColor.getGreen(),
    headerVtrColor.getBlue(), headerVtrColor.getAlpha());

  te::color::RGBAColor headerHrzColor = model->getHeaderHorizontalColor();
  QColor qHeaderHrzColor(headerHrzColor.getRed(), headerHrzColor.getGreen(),
    headerHrzColor.getBlue(), headerHrzColor.getAlpha());

  te::color::RGBAColor borderColor = model->getBorderGridColor();
  QColor qBorderColor(borderColor.getRed(), borderColor.getGreen(),
    borderColor.getBlue(), borderColor.getAlpha());

  te::color::RGBAColor evenRowColor = model->getEvenRow();
  QColor qEvenRowColor(evenRowColor.getRed(), evenRowColor.getGreen(),
    evenRowColor.getBlue(), evenRowColor.getAlpha());

  te::color::RGBAColor oddRowColor = model->getOddRow();
  QColor qOddRowColor(oddRowColor.getRed(), oddRowColor.getGreen(),
    oddRowColor.getBlue(), oddRowColor.getAlpha());

  //Header
  QBrush headerVrt(qHeaderVtrColor);
  QTextTableCell cellTwo = m_table->cellAt(0, 0);
  QTextCharFormat fmtTwo = cellTwo.format(); 
  fmtTwo.setBackground(headerVrt); 
  cellTwo.setFormat(fmtTwo);
  QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();
  std::string title = model->getTitle();
  cellCursorTwo.insertText(title.c_str(), format);

  // table
  QBrush headerHrz(qHeaderHrzColor);
  QTextTableCell cellOne = m_table->cellAt(1, 0);
  QTextCharFormat fmtOne = cellOne.format(); 
  fmtOne.setBackground(headerHrz); 
  cellOne.setFormat(fmtOne);
  std::string txt = model->getText();
  QTextCursor cellCursorOne = cellOne.firstCursorPosition();  
  cellCursorOne.insertText(txt.c_str(), format);
}

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

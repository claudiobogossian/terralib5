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

// TerraLib
#include "TitleController1.h"

#include "../core/pattern/command/ChangePropertyCommand.h"
#include "../../core/enum/EnumAlignmentType.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/property/TextGridSettingsConfigProperties.h"
#include "../../qt/core/Scene.h"
#include "../../qt/item/TitleItem.h"

#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QFontMetricsF>
#include <QTextCursor>
#include <QTextTable>

te::layout::TitleController1::TitleController1( te::layout::AbstractItemModel* model)
  : TextController1(model)
  , m_table(0)
{
}

te::layout::TitleController1::~TitleController1()
{
}

QSizeF te::layout::TitleController1::updateView()
{
  TextItem* view = dynamic_cast<TextItem*>(m_view);
  if(view == 0)
  {
    return QSizeF();
  }

  TextGridSettingsConfigProperties propertyNames;

  const Property& pFont = m_model->getProperty("font");
  const Property& pVerticalColor = m_model->getProperty(propertyNames.getHeaderVerticalColor());
  const Property& pHorizontalColor = m_model->getProperty(propertyNames.getHeaderHorizontalColor());
  const Property& pGridBorderColor = m_model->getProperty(propertyNames.getBorderGridColor());
  const Property& pOddRow = m_model->getProperty(propertyNames.getOddRow());
  const Property& pEvenRow = m_model->getProperty(propertyNames.getEvenRow());
  const Property& pTitle = m_model->getProperty(propertyNames.getTitle());
  const Property& pText = m_model->getProperty("text");

  const Font& font = pFont.getValue().toFont();
  const te::color::RGBAColor& verticalColor = pVerticalColor.getValue().toColor();
  const te::color::RGBAColor& horizontalColor = pHorizontalColor.getValue().toColor();
  const te::color::RGBAColor& gridBorderColor = pGridBorderColor.getValue().toColor();
  const te::color::RGBAColor& oddRowColor = pOddRow.getValue().toColor();
  const te::color::RGBAColor& evenRowColor = pEvenRow.getValue().toColor();
  const std::string& title = pTitle.getValue().toString();
  const std::string& text = pText.getValue().toString();

  QTextDocument* doc = view->document();
  doc->clear();

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
  format.setFontPointSize(font.getPointSize());

  QColor qHeaderVtrColor(verticalColor.getRed(), verticalColor.getGreen(), verticalColor.getBlue(), verticalColor.getAlpha());
  QColor qHeaderHrzColor(horizontalColor.getRed(), horizontalColor.getGreen(), horizontalColor.getBlue(), horizontalColor.getAlpha());
  QColor qBorderColor(gridBorderColor.getRed(), gridBorderColor.getGreen(), gridBorderColor.getBlue(), gridBorderColor.getAlpha());
  QColor qEvenRowColor(evenRowColor.getRed(), evenRowColor.getGreen(), evenRowColor.getBlue(), evenRowColor.getAlpha());
  QColor qOddRowColor(oddRowColor.getRed(), oddRowColor.getGreen(), oddRowColor.getBlue(), oddRowColor.getAlpha());

  //Header
  QBrush headerVrt(qHeaderVtrColor);
  QTextTableCell cellTwo = m_table->cellAt(0, 0);
  QTextCharFormat fmtTwo = cellTwo.format(); 
  fmtTwo.setBackground(headerVrt); 
  cellTwo.setFormat(fmtTwo);
  QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();
  cellCursorTwo.insertText(title.c_str(), format);

  // table
  QBrush headerHrz(qHeaderHrzColor);
  QTextTableCell cellOne = m_table->cellAt(1, 0);
  QTextCharFormat fmtOne = cellOne.format(); 
  fmtOne.setBackground(headerHrz); 
  cellOne.setFormat(fmtOne);
  QTextCursor cellCursorOne = cellOne.firstCursorPosition();  
  cellCursorOne.insertText(text.c_str(), format);

  //we get the size in pixels
  QRectF rect = view->boundingRect();
  QRectF boundingRect(QPointF(0, 0), rect.size());

  //then we convert to millimiters
  boundingRect = view->mapRectToScene(boundingRect);

  //we finally return the new size in millimiters
  return boundingRect.size();
}

void te::layout::TitleController1::textChanged()
{
  TextItem* view = dynamic_cast<TextItem*>(m_view);
  if(view != 0)
  {
    TextGridSettingsConfigProperties propertyNames;

    const Property& pTitle = getProperty(propertyNames.getTitle());
    const Property& pText = getProperty("text");

    std::string currentTitle = pTitle.getValue().toString();
    std::string currentText = pText.getValue().toString();

    QString qCurrentTitle(currentTitle.c_str());
    QString qCurrentText(currentText.c_str());

    QTextTableCell cellOne = m_table->cellAt(0, 0);
    QTextCursor cellCursorOne = cellOne.firstCursorPosition();
    QString qNewTitle = cellCursorOne.block().text();

    QTextTableCell cellTwo = m_table->cellAt(1, 0);
    QTextCursor cellCursorTwo = cellTwo.firstCursorPosition();
    QString qNewText = cellCursorTwo.block().text();

    if(qCurrentTitle != qNewTitle || qCurrentText != qNewText)
    {
      EnumDataType* dataType = Enums::getInstance().getEnumDataType();

      Property propertyTitle(0);
      propertyTitle.setName(propertyNames.getTitle());
      propertyTitle.setValue(qNewTitle.toStdString(), dataType->getDataTypeString());

      Property propertyText(0);
      propertyText.setName("text");
      propertyText.setValue(qNewText.toStdString(), dataType->getDataTypeString());

      Properties properties("");
      properties.addProperty(propertyTitle);
      properties.addProperty(propertyText);

      Properties* oldCommand = new Properties(m_model->getProperties());
      m_model->setProperties(properties);
      Properties* newCommand = new Properties(m_model->getProperties());

      QUndoCommand* command = new ChangePropertyCommand(view, oldCommand, newCommand);

      Scene* sc = dynamic_cast<Scene*>(view->scene());
      if(sc)
      {
        sc->addUndoStack(command);
      }
    }
  }
}
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

/*!
  \file terralib/qt/widgets/se/ColorMapWidget.cpp

  \brief A dialog used to build a Color Map element.
*/

// TerraLib
#include "../../../se/ColorMap.h"
#include "../../../color/ColorBar.h"
#include "../../qwt/ColorBar.h"
#include "ColorMapWidget.h"
#include "ui_ColorMapWidgetForm.h"
#include "../../../common/STLUtils.h"

// Qt
#include <QtGui/QPainter>

// STL
#include <cassert>


te::qt::widgets::ColorMapWidget::ColorMapWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ColorMapWidgetForm),
    m_cm(new te::se::ColorMap)
{
  m_ui->setupUi(this);

  QGridLayout* l = new QGridLayout(m_ui->m_colorBarWidget);
  l->setContentsMargins(0,0,0,0);
  m_cbWidget = new te::qt::qwt::ColorBar(m_ui->m_colorBarWidget);
  l->addWidget(m_cbWidget);
  
  initialize();

  // Signals & slots
  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), this, SLOT(onApplyPushButtonClicked()));
}

te::qt::widgets::ColorMapWidget::~ColorMapWidget()
{
  delete m_cb;
}

void te::qt::widgets::ColorMapWidget::setColorMap(te::se::ColorMap* cm) 
{
  assert(cm);

  m_cm = cm;

  updateUi();
}

te::se::ColorMap* te::qt::widgets::ColorMapWidget::getColorMap() const
{
  return m_cm;
}

void te::qt::widgets::ColorMapWidget::initialize()
{
  m_cb = new te::color::ColorBar(te::color::RGBAColor(255, 0, 0, TE_OPAQUE), te::color::RGBAColor(255, 255, 255, TE_OPAQUE), 256);

  m_cbWidget->setHeight(20);
  m_cbWidget->setColorBar(m_cb);
}

void te::qt::widgets::ColorMapWidget::updateUi()
{
  //extract color bar from color map and fill inteface
  int sliceValue = m_ui->m_slicesSpinBox->value();

  std::vector<te::color::RGBAColor> colorVec = m_cb->getSlices(sliceValue);

  m_ui->m_tableWidget->setRowCount(colorVec.size());

  for(size_t i = 0; i < colorVec.size(); ++i)
  {
    QPixmap pix(16, 16);
    pix.fill(QColor(colorVec[i].getRed(), colorVec[i].getGreen(), colorVec[i].getBlue(), colorVec[i].getAlpha()));
    
    QPolygon poly;
    poly.append(QPoint(0,0));
    poly.append(QPoint(0,15));
    poly.append(QPoint(15,15));
    poly.append(QPoint(15,0));
    poly.append(QPoint(0,0));

    QPainter p(&pix);
    QPen pen(Qt::black);
    p.setPen(pen);
    p.drawPolygon(poly);

    QIcon icon(pix);

    QTableWidgetItem* item = new QTableWidgetItem();
    item->setIcon(icon);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    m_ui->m_tableWidget->setItem(i, 1, item);
  }

  m_ui->m_tableWidget->resizeColumnToContents(1);
}


void te::qt::widgets::ColorMapWidget::onApplyPushButtonClicked()
{
  //get inteface values
  QString bandValue = m_ui->m_bandComboBox->currentText();
  
  QString transfValue = m_ui->m_transformComboBox->currentText();

  QString typeValue = m_ui->m_typeComboBox->currentText();

  updateUi();
}
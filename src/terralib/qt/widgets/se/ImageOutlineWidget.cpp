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
  \file terralib/qt/widgets/se/ImageOutlineWidget.cpp

  \brief A dialog used to build a Symbolizer element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../se.h"
#include "ImageOutlineWidget.h"
#include "LineSymbolizerWidget.h"
#include "PolygonSymbolizerWidget.h"
#include "SymbolPreviewWidget.h"
#include "ui_ImageOutlineWidgetForm.h"

// STL
#include <cassert>

te::qt::widgets::ImageOutlineWidget::ImageOutlineWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ImageOutlineWidgetForm),
    m_psWidget(0),
    m_io(new te::se::ImageOutline)
{
  m_ui->setupUi(this);

  //adjust interface
  m_layout = new QGridLayout(m_ui->m_scrollArea);

  m_ui->m_previewGroupBox->setVisible(false);

  // Preview
  QGridLayout* l = new QGridLayout(m_ui->m_previewWidget);
  l->setContentsMargins(0,0,0,0);
  l->setAlignment(Qt::AlignTop);
  
  m_preview = new te::qt::widgets::SymbolPreviewWidget(QSize(120, 120), m_ui->m_previewWidget);

  l->addWidget(m_preview);

  // Signals & slots
  connect(m_ui->m_NoneSymbolizerRadioButton, SIGNAL(clicked()), SLOT(onNoneSymbolizerClicked()));
  connect(m_ui->m_polygonSymbolizerRadioButton, SIGNAL(clicked()), SLOT(onPolygonSymbolizerClicked()));
}

te::qt::widgets::ImageOutlineWidget::~ImageOutlineWidget()
{
  delete m_io;
}

void te::qt::widgets::ImageOutlineWidget::setImageOutline(const te::se::ImageOutline* io)
{
  assert(io);

  delete m_io;

  m_io = io->clone();

  if(m_io->getSymbolizer())
  {
    m_ui->m_polygonSymbolizerRadioButton->setChecked(true);

    onPolygonSymbolizerClicked();
  }
  else
  {
    m_ui->m_NoneSymbolizerRadioButton->setChecked(true);

    onNoneSymbolizerClicked();
  }
}

te::se::ImageOutline* te::qt::widgets::ImageOutlineWidget::getImageOutline() const
{
  return m_io->clone();
}

void te::qt::widgets::ImageOutlineWidget::updateUi()
{
  te::se::Symbolizer* s = m_io->getSymbolizer();

  m_ui->m_previewGroupBox->setVisible(true);

  m_preview->updatePreview(s);

  delete s;
}

void te::qt::widgets::ImageOutlineWidget::deleteInterfaces()
{
  delete m_psWidget;
  m_psWidget = 0;
}

void te::qt::widgets::ImageOutlineWidget::onNoneSymbolizerClicked()
{
  deleteInterfaces();

  m_io->setSymbolizer(0);

  m_ui->m_previewGroupBox->setVisible(false);
}

void te::qt::widgets::ImageOutlineWidget::onPolygonSymbolizerClicked()
{
  deleteInterfaces();

  m_psWidget = new te::qt::widgets::PolygonSymbolizerWidget(m_ui->m_scrollArea);

  if(m_io->getSymbolizer())
  {
    te::se::Symbolizer* s = m_io->getSymbolizer();

    m_psWidget->setSymbolizer((te::se::PolygonSymbolizer*)s);

    delete s;
  }

  connect(m_psWidget, SIGNAL(symbolizerChanged()), SLOT(onPolygonSymbolizerCreated()));

  m_layout->addWidget(m_psWidget);

  m_psWidget->show();

  m_io->setSymbolizer(m_psWidget->getSymbolizer());

  updateUi();
}

void te::qt::widgets::ImageOutlineWidget::onPolygonSymbolizerCreated()
{
  m_io->setSymbolizer(m_psWidget->getSymbolizer());

  updateUi();
}

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
  \file terralib/qt/widgets/se/ImageOutlineWidget.cpp

  \brief A dialog used to build a Symbolizer element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "ImageOutlineWidget.h"
#include "LineSymbolizerWidget.h"
#include "PolygonSymbolizerWidget.h"
#include "SymbolizerPreviewWidget.h"
#include "ui_ImageOutlineWidgetForm.h"
#include "../../../se.h"

// Qt


// STL
#include <cassert>

te::qt::widgets::ImageOutlineWidget::ImageOutlineWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ImageOutlineWidgetForm),
    m_io(new te::se::ImageOutline),
    m_lsWidget(0),
    m_psWidget(0)
{
  m_ui->setupUi(this);

  //adjust interface
  m_layout = new QGridLayout(m_ui->m_scrollArea);

  m_ui->m_previewGroupBox->setVisible(false);

  // Preview
  QGridLayout* l = new QGridLayout(m_ui->m_previewWidget);
  l->setContentsMargins(0,0,0,0);
  l->setAlignment(Qt::AlignTop);
  
  m_preview = new te::qt::widgets::SymbolizerPreviewWidget(QSize(120, 120), te::se::RASTER_SYMBOLIZER, m_ui->m_previewWidget);

  l->addWidget(m_preview);

  // Signals & slots
  connect(m_ui->m_NoneSymbolizerRadioButton, SIGNAL(clicked()), SLOT(onNoneSymbolizerClicked()));
  connect(m_ui->m_lineSymbolizerRadioButton, SIGNAL(clicked()), SLOT(onLineSymbolizerClicked()));
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

  updateUi();
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
  delete m_lsWidget;
  m_lsWidget = 0;

  delete m_psWidget;
  m_psWidget = 0;
}

void te::qt::widgets::ImageOutlineWidget::onNoneSymbolizerClicked()
{
  deleteInterfaces();

  m_io->setSymbolizer(0);

  m_ui->m_previewGroupBox->setVisible(false);
}

void te::qt::widgets::ImageOutlineWidget::onLineSymbolizerClicked()
{
  deleteInterfaces();

  m_lsWidget = new te::qt::widgets::LineSymbolizerWidget(m_ui->m_scrollArea);

  connect(m_lsWidget, SIGNAL(symbolizerChanged()), SLOT(onLineSymbolizerCreated()));

  m_layout->addWidget(m_lsWidget);

  m_io->setSymbolizer(m_lsWidget->getSymbolizer());
  
  m_lsWidget->show();

  m_preview->setSymbolizerType(te::se::LINE_SYMBOLIZER);

  updateUi();
}

void te::qt::widgets::ImageOutlineWidget::onPolygonSymbolizerClicked()
{
  deleteInterfaces();

  m_psWidget = new te::qt::widgets::PolygonSymbolizerWidget(m_ui->m_scrollArea);

  connect(m_psWidget, SIGNAL(symbolizerChanged()), SLOT(onPolygonSymbolizerCreated()));

  m_layout->addWidget(m_psWidget);

  m_io->setSymbolizer(m_psWidget->getSymbolizer());
  
  m_psWidget->show();

  m_preview->setSymbolizerType(te::se::POLYGON_SYMBOLIZER);

  updateUi();
}

void te::qt::widgets::ImageOutlineWidget::onLineSymbolizerCreated()
{
  m_io->setSymbolizer(m_lsWidget->getSymbolizer());

  updateUi();
}

void te::qt::widgets::ImageOutlineWidget::onPolygonSymbolizerCreated()
{
  m_io->setSymbolizer(m_psWidget->getSymbolizer());

  updateUi();
}

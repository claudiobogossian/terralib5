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
  \file terralib/qt/widgets/raster/MultiResolutionDialog.cpp

  \brief A dialog to create multi resolution over a raster.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../Exception.h"
#include "MultiResolutionDialog.h"
#include "MultiResolutionWidget.h"
#include "ui_MultiResolutionDialogForm.h"
#include "ui_MultiResolutionWidgetForm.h"

// Qt
#include <QMessageBox>
#include <QGridLayout>

te::qt::widgets::MultiResolutionDialog::MultiResolutionDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::MultiResolutionDialogForm)
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("raster-multiresolution-hint").pixmap(112,48));

//add widget
  m_widget.reset(new MultiResolutionWidget(m_ui->m_widget, f));

  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->addWidget(m_widget.get());
  layout->setContentsMargins(0,0,0,0);
  m_ui->m_widget->setLayout(layout);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  
  m_ui->m_helpPushButton->setPageReference("widgets/raster/raster_multiresolution.html");
}

te::qt::widgets::MultiResolutionDialog::~MultiResolutionDialog()
{
}

te::qt::widgets::MultiResolutionWidget* te::qt::widgets::MultiResolutionDialog::getWidget() const
{
  return m_widget.get();
}

void te::qt::widgets::MultiResolutionDialog::setLayerList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_widget->setLayerList(layerList);
}

void te::qt::widgets::MultiResolutionDialog::onOkPushButtonClicked()
{
  bool res = m_widget->createMultiResolution();

  if(!res)
    return;

  accept();
}

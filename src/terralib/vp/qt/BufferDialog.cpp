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
  \file terralib/vp/BufferDialog.cpp

  \brief A dialog for buffer operation
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"
#include "../core/Exception.h"
#include "BufferDialog.h"
#include "ui_BufferDialogForm.h"
#include "VectorProcessingConfig.h"

// Qt
#include <QtCore/QList>
#include <QtCore/QSize.h>
#include <QtGui/QFileDialog>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMessageBox>

te::vp::BufferDialog::BufferDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::BufferDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_selectedLayer(0)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme(VP_IMAGES"/vp-buffer-hint").pixmap(112,48));
  
  QSize iconSize(96, 48);

  m_ui->m_withoutBoundRadioButton->setIconSize(iconSize);
  m_ui->m_withoutBoundRadioButton->setIcon(QIcon::fromTheme("buffer-without-boundaries"));

  m_ui->m_withBoundRadioButton->setIconSize(iconSize);
  m_ui->m_withBoundRadioButton->setIcon(QIcon::fromTheme("buffer-with-boundaries"));

  m_ui->m_ruleImgLabel->setPixmap(QIcon::fromTheme("buffer-inside-outside").pixmap(150,60));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));  

//signals
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::vp::BufferDialog::~BufferDialog()
{
}

void te::vp::BufferDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {  
    m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

void te::vp::BufferDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::vp::BufferDialog::onOkPushButtonClicked()
{
  QMessageBox::information(this, "Ok", "Under development");
}

void te::vp::BufferDialog::onCancelPushButtonClicked()
{
  reject();
}



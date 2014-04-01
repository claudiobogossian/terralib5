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
  \file terralib/vp/BasicGeographicOperationDialog.cpp

  \brief A dialog with general geographic operation
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../Config.h"
#include "../Exception.h"
#include "BasicGeographicOperationDialog.h"
#include "ui_BasicGeographicOperationDialogForm.h"

// Qt
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QSize>

te::vp::BasicGeographicOperationDialog::BasicGeographicOperationDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::BasicGeographicOperationDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_selectedLayer(0)
{
// add controls
  m_ui->setupUi(this);

// add icons

  QSize iconSize(32, 32);

  m_ui->m_convexHullCheckBox->setIconSize(iconSize);
  m_ui->m_convexHullCheckBox->setIcon(QIcon::fromTheme("vp-convex-hull-hint"));

  m_ui->m_centroidCheckBox->setIconSize(iconSize);
  m_ui->m_centroidCheckBox->setIcon(QIcon::fromTheme("vp-centroid-hint"));

  m_ui->m_areaCheckBox->setIconSize(iconSize);
  m_ui->m_areaCheckBox->setIcon(QIcon::fromTheme("vp-area-hint"));

  m_ui->m_lineLengthCheckBox->setIconSize(iconSize);
  m_ui->m_lineLengthCheckBox->setIcon(QIcon::fromTheme("vp-line-length-hint"));

  m_ui->m_perimeterPolygonsCheckBox->setIconSize(iconSize);
  m_ui->m_perimeterPolygonsCheckBox->setIcon(QIcon::fromTheme("vp-perimeter-hint"));

  m_ui->m_mbrCheckBox->setIconSize(iconSize);
  m_ui->m_mbrCheckBox->setIcon(QIcon::fromTheme("vp-mbr-hint"));

//signals
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::vp::BasicGeographicOperationDialog::~BasicGeographicOperationDialog()
{
}

void te::vp::BasicGeographicOperationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {  
    m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

void te::vp::BasicGeographicOperationDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::vp::BasicGeographicOperationDialog::onOkPushButtonClicked()
{
  QMessageBox::information(this, "Ok", "Under development");
}

void te::vp::BasicGeographicOperationDialog::onCancelPushButtonClicked()
{
  reject();
}



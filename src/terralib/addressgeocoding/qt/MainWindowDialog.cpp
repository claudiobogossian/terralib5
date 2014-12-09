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
  \file terralib/addressgeocoding/MainWindowDialog.cpp

  \brief A dialog for Address Geocoding operation
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/af/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../Config.h"
#include "../Exception.h"
#include "MainWindowDialog.h"
#include "ConfigInputAddressDialog.h"
#include "ConfigInputLayerDialog.h"
#include "ui_MainWindowDialogForm.h"

// Qt
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTreeWidget>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::addressgeocoding::MainWindowDialog::MainWindowDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::MainWindowDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
		m_selectedLayer(0),
    m_toFile(false)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("addressgeocoding-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

//  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
//  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

	connect(m_ui->m_configureLayerPushButton, SIGNAL(clicked()), this, SLOT(onConfigureLayerPressed()));
	connect(m_ui->m_configureAddressPushButton, SIGNAL(clicked()), this, SLOT(onConfigureAddressPressed()));

	connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

//  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
 // m_ui->m_helpPushButton->setPageReference("plugins/addressgeocoding/addressgeocoding.html");

//  m_outputDatasource = te::da::DataSourceInfoPtr();
 // m_ui->m_newLayerNameLineEdit->setEnabled(true);
}

te::addressgeocoding::MainWindowDialog::~MainWindowDialog()
{
}

void te::addressgeocoding::MainWindowDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
}

te::map::AbstractLayerPtr te::addressgeocoding::MainWindowDialog::getLayer()
{
  return m_layer;
}


void te::addressgeocoding::MainWindowDialog::onConfigureLayerPressed()
{
	te::addressgeocoding::ConfigInputLayerDialog dlg(this);
	dlg.setLayers(m_layers);

	if(dlg.exec() != QDialog::Accepted)
  return;
}

void te::addressgeocoding::MainWindowDialog::onConfigureAddressPressed()
{
	te::addressgeocoding::ConfigInputAddressDialog dlg(this);
	
	if(dlg.exec() != QDialog::Accepted)
  return;
}

void te::addressgeocoding::MainWindowDialog::onTargetDatasourceToolButtonPressed()
{
  //m_ui->m_newLayerNameLineEdit->clear();
  //m_ui->m_newLayerNameLineEdit->setEnabled(true);
  //te::qt::widgets::DataSourceSelectorDialog dlg(this);
  //dlg.exec();

  //std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  //if(dsPtrList.size() <= 0)
  //  return;

  //std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  //m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  //m_outputDatasource = *it;
  //
  //m_toFile = false;
}

void te::addressgeocoding::MainWindowDialog::onTargetFileToolButtonPressed()
{
  //m_ui->m_newLayerNameLineEdit->clear();
  //m_ui->m_repositoryLineEdit->clear();
  //
  //QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
  //                                                      QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);

  //if (fileName.isEmpty())
  //  return;
  //
  //boost::filesystem::path outfile(fileName.toStdString());
  //std::string aux = outfile.leaf().string();
  //m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  //aux = outfile.string();
  //m_ui->m_repositoryLineEdit->setText(aux.c_str());
  //
  //m_toFile = true;
  //m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::addressgeocoding::MainWindowDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::addressgeocoding::MainWindowDialog::onOkPushButtonClicked()
{
  
}

void te::addressgeocoding::MainWindowDialog::onCancelPushButtonClicked()
{
  reject();
}


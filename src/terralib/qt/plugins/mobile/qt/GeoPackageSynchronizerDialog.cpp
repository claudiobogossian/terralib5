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
\file src/terraMobilePlugin/qt/GeoPackageSynchronizerDialog.cpp

\brief This interface is used to get the input parameters for GeoPackage Synchronizer operation.
*/

#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../core/GeopackageSynchronizer.h"
#include "GeoPackageSynchronizerDialog.h"
#include "ui_GeoPackageSynchronizerDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::plugins::terramobile::GeoPackageSynchronizerDialog::GeoPackageSynchronizerDialog(QWidget* parent, Qt::WindowFlags f) 
  : QDialog(parent, f),
  m_ui(new Ui::GeoPackageSynchronizerDialogForm)
{
  // add controls
  m_ui->setupUi(this);

  //connects
  connect(m_ui->m_geopackageToolButton, SIGNAL(pressed()), this, SLOT(onGeopackageToolButtonClicked()));
  connect(m_ui->m_synchPushButton, SIGNAL(clicked()), this, SLOT(onSynchronizePushButtonClicked()));

}


te::qt::plugins::terramobile::GeoPackageSynchronizerDialog::~GeoPackageSynchronizerDialog()
{

}


void te::qt::plugins::terramobile::GeoPackageSynchronizerDialog::onGeopackageToolButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open GeoPackage File"), "", tr("GeoPackage File (*.gpkg *.GPKG)"));

  if (fileName.isEmpty())
  {
    return;
  }

  m_ui->m_geopackageLineEdit->setText(fileName);

  //list gpkg datasets
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = fileName.toStdString();

  std::auto_ptr<te::da::DataSource> dsGPKG = te::da::DataSourceFactory::make("GPKG");
  dsGPKG->setConnectionInfo(connInfo);
  dsGPKG->open();

  std::vector<std::string> dsNames = dsGPKG->getDataSetNames();

  m_ui->m_gatheringComboBox->clear();

  for (std::size_t t = 0; t < dsNames.size(); ++t)
  {
    m_ui->m_gatheringComboBox->addItem(dsNames[t].c_str());
  }
}

void te::qt::plugins::terramobile::GeoPackageSynchronizerDialog::onSynchronizePushButtonClicked()
{
  //input ds
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = m_ui->m_geopackageLineEdit->text().toStdString();

  std::auto_ptr<te::da::DataSource> dsGPKG = te::da::DataSourceFactory::make("GPKG");
  dsGPKG->setConnectionInfo(connInfo);
  dsGPKG->open();

  //input dataset
  std::string inputDs = m_ui->m_gatheringComboBox->currentText().toStdString();

  //output ds
  QVariant varLayer = m_ui->m_layerComboBox->currentData(Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

  std::string outputDataSet = dsType->getTitle();

  te::da::DataSourcePtr outDs = te::da::GetDataSource(l->getDataSourceId());

  te::qt::plugins::terramobile::GeoPackageSynchronizer gpSync;

  gpSync.setInputParameters(dsGPKG.get(), inputDs, outDs.get(), outputDataSet);

  try
  {
    gpSync.synchronize();
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Internal Error."));
  }

  QMessageBox::information(this, tr("Information"), tr("Synchronizer Done."));
}

void te::qt::plugins::terramobile::GeoPackageSynchronizerDialog::setLayerList(std::list<te::map::AbstractLayerPtr> list)
{
  m_ui->m_layerComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = list.begin();

  while (it != list.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if (dsType->hasGeom())
      m_ui->m_layerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}
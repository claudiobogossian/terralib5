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
\file src/terraMobilePlugin/qt/GeoPackagePublisherDialog.cpp

\brief This interface is used to get the input parameters for GeoPackage Publisher operation.
*/

// TerraLib
#include "../../../../common/progress/ProgressManager.h"
#include "../../../../common/progress/TaskProgress.h"
#include "../../../../common/STLUtils.h"
#include "GeoPackagePublisherDialog.h"
#include "ui_GeoPackagePublisherDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>


Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::plugins::terramobile::GeoPackagePublisherDialog::GeoPackagePublisherDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::GeoPackagePublisherDialogForm)
{
  // add controls
  m_ui->setupUi(this);

  //connects
  connect(m_ui->m_importSearchPushButton, SIGNAL(clicked()), this, SLOT(onImportSearchPushButtonClicked()));
  connect(m_ui->m_importPushButton, SIGNAL(clicked()), this, SLOT(onImportPushButtonClicked()));
  connect(m_ui->m_exportFilePushButton, SIGNAL(clicked()), this, SLOT(onExportFilePushButtonClicked()));
  connect(m_ui->m_exportPushButton, SIGNAL(clicked()), this, SLOT(onExportPushButtonClicked()));
}

te::qt::plugins::terramobile::GeoPackagePublisherDialog::~GeoPackagePublisherDialog()
{

}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onImportSearchPushButtonClicked()
{
  if (m_ui->m_importURLLineEdit->text().isEmpty())
  {
    m_ui->m_importTableWidget->setRowCount(0);

    QMessageBox::warning(this, tr("Warning"), tr("Server not defined."));
    return;
  }

  //check server

  //fill list
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onImportPushButtonClicked()
{
  if (m_ui->m_importTableWidget->selectedItems().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Select at least one geopackage file to import."));
    return;
  }

  //import
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onExportFilePushButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open GeoPackage File"), "", tr("GeoPackage File (*.gpkg *.GPKG)"));

  if (fileName.isEmpty())
    m_ui->m_exportFileLineEdit->clear();
  else
    m_ui->m_exportFileLineEdit->setText(fileName);
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onExportPushButtonClicked()
{
  if (m_ui->m_exportFileLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Geopackage File not selected."));
    return;
  }

  if (m_ui->m_exportServerLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Server not defined."));
    return;
  }

  //check server

  //export gpkg
}

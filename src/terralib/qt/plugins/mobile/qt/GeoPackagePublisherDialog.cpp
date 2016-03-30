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
#include "../../../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../../../qt/widgets/utils/ScopedCursor.h"
#include "../core/GeopackagePublisher.h"
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
  connect(m_ui->m_importOutputPushButton, SIGNAL(clicked()), this, SLOT(onImportOutputPushButtonClicked()));
  connect(m_ui->m_importPushButton, SIGNAL(clicked()), this, SLOT(onImportPushButtonClicked()));
  connect(m_ui->m_exportFilePushButton, SIGNAL(clicked()), this, SLOT(onExportFilePushButtonClicked()));
  connect(m_ui->m_exportPushButton, SIGNAL(clicked()), this, SLOT(onExportPushButtonClicked()));

  m_task = 0;
}

te::qt::plugins::terramobile::GeoPackagePublisherDialog::~GeoPackagePublisherDialog()
{
  delete m_task;
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onImportSearchPushButtonClicked()
{
  if (m_ui->m_importURLLineEdit->text().isEmpty())
  {
    m_ui->m_importTableWidget->setRowCount(0);

    QMessageBox::warning(this, tr("Warning"), tr("Server not defined."));
    return;
  }

  m_ui->m_importTableWidget->setRowCount(0);

  te::qt::widgets::ScopedCursor c(Qt::WaitCursor);

  //fill list
  GeopackagePublisher gpkgPub;

  GeopackageFiles gpkgFiles = gpkgPub.getGeopackageFiles(m_ui->m_importURLLineEdit->text().toStdString());

  for (std::size_t t = 0; t < gpkgFiles.size(); ++t)
  {
    int row = m_ui->m_importTableWidget->rowCount();
    m_ui->m_importTableWidget->insertRow(row);

    GeopackageFile gpkgFile = gpkgFiles[t];

    QTableWidgetItem* nameItem = new QTableWidgetItem(gpkgFile.m_name.c_str());
    nameItem->setFlags(Qt::ItemIsSelectable);
    m_ui->m_importTableWidget->setItem(row, 0, nameItem);

    QTableWidgetItem* objIdItem = new QTableWidgetItem(gpkgFile.m_objId.c_str());
    objIdItem->setFlags(Qt::ItemIsSelectable);
    m_ui->m_importTableWidget->setItem(row, 1, objIdItem);

    QTableWidgetItem* descItem = new QTableWidgetItem(gpkgFile.m_desc.c_str());
    descItem->setFlags(Qt::ItemIsSelectable);
    m_ui->m_importTableWidget->setItem(row, 2, descItem);
  }

  m_ui->m_importTableWidget->resizeColumnsToContents();

#if (QT_VERSION >= 0x050000)
  m_ui->m_importTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  m_ui->m_importTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onImportOutputPushButtonClicked()
{
  QString dirName = QFileDialog::getExistingDirectory(this, tr("Output GeoPackage File location"));

  if (dirName.isEmpty())
    m_ui->m_importOutputLineEdit->clear();
  else
  {
    QString path = dirName.replace(QRegExp("\\\\"), "/");

    m_ui->m_importOutputLineEdit->setText(path);
  }
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onImportPushButtonClicked()
{
  if (m_ui->m_importOutputLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Output directory not defined."));
    return;
  }

  std::string dir = m_ui->m_importOutputLineEdit->text().toStdString();

  QItemSelectionModel* select = m_ui->m_importTableWidget->selectionModel();

  if (select->selectedRows().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Select at least one geopackage file to import."));
    return;
  }

  std::string url = m_ui->m_importURLLineEdit->text().toStdString();

  te::qt::widgets::ScopedCursor c(Qt::WaitCursor);

  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  //import
  GeopackagePublisher gpkgPub;

  connect(&gpkgPub, SIGNAL(setCurrentStep(double, double, std::string)), this, SLOT(onSetCurrentStep(double, double, std::string)));

  for (int i = 0; i < m_ui->m_importTableWidget->rowCount(); ++i)
  {
    if (m_ui->m_importTableWidget->item(i, 0)->isSelected())
    {
      GeopackageFile gpkgFile;

      gpkgFile.m_name = m_ui->m_importTableWidget->item(i, 0)->text().toStdString();
      gpkgFile.m_objId = m_ui->m_importTableWidget->item(i, 1)->text().toStdString();
      gpkgFile.m_desc = m_ui->m_importTableWidget->item(i, 2)->text().toStdString();

      try
      {
        gpkgPub.downloadGeopackageFile(url, gpkgFile, dir);
      }
      catch (std::exception const& e)
      {
        QMessageBox::warning(this, tr("Warning"), e.what());
        return;
      }
      catch (...)
      {
        QMessageBox::warning(this, tr("Warning"), gpkgPub.getErrorMessage().c_str());
        return;
      }
    }
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QMessageBox::information(this, tr("Information"), tr("Downloaded Successfully"));
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

  std::string pathFile = m_ui->m_exportFileLineEdit->text().toStdString();

  if (m_ui->m_exportServerLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Server not defined."));
    return;
  }

  std::string url = m_ui->m_exportServerLineEdit->text().toStdString();


  QFileInfo file(m_ui->m_exportFileLineEdit->text());

  te::qt::widgets::ScopedCursor c(Qt::WaitCursor);

  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  //export gpkg
  GeopackagePublisher gpkgPub;

  connect(&gpkgPub, SIGNAL(setCurrentStep(double, double, std::string)), this, SLOT(onSetCurrentStep(double, double, std::string)));

  try
  {
    gpkgPub.uploadGeopackageFile(url, pathFile, file.fileName().toStdString());
  }
  catch (std::exception const& e)
  {
    QMessageBox::warning(this, tr("Warning"), e.what());
    return;
  }
  catch (...)
  {
    QMessageBox::warning(this, tr("Warning"), gpkgPub.getErrorMessage().c_str());
    return;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QMessageBox::information(this, tr("Information"), tr("Uploaded Successfully"));
}

void te::qt::plugins::terramobile::GeoPackagePublisherDialog::onSetCurrentStep(double curStep, double totalStep, std::string msg)
{
  if (curStep == 0.)
    return;

  if (!m_task)
  {
    m_task = new te::common::TaskProgress(msg);
    m_task->setTotalSteps(100);
  }

  int nStep = (100. * curStep) / totalStep;

  m_task->setCurrentStep(nStep);

  if (curStep == totalStep)
  {
    delete m_task;
    m_task = 0;
  }
}



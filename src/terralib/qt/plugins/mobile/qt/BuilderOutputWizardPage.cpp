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
\file src/terraMobilePlugin/qt/BuilderOutputWizardPage.cpp

\brief This interface is used to get the output parameters for GeoPackage Builder wizard operation.
*/

// TerraLib
#include "BuilderOutputWizardPage.h"
#include "ui_BuilderOutputWizardPageForm.h"

// Qt
#include <QFileDialog>

te::qt::plugins::terramobile::BuilderOutputWizardPage::BuilderOutputWizardPage(QWidget* parent)
: QWizardPage(parent),
m_ui(new Ui::BuilderOutputWizardPageForm)
{
  // setup controls
  m_ui->setupUi(this);

  //configure page
  this->setTitle(tr("GeoPackage Builder Output Parameters"));
  this->setSubTitle(tr("Used to set the output parameters to build the geopackage file."));

  //connect slots
  connect(m_ui->m_dirToolButton, SIGNAL(pressed()), this, SLOT(onDirToolButtonPressed()));
}

te::qt::plugins::terramobile::BuilderOutputWizardPage::~BuilderOutputWizardPage()
{

}

bool te::qt::plugins::terramobile::BuilderOutputWizardPage::isComplete() const
{
  if (m_ui->m_geopackageFileNameLineEdit->text().isEmpty())
    return false;

  return true;
}

std::string te::qt::plugins::terramobile::BuilderOutputWizardPage::getGeoPackageFilePath()
{
  if (!m_ui->m_geopackageFileNameLineEdit->text().isEmpty())
    return m_ui->m_geopackageFileNameLineEdit->text().toStdString();

  return "";
}

bool te::qt::plugins::terramobile::BuilderOutputWizardPage::useVisibleArea()
{
  return m_ui->m_visibleAreaCheckBox->isChecked();
}

void te::qt::plugins::terramobile::BuilderOutputWizardPage::appendLogMesssage(std::string msg)
{
  m_ui->m_logPlainTextEdit->appendPlainText(QString::fromStdString(msg));
  this->repaint();
}

void te::qt::plugins::terramobile::BuilderOutputWizardPage::onDirToolButtonPressed()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("Geopackage (*.gpkg *.GPKG);;"), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  m_ui->m_geopackageFileNameLineEdit->setText(fileName);

  emit completeChanged();
}

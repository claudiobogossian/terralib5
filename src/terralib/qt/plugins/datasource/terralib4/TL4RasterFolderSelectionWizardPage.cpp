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
  \file terralib/qt/plugins/terralib4/TL4RasterFolderSelectionWizardPage.cpp

  \brief A Qt dialog for ....
*/

// TerraLib
#include "../../../../qt/widgets/utils/FileChooser.h"
#include "ui_TL4RasterFolderSelectionWizardPageForm.h"
#include "TL4RasterFolderSelectionWizardPage.h"

//Qt
#include <QFileDialog>
#include <QLineEdit>

te::qt::plugins::terralib4::TL4RasterFolderSelectionWizardPage::TL4RasterFolderSelectionWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::TL4RasterFolderSelectionWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);
  m_ui->m_chooseToolButton->setIcon(QIcon::fromTheme("folder"));

  connect(m_ui->m_chooseToolButton, SIGNAL(clicked()), this, SLOT(onChooseToolButtonClicked()));
}

te::qt::plugins::terralib4::TL4RasterFolderSelectionWizardPage::~TL4RasterFolderSelectionWizardPage()
{
}

std::string te::qt::plugins::terralib4::TL4RasterFolderSelectionWizardPage::getPath()
{
  return m_ui->m_pathLineEdit->text().toStdString();
}

void te::qt::plugins::terralib4::TL4RasterFolderSelectionWizardPage::onChooseToolButtonClicked()
{
  QString dir;
  dir = QFileDialog::getExistingDirectory(this, tr("Choose directory"));

  m_ui->m_pathLineEdit->setText(dir);
}
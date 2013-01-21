/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A Free and Open Source GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*!
  \file terraview/AboutDialog.cpp

  \brief A Qt dialog showing TerraView about window.
*/

// TerraView
#include "ui_AboutDialogForm.h"
#include "AboutDialog.h"

// TerraLib
//#include <terralib/common/SystemApplicationSettings.h>

// Qt
#include <QtGui/QPixmap>

AboutDialog::AboutDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::AboutDialogForm)
{
  m_ui->setupUi(this);

  {
    std::string logoFileName = "C:/teste.teste";
   // te::common::SystemApplicationSettings::getInstance().getValue("Application.AboutDialogLogo.<xmlattr>.xlink:href");

    QPixmap pixmap(logoFileName.c_str());

    m_ui->m_applicationLogo->setPixmap(pixmap);
  }

  {
    std::string logoFileName = "C:/teste.teste";
   // te::common::SystemApplicationSettings::getInstance().getValue("Application.TerraLibLogo.<xmlattr>.xlink:href");

    QPixmap pixmap(logoFileName.c_str());

    m_ui->m_terralibLogo->setPixmap(pixmap);
  }
}

AboutDialog::~AboutDialog()
{
}

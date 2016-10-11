/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
\file terralib/qt/af/InternalSettingsDialog.cpp

\brief A Dialog to be used to inform about internal settings.
*/

#include "InternalSettingsDialog.h"
#include "ui_InternalSettingsDialogForm.h"

// terralib includes
#include "../../core/filesystem/FileSystem.h"
#include "../../common/PlatformUtils.h"
#include "../../common/TerraLib.h"
#include "../../plugin/PluginManager.h"
#include "../../BuildConfig.h"

// Qt includes
#include <QDesktopWidget>

#if (QT_VERSION >= 0x050000)
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
// Boost includes
#include <boost/filesystem.hpp>


te::qt::af::InternalSettingsDialog::InternalSettingsDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::InternalSettingsDialogForm)
{
  //build form
  m_ui->setupUi(this);

  //set path informations
  setPaths();

  //set te modules information
  setModules();

  //set te plugins information
  setPlugins();

  //set system information
  setSystemInfo();
}


te::qt::af::InternalSettingsDialog::~InternalSettingsDialog()
{

}

void te::qt::af::InternalSettingsDialog::setPaths()
{
  //global
  QString userDataDir;

#if (QT_VERSION >= 0x050000)
  userDataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
  userDataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif

  m_ui->m_userPathLineEdit->setText(userDataDir);

  m_ui->m_gdalDirLineEdit->setText(TERRALIB_GDAL_DATA);

  // terralib home
  boost::filesystem::path curPath = te::core::FileSystem::currentPath();
  m_ui->m_curPathLineEdit->setText(curPath.string().c_str());

  curPath /= "..";
  m_ui->m_previousCurPathLineEdit->setText(curPath.string().c_str());


  std::string terraDir = "";
  const char* teHomeEnv = getenv(TERRALIB_DIR_VAR_NAME);
  if (teHomeEnv != 0)
    terraDir = getenv(TERRALIB_DIR_VAR_NAME);
  m_ui->m_terraLibHomeLineEdit->setText(terraDir.c_str());

  m_ui->m_installPrefixLineEdit->setText(TERRALIB_INSTALL_PREFIX_PATH);

  m_ui->m_codebasePathLineEdit->setText(TERRALIB_CODEBASE_PATH);
}

void te::qt::af::InternalSettingsDialog::setModules()
{
  m_ui->m_modulesListWidget->clear();

  std::size_t nModules = TerraLib::getInstance().getNumRegModules();

  for (std::size_t t = 0; t < nModules; ++t)
  {
    std::string name = TerraLib::getInstance().getModuleName(t);

    m_ui->m_modulesListWidget->addItem(name.c_str());
  }
}

void te::qt::af::InternalSettingsDialog::setPlugins()
{
  m_ui->m_pluginsListWidget->clear();

  std::vector<std::string> plugins = te::plugin::PluginManager::getInstance().getPlugins();

  for (std::size_t t = 0; t < plugins.size(); ++t)
  {
    std::string name = plugins[t];

    m_ui->m_pluginsListWidget->addItem(name.c_str());
  }
}

void te::qt::af::InternalSettingsDialog::setSystemInfo()
{
  m_ui->m_dpiXLineEdit->setText(QString::number(qApp->desktop()->logicalDpiX()));

  m_ui->m_dpiYLineEdit->setText(QString::number(qApp->desktop()->logicalDpiY()));

  m_ui->m_freePhysMemLineEdit->setText(QString("%L1").arg(te::common::GetFreePhysicalMemory()));

  m_ui->m_totalPhysMemLineEdit->setText(QString("%L1").arg(te::common::GetTotalPhysicalMemory()));

  m_ui->m_usedVirtualMemLineEdit->setText(QString("%L1").arg(te::common::GetUsedVirtualMemory()));

  m_ui->m_totalVirtualMemLineEdit->setText(QString("%L1").arg(te::common::GetTotalVirtualMemory()));

  m_ui->m_physProcNumberLineEdit->setText(QString::number(te::common::GetPhysProcNumber()));
}

/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/plugin/builder/PluginBuilderWizard.cpp

  \brief A Qt dialog that allows users to create new plugins based on TerraLib framework.
*/

// TerraLib
#include "../../../../common/StringUtils.h"
#include "../../../../common/Version.h"
#include "../../../../plugin/PluginManager.h"
#include "../../../../plugin/PluginInfo.h"
#include "../../../../plugin/Provider.h"
#include "../../utils/DoubleListWidget.h"
#include "../../utils/ParameterTableWidget.h"
#include "PluginBuilderWizard.h"
#include "PluginCMakeWriter.h"
#include "PluginSourceWriter.h"
#include "ui_DoubleListWidgetForm.h"
#include "ui_ParameterTableWidgetForm.h"
#include "ui_PluginBuilderWizardForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QVBoxLayout>

te::qt::widgets::PluginBuilderWizard::PluginBuilderWizard(QWidget* parent)
  : QWizard(parent),
    m_ui(new Ui::PluginBuilderWizardForm),
    m_curDir("")
{
  m_ui->setupUi(this);

  //terralib modules
  std::vector<std::string> teModulesNeeded;
  teModulesNeeded.push_back("te.common");               //MANDATORY
  teModulesNeeded.push_back("te.plugin");               //MANDATORY

  std::vector<std::string> teModules;
  teModules.push_back("te.annotationtext");
  teModules.push_back("te.color");
  teModules.push_back("te.dataaccess");
  teModules.push_back("te.datatype");
  teModules.push_back("te.filter_encoding");
  teModules.push_back("te.da.gdal");
  teModules.push_back("te.geometry");
  teModules.push_back("te.gml");
  teModules.push_back("te.maptools");
  teModules.push_back("te.da.memory");
  teModules.push_back("te.qt.af");
  teModules.push_back("te.raster");
  teModules.push_back("te.rasterprocessing");
  teModules.push_back("te.srs");
  teModules.push_back("te.symbology_encoding");
  teModules.push_back("te.xlinks");
  teModules.push_back("te.xml");

  //license types
  QStringList licenses;
  licenses.push_back("Berkeley Database License");
  licenses.push_back("Boost Software License");
  licenses.push_back("BSD Documentation License");
  licenses.push_back("Eclipse Distribution License");
  licenses.push_back("GPL / GNU General Public License");
  licenses.push_back("Intel Open Source License");
  licenses.push_back("LGPL / GNU Lesser General Public License");
  licenses.push_back("License of Python");
  licenses.push_back("MIT license");
  licenses.push_back("Public Domain");
  licenses.push_back("W3C Software Notice and License");

  //category tpyes
  QStringList categList;
  categList.push_back("Cellular Space");
  categList.push_back("Data Access");
  categList.push_back("Data Management");
  categList.push_back("Examples");
  categList.push_back("Language Bindings");
  categList.push_back("Location Base Services");
  categList.push_back("Plugin Managememt");
  categList.push_back("Spatial Analysis");
  categList.push_back("Spatial Operations");
  categList.push_back("Web Services");

  //fill interface
  m_ui->m_terralibVersionLineEdit->setText(te::common::Version::asString().c_str());
  m_ui->m_pluginCategoryComboBox->addItems(categList);
  m_ui->m_pluginLicenseComboBox->addItems(licenses);

  setButtonText(QWizard::CustomButton1, tr("&Settings"));

  // plugin dependencies
  m_pluginDependencies.reset(new DoubleListWidget(m_ui->m_pluginDependenciesPage));
  m_pluginDependencies->getForm()->m_leftItemsLabel->setText(tr("Available Plugins"));
  m_pluginDependencies->getForm()->m_rightItemsLabel->setText(tr("Required Plugins"));
  m_ui->m_pluginDependenciesGridLayout->addWidget(m_pluginDependencies.get());

  std::vector<std::string> plugins;
  te::plugin::PluginManager::getInstance().getPlugins(plugins);
  m_pluginDependencies->setInputValues(plugins);

// category dependencies
  m_categoryDependencies.reset(new DoubleListWidget(m_ui->m_categoryDependenciesPage));
  m_categoryDependencies->getForm()->m_leftItemsLabel->setText(tr("Available plugin categories"));
  m_categoryDependencies->getForm()->m_rightItemsLabel->setText(tr("Required categories for your plugin"));
  m_ui->m_categoryDependenciesGridLayout->addWidget(m_categoryDependencies.get());

  std::vector<std::string> categories;
  te::plugin::PluginManager::getInstance().getCategories(categories);
  m_categoryDependencies->setInputValues(categories);

// module dependencies
  m_moduleDependencies.reset(new DoubleListWidget(m_ui->m_moduleDependenciesPage));
  m_moduleDependencies->getForm()->m_leftItemsLabel->setText(tr("Available modules"));
  m_moduleDependencies->getForm()->m_rightItemsLabel->setText(tr("Required modules for your plugin"));
  m_ui->m_moduleDependenciesGridLayout->addWidget(m_moduleDependencies.get());

  m_moduleDependencies->setInputValues(teModules);
  m_moduleDependencies->setOutputValues(teModulesNeeded);

// plugin resources
  m_pluginResources.reset(new ParameterTableWidget(m_ui->m_resourcesPage));
  m_pluginResources->getForm()->m_parameterTitle->setText(tr("Resources"));
  m_ui->m_resourcesGridLayout->addWidget(m_pluginResources.get());

// plugin parameters
  m_pluginParameters.reset(new ParameterTableWidget(m_ui->m_parametersPage));
  m_ui->m_parametersGridLayout->addWidget(m_pluginParameters.get());

// connect
  connect(m_ui->m_terraLibIncludeDirToolButton, SIGNAL(clicked()), this, SLOT(onTeIncludeDirButtonClicked()));
  connect(m_ui->m_terraLibCmakeDirToolButton, SIGNAL(clicked()), this, SLOT(onTeCmakeDirButtonClicked()));
  connect(m_ui->m_sourceCodeLocationToolButton, SIGNAL(clicked()), this, SLOT(onPluginSrcDirButtonClicked()));
  connect(m_ui->m_buildLocationToolButton, SIGNAL(clicked()), this, SLOT(onPluginBuildDirButtonClicked()));
}

te::qt::widgets::PluginBuilderWizard::~PluginBuilderWizard()
{
}

bool te::qt::widgets::PluginBuilderWizard::validateCurrentPage()
{
  if(currentPage() == m_ui->m_basicPluginInfoPage)
  {
    return pluginInfoPageCheck();
  }
  else if(currentPage() == m_ui->m_licenseAndProviderPage)
  {
    return providerPageCheck();
  }
  else if(currentPage() == m_ui->m_pluginTargetLocationPage)
  {
    if(dirPageCheck())
    {
      buildPlugin();
    }
    else
    {
      return false;
    }
  }

  return true;
}

void te::qt::widgets::PluginBuilderWizard::onTeIncludeDirButtonClicked()
{
  m_curDir = QFileDialog::getExistingDirectory(this, tr("Select the Include directory of TerraLib"), m_curDir);

  if(m_curDir.isEmpty() == false)
  {
    m_ui->m_terralibIncludeDirLineEdit->setText(m_curDir);
  }
}

void te::qt::widgets::PluginBuilderWizard::onTeCmakeDirButtonClicked()
{
  m_curDir = QFileDialog::getExistingDirectory(this, tr("Select the Cmake Configure Files directory of TerraLib"), m_curDir);

  if(m_curDir.isEmpty() == false)
  {
    m_ui->m_terralibCmakeDirLineEdit->setText(m_curDir);
  }
}

void te::qt::widgets::PluginBuilderWizard::onPluginSrcDirButtonClicked()
{
  m_curDir = QFileDialog::getExistingDirectory(this, tr("Select the location to create the source code files"), m_curDir);

  if(m_curDir.isEmpty() == false)
  {
    m_ui->m_sourceCodeLocationLineEdit->setText(m_curDir);
  }
}

void te::qt::widgets::PluginBuilderWizard::onPluginBuildDirButtonClicked()
{
  m_curDir = QFileDialog::getExistingDirectory(this, tr("Select the location to create the build files"), m_curDir);

  if(m_curDir.isEmpty() == false)
  {
    m_ui->m_buildLocationLineEdit->setText(m_curDir);
  }
}

bool te::qt::widgets::PluginBuilderWizard::pluginInfoPageCheck()
{
  if(m_ui->m_pluginNameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Plugin Name not defined."));
    return false;
  }
  else
  {
    std::string name = m_ui->m_pluginNameLineEdit->text().toStdString();
    m_pluginResources->add("SharedLibraryName", name);

    //perhaps this information should be generated elsewhere
    int pos = name.rfind("_");
    std::string nameSpace = name.substr(pos + 1, name.size() - 1);
    std::string macroExport = "TEPLUGIN" + te::common::Convert2UCase(nameSpace) + "DLL";

    m_ui->m_cPlusPlusNameSpaceLineEdit->setText(nameSpace.c_str());
    m_ui->m_cPlusPlusMacroExportLineEdit->setText(macroExport.c_str());
  }

  if(m_ui->m_pluginVersionLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Plugin Version not defined."));
    return false;
  }

  if(m_ui->m_pluginDisplayTextLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Plugin Display Text not defined."));
    return false;
  }

  if(m_ui->m_pluginSiteLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Plugin Site not defined."));
    return false;
  }

  if(m_ui->m_pluginDescriptionTextEdit->document()->toPlainText().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Plugin Description not defined."));
    return false;
  }

  return true;
}

bool te::qt::widgets::PluginBuilderWizard::providerPageCheck()
{
  if(m_ui->m_pluginLicenseSiteLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("License Site not defined."));
    return false;
  }

  if(m_ui->m_pluginProviderNameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Provider Name not defined."));
    return false;
  }

  if(m_ui->m_pluginProviderSiteLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Provider Site not defined."));
    return false;
  }

  if(m_ui->m_pluginProviderEmailLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Provider Email not defined."));
    return false;
  }

  return true;
}

bool te::qt::widgets::PluginBuilderWizard::dirPageCheck()
{
  if(m_ui->m_terralibIncludeDirLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("TerraLib Include dir not defined."));
    return false;
  }

  if(m_ui->m_terralibCmakeDirLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("TerraLib Cmake dir not defined."));
    return false;
  }

  if(m_ui->m_sourceCodeLocationLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Plugin Source dir not defined."));
    return false;
  }

  if(m_ui->m_buildLocationLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Plugin Builder"), tr("Plugin Build dir not defined."));
    return false;
  }

  return true;
}

void te::qt::widgets::PluginBuilderWizard::buildPlugin()
{
  te::plugin::PluginInfo pi;
  pi.m_name = m_ui->m_pluginNameLineEdit->text().toStdString();
  pi.m_displayName = m_ui->m_pluginDisplayTextLineEdit->text().toStdString();
  pi.m_description = m_ui->m_pluginDescriptionTextEdit->document()->toPlainText().toStdString();
  pi.m_version = m_ui->m_pluginVersionLineEdit->text().toStdString();
  pi.m_release = m_ui->m_pluginReleaseDateTime->text().toStdString();
  pi.m_engine = m_ui->m_cPlusPlusLanguageRadioButton->text().toStdString();       // SET TO C++  CHANGE THIS IN THE FUTURE
  pi.m_terralibVersion = m_ui->m_terralibVersionLineEdit->text().toStdString();
  pi.m_licenseDescription = m_ui->m_pluginLicenseComboBox->currentText().toStdString();
  pi.m_licenseURL = m_ui->m_pluginLicenseSiteLineEdit->text().toStdString();
  pi.m_category = m_ui->m_pluginCategoryComboBox->currentText().toStdString();
  pi.m_site = m_ui->m_pluginSiteLineEdit->text().toStdString();
  pi.m_folder = m_ui->m_buildLocationLineEdit->text().toStdString();

  pi.m_requiredPlugins = m_pluginDependencies->getOutputValues();
  pi.m_requiredPluginCategories = m_categoryDependencies->getOutputValues();
  pi.m_requiredModules = m_moduleDependencies->getOutputValues();

  std::map<std::string, std::string> pluginResMap = m_pluginResources->getMap();
  std::map<std::string, std::string>::iterator it = pluginResMap.begin();

  while(it != pluginResMap.end())
  {
    te::plugin::PluginInfo::Resource r(it->first, it->second);
    pi.m_resources.push_back(r);
    ++it;
  }

  std::map<std::string, std::string> pluginParamsMap = m_pluginParameters->getMap();
  it = pluginParamsMap.begin();

  while(it != pluginParamsMap.end())
  {
    te::plugin::PluginInfo::Parameter p(it->first, it->second);
    pi.m_parameters.push_back(p);
    ++it;
  }

  //aquire provider info
  te::plugin::Provider p;
  p.m_name = m_ui->m_pluginProviderNameLineEdit->text().toStdString();
  p.m_site = m_ui->m_pluginProviderSiteLineEdit->text().toStdString();
  p.m_email = m_ui->m_pluginProviderEmailLineEdit->text().toStdString();

  //acquire dir informations
  std::string teIncludeDir = m_ui->m_terralibIncludeDirLineEdit->text().replace(QRegExp("\\\\"), "/").toStdString();
  std::string teCmakeDir = m_ui->m_terralibCmakeDirLineEdit->text().replace(QRegExp("\\\\"), "/").toStdString();
  std::string pluginSrcDir = m_ui->m_sourceCodeLocationLineEdit->text().replace(QRegExp("\\\\"), "/").toStdString();
  std::string pluginBuildDir = m_ui->m_buildLocationLineEdit->text().replace(QRegExp("\\\\"), "/").toStdString();
  // \\\\ is used to represente a back slash - Believe... QRegExp - Qt Documentation

  //targuet language information
  std::string nameSpace = ""; 
  std::string macroExport = ""; 

  if(m_ui->m_cPlusPlusLanguageRadioButton->isChecked())
  {
    nameSpace = m_ui->m_cPlusPlusNameSpaceLineEdit->text().toStdString();
    macroExport = m_ui->m_cPlusPlusMacroExportLineEdit->text().toStdString();
  }

  //create cmake files
  te::qt::widgets::PluginCMakeWriter cmakeWriter;
  cmakeWriter.createCmakeFile(pluginBuildDir, pluginSrcDir, pi.m_name, macroExport, teCmakeDir, pi);

  cmakeWriter.createPluginInfoFile(pluginBuildDir, pi, p);

  //create source files
  te::qt::widgets::PluginSourceWriter sourceWriter;
  sourceWriter.createHeaderFile(pluginSrcDir, nameSpace);

  sourceWriter.createCppFile(pluginSrcDir, nameSpace, pi.m_name);

  sourceWriter.createConfigFile(pluginSrcDir, nameSpace, macroExport, pi.m_name);

  QMessageBox::information(this, tr("Plugin Builder"), tr("Plugin built successfully!"));
}


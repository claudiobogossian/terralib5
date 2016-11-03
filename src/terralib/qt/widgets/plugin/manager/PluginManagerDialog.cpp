/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled
   applications.

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
  \file terralib/plugins/manager/PluginManagerDialog.cpp

  \brief A Qt dialog for plugin management.
*/

// TerraLib
#include "PluginManagerDialog.h"
#include "../../../../Exception.h"
#include "../../../../core/plugin/Exception.h"
#include "../../../../core/plugin/PluginManager.h"
#include "../../../../core/plugin/Serializers.h"
#include "../../../../core/plugin/Utils.h"
#include "../../../../plugin/AbstractPlugin.h"
#include "../../../../plugin/PluginInfo.h"
#include "../../../../plugin/PluginManager.h"
#include "../../../../plugin/Utils.h"
#include "../../help/HelpManager.h"
#include "../../utils/CentralizedCheckBoxDelegate.h"
#include "../../utils/ResourceChooser.h"
#include "ui_PluginManagerDialogForm.h"

// Boost
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>

// Qt
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

void GetDependents(const std::string& plugin_name,
                   std::vector<std::string>& dependents)
{
  std::vector<std::string> curDeps =
      te::core::PluginManager::instance().getDependents(plugin_name);

  for(auto dep : curDeps)
  {
    GetDependents(dep, dependents);
  }
  auto it = std::find(dependents.begin(), dependents.end(), plugin_name);
  if(it == dependents.end())
    dependents.push_back(plugin_name);
}

void ShowException(const boost::exception& e)
{
  if(const std::string* d = boost::get_error_info<te::ErrorDescription>(e))
    QMessageBox::warning(0, "Plugin manager", d->c_str());
  else
    QMessageBox::warning(0, "Plugin manager",
                         QObject::tr("Unknown error has occurred"));
}


te::qt::widgets::PluginManagerDialog::PluginManagerDialog(QWidget* parent,
                                                          Qt::WindowFlags f)
    : QDialog(parent, f), m_ui(new Ui::PluginManagerDialogForm)
{
  m_ui->setupUi(this);

  setWindowTitle(tr("Manage Application Plugins"));
  // configure the table
  QStringList labels;
  labels << QObject::tr("Enabled") << QObject::tr("Plugin")
         << QObject::tr("Version") << QObject::tr("License")
         << QObject::tr("Site") << QObject::tr("Provider")
         << QObject::tr("Provider site") << QObject::tr("Provider email")
         << QObject::tr("Name");

  m_ui->m_tableWidget->setColumnCount(labels.size());
  m_ui->m_tableWidget->setHorizontalHeaderLabels(labels);
  m_ui->m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

  // fill the table with all the available plugins
  fillInstalledPlugins();

  // set icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("plugin").pixmap(48, 48));
  m_ui->m_addButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeButton->setIcon(QIcon::fromTheme("list-remove"));

  // configure help button
  m_ui->m_helpPushButton->setPageReference(
      "widgets/pluginmanager/PluginManager.html");

  connect(m_ui->m_addButton, SIGNAL(clicked()), this,
          SLOT(onAddPushButtonClicked()));
  connect(m_ui->m_removeButton, SIGNAL(clicked()), this,
          SLOT(onRemovePushButtonClicked()));
  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), this,
          SLOT(onApplyPushButtonClicked()));
  connect(m_ui->m_closePushButton, SIGNAL(clicked()), this,
          SLOT(onClosePushButtonClicked()));
  connect(m_ui->m_enableAll, SIGNAL(stateChanged(int)), this,
          SLOT(onEnableAllChanged(int)));
}

te::qt::widgets::PluginManagerDialog::~PluginManagerDialog()
{
}

void te::qt::widgets::PluginManagerDialog::addEntry(
    const te::core::PluginInfo& pinfo, PluginStatus status)
{
  int nRow = m_ui->m_tableWidget->rowCount();
  int nCol = m_ui->m_tableWidget->columnCount();

  m_ui->m_tableWidget->insertRow(nRow);

  // creates the checkbox

  QWidget* widget = new QWidget();
  QCheckBox* checkBox = new QCheckBox();
  checkBox->setObjectName(pinfo.name.c_str());
  QHBoxLayout* layout = new QHBoxLayout(widget);
  checkBox->setChecked((status == PluginStatus::loaded) ? true : false);
  if(status == PluginStatus::broken)
    checkBox->setVisible(false);
  layout->addWidget(checkBox);
  layout->setAlignment(Qt::AlignCenter);
  layout->setContentsMargins(0, 0, 0, 0);
  widget->setLayout(layout);
  m_ui->m_tableWidget->setCellWidget(nRow, PluginHeader::enabled, widget);
  connect(checkBox, SIGNAL(stateChanged(int)), this,
          SLOT(onEnabledChanged(int)));
  // populate each cell
  for(int col = 1; col < nCol; col++)
  {
    QTableWidgetItem* item = new QTableWidgetItem();
    switch(col)
    {
      case PluginHeader::display_name:
        item->setText(QString::fromUtf8(pinfo.display_name.c_str()));
        break;
      case PluginHeader::version:
        item->setText(QString::fromUtf8(pinfo.version.c_str()));
        break;
      case PluginHeader::license_description:
        item->setText(QString::fromUtf8(pinfo.license_description.c_str()));
        break;
      case PluginHeader::site:
        item->setText(QString::fromUtf8(pinfo.site.c_str()));
        break;
      case PluginHeader::provider_name:
        item->setText(QString::fromUtf8(pinfo.provider.name.c_str()));
        break;
      case PluginHeader::provider_site:
        item->setText(QString::fromUtf8(pinfo.provider.site.c_str()));
        break;
      case PluginHeader::provider_email:
        item->setText(QString::fromUtf8(pinfo.provider.email.c_str()));
        break;
      case PluginHeader::name:
        item->setText(QString::fromUtf8(pinfo.name.c_str()));
        break;
      default:
        break;
    }
    item->setFlags(Qt::ItemFlags(item->flags() ^ Qt::ItemIsEditable));
    m_ui->m_tableWidget->setItem(nRow, col, item);
  }
  m_ui->m_tableWidget->resizeColumnsToContents();
}

void te::qt::widgets::PluginManagerDialog::changePluginStatus(
    const std::string& plugin_name)
{
  QLayout* layout =
      m_ui->m_tableWidget
          ->cellWidget(getPluginRow(plugin_name), PluginHeader::enabled)
          ->layout();
  QCheckBox* checkBox =
      findChild<QCheckBox*>(layout->itemAt(0)->widget()->objectName());

  checkBox->setCheckState(
      (checkBox->checkState() == Qt::Checked) ? Qt::Unchecked : Qt::Checked);
}

void te::qt::widgets::PluginManagerDialog::disableRow(const int row)
{
  QWidget* widget = m_ui->m_tableWidget->cellWidget(row, 0);
  widget->setEnabled(false);
  int nCol = m_ui->m_tableWidget->columnCount();
  for(int col = 0; col < nCol; ++col)
  {
    if(col == PluginHeader::enabled)
    {
      QWidget* widget = m_ui->m_tableWidget->cellWidget(row, col);
      widget->setEnabled(false);
      continue;
    }
    QTableWidgetItem* item = m_ui->m_tableWidget->item(row, col);
    item->setFlags(Qt::ItemFlags(item->flags() ^ Qt::ItemIsEnabled));
  }
}

void te::qt::widgets::PluginManagerDialog::fillInstalledPlugins()
{
  std::vector<te::core::PluginInfo> loaded =
      te::core::PluginManager::instance().getLoadedPlugins();
  std::vector<te::core::PluginInfo> unloaded =
      te::core::PluginManager::instance().getUnloadedPlugins();
  std::vector<te::core::PluginInfo> broken =
      te::core::PluginManager::instance().getBrokenPlugins();

  for(int i = 0; i < (int)loaded.size(); ++i)
    addEntry(loaded[i], PluginStatus::loaded);

  for(int i = 0; i < (int)unloaded.size(); ++i)
    addEntry(unloaded[i], PluginStatus::unloaded);

  for(int i = 0; i < (int)broken.size(); ++i)
    addEntry(broken[i], PluginStatus::broken);
}

std::string te::qt::widgets::PluginManagerDialog::getPluginName(int row)
{
  QTableWidgetItem* item = m_ui->m_tableWidget->item(
      row, te::qt::widgets::PluginManagerDialog::PluginHeader::name);
  std::string plugin_name = item->text().toUtf8().data();
  return plugin_name;
}

int te::qt::widgets::PluginManagerDialog::getPluginRow(
    const std::string& plugin_name)
{
  int nRow = m_ui->m_tableWidget->rowCount();
  for(int row = 0; row < nRow; ++row)
  {
    QTableWidgetItem* item = m_ui->m_tableWidget->item(
        row, te::qt::widgets::PluginManagerDialog::PluginHeader::name);
    if(item->text().toUtf8().data() == plugin_name)
      return row;
  }
  return -1;
}

te::qt::widgets::PluginManagerDialog::PluginStatus
te::qt::widgets::PluginManagerDialog::getPluginStatus(const int row)
{
  QTableWidgetItem* item = m_ui->m_tableWidget->item(row, PluginHeader::name);
  std::string plugin_name = item->text().toUtf8().data();

  QLayout* layout = m_ui->m_tableWidget->cellWidget(row, 0)->layout();
  QCheckBox* checkBox =
      findChild<QCheckBox*>(layout->itemAt(0)->widget()->objectName());

  if(!checkBox->isEnabled())
    return PluginStatus::to_remove;
  if(!checkBox->isVisible())
    return PluginStatus::broken;
  if(checkBox->isChecked())
  {
    if(te::core::PluginManager::instance().isUnloaded(plugin_name))
      return PluginStatus::to_load;
    return PluginStatus::loaded;
  }
  else
  {
    if(te::core::PluginManager::instance().isLoaded(plugin_name))
      return PluginStatus::to_unload;
    return PluginStatus::unloaded;
  }
}

void te::qt::widgets::PluginManagerDialog::updateBroken()
{
  std::vector<te::core::PluginInfo> v_broken =
      te::core::PluginManager::instance().getBrokenPlugins();
  for(auto broken : v_broken)
  {
    if(te::core::PluginManager::instance().isFixed(broken.name))
    {
      QLayout* layout =
          m_ui->m_tableWidget->cellWidget(getPluginRow(broken.name), 0)
              ->layout();
      QCheckBox* checkBox =
          findChild<QCheckBox*>(layout->itemAt(0)->widget()->objectName());
      checkBox->setVisible(true);
    }
  }
}

void te::qt::widgets::PluginManagerDialog::loadPlugins(
    std::vector<te::core::PluginInfo> v_pInfo)
{
  v_pInfo = te::core::plugin::TopologicalSort(v_pInfo);
  for(const te::core::PluginInfo& pinfo : v_pInfo)
  {
    try
    {
      te::core::PluginManager::instance().load(pinfo.name);
      setChanged(getPluginRow(pinfo.name), false);
    }
    catch(const boost::exception& e)
    {
      ShowException(e);
      int row = getPluginRow(pinfo.name);
      QLayout* layout = m_ui->m_tableWidget->cellWidget(row, 0)->layout();
      QCheckBox* checkBox =
          findChild<QCheckBox*>(layout->itemAt(0)->widget()->objectName());
      checkBox->setVisible(false);
      checkBox->setChecked(false);
      setChanged(getPluginRow(pinfo.name), false);
    }
  }
  updateBroken();
}

void te::qt::widgets::PluginManagerDialog::onAddPushButtonClicked()
{
  QString rsc = ResourceChooser::getResource(
      qApp->applicationDirPath(),
      tr("TerraLib Plug-in Files (*.teplg.json *.TEPLG.JSON)"), this);

  try
  {
    if(rsc.isEmpty())
      return;

    QFileInfo info(rsc);

    te::core::PluginInfo pInfo;

    if(info.isFile())
    {
      pInfo = te::core::JSONPluginInfoSerializer(
          info.absoluteFilePath().toUtf8().data());

      te::core::PluginManager::instance().insert(pInfo);

      addEntry(pInfo, PluginStatus::unloaded);
    }
    else
    {
      if(!info.isDir())
        throw tr("There's no resource selected, no plugins found.");

      QStringList filters;
      filters << "*.teplg.json"
              << "*.TEPLG.JSON";

      QDir dir(info.canonicalFilePath());

      QString qdir_s = dir.absolutePath();

      QStringList plgs = dir.entryList(filters, QDir::Files);

      if(plgs.isEmpty())
      {
        boost::format err_msg(
            tr("There is no plugin in the following directory: %1%")
                .toUtf8()
                .data());
        throw te::InvalidArgumentException() << te::ErrorDescription(
            (err_msg % dir.absolutePath().toUtf8().data()).str());
      }
      QStringList::iterator it;

      for(it = plgs.begin(); it != plgs.end(); ++it)
      {
        pInfo = te::core::JSONPluginInfoSerializer(
            dir.absoluteFilePath(*it).toUtf8().data());

        te::core::PluginManager::instance().insert(pInfo);

        addEntry(pInfo, PluginStatus::unloaded);
      }
    }
  }
  catch(const boost::exception& e)
  {
    ShowException(e);
  }
  m_ui->m_applyPushButton->setFocus();
  m_ui->m_applyPushButton->setEnabled(true);
}

void te::qt::widgets::PluginManagerDialog::onApplyPushButtonClicked()
{
  int nRow = m_ui->m_tableWidget->rowCount();
  std::vector<te::core::PluginInfo> toUnload;
  std::vector<te::core::PluginInfo> toLoad;
  std::vector<te::core::PluginInfo> toRemove;
  for(int row = 0; row < nRow; ++row)
  {
    std::string plugin_name = getPluginName(row);
    try
    {
      switch(getPluginStatus(row))
      {
        case PluginStatus::to_load:
          toLoad.push_back(
              te::core::PluginManager::instance().getPluginInfo(plugin_name));
          break;
        case PluginStatus::to_unload:
          toUnload.push_back(
              te::core::PluginManager::instance().getPluginInfo(plugin_name));
          break;
        case PluginStatus::to_remove:
          toRemove.push_back(
              te::core::PluginManager::instance().getPluginInfo(plugin_name));
          break;
        default:
          break;
      }
    }
    catch(const boost::exception& e)
    {
      ShowException(e);
    }
  }
  if(!toUnload.empty())
    unloadPlugins(toUnload);
  if(!toRemove.empty())
    removeEntries(toRemove);
  if(!toLoad.empty())
    loadPlugins(toLoad);

  m_ui->m_applyPushButton->setEnabled(false);
}

void te::qt::widgets::PluginManagerDialog::onClosePushButtonClicked()
{
  reject();
}

void te::qt::widgets::PluginManagerDialog::onEnableAllChanged(int state)
{
  int nRow = m_ui->m_tableWidget->rowCount();
  switch(state)
  {
    case Qt::Checked:
      for(int row = 0; row < nRow; ++row)
      {
        QLayout* layout =
            m_ui->m_tableWidget->cellWidget(row, PluginHeader::enabled)
                ->layout();
        QCheckBox* checkBox =
            findChild<QCheckBox*>(layout->itemAt(0)->widget()->objectName());
        if(checkBox->isVisible())
          checkBox->setChecked(true);
      }
      break;
    case Qt::Unchecked:
      for(int row = 0; row < nRow; ++row)
      {
        QLayout* layout =
            m_ui->m_tableWidget->cellWidget(row, PluginHeader::enabled)
                ->layout();
        QCheckBox* checkBox =
            findChild<QCheckBox*>(layout->itemAt(0)->widget()->objectName());
        if(checkBox->isVisible())
          checkBox->setChecked(false);
      }
      break;
    default:
      break;
  }
}

void te::qt::widgets::PluginManagerDialog::onEnabledChanged(int state)
{
  QCheckBox* checkBox = qobject_cast<QCheckBox*>(QObject::sender());
  std::string plugin_name = checkBox->objectName().toUtf8().data();
  int row = getPluginRow(plugin_name);
  switch(getPluginStatus(row))
  {
    case PluginStatus::to_load:
    case PluginStatus::to_unload:
    case PluginStatus::to_remove:
      setChanged(row, true);
      break;
    default:
      setChanged(row, false);
      break;
  }
  m_ui->m_applyPushButton->setEnabled(true);
}

void te::qt::widgets::PluginManagerDialog::onHelpPushButtonClicked()
{
  te::qt::widgets::HelpManager::getInstance().showHelp(
      "widgets/pluginmanager/PluginManager.html", "dpi.inpe.br.qtwidgets");
}

void te::qt::widgets::PluginManagerDialog::onRemovePushButtonClicked()
{
  QModelIndexList indexList =
      m_ui->m_tableWidget->selectionModel()->selectedIndexes();
  for(const auto index : indexList)
  {
    disableRow(index.row());
  }
  m_ui->m_applyPushButton->setFocus();
  m_ui->m_applyPushButton->setEnabled(true);
}

void te::qt::widgets::PluginManagerDialog::removeEntries(
    std::vector<te::core::PluginInfo> v_pInfo)
{
  v_pInfo = te::core::plugin::TopologicalSort(v_pInfo);
  unloadPlugins(v_pInfo);
  for(const te::core::PluginInfo& pinfo : v_pInfo)
  {
    te::core::PluginManager::instance().remove(pinfo.name);
    int row = getPluginRow(pinfo.name);
    m_ui->m_tableWidget->removeRow(row);
  }
}


void te::qt::widgets::PluginManagerDialog::setChanged(const int row, bool bold)
{
  int nCol = m_ui->m_tableWidget->columnCount();

  for(int col = 1; col < nCol; ++col)
  {
    QFont font;
    font.setBold(bold);
    m_ui->m_tableWidget->item(row, col)->setFont(font);
  }
  m_ui->m_tableWidget->resizeColumnsToContents();
}

void te::qt::widgets::PluginManagerDialog::unloadPlugins(
    std::vector<te::core::PluginInfo> v_pInfo)
{
  v_pInfo = te::core::plugin::TopologicalSort(v_pInfo);
  for(auto plugin = v_pInfo.rbegin(); plugin != v_pInfo.rend(); ++plugin)
  {
    int row = getPluginRow(plugin->name);
    try
    {
      te::core::PluginManager::instance().stop(plugin->name);
      te::core::PluginManager::instance().unload(plugin->name);
      setChanged(row, false);
    }
    catch(const te::core::PluginHasDependentException& e)
    {
      QMessageBox::StandardButton reply;
      boost::format msg(tr("Unloading '%1%' will unload the following:\n"
                           "%2%\n"
                           "Do you want to continue?")
                            .toUtf8()
                            .data());

      std::vector<std::string> dependents;
      GetDependents(plugin->name, dependents);

      dependents.erase(
          std::remove(dependents.begin(), dependents.end(), plugin->name),
          dependents.end());

      reply = QMessageBox::question(
          this, "Warning!",
          (msg % plugin->name % boost::algorithm::join(dependents, "\n"))
              .str()
              .c_str(),
          QMessageBox::Yes | QMessageBox::No);

      te::core::PluginManager::instance().recursiveUnload(plugin->name);
      if(reply == QMessageBox::Yes)
      {
        for(auto it = dependents.begin(); it != dependents.end(); ++it)
        {
          if(*it != plugin->name)
            changePluginStatus(*it);
        }
        setChanged(row, false);
      }
    }
  }
}


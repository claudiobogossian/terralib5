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
  \file terralib/plugins/manager/PluginManagerDialog.cpp

  \brief A Qt dialog for plugin management.
*/

// TerraLib
#include "../../utils/CentralizedCheckBoxDelegate.h"
#include "../../utils/ResourceChooser.h"
#include "../../../../plugin/AbstractPlugin.h"
#include "../../../../plugin/PluginInfo.h"
#include "../../../../plugin/PluginManager.h"
#include "../../../../plugin/Utils.h"
#include "../../help/HelpManager.h"
#include "../../Exception.h"
#include "PluginManagerDialog.h"
#include "PluginsModel.h"
#include "ui_PluginManagerDialogForm.h"

// STL
#include <algorithm>

// Qt
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QMessageBox>
#include <QPixmap>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

bool PluginExists(const std::string& pluginName)
{
  try
  {
    te::plugin::PluginManager::getInstance().getPlugin(pluginName);
    return true;
  }
  catch(te::plugin::Exception&)
  {
    return false;
  }
}

void CheckLoadedDependencies(te::plugin::PluginInfo* plg, std::vector<std::string>& deps)
{
  deps.clear();

  std::vector<std::string> plgs = te::plugin::PluginManager::getInstance().getDependents(plg->m_name);
  std::vector<std::string>::iterator it;

  for(it=plgs.begin(); it!=plgs.end(); ++it)
  {
    std::string plgName = *it;

    if(te::plugin::PluginManager::getInstance().isLoaded(plgName))
      deps.push_back(plgName);
  }
}

void CheckRequiredDependencies(te::plugin::PluginInfo* plg, std::vector<std::string>& deps)
{
  deps.clear();

  std::vector<std::string> plgs = plg->m_requiredPlugins;
  std::vector<std::string>::iterator it;

  for(it=plgs.begin(); it!=plgs.end(); ++it)
  {
    std::string plgName = *it;

    if(!PluginExists(plgName))
    {
      QString msg("Unable to load <b>%1</b>. Dependency missed: <ul><li><b>%2</b></li></ul>.");
      msg = msg.arg(plg->m_name.c_str(), plgName.c_str()); 

      throw te::qt::widgets::Exception(msg.toStdString());
    }

    if(!te::plugin::PluginManager::getInstance().isLoaded(plgName))
      deps.push_back(plgName);
  }
}

QString GetPluginDepsMessage(const std::string& plg, const std::vector<std::string>& dps)
{
  QString result;

  if(!dps.empty())
  {
    std::vector<std::string>::const_iterator it;
  
    result = QObject::tr("Unloading <b>%1</b> will also unload the plugins: <ul>");
    result = result.arg(plg.c_str());

    for(it=dps.begin(); it!=dps.end(); ++it)
    {
      result += QString("<li><b>%1</b></li>");
      result = result.arg((*it).c_str()); 
    }

    result +=  QObject::tr("</ul> Did you want to continue?");
  }

  return result;
}

QString GetPluginReqsMessage(const std::string& plg, const std::vector<std::string>& dps)
{
  QString result;

  if(!dps.empty())
  {
    std::vector<std::string>::const_iterator it;
  
    result = QObject::tr("In order to be able to load <b>%1</b> plugin, the following plugins must also be enabled: <ul>");
    result = result.arg(plg.c_str());

    for(it=dps.begin(); it!=dps.end(); ++it)
    {
      result += QString("<li><b>%1</b></li>");
      result = result.arg((*it).c_str()); 
    }

    result += QObject::tr("</ul> Allow them to be anabled?");
  }

  return result;
}

void MakeRemove(const std::vector<te::plugin::PluginInfo*>& plgs, const std::vector<te::qt::widgets::PluginsModel::PluginsStatus>& status, QWidget* parent)
{
  std::vector<std::string> dps;

  for(size_t i=0; i<status.size(); i++)
    if(status[i].testFlag(te::qt::widgets::PluginsModel::To_remove) && PluginExists(plgs[i]->m_name))
    {
      CheckLoadedDependencies(plgs[i], dps);

      if(!dps.empty())
      {
        QString msg = GetPluginDepsMessage(plgs[i]->m_name, dps);
        if(QMessageBox::question(parent, QObject::tr("Remove plugins"), msg, QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
          continue;
      }

      te::plugin::PluginManager::getInstance().remove(plgs[i]->m_name);
    }
}

void MakeDisable(const std::vector<te::plugin::PluginInfo*>& plgs, const std::vector<te::qt::widgets::PluginsModel::PluginsStatus>& status, QWidget* parent)
{
  std::vector<std::string> dps;

  for(size_t i=0; i<status.size(); i++)
    if(status[i].testFlag(te::qt::widgets::PluginsModel::To_disable) && te::plugin::PluginManager::getInstance().isLoaded(plgs[i]->m_name))
    {
      CheckLoadedDependencies(plgs[i], dps);

      if(!dps.empty())
      {
        QString msg = GetPluginDepsMessage(plgs[i]->m_name, dps);
        if(QMessageBox::question(parent, QObject::tr("Unload plugins"), msg, QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
          continue;
      }

      te::plugin::PluginManager::getInstance().unload(plgs[i]->m_name);
    }
}

void MakeAdd(const std::vector<te::plugin::PluginInfo*>& plgs, const std::vector<te::qt::widgets::PluginsModel::PluginsStatus>& status)
{
  for(size_t i=0; i<status.size(); i++)
    if(status[i].testFlag(te::qt::widgets::PluginsModel::To_add) && (!PluginExists(plgs[i]->m_name)))
      te::plugin::PluginManager::getInstance().add(*plgs[i]);
}

void MakeEnable(const std::vector<te::plugin::PluginInfo*>& plgs, const std::vector<te::qt::widgets::PluginsModel::PluginsStatus>& status, QWidget* parent)
{
  std::vector<std::string> dps;
  std::vector<std::string>::iterator it;

  for(size_t i=0; i<status.size(); i++)
  {
    try
    {
      if(status[i].testFlag(te::qt::widgets::PluginsModel::To_enable) && te::plugin::PluginManager::getInstance().isUnloadedPlugin(plgs[i]->m_name))
      {
        CheckRequiredDependencies(plgs[i], dps);

        if(!dps.empty())
        {
          QString msg = GetPluginReqsMessage(plgs[i]->m_name, dps);

          if(QMessageBox::question(parent, QObject::tr("Enable plugin"), msg, QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
            continue;

          for(it=dps.begin(); it!=dps.end(); ++it)
            te::plugin::PluginManager::getInstance().load(*it);
        }

        te::plugin::PluginManager::getInstance().load(plgs[i]->m_name);
      }
    }
    catch(const std::exception& e)
    {
      QMessageBox::warning(parent, QObject::tr("Fail to load plugin"), e.what());

      continue;
    }
  }
}

void AddPlugin(const QString& fileName, te::qt::widgets::PluginsModel* model)
{
  te::plugin::PluginInfo* pInfo = te::plugin::GetInstalledPlugin(QDir::toNativeSeparators(fileName).toStdString());

  if(PluginExists(pInfo->m_name))
    return;

  model->addPlugin(pInfo, te::qt::widgets::PluginsModel::To_add);

  delete pInfo;
}

Qt::CheckState GetCheckState(te::qt::widgets::PluginsModel* model)
{
  int rows = model->rowCount(QModelIndex());
  int st = model->index(0,0).data(Qt::CheckStateRole).toInt();

  for(int i=1; i<rows; i++)
  {
    int st2 = model->index(i,0).data(Qt::CheckStateRole).toInt();

    if(st2 != st)
      return Qt::PartiallyChecked;
  }

  return (Qt::CheckState) st;
}

te::qt::widgets::PluginManagerDialog::PluginManagerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::PluginManagerDialogForm)
{
  m_ui->setupUi(this);

  setWindowTitle(tr("Manage Application Plugins"));

  m_model = new PluginsModel(this);
  m_ui->m_installedPluginsTableWidget->setModel(m_model);
  m_ui->m_installedPluginsTableWidget->setItemDelegate(new CentralizedCheckBoxDelegate(this));

  fillInstalledPlugins();
  filliPlugins();

  connect(m_ui->m_applyPushButton, SIGNAL(pressed()), this, SLOT(applyPushButtonPressed()));
  connect(m_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));
//  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), SLOT(onHelpPushButtonPressed()));
  connect(m_ui->m_enableAll, SIGNAL(clicked(bool)), SLOT(onEnableAllChanged(bool)));

  m_ui->m_addButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeButton->setIcon(QIcon::fromTheme("list-remove"));

  m_ui->m_installedPluginsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

  m_ui->m_pluginsTabWidget->setTabEnabled(1, false);

  m_ui->m_enableAll->setCheckState(GetCheckState(m_model));

  m_ui->m_helpPushButton->setPageReference("widgets/pluginmanager/PluginManager.html");
}

te::qt::widgets::PluginManagerDialog::~PluginManagerDialog()
{
}

void te::qt::widgets::PluginManagerDialog::applyPushButtonPressed()
{
  std::vector<te::plugin::PluginInfo*> plgs;
  std::vector<te::qt::widgets::PluginsModel::PluginsStatus> status;

  m_model->getPluginsInfo(plgs, status);

  MakeRemove(plgs, status, this);
  MakeDisable(plgs, status, this);
  MakeAdd(plgs, status);
  MakeEnable(plgs, status, this);

  m_model->clear();

  fillInstalledPlugins();

  m_ui->m_applyPushButton->setEnabled(false);
}

void te::qt::widgets::PluginManagerDialog::tableWidgetClicked(QTableWidgetItem* item)
{
  //if((item == 0) || (item->column() != 0))
  //  return;

  //if(item->tableWidget() == m_ui->m_installedPluginsTableWidget)
  //  m_changedInstalledPlugins.insert(item->row());
  //else if(item->tableWidget() == m_ui->m_netPluginsTableWidget)
  //  m_downloadPlugins.insert(item->row()); 

  //if(m_ui->m_installedPluginsTableWidget->item(item->row(), 0)->flags() & Qt::ItemIsUserCheckable)
  //  m_ui->m_applyPushButton->setEnabled(true);
}

void te::qt::widgets::PluginManagerDialog::replyFinished(QNetworkReply* reply)
{
  //if(reply->error() != QNetworkReply::NoError)
  //{
  //  QMessageBox::warning(this,
  //                       tr("TerraLib Qt Components"),
  //                       tr("Could not retrieve available plugins from the Internet!"));

  //  return;
  //}

  //QVariant contenttype = reply->header(QNetworkRequest::ContentTypeHeader);

  //if(!contenttype.isValid())
  //{
  //  QMessageBox::warning(this,
  //                       tr("TerraLib Qt Components"),
  //                       tr("Plugins list missing content type!"));

  //  return;
  //}

  //if(contenttype.toString() != QString("application/json"))
  //{
  //  QString errmsg(tr("Available plugins from Internet provided with wrong content type: %s!"));
  //  errmsg.arg(0, contenttype.toString());

  //  QMessageBox::warning(this,
  //                       tr("TerraLib Qt Components"),
  //                       errmsg);

  //  return;
  //}

  //QByteArray data = reply->readAll();

  //if(data.isEmpty())
  //{
  //  QMessageBox::warning(this,
  //                       tr("TerraLib Qt Components"),
  //                       tr("The plugin list from the Internet is empty!"));

  //  return;
  //}

  //m_iplugins.clear();

  //ReadPluginInfo(data, m_iplugins);

  //int nrows = static_cast<int>(m_iplugins.size());

  //m_ui->m_netPluginsTableWidget->setRowCount(nrows);

  //std::set<iPluginInfo>::const_iterator it = m_iplugins.begin();
  //std::set<iPluginInfo>::const_iterator itend = m_iplugins.end();

  //int i = 0;

  //while(it != itend)
  //{
  //  te::plugin::PluginInfo plugin = Convert(*it);
  //  addEntry(i, plugin, false, m_ui->m_netPluginsTableWidget);

  //  ++it;
  //  ++i;
  //}

  //m_ui->m_netPluginsTableWidget->resizeColumnsToContents();
  //m_ui->m_netPluginsTableWidget->sortByColumn(1, Qt::AscendingOrder);
}

void te::qt::widgets::PluginManagerDialog::addPlugins()
{
  QString rsc = ResourceChooser::getResource(qApp->applicationDirPath(), tr("TerraLib Plug-in Files (*.teplg *.TEPLG)"), this);

  try
  {
    if(rsc.isEmpty())
      return;

    QFileInfo info(rsc);

    if(info.isFile())
      AddPlugin(info.absoluteFilePath(), m_model);
    else
    {
      if(!info.isDir())
        throw tr("There's no resource selected, no plugins found.");

      QStringList filters;
      filters <<"*.teplg" <<"*.TEPLG";

      QDir dir(info.canonicalFilePath());

      QString qdir_s = dir.absolutePath();

      QStringList plgs = dir.entryList(filters, QDir::Files);

      if(plgs.isEmpty())
        throw tr("There's no plugins found.");

      QStringList::iterator it;

      for(it = plgs.begin(); it != plgs.end(); ++it)
        AddPlugin(QDir::toNativeSeparators(dir.absoluteFilePath(*it)), m_model);
    }

    m_ui->m_applyPushButton->setEnabled(true);
  }
  catch(QString& exc)
  {
    QMessageBox msg;
    msg.setWindowTitle(tr("Fail to load plugin files"));
    msg.setIcon(QMessageBox::Warning);
    msg.setText(exc);
    msg.exec();
  }
  catch(te::plugin::Exception& exc)
  {
    QMessageBox msg;
    msg.setWindowTitle(tr("Fail to load plugin files"));
    msg.setIcon(QMessageBox::Warning);
    msg.setText(exc.what());
    msg.exec();
  }
}

void te::qt::widgets::PluginManagerDialog::removePlugins()
{
  QModelIndexList lst = m_ui->m_installedPluginsTableWidget->selectionModel()->selectedRows(0);

  if(lst.isEmpty())
  {
    QMessageBox q(this);

    q.setIcon(QMessageBox::Warning);
    q.setWindowTitle(tr("Remove installed plugins"));
    q.setText(tr("There are NOT selected plugins."));
    q.exec();

    return;
  }

  m_model->removePlugins(lst);

  m_ui->m_applyPushButton->setEnabled(true);
}

void te::qt::widgets::PluginManagerDialog::dataChanged(const QModelIndex& index, const QModelIndex&)
{
  m_ui->m_applyPushButton->setEnabled(true);
  m_ui->m_enableAll->setCheckState(GetCheckState(m_model));
}

//void te::qt::widgets::PluginManagerDialog::onHelpPushButtonPressed()
//{
//  te::qt::widgets::HelpManager::getInstance().showHelp("widgets/pluginmanager/PluginManager.html", "dpi.inpe.br.qtwidgets");
//}

void te::qt::widgets::PluginManagerDialog::onEnableAllChanged(bool st)
{
  Qt::CheckState state = (st) ? Qt::Checked : Qt::Unchecked;

  m_ui->m_enableAll->setCheckState(state);
  {
    int nrows = m_model->rowCount(QModelIndex());

    for(int i=0; i<nrows; i++)
    {
      QModelIndex idx = m_model->index(i, 0);
      m_model->setData(idx, QVariant(state), Qt::CheckStateRole);
    }
  }

  m_ui->m_enableAll->setCheckState(GetCheckState(m_model));
}

void te::qt::widgets::PluginManagerDialog::fillInstalledPlugins()
{
  const te::plugin::PluginManager& pm = te::plugin::PluginManager::getInstance();

  std::vector<std::string> plugins;

  pm.getPlugins(plugins);

  int nrows = static_cast<int>(plugins.size());

  for(int i = 0; i < nrows; ++i)
  {
    const te::plugin::PluginInfo& plugin = pm.getPlugin(plugins[i]);
    addEntry(i, plugin, pm.isLoaded(plugin.m_name));
  }

  m_ui->m_installedPluginsTableWidget->resizeColumnsToContents();
  m_ui->m_installedPluginsTableWidget->sortByColumn(1, Qt::AscendingOrder);
}

void te::qt::widgets::PluginManagerDialog::filliPlugins()
{
  //m_ui->m_netPluginsTableWidget->clearContents();
  //m_ui->m_netPluginsTableWidget->verticalHeader()->hide();
  //m_ui->m_netPluginsTableWidget->hideColumn(9);


  //QNetworkAccessManager* manager = new QNetworkAccessManager(this);

  //connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

  //manager->get(QNetworkRequest(QUrl("http://www.dpi.inpe.br/~gribeiro/terralib5/plugins/win32/plugins.json")));
}

void te::qt::widgets::PluginManagerDialog::addEntry(int i, const te::plugin::PluginInfo& pinfo, bool checked)
{
  bool bk = te::plugin::PluginManager::getInstance().isBrokenPlugin(pinfo.m_name);
  te::qt::widgets::PluginsModel::PluginsStatus st;

  if(bk)
    st = te::qt::widgets::PluginsModel::Broked;
  else
  {
    if(checked) 
      st = te::qt::widgets::PluginsModel::Loaded;
    else
      st = te::qt::widgets::PluginsModel::Unloaded;
  }

  m_model->addPlugin(&pinfo, st);
}


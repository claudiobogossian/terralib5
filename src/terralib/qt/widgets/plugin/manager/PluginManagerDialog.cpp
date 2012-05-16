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
#include "../../../../plugin/AbstractPlugin.h"
#include "../../../../plugin/PluginInfo.h"
#include "../../../../plugin/PluginManager.h"
#include "ui/PluginManagerDialogForm.h"
#include "PluginManagerDialog.h"
//#include "Utils.h"

// STL
#include <algorithm>

// Qt
#include <QtCore/QUrl>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

te::qt::widgets::PluginManagerDialog::PluginManagerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::PluginManagerDialogForm)
{
  m_ui->setupUi(this);

  setWindowTitle(tr("Manage Application Plugins"));

  fillInstalledPlugins();
  filliPlugins();

  connect(m_ui->m_applyPushButton, SIGNAL(pressed()), this, SLOT(applyPushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
  connect(m_ui->m_installedPluginsTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(tableWidgetClicked(QTableWidgetItem*)));
  connect(m_ui->m_netPluginsTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(tableWidgetClicked(QTableWidgetItem*)));

  //m_ui->m_installedPluginsTableWidget->setAlternatingRowColors(true);
  //m_ui->m_installedPluginsTableWidget->setColumnCount(9);
  //m_ui->m_installedPluginsTableWidget->horizontalHeader()->hide();
}

te::qt::widgets::PluginManagerDialog::~PluginManagerDialog()
{
}

void te::qt::widgets::PluginManagerDialog::applyPushButtonPressed()
{
  for(std::set<int>::const_iterator it = m_changedInstalledPlugins.begin(); it != m_changedInstalledPlugins.end(); ++it)
  {
    QTableWidgetItem* itemName = m_ui->m_installedPluginsTableWidget->item(*it, 9);
    QTableWidgetItem* itemState = m_ui->m_installedPluginsTableWidget->item(*it, 0);

    if(itemName == 0 || itemState == 0)
    {
      QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Could not apply all changes!"));

      continue;
    }

    QString pluginName = itemName->text();
    bool isChecked = itemState->checkState() == Qt::Checked ? true : false;

    if(te::plugin::PluginManager::getInstance().isLoaded(pluginName.toStdString()) && !isChecked)
      te::plugin::PluginManager::getInstance().unload(pluginName.toStdString());
    else if(!te::plugin::PluginManager::getInstance().isLoaded(pluginName.toStdString()) && isChecked)
      te::plugin::PluginManager::getInstance().load(pluginName.toStdString());
  }

  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("For now we are not installing plugins from Internet!\nWe will provide this functionality soon!"));

  accept();
}

void te::qt::widgets::PluginManagerDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("We will provide this functionality soon!"));
}

void te::qt::widgets::PluginManagerDialog::tableWidgetClicked(QTableWidgetItem* item)
{
  if((item == 0) || (item->column() != 0))
    return;

  if(item->tableWidget() == m_ui->m_installedPluginsTableWidget)
    m_changedInstalledPlugins.insert(item->row());
  else if(item->tableWidget() == m_ui->m_netPluginsTableWidget)
    m_downloadPlugins.insert(item->row());

  m_ui->m_applyPushButton->setEnabled(true);
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

void te::qt::widgets::PluginManagerDialog::fillInstalledPlugins()
{
  m_ui->m_installedPluginsTableWidget->clearContents();
  m_ui->m_installedPluginsTableWidget->verticalHeader()->hide();
  m_ui->m_installedPluginsTableWidget->hideColumn(9);

  const te::plugin::PluginManager& pm = te::plugin::PluginManager::getInstance();

  std::vector<std::string> plugins;

  pm.getPlugins(plugins);

  int nrows = static_cast<int>(plugins.size());

  m_ui->m_installedPluginsTableWidget->setRowCount(nrows);

  for(int i = 0; i < nrows; ++i)
  {
    const te::plugin::PluginInfo& plugin = pm.getPlugin(plugins[i]);
    addEntry(i, plugin, pm.isLoaded(plugin.m_name), m_ui->m_installedPluginsTableWidget);
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

void te::qt::widgets::PluginManagerDialog::addEntry(int i, const te::plugin::PluginInfo& pinfo, bool checked, QTableWidget* table)
{
  QTableWidgetItem* pitem = new QTableWidgetItem("");
  pitem->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
  table->setItem(i, 0, pitem);

  QTableWidgetItem* pdisplayname = new QTableWidgetItem(pinfo.m_displayName.c_str());
  table->setItem(i, 1, pdisplayname);

  QTableWidgetItem* pversion = new QTableWidgetItem(pinfo.m_version.c_str());
  table->setItem(i, 2, pversion);

  QTableWidgetItem* plicense = new QTableWidgetItem(pinfo.m_licenseDescription.c_str());
  table->setItem(i, 3, plicense);

  QTableWidgetItem* pcategory = new QTableWidgetItem(pinfo.m_category.c_str());
  table->setItem(i, 4, pcategory);

  QTableWidgetItem* psite = new QTableWidgetItem(pinfo.m_site.c_str());
  table->setItem(i, 5, psite);

  QTableWidgetItem* pprovidername = new QTableWidgetItem(pinfo.m_provider.m_name.c_str());
  table->setItem(i, 6, pprovidername);

  QTableWidgetItem* pprovidersite = new QTableWidgetItem(pinfo.m_provider.m_site.c_str());
  table->setItem(i, 7, pprovidersite);

  QTableWidgetItem* pprovideremail = new QTableWidgetItem(pinfo.m_provider.m_email.c_str());
  table->setItem(i, 8, pprovideremail);

  QTableWidgetItem* pname = new QTableWidgetItem(pinfo.m_name.c_str());
  table->setItem(i, 9, pname);
}


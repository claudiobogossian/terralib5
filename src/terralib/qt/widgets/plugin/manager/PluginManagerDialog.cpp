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
#include "ui_PluginManagerDialogForm.h"
#include "PluginManagerDialog.h"
#include <terralib/qt/widgets/utils/ResourceChooser.h>
#include <terralib/plugin/Utils.h>

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
#include <QFileInfo>
#include <QDir>

QTableWidgetItem* getCheckableItem(const bool& isBroked)
{
  QTableWidgetItem* item = new QTableWidgetItem;

  if(isBroked)
  {
    item->setFlags(item->flags() &= ~Qt::ItemIsUserCheckable);
    item->setToolTip(QObject::tr("Plug-in broked. (Missing dependencies)"));
  }
  else
  {
    item->setFlags(item->flags() |= Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    item->setToolTip(QObject::tr("Check for enable uncheck for disable the plug-in"));
  }

  return item;
}

bool pluginExists(const std::string& pluginName)
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

void addPlugin(const QString& fileName, std::map<std::string, std::string>& added)
{
  te::plugin::PluginInfo* pInfo = te::plugin::GetInstalledPlugin(QDir::toNativeSeparators(fileName).toStdString());

  if(pluginExists(pInfo->m_name))
    return;

  te::plugin::PluginManager::getInstance().add(pInfo);

  added[pInfo->m_name] = fileName.toStdString();
}

void removePluginsImpl(const QModelIndexList& lst, QTableWidget* table, std::vector<std::string>& removed)
{
  table->clearSelection();

  if(lst.isEmpty())
    return;

  QList<int> rows;
  QModelIndexList::ConstIterator it;

  for(it=lst.constBegin(); it!=lst.constEnd(); ++it)
    rows.append((*it).row());

  qSort(rows);

  for(int i=rows.count()-1; i>=0; i--)
  {
    int row=rows.value(i);
    std::string plg_name = table->item(row, 9)->text().toStdString();

    te::plugin::PluginManager::getInstance().remove(plg_name);
    table->removeRow(row);
    removed.push_back(plg_name);
  }

  for(int i=0; i<table->rowCount(); i++)
  {
    QString p_name = table->item(i, 9)->text();
    bool plg_bk = te::plugin::PluginManager::getInstance().isBrokenPlugin(p_name.toStdString());

    for(int j=0; j<10; j++)
    {
      QTableWidgetItem* item = table->item(i, j);
      Qt::ItemFlags flgs = item->flags();

      if(plg_bk) 
      {
        if(j==0)
        {
          table->setItem(i, j, getCheckableItem(true));
          continue;
        }

        flgs &= ~Qt::ItemIsEditable;
      }
      else
      {
        if(j==0)
        {
          QTableWidgetItem* aux = getCheckableItem(false);
          aux->setCheckState((te::plugin::PluginManager::getInstance().isLoaded(p_name.toStdString()) ? Qt::Checked : Qt::Unchecked));
          table->setItem(i, j, aux);
          continue;
        }

        flgs |= Qt::ItemIsEditable;
      }

      item->setFlags(flgs);
    }
  }
}

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

  m_ui->m_addButton->setIcon(QIcon::fromTheme("plus"));
  m_ui->m_removeButton->setIcon(QIcon::fromTheme("minus"));

  m_ui->m_installedPluginsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

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

//  QMessageBox::warning(this,
  //                     tr("TerraLib Qt Components"),
    //                   tr("For now we are not installing plugins from Internet!\nWe will provide this functionality soon!"));

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

  if(m_ui->m_installedPluginsTableWidget->item(item->row(), 0)->flags() & Qt::ItemIsUserCheckable)
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

void te::qt::widgets::PluginManagerDialog::addPlugins()
{
  QString rsc = ResourceChooser::getResource(qApp->applicationDirPath(), tr("XML Plug-in Files (*.xml *.XML)"), this);
  std::map<std::string, std::string> added;

  try
  {
    if(rsc.isEmpty())
      return;

    QFileInfo info(rsc);

    if(info.isFile())
      addPlugin(info.absoluteFilePath(), added);
    else
    {
      if(!info.isDir())
        throw tr("There's no resource selected, no plug-ins found.");

      QStringList filters;
      filters <<"*.xml" <<"*.XML";

      QDir dir(info.canonicalFilePath());

      QString qdir_s = dir.absolutePath();

      QStringList plgs = dir.entryList(filters, QDir::Files);

      if(plgs.isEmpty())
        throw tr("There's no plug-ins found.");

      QStringList::iterator it;

      for(it = plgs.begin(); it != plgs.end(); ++it)
        addPlugin(dir.absoluteFilePath(*it), added);
    }

    if(!added.empty())
      emit pluginsAdded(added);

    fillInstalledPlugins();
  }
  catch(QString& exc)
  {
    QMessageBox msg;
    msg.setWindowTitle(tr("Fail to load plug-in files"));
    msg.setIcon(QMessageBox::Warning);
    msg.setText(exc);
    msg.exec();
  }
  catch(te::plugin::Exception& exc)
  {
    QMessageBox msg;
    msg.setWindowTitle(tr("Fail to load plug-in files"));
    msg.setIcon(QMessageBox::Warning);
    msg.setText(exc.what());
    msg.exec();
  }
}

void te::qt::widgets::PluginManagerDialog::removePlugins()
{
  QModelIndexList lst = m_ui->m_installedPluginsTableWidget->selectionModel()->selectedRows(0);
  std::vector<std::string> removed;
  QMessageBox q(this);

  if(lst.isEmpty())
  {
    q.setIcon(QMessageBox::Warning);
    q.setWindowTitle(tr("Remove installed plug-ins"));
    q.setText(tr("There are NOT selected plug-ins."));
    q.exec();

    return;
  }

  q.setIcon(QMessageBox::Question);
  q.setWindowTitle(tr("Remove installed plug-ins"));
  q.setText(tr("Do you really want to remove the selected plug-ins?"));
  q.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

  if(q.exec() == QMessageBox::Yes)
    removePluginsImpl(lst, m_ui->m_installedPluginsTableWidget, removed);

  if(!removed.empty())
    emit pluginsRemoved(removed);
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
  bool bk = te::plugin::PluginManager::getInstance().isBrokenPlugin(pinfo.m_name);

  QTableWidgetItem* pitem = getCheckableItem(bk);
  table->setItem(i, 0, pitem);
  if(!bk)
    pitem->setCheckState((checked) ? Qt::Checked : Qt::Unchecked);

  pitem = new QTableWidgetItem(pinfo.m_displayName.c_str());
  table->setItem(i, 1, pitem);
  if(bk)
    pitem->setFlags(pitem->flags() &= ~Qt::ItemIsEditable);

  pitem = new QTableWidgetItem(pinfo.m_version.c_str());
  table->setItem(i, 2, pitem);
  if(bk)
    pitem->setFlags(pitem->flags() &= ~Qt::ItemIsEditable);

  pitem = new QTableWidgetItem(pinfo.m_licenseDescription.c_str());
  table->setItem(i, 3, pitem);
  if(bk)
    pitem->setFlags(pitem->flags() &= ~Qt::ItemIsEditable);

  pitem = new QTableWidgetItem(pinfo.m_category.c_str());
  table->setItem(i, 4, pitem);
  if(bk)
    pitem->setFlags(pitem->flags() &= ~Qt::ItemIsEditable);

  pitem = new QTableWidgetItem(pinfo.m_site.c_str());
  table->setItem(i, 5, pitem);
  if(bk)
    pitem->setFlags(pitem->flags() &= ~Qt::ItemIsEditable);

  pitem = new QTableWidgetItem(pinfo.m_provider.m_name.c_str());
  table->setItem(i, 6, pitem);
  if(bk)
    pitem->setFlags(pitem->flags() &= ~Qt::ItemIsEditable);

  pitem = new QTableWidgetItem(pinfo.m_provider.m_site.c_str());
  table->setItem(i, 7, pitem);
  if(bk)
    pitem->setFlags(pitem->flags() &= ~Qt::ItemIsEditable);

  pitem = new QTableWidgetItem(pinfo.m_provider.m_email.c_str());
  table->setItem(i, 8, pitem);
  if(bk)
    pitem->setFlags(pitem->flags() &= ~Qt::ItemIsEditable);

  pitem = new QTableWidgetItem(pinfo.m_name.c_str());
  table->setItem(i, 9, pitem);
}


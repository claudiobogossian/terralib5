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
  \file terralib/widgets/srs/SRSManagerDialog.cpp

  \brief A dialog used to select the Spatial Reference Systems for coordinates.
*/

#include "SRSManagerDialog.h"

#include "../../../srs/SpatialReferenceSystemManager.h"

#include "SRSDialog.h"
#include <ui_SRSManagerDialogForm.h>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QString>

#include <iostream>

te::qt::widgets::SRSManagerDialog::SRSManagerDialog(QWidget* parent, Qt::WindowFlags f):
  QDialog(parent, f),
  m_ui(new Ui::SRSManagerDialogForm)
{
  m_ui->setupUi(this);
  m_selSrsId.first = -1;
  m_selSrsId.second = "";
  
  // Assign the edit/add/remove button icons
  m_ui->m_addSRSToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeSRSToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_editSRSToolButton->setIcon(QIcon::fromTheme("preferences-system"));
  
  // Signals & slots
  connect(m_ui->m_SRSTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), SLOT(onSRSTreeWidgetItemClicked(QTreeWidgetItem* , int)));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), SLOT(onCancelPushButtonClicked()));   
  //connect(m_ui->m_helpPushButton, SIGNAL(clicked()), SLOT(onHelpPushButtonClicked())); 
  connect(m_ui->m_searchedSRSLineEdit, SIGNAL(textChanged(const QString&)),SLOT(onSearchLineEditTextChanged(const QString&)));
  connect(m_ui->m_SRSRecentTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), SLOT(onSRSRecentTableWidgetItemClicked(QTableWidgetItem*)));
  connect(m_ui->m_addSRSToolButton, SIGNAL(clicked()), SLOT(onAddSRSPushButtonClicked()));
  connect(m_ui->m_editSRSToolButton, SIGNAL(clicked()), SLOT(onEditSRSPushButtonClicked()));
  connect(m_ui->m_removeSRSToolButton, SIGNAL(clicked()), SLOT(onRemoveSRSPushButtonClicked()));
 
  m_ui->m_helpPushButton->setPageReference("widgets/srs/srs.html");

  // Builds the table with the recently used SRS ids  
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  QString value = sett.value("SRSRecentlyUsed", "").toString();
  
  m_ui->m_SRSRecentTableWidget->clear();
  if (!value.isEmpty())
  {
    m_recentSRS = value.split(',');
    int aux = 0;
    int srid;
    
    QStringList::const_iterator it;
    for (it = m_recentSRS.constBegin(); it != m_recentSRS.constEnd(); ++it)
    {
      srid = (*it).toInt();
      std::string auth("EPSG");
      if (srid >= 100000)
        auth = "USER";
      m_ui->m_SRSRecentTableWidget->setItem(aux, 0, new QTableWidgetItem(te::srs::SpatialReferenceSystemManager::getInstance().getName(srid,auth).c_str()));
      m_ui->m_SRSRecentTableWidget->setItem(aux, 1, new QTableWidgetItem(QString::number(srid)));
      m_ui->m_SRSRecentTableWidget->setItem(aux, 2, new QTableWidgetItem(auth.c_str()));
      ++aux;
    }
  }
  
  QStringList hLabels;
  hLabels << "Name" << "ID" << "Authority";
  m_ui->m_SRSRecentTableWidget->setHorizontalHeaderLabels(hLabels);
  
  m_ui->m_SRSRecentTableWidget->resizeColumnToContents(0);
  m_ui->m_SRSRecentTableWidget->resizeColumnToContents(1);
  m_ui->m_SRSRecentTableWidget->resizeColumnToContents(2);
  
  
  // Builds the available SRS tree
  QList<QTreeWidgetItem *> items;
  
  items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Geoographic SRS"))));
  items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Projected SRS"))));
  items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("User defined SRS"))));
  m_ui->m_SRSTreeWidget->insertTopLevelItems(0, items);
  
  std::pair<te::srs::SpatialReferenceSystemManager::iterator,
            te::srs::SpatialReferenceSystemManager::iterator> its = te::srs::SpatialReferenceSystemManager::getInstance().getIterators();
  
  while (its.first != its.second) 
  {
    if (its.first->m_auth_name == "EPSG")
    {
      if (its.first->m_auth_id < 5000)
      {
        QTreeWidgetItem *geog = new QTreeWidgetItem(items[0]);
        geog->setText(0, its.first->m_name.c_str());
        geog->setText(1, QString::number(its.first->m_auth_id));
        geog->setText(2, "EPSG");
      }
      else
      {
        QTreeWidgetItem *proj = new QTreeWidgetItem(items[1]);
        proj->setText(0, its.first->m_name.c_str());
        proj->setText(1, QString::number(its.first->m_auth_id));
        proj->setText(2, "EPSG");
      }
    }
    else
    {
      QTreeWidgetItem *userd = new QTreeWidgetItem(items[2]);
      userd->setText(0, its.first->m_name.c_str());
      userd->setText(1, QString::number(its.first->m_auth_id));
      userd->setText(2, its.first->m_auth_name.c_str());
    }
    ++its.first;
  }
  
  unsigned int ntl =  m_ui->m_SRSTreeWidget->topLevelItemCount();
  for(unsigned int i = 0; i < ntl; ++i)
  {
    QTreeWidgetItem* item = m_ui->m_SRSTreeWidget->topLevelItem(i);
    item->setExpanded(true);
  }
  
  m_ui->m_SRSTreeWidget->resizeColumnToContents(0);
  m_ui->m_SRSTreeWidget->resizeColumnToContents(1);
  m_ui->m_SRSTreeWidget->resizeColumnToContents(2);
  
  // Set the field with the PJ4 description
  m_ui->m_p4descPlainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
  QFontMetrics m (m_ui->m_p4descPlainTextEdit->font());
  int RowHeight = m.lineSpacing() ;
  m_ui->m_p4descPlainTextEdit->setFixedHeight(3*RowHeight);
}


te::qt::widgets::SRSManagerDialog::~SRSManagerDialog()
{
  delete m_ui;
}

const std::pair<int, std::string>& te::qt::widgets::SRSManagerDialog::getSelectedSRS() const
{
  return m_selSrsId;
}


std::pair<int, std::string> te::qt::widgets::SRSManagerDialog::getSRS(QWidget* parent, const QString& title)
{
  SRSManagerDialog dlg(parent);
  
  if(!title.isEmpty())
    dlg.setWindowTitle(title);
  
  if(dlg.exec() == QDialog::Accepted)
    return std::pair<int, std::string>(dlg.m_selSrsId.first, dlg.m_selSrsId.second);
  
  return std::pair<int, std::string>(TE_UNKNOWN_SRS, "");
}

void te::qt::widgets::SRSManagerDialog::onSearchLineEditTextChanged(const QString& text)
{
  QList<QTreeWidgetItem*> items = m_ui->m_SRSTreeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive, 0);
  items.append(m_ui->m_SRSTreeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive, 1));  
  filter(items);
}


void te::qt::widgets::SRSManagerDialog::filter(const QList<QTreeWidgetItem*>& items)
{
  for(int i = 0; i < m_ui->m_SRSTreeWidget->topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* item = m_ui->m_SRSTreeWidget->topLevelItem(i);
    
    for(int j = 0; j < item->childCount(); ++j)
    {
      QTreeWidgetItem* srs = item->child(j);
      bool hide = items.indexOf(srs) == -1;
      srs->setHidden(hide);
    }
  }
  update();
}


void te::qt::widgets::SRSManagerDialog::onSRSTreeWidgetItemClicked(QTreeWidgetItem * item, int /*column*/)
{
  if (!item || item->text(1).isEmpty()) 
  {
    m_selSrsId.first = TE_UNKNOWN_SRS;  
    m_selSrsId.second = "";
    m_ui->m_p4descPlainTextEdit->clear();
    return;
  }
  m_selSrsId.first = item->text(1).toUInt();  
  m_selSrsId.second = std::string(item->text(2).toLatin1());
  m_ui->m_p4descPlainTextEdit->setPlainText(te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(m_selSrsId.first,std::string(item->text(2).toLatin1())).c_str());
}

void te::qt::widgets::SRSManagerDialog::onSRSRecentTableWidgetItemClicked(QTableWidgetItem* item)
{
  if (!item)
    return;
  QString val = m_ui->m_SRSRecentTableWidget->item(item->row(),1)->text();
  QList<QTreeWidgetItem *> ilist = m_ui->m_SRSTreeWidget->findItems(val,Qt::MatchContains|Qt::MatchRecursive, 1);
  if (ilist.empty())
    return;
  m_ui->m_SRSTreeWidget->setCurrentItem(ilist[0]);
  onSRSTreeWidgetItemClicked(ilist[0],0);
}

void te::qt::widgets::SRSManagerDialog::onOkPushButtonClicked()
{
  if (m_selSrsId.first == TE_UNKNOWN_SRS || m_selSrsId.first == -1)
    accept();
  else 
  {
    QStringList aux;
    aux << QString::number(m_selSrsId.first);    
    for (int i=0; (i<=3 && i<m_recentSRS.size()); ++i)
      if (QString::number(m_selSrsId.first) != m_recentSRS[i])
        aux << m_recentSRS[i];
    
    QString value = aux.join(",");
    QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    sett.setValue("SRSRecentlyUsed", value);
    accept();
  }
}

void te::qt::widgets::SRSManagerDialog::onCancelPushButtonClicked()
{
  m_selSrsId.first = -1;  
  m_selSrsId.second = "";
  reject();
}

void te::qt::widgets::SRSManagerDialog::onHelpPushButtonClicked()
{
 QMessageBox::critical(this, "", tr("Not implemented yet!"));
}

void te::qt::widgets::SRSManagerDialog::onAddSRSPushButtonClicked()
{
  try
  {
    te::qt::widgets::SRSDialog dlg (this);
    int res = dlg.exec();
    if (res == QDialog::Accepted)
    {
      // insert into the manager
      te::srs::SpatialReferenceSystemManager::getInstance().add(dlg.getName(), dlg.getP4Desc(),"",dlg.getSRID(),"USER");
      
      // insert into the user interface
      QString srid = QString::number(dlg.getSRID());
      QTreeWidgetItem* udSRSItem = m_ui->m_SRSTreeWidget->topLevelItem(2);
      QTreeWidgetItem *userd = new QTreeWidgetItem(udSRSItem);
      userd->setText(0,dlg.getName().c_str());
      userd->setText(1,srid);
      userd->setText(2,"USER");
      m_ui->m_SRSTreeWidget->resizeColumnToContents(0);
      m_ui->m_SRSTreeWidget->resizeColumnToContents(1);
      m_ui->m_SRSTreeWidget->resizeColumnToContents(2);
      
      QList<QTreeWidgetItem *> ilist = m_ui->m_SRSTreeWidget->findItems(srid,Qt::MatchContains|Qt::MatchRecursive, 1);
      if (ilist.empty())
        return;
      m_ui->m_SRSTreeWidget->setCurrentItem(ilist[0]);
      onSRSTreeWidgetItemClicked(ilist[0],0);
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, "SRSDialog", e.what());
  }
  catch(...)
  {
    QMessageBox::warning(this, "SRSDialog",tr("Unknown error while trying to define a new SRS!"));
  }
}

void te::qt::widgets::SRSManagerDialog::onEditSRSPushButtonClicked()
{
  if (m_selSrsId.first == -1)
  {
    QMessageBox::warning(this, "SRSDialog",tr("Select a USER defined SRS to modify."));
    return;
  }
  
  if (m_selSrsId.first <= 100000)
  {
    QMessageBox::warning(this, "SRSDialog",tr("Only a USER defined SRS can be modified."));
    return;
  }

  // call the interface with the data selected
  std::string name = te::srs::SpatialReferenceSystemManager::getInstance().getName(m_selSrsId.first,"USER");
  te::qt::widgets::SRSDialog dlg(this,0,m_selSrsId.first,name,m_selSrsId.second);
  int res = dlg.exec();
  if (res == QDialog::Accepted)
  {
    
    // update the manager
    te::srs::SpatialReferenceSystemManager::getInstance().remove(m_selSrsId.first,"USER");
    te::srs::SpatialReferenceSystemManager::getInstance().add(dlg.getName(), dlg.getP4Desc(),"",dlg.getSRID(),"USER");
    
    // update the user interface
    QString srid = QString::number(m_selSrsId.first);
    QList<QTreeWidgetItem *> ilist = m_ui->m_SRSTreeWidget->findItems(srid,Qt::MatchContains|Qt::MatchRecursive, 1);
    if (!ilist.empty())
    {
      ilist[0]->setText(0,dlg.getName().c_str());
      ilist[0]->setText(1,srid);
      ilist[0]->setText(2,"USER");
    }
  }
}

void te::qt::widgets::SRSManagerDialog::onRemoveSRSPushButtonClicked()
{
  if (m_selSrsId.first == -1)
  {
    QMessageBox::warning(this,"SRSDialog",tr("Select a USER defined SRS to remove."));
    return;
  }
  
  if (m_selSrsId.first <= 100000)
  {
    QMessageBox::warning(this,"SRSDialog",tr("Only a USER defined SRS can be removed."));
    return;
  }
  
  // remove it from the manager
  te::srs::SpatialReferenceSystemManager::getInstance().remove(m_selSrsId.first,"USER");
  
  // remove it from the manager interface
  QString srid = QString::number(m_selSrsId.first);
  QList<QTreeWidgetItem *> ilist = m_ui->m_SRSTreeWidget->findItems(srid,Qt::MatchContains|Qt::MatchRecursive, 1);
  if (!ilist.empty())
  {
    QTreeWidgetItem* parent = ilist[0]->parent();
    parent->removeChild(ilist[0]);
  }
  
  // check if it also should be removed from the recently used list and interface
  QList<QTableWidgetItem *> rilist = m_ui->m_SRSRecentTableWidget->findItems(srid, Qt::MatchContains|Qt::MatchRecursive);
  if (!rilist.empty())
  {
    m_ui->m_SRSRecentTableWidget->removeRow(m_ui->m_SRSRecentTableWidget->row(rilist[0]));
    m_recentSRS.removeOne(srid);
    
    QStringList aux;
    for (int i=0; (i<=3 && i<m_recentSRS.size()); ++i)
      aux << m_recentSRS[i];
    
    QString value = aux.join(",");
    QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    sett.setValue("SRSRecentlyUsed", value);
  }
  m_selSrsId.first=TE_UNKNOWN_SRS;
  m_selSrsId.second="";
}

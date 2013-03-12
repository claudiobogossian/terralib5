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
  \file terralib/widgets/srs/SRSManagerDialog.cpp

  \brief A dialog used to select the Spatial Reference Systems for coordinates.
*/

#include "SRSManagerDialog.h"
#include "SRSDialog.h"

#include "../../../srs/SpatialReferenceSystemManager.h"

#include <ui_SRSManagerDialogForm.h>
#include <QtGui/QMessageBox>

// TerraLib

te::qt::widgets::SRSManagerDialog::SRSManagerDialog(QWidget* parent, Qt::WindowFlags f):
  QDialog(parent, f),
  m_ui(new Ui::SRSManagerDialogForm)
{
  m_ui->setupUi(this);
  m_selSrsId.first = -1;  
  m_selSrsId.second = "";
  
  // Signals & slots
  connect(m_ui->m_SRSTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), SLOT(onSRSTreeWidgetItemClicked(QTreeWidgetItem* , int)));
  connect(m_ui->m_moreInfoToolButton, SIGNAL(clicked()), SLOT(onMoreInfoToolButtonClicked())); 
  connect(m_ui->m_addSRSToolButton, SIGNAL(clicked()), SLOT(onAddSRSToolButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), SLOT(onCancelPushButtonClicked()));   
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), SLOT(onHelpPushButtonClicked())); 
  connect(m_ui->m_findSRSPushButton, SIGNAL(clicked()), SLOT(onFindSRSPushButtonClicked()));
  connect(m_ui->m_searchedSRSLineEdit, SIGNAL(textChanged(const QString&)),SLOT(onSearchLineEditTextChanged(const QString&)));
  
  QList<QTreeWidgetItem *> items;
//  QTreeWidgetItem* gs = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Geoographic Spatial Coordinate Systems")));
//  QTreeWidgetItem* ps = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Projected Spatial Coordinate Systems")));
//  QTreeWidgetItem* us = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("User Defined Coordinate Systems")));
//  gs->setExpanded(true);
//  ps->setExpanded(true);
//  us->setExpanded(true);
//  items.append(gs);
//  items.append(ps);
//  items.append(us);
  
  items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Geoographic Spatial Coordinate Systems"))));
  items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Projected Spatial Coordinate Systems"))));
  items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("User Defined Coordinate Systems"))));
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
          geog->setText(1, QString("%1").arg(its.first->m_auth_id));
          geog->setText(2, "EPSG");
        }
        else 
        {
          QTreeWidgetItem *proj = new QTreeWidgetItem(items[1]);
          proj->setText(0, its.first->m_name.c_str());
          proj->setText(1, QString("%1").arg(its.first->m_auth_id));
          proj->setText(2, "EPSG");
        }
      }
    else 
    {
      QTreeWidgetItem *userd = new QTreeWidgetItem(items[2]);
      userd->setText(0, its.first->m_name.c_str());
      userd->setText(1, QString("%1").arg(its.first->m_auth_id));
      userd->setText(2, its.first->m_auth_name.c_str());
    }
    ++its.first;
  }  
  m_ui->m_SRSTreeWidget->resizeColumnToContents(0);
  
  unsigned int ntl =  m_ui->m_SRSTreeWidget->topLevelItemCount();
  for(int i = 0; i < ntl; ++i)
  {
    QTreeWidgetItem* item = m_ui->m_SRSTreeWidget->topLevelItem(i);
    item->setExpanded(true);
  }
  
  m_srsDiag = new te::qt::widgets::SRSDialog(this); 
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
    return std::make_pair<int, std::string>(dlg.m_selSrsId.first, dlg.m_selSrsId.second);
  
  return std::make_pair<int, std::string>(TE_UNKNOWN_SRS, "");
}

void te::qt::widgets::SRSManagerDialog::onSearchLineEditTextChanged(const QString& text)
{
  QList<QTreeWidgetItem*> itens = m_ui->m_SRSTreeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive, 0);
  itens.append(m_ui->m_SRSTreeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive, 1));  
  filter(itens);
}


void te::qt::widgets::SRSManagerDialog::filter(const QList<QTreeWidgetItem*>& itens)
{
  for(int i = 0; i < m_ui->m_SRSTreeWidget->topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* item = m_ui->m_SRSTreeWidget->topLevelItem(i);
    
    for(int j = 0; j < item->childCount(); ++j)
    {
      QTreeWidgetItem* srs = item->child(j);
      bool hide = itens.indexOf(srs) == -1;
      srs->setHidden(hide);
    }
  }
  update();
}

void te::qt::widgets::SRSManagerDialog::onFindSRSPushButtonClicked() 
{
  QString itemText = m_ui->m_searchedSRSLineEdit->text();
  
  if (itemText.isEmpty())
  {
    QMessageBox::warning(this, "", tr("Enter a SRS name to search for."));
    return;   
  }
  
  QList<QTreeWidgetItem *> found = m_ui->m_SRSTreeWidget->findItems(itemText, Qt::MatchWildcard | Qt::MatchRecursive);
  
  if (!found.isEmpty())
  {
    QTreeWidgetItem *item = found[0];
    if (item->parent() != 0)
    {
      item->parent()->setExpanded(true);
      m_ui->m_SRSTreeWidget->setItemSelected(item, true);
      m_ui->m_SRSTreeWidget->setCurrentItem(item);
      return;
    }
  }
  
  QMessageBox::warning(this, "", tr("No SRS matching the searching string."));
  return;
}

void te::qt::widgets::SRSManagerDialog::onMoreInfoToolButtonClicked()
{
  QTreeWidgetItem* curItem = m_ui->m_SRSTreeWidget->currentItem();
  if (!curItem) 
  {
    QMessageBox::warning(this, "", tr("Select a spatial reference system."));
    return;
  }
  
  if (curItem->text(1).isEmpty())
  {
    QMessageBox::warning(this, "", tr("Select a spatial reference system."));
    return;
  }    
  unsigned int srid = curItem->text(1).toUInt();
  std::string auth = std::string(curItem->text(2).toLatin1());
  te::srs::SpatialReferenceSystemManager::getInstance().getName(srid,auth);
  
  m_srsDiag->setAddMode(false);  
  m_srsDiag->setSRSInformation(te::srs::SpatialReferenceSystemManager::getInstance().getName(srid,auth), 
                               te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(srid,auth), 
                               te::srs::SpatialReferenceSystemManager::getInstance().getWkt(srid,auth), 
                               auth,srid);
  m_srsDiag->exec();
}

void te::qt::widgets::SRSManagerDialog::onAddSRSToolButtonClicked()
{
  m_srsDiag->setAddMode(true); 
  if (m_srsDiag->exec() == QDialog::Accepted)
  {
    std::string name; 
    std::string p4Txt;
    std::string wkt;
    std::string authName;
    unsigned int srid = m_srsDiag->getSRSInformation(name,p4Txt,wkt,authName);
    
    QTreeWidgetItem *proj = new QTreeWidgetItem(m_ui->m_SRSTreeWidget->topLevelItem(2));
    proj->setText(0, name.c_str());
    proj->setText(1, QString("%1").arg(srid));
    proj->setText(2, authName.c_str());
    m_ui->m_SRSTreeWidget->setCurrentItem(proj);    
    m_ui->m_SRSTreeWidget->resizeColumnToContents(0);    
    return;
  }
}

void te::qt::widgets::SRSManagerDialog::onSRSTreeWidgetItemClicked(QTreeWidgetItem * item, int /*column*/)
{
  if (!item || item->text(1).isEmpty()) 
  {
    m_selSrsId.first = -1;  
    m_selSrsId.second = "";
    return;
  }
  m_selSrsId.first = item->text(1).toUInt();  
  m_selSrsId.second = std::string(item->text(2).toLatin1());
}

void te::qt::widgets::SRSManagerDialog::onOkPushButtonClicked()
{
  if (m_selSrsId.first == -1)
    reject();
  else 
    accept();
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


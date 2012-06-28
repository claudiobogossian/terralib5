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
  connect(m_ui->m_SRSTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onSRSTreeWidgetItemActivated(QTreeWidgetItem* , int)));
  connect(m_ui->m_moreInfoToolButton, SIGNAL(clicked()), SLOT(onMoreInfoToolButtonClicked())); 
  connect(m_ui->m_addSRSToolButton, SIGNAL(clicked()), SLOT(onAddSRSToolButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), SLOT(onCancelPushButtonClicked()));   
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), SLOT(onHelpPushButtonClicked())); 
  
  QList<QTreeWidgetItem *> items;
  items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Geoographic Spatial Coordinate Systems"))));
  items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Projected Spatial Coordinate Systems"))));
  items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("User Defined Coordinate Systems"))));
  m_ui->m_SRSTreeWidget->insertTopLevelItems(0, items);
  
  std::pair<std::vector<std::pair<std::string,unsigned int> >::const_iterator,
            std::vector<std::pair<std::string,unsigned int> >::const_iterator> iterId = te::srs::SpatialReferenceSystemManager::getInstance().getIteratorIds();
  
  std::pair<std::vector<std::string>::const_iterator,
            std::vector<std::string>::const_iterator> iterName = te::srs::SpatialReferenceSystemManager::getInstance().getIteratorNames();
  
  while (iterId.first != iterId.second) 
  {
      if (iterId.first->first == "EPSG")
      {
        if (iterId.first->second < 5000)
        {
          QTreeWidgetItem *geog = new QTreeWidgetItem(items[0]);
          geog->setText(0, (*(iterName.first)).c_str());
          geog->setText(1, QString("%1").arg(iterId.first->second));
          geog->setText(2, "EPSG");
        }
        else 
        {
          QTreeWidgetItem *proj = new QTreeWidgetItem(items[1]);
          proj->setText(0, (*(iterName.first)).c_str());
          proj->setText(1, QString("%1").arg(iterId.first->second));
          proj->setText(2, "EPSG");
        }
      }
    else 
    {
      QTreeWidgetItem *userd = new QTreeWidgetItem(items[2]);
      userd->setText(0, (*(iterName.first)).c_str());
      userd->setText(1, QString("%1").arg(iterId.first->second));
      userd->setText(2, (iterId.first->first).c_str());
    }
    ++iterId.first;
    ++iterName.first;
  }  
  
  m_ui->m_SRSTreeWidget->resizeColumnToContents(0);

  m_srsDiag = new te::qt::widgets::SRSDialog(this); 
}

te::qt::widgets::SRSManagerDialog::~SRSManagerDialog()
{
}

const std::pair<int, std::string>& te::qt::widgets::SRSManagerDialog::getSelectedSRS() const
{
  return m_selSrsId;
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

void te::qt::widgets::SRSManagerDialog::onSRSTreeWidgetItemActivated(QTreeWidgetItem * item, int /*column*/)
{
  if (!item || item->text(1).isEmpty()) 
    return;
  
  m_selSrsId.first = item->text(1).toUInt();  
  m_selSrsId.second = std::string(item->text(2).toLatin1());
}

void te::qt::widgets::SRSManagerDialog::onOkPushButtonClicked()
{
  accept();
}

void te::qt::widgets::SRSManagerDialog::onCancelPushButtonClicked()
{
  reject();
}

void te::qt::widgets::SRSManagerDialog::onHelpPushButtonClicked()
{
 QMessageBox::critical(this, "", tr("Not implemented yet! 3"));
}


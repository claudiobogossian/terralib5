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
  \file terralib/widgets/srs/SRSDialog.cpp

  \brief A dialog used to select the Spatial Reference Systems for coordinates.
*/

#include "SRSDialog.h"
#include <ui_SRSDialogForm.h>

#include <QtGui/QMessageBox>

// TerraLib
#include "../../../srs/SpatialReferenceSystemManager.h"

te::qt::widgets::SRSDialog::SRSDialog(QWidget* parent, Qt::WindowFlags f):
  QDialog(parent, f),
  m_ui(new Ui::SRSDialogForm),
  m_addMode(false)
{
  m_ui->setupUi(this);
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), SLOT(onCancelPushButtonClicked()));   
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), SLOT(onHelpPushButtonClicked())); 
}

te::qt::widgets::SRSDialog::~SRSDialog()
{  
}

void te::qt::widgets::SRSDialog::clear()
{
  m_ui->m_nameLineEdit->clear();
  m_ui->m_p4TxtLineEdit->clear();
  m_ui->m_wktLineEdit->clear();
  m_ui->m_idLineEdit->clear();
  m_ui->m_authIdLineEdit->clear();
}

unsigned int te::qt::widgets::SRSDialog::getSRSInformation(std::string& name, 
                                                           std::string& p4Txt,
                                                           std::string& wkt,
                                                           std::string& authName)
{
  name = std::string(m_ui->m_nameLineEdit->text().toLatin1());
  p4Txt = std::string(m_ui->m_p4TxtLineEdit->text().toLatin1());
  wkt = std::string(m_ui->m_wktLineEdit->text().toLatin1());
  authName = std::string(m_ui->m_authIdLineEdit->text().toLatin1());
  return m_ui->m_idLineEdit->text().toUInt();
}

void te::qt::widgets::SRSDialog::setSRSInformation(const std::string& name, 
                                                   const std::string& p4Txt, 
                                                   const std::string& wkt,
                                                   const std::string& authName,
                                                   unsigned int id)
{
  m_ui->m_nameLineEdit->setText(QString(name.c_str()));
  m_ui->m_p4TxtLineEdit->setText(QString(p4Txt.c_str()));
  m_ui->m_wktLineEdit->setText(QString(wkt.c_str()));
  m_ui->m_idLineEdit->setText(QString("%1").arg(id));
  m_ui->m_authIdLineEdit->setText(QString(authName.c_str()));
  
  m_ui->m_nameLineEdit->home(false);
  m_ui->m_p4TxtLineEdit->home(false);
  m_ui->m_wktLineEdit->home(false);
  m_ui->m_idLineEdit->home(false);
  m_ui->m_authIdLineEdit->home(false);
}

void te::qt::widgets::SRSDialog::onOkPushButtonClicked()
{
  if (!m_addMode)
  {
    accept();
    return;
  } 
  
  if (m_ui->m_nameLineEdit->text().isEmpty()) 
  {
    QMessageBox msgBox;
    msgBox.setText("The SRS name can not be empty().");
    msgBox.exec();
    return;
  }
  else if (m_ui->m_idLineEdit->text().isEmpty() || m_ui->m_authIdLineEdit->text().isEmpty()) 
  {
    QMessageBox msgBox;
    msgBox.setText("The SRS id and the authority name can not be empty.");
    msgBox.exec();
    return;
  }
  else if (m_ui->m_p4TxtLineEdit->text().isEmpty())
  {
    QMessageBox msgBox;
    msgBox.setText("The SRS PROJ4 description can not be empty.");
    msgBox.exec();
    return;
  }
  
  unsigned int srid = m_ui->m_idLineEdit->text().toUInt();
  std::string auth = std::string(m_ui->m_authIdLineEdit->text().toLatin1());
  
  if (te::srs::SpatialReferenceSystemManager::getInstance().recognizes(srid,auth))
  {
    QMessageBox msgBox;
    msgBox.setText("A SRS id, given by this authority, already exists.");
    msgBox.exec();
    return;
  }
  
  if (auth != "EPSG" && (srid > 0 && srid <= 32767))
  {
    QMessageBox msgBox;
    msgBox.setText("Code range from 0 to 32767 are reserved to EPSG authority.");
    msgBox.exec();
    return;
  }
      
  te::srs::SpatialReferenceSystemManager::getInstance().add(std::string(m_ui->m_nameLineEdit->text().toLatin1()), 
                                        std::string(m_ui->m_p4TxtLineEdit->text().toLatin1()), 
                                        std::string(m_ui->m_p4TxtLineEdit->text().toLatin1()), 
                                        srid,auth);

  accept();
}

void te::qt::widgets::SRSDialog::onCancelPushButtonClicked()
{
  clear();
  reject();
}

void te::qt::widgets::SRSDialog::setAddMode(bool flag)
{
  m_addMode = flag;
  if (m_addMode)
    m_ui->m_okPushButton->setText(tr("Add"));
  else 
    m_ui->m_okPushButton->setText(tr("Ok"));
  
  m_ui->m_nameLineEdit->clear();
  m_ui->m_idLineEdit->clear();
  m_ui->m_authIdLineEdit->clear();
}

void te::qt::widgets::SRSDialog::onHelpPushButtonClicked()
{
 QMessageBox::critical(this, "", tr("Not implemented yet! 3"));
}


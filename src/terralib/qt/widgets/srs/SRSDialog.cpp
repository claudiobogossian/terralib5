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
  \file terralib/widgets/srs/SRSDialog.cpp

  \brief A dialog used to select the Spatial Reference Systems for coordinates.
*/

#include "SRSDialog.h"

#include "../../../srs/SpatialReferenceSystemManager.h"

#include <ui_SRSDialogForm.h>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QString>

#include <iostream>

te::qt::widgets::SRSDialog::SRSDialog(QWidget* parent, Qt::WindowFlags f):
  QDialog(parent, f),
  m_ui(new Ui::SRSDialogForm),
  m_isUpdate(false)
{
  m_ui->setupUi(this);
  
  // Signals & slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), SLOT(onCancelPushButtonClicked()));
 
  m_ui->m_helpPushButton->setPageReference("widgets/srs/srs.html");
  
  m_ui->m_authority->setPlainText("USER");
  
  m_ui->m_srid->setPlainText(te::srs::SpatialReferenceSystemManager::getInstance().getNewUserDefinedSRID().c_str());
}


te::qt::widgets::SRSDialog::SRSDialog(QWidget* parent, Qt::WindowFlags f,
                                      unsigned int srid, const std::string& name, const std::string& p4desc):
  QDialog(parent, f),
  m_ui(new Ui::SRSDialogForm),
  m_isUpdate(true)
{
  QString sridt = QString("%1").arg(srid);
  m_ui->m_srid->setPlainText(sridt);
  m_ui->m_name->setPlainText(name.c_str());
  m_ui->m_p4desc->setPlainText(p4desc.c_str());
}


te::qt::widgets::SRSDialog::~SRSDialog()
{
  delete m_ui;
}

void te::qt::widgets::SRSDialog::onOkPushButtonClicked()
{
  if (m_ui->m_name->toPlainText().isEmpty())
  {
    QMessageBox::warning(this, tr("SRS Dialog"), tr("Inform a name for the new SRS."));
    return;
  }
  
  if (m_ui->m_p4desc->toPlainText().isEmpty())
  {
    QMessageBox::warning(this, tr("SRS Dialog"), tr("Inform the PROJ.4 description for the new SRS."));
    return;
  }
  
  bool exists = true;
  std::string name = m_ui->m_name->toPlainText().toStdString();
  try
  {
    std::pair<std::string,unsigned int> check = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromName(name);
  }
  catch(...)
  {
    exists = false;
  }
  
  if (exists & !m_isUpdate)
  {
    QMessageBox::warning(this, tr("SRS Dialog"), tr("There is already a SRS with the given name. Please inform a different name."));
    return;
  }
  
  name = m_ui->m_p4desc->toPlainText().toStdString();
  exists = true;
  try
  {
    std::pair<std::string,unsigned int> check = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(name);
  }
  catch (...)
  {
    exists = false;
  }
  
  if (exists & !m_isUpdate)
  {
    QMessageBox::warning(this, tr("SRS Dialog"), tr("There is already a SRS with the PROJ.4 description. Please inform a different description."));
    return;
  }
  
  QString proj4 = m_ui->m_p4desc->toPlainText();
  
  QRegExp rx("[, ]");// match a comma or a space
  QStringList list = proj4.split(rx, QString::SkipEmptyParts);
  
  if (list.size() < 2 ||
      (!list[0].contains("+proj=",Qt::CaseInsensitive) &&
       !list[0].contains("+proj =",Qt::CaseInsensitive)))
  {
    QMessageBox::warning(this, tr("SRS Dialog"), tr("PROJ.4 description is not valid."));
    return;
  }
	 accept();
}

void te::qt::widgets::SRSDialog::onCancelPushButtonClicked()
{
  reject();
}

unsigned int te::qt::widgets::SRSDialog::getSRID() const
{
  std::string id = m_ui->m_srid->toPlainText().toStdString();
  return boost::lexical_cast<unsigned int >(id);
}

std::string te::qt::widgets::SRSDialog::getName() const
{
  return m_ui->m_name->toPlainText().toStdString();
}

std::string te::qt::widgets::SRSDialog::getP4Desc() const
{
  return m_ui->m_p4desc->toPlainText().toStdString();
}
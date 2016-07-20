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
  \file terralib/tools/dscopy/qt/DSCopyDialog.cpp

  \brief An utility class to provide methods to copy a Data Source
 */

// DSCopy
#include "DSCopyDialog.h"
#include "DSCopyDialogController.h"

// Qt
#include <QtGui/QtGui>
#include <QtGui/QComboBox>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QListView>

// STL
#include <string>

te::tools::dscopy::DSCopyDialog::DSCopyDialog(QWidget * parent, Qt::WindowFlags f)
  : QDialog(parent, f)
{
  setupUi(this);

  m_controller = new DSCopyDialogController();

  backPushButton->setEnabled(false);

  inComboBox->addItem("");
  outComboBox->addItem("");

  mainStackedWidget->setCurrentIndex(0);

  connStackedWidget->setCurrentIndex(0);
  connOutStackedWidget->setCurrentIndex(0);

  std::vector<std::string> dictionary = m_controller->getDictionary();

  for(size_t i = 0; i < dictionary.size(); i++)
  {
    inComboBox->addItem(dictionary[i].c_str());
    outComboBox->addItem(dictionary[i].c_str());
  }
  
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButton_clicked()));
  connect(backPushButton, SIGNAL(clicked()), this, SLOT(backPushButton_clicked()));
  connect(nextPushButton, SIGNAL(clicked()), this, SLOT(nextPushButton_clicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButton_clicked()));

  connect(inComboBox, SIGNAL(activated(const QString&)), this, SLOT(inComboBox_activated(const QString&)));
  connect(outComboBox, SIGNAL(activated(const QString&)), this, SLOT(outComboBox_activated(const QString&)));

  connect(inPathPushButton, SIGNAL(clicked()), this, SLOT(inPathPushButton_clicked()));
  connect(outPathPushButton, SIGNAL(clicked()), this, SLOT(outPathPushButton_clicked()));
}

te::tools::dscopy::DSCopyDialog::~DSCopyDialog()
{
}

void te::tools::dscopy::DSCopyDialog::helpPushButton_clicked()
{

}

void te::tools::dscopy::DSCopyDialog::backPushButton_clicked()
{
  if(mainStackedWidget->currentIndex() == 2)
  {
    nextPushButton->setText(tr("&Next"));
    mainStackedWidget->setCurrentIndex(1);
  }
  else if(mainStackedWidget->currentIndex() == 1)
  {
    backPushButton->setEnabled(false);
    mainStackedWidget->setCurrentIndex(0);
  }
}

void te::tools::dscopy::DSCopyDialog::nextPushButton_clicked()
{
  std::string errorMessage = std::string();

  // Input Data Source Connection Informations
  if(mainStackedWidget->currentIndex() == 0)
  {
    dsListWidget->clear();

    if(inComboBox->currentText() == "")
      return;

    if(connStackedWidget->currentIndex() == 1)
    {
      if(!inPathLineEdit->text().isEmpty())
      {
        if(!m_controller->setOriginConnectionInfo(inComboBox->currentText().toStdString(), inPathLineEdit->text().toStdString(), errorMessage))
        {
          QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
          return;
        }
      }
      else
      {
        QMessageBox::warning(this, tr("Warning"), tr("Select a path!"));
        return;
      }
    }    
    else if(connStackedWidget->currentIndex() == 2)
    {
      if(hostLineEdit->text().isEmpty() || portLineEdit->text().isEmpty() 
        || dsNameLineEdit->text().isEmpty() || userLineEdit->text().isEmpty() || pwLineEdit->text().isEmpty())
      {
        QString warning = tr("Some required information is missing:");
        if(hostLineEdit->text().isEmpty())
          warning += tr("\n - Host");
        if(portLineEdit->text().isEmpty())
          warning += tr("\n - Port");
        if(dsNameLineEdit->text().isEmpty())
          warning += tr("\n - Data Source Name");
        if(userLineEdit->text().isEmpty())
          warning += tr("\n - User");
        if(pwLineEdit->text().isEmpty())
          warning += tr("\n - Password");

        QMessageBox::warning(this, tr("Warning"), warning);
        return;
      }
      else
      {

        if(!m_controller->setOriginConnectionInfo(inComboBox->currentText().toStdString(), 
                                              hostLineEdit->text().toStdString(),
                                              portLineEdit->text().toStdString(),
                                              dsNameLineEdit->text().toStdString(),
                                              userLineEdit->text().toStdString(),
                                              pwLineEdit->text().toStdString(), errorMessage))
        {
          QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
          return;
        }

      }
    }


    std::vector<std::string*> dataSetsNames = m_controller->getDatasetsName();


    for(size_t i = 0; i < dataSetsNames.size(); i++)
    {
      dsListWidget->addItem(new QListWidgetItem((*dataSetsNames[i]).c_str()));
    }
    
    mainStackedWidget->setCurrentIndex(1);

  }

  // DataSets informations
  else if(mainStackedWidget->currentIndex() == 1)
  {
    if(dsListWidget->selectedItems().empty())
    { 
      QMessageBox::warning(this, tr("Warnign"), tr("Select at least one Data Set"));
      return;
    }

    std::vector<std::string> datasetsToCopy;
    for(size_t i = 0; i < dsListWidget->selectedItems().count(); i++)
    {
      datasetsToCopy.push_back(dsListWidget->selectedItems().at(i)->text().toStdString());
    }

    m_controller->setDatasetsToCopy(datasetsToCopy);

    mainStackedWidget->setCurrentIndex(2);
    
    nextPushButton->setText(tr("Copy"));
  }

  // Make the copy
  else if(mainStackedWidget->currentIndex() == 2)
  {
    if(outComboBox->currentText() == "")
      return;

    std::string errorMessage = std::string();

    if(connOutStackedWidget->currentIndex() == 1)
    {
      if(!outPathLineEdit->text().isEmpty())
      {
        if(!m_controller->setDestinationConnectionInfo(outComboBox->currentText().toStdString(), outPathLineEdit->text().toStdString(), errorMessage))
        {
          QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
          return;
        }
      }
      else
      {
        QMessageBox::warning(this, tr("Warning"), tr("Select a path!"));
        return;
      }
    }
    else if(connOutStackedWidget->currentIndex() == 2)
    {
      if(outHostLineEdit->text().isEmpty() || outPortLineEdit->text().isEmpty() 
        || outDsNameLineEdit->text().isEmpty() || outUserLineEdit->text().isEmpty() || outPwLineEdit->text().isEmpty())
      {
        QString warning = tr("Some required information is missing:");
        if(outHostLineEdit->text().isEmpty())
          warning += tr("\n - Host");
        if(outPortLineEdit->text().isEmpty())
          warning += tr("\n - Port");
        if(outDsNameLineEdit->text().isEmpty())
          warning += tr("\n - Data Source Name");
        if(outUserLineEdit->text().isEmpty())
          warning += tr("\n - User");
        if(outPwLineEdit->text().isEmpty())
          warning += tr("\n - Password");

        QMessageBox::warning(this, tr("Warning"), warning);
        return;
      }
      else
      {

        if(!m_controller->setDestinationConnectionInfo(outComboBox->currentText().toStdString(), 
                                              outHostLineEdit->text().toStdString(),
                                              outPortLineEdit->text().toStdString(),
                                              outDsNameLineEdit->text().toStdString(),
                                              outUserLineEdit->text().toStdString(),
                                              outPwLineEdit->text().toStdString(), errorMessage))
        {
          QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
          return;
        }

      }
    }

    if(!m_controller->copy(errorMessage))
    {
      QMessageBox::warning(this, tr("ERROR"), errorMessage.c_str());
      return;
    }
    else
      QMessageBox::information(this, tr("Success"), tr("Copy executed successfully!"));

  }

  backPushButton->setEnabled(true);

}

void te::tools::dscopy::DSCopyDialog::cancelPushButton_clicked()
{
  close();
}

void te::tools::dscopy::DSCopyDialog::inComboBox_activated(const QString& type)
{
  if(type == "OGR" || type == "GDAL")
    connStackedWidget->setCurrentIndex(1);
  else if(type == "POSTGIS")
    connStackedWidget->setCurrentIndex(2);
  else
    connStackedWidget->setCurrentIndex(0);
}

void te::tools::dscopy::DSCopyDialog::outComboBox_activated(const QString& type)
{
  if(type == "OGR" || type == "GDAL")
    connOutStackedWidget->setCurrentIndex(1);
  else if(type == "POSTGIS")
    connOutStackedWidget->setCurrentIndex(2);
  else
    connOutStackedWidget->setCurrentIndex(0);
}

void te::tools::dscopy::DSCopyDialog::inPathPushButton_clicked()
{
  std::string path;

  QFileDialog fd(this);
  fd.setFileMode(QFileDialog::DirectoryOnly);

  path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks).toStdString();

  if(!path.empty())
    if(connStackedWidget->currentIndex() == 1)
      inPathLineEdit->setText(path.c_str());

}

void te::tools::dscopy::DSCopyDialog::outPathPushButton_clicked()
{
  std::string path;

  QFileDialog fd(this);
  fd.setFileMode(QFileDialog::DirectoryOnly);

  path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks).toStdString();

  if(!path.empty())
    if(connOutStackedWidget->currentIndex() == 1)
      outPathLineEdit->setText(path.c_str());
}

void te::tools::dscopy::DSCopyDialog::closeEvent(QCloseEvent* /*e*/)
{
  delete m_controller;
}
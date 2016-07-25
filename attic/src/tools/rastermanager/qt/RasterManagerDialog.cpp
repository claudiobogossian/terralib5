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
  \file terralib/tools/rastermanager/qt/RasterManagerDialog.cpp

  \brief Raster Manager GUI
 */

// Raster Manager
#include "RasterManagerDialog.h"
#include "../core/Utils.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/raster.h>

// Qt
#include <QtGui/QtGui>

// Boost
#include <boost/filesystem.hpp>

void setWaitCursor(QWidget* w)
{
  
  QCursor c(Qt::CursorShape::WaitCursor);
  w->setCursor(c);
}

void setReleaseCursor(QWidget* w)
{
  QCursor c(Qt::CursorShape::ArrowCursor);
  w->setCursor(c);
}

te::tools::rastermanager::RasterManagerDialog::RasterManagerDialog(QWidget * parent, Qt::WindowFlags f)
  : QDialog(parent, f)
{
  setupUi(this);

  m_rm = new RasterManager();

  copyBandTypeComboBox->setEnabled(false);

  // Validators
  sridLineEdit->setValidator(new QIntValidator());
  llxLineEdit->setValidator(new QDoubleValidator());
  llyLineEdit->setValidator(new QDoubleValidator());
  urxLineEdit->setValidator(new QDoubleValidator());
  uryLineEdit->setValidator(new QDoubleValidator());

  connect(functionsTabWidget, SIGNAL(currentChanged(int)), this, SLOT(functionsTabWidget_currentChanged(int)));

  connect(inputPushButton, SIGNAL(clicked()), this, SLOT(inputPushButton_clicked()));
  connect(outputPushButton, SIGNAL(clicked()), this, SLOT(outputPushButton_clicked()));

  connect(copyPushButton, SIGNAL(clicked()), this, SLOT(copyPushButton_clicked()));
  connect(reprojectPushButton, SIGNAL(clicked()), this, SLOT(reprojectPushButton_clicked()));
  connect(convertPushButton, SIGNAL(clicked()), this, SLOT(convertPushButton_clicked()));
  connect(resolutionPushButton, SIGNAL(clicked()), this, SLOT(resolutionPushButton_clicked()));
  connect(trimPushButton, SIGNAL(clicked()), this, SLOT(trimPushButton_clicked()));  
}

te::tools::rastermanager::RasterManagerDialog::~RasterManagerDialog()
{
}

void te::tools::rastermanager::RasterManagerDialog::inputPushButton_clicked()
{  
  std::string file = QFileDialog::getOpenFileName(this, tr("Open Raster"), "", "").toStdString();

  if(file.empty())
    return;

  inputLineEdit->setText(file.c_str());

  std::string errorMessage;
  if(!m_rm->init(file, errorMessage))
  {
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  } 

  std::ostringstream output;
  if(!m_rm->getRasterInfo(output, errorMessage))
  {
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  infoTextEdit->setText(output.str().c_str());

  te::rst::Raster* inputRaster;
  if(!Utils::getRaster(file, inputRaster, errorMessage))
  {
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  // Copy
  {
    copyBandsListWidget->clear();
    for(size_t i = 0; i < inputRaster->getNumberOfBands(); i++)
      copyBandsListWidget->addItem(te::common::Convert2String(i).c_str());
  }
  
  // Convert
  {
    std::vector<std::string> extents;
    m_rm->getSupportedExtensions(extents, errorMessage);
    for(size_t i = 0; i < extents.size(); i++)
      extensionComboBox->addItem(extents[i].c_str());
  }

  // Resolution
  {
    methodComboBox->addItem("");
    methodComboBox->addItem(tr("Nearest Neighbor"));
    methodComboBox->addItem(tr("Bilinear method"));
    methodComboBox->addItem(tr("Bicubic method"));
  }
}

// Copy
void te::tools::rastermanager::RasterManagerDialog::outputPushButton_clicked()
{

  std::string extension = boost::filesystem3::extension(inputLineEdit->text().toStdString());
  std::string inName = boost::filesystem3::basename(inputLineEdit->text().toStdString());
  inName += extension;
  
  std::string file = QFileDialog::getSaveFileName(this, tr("Save Raster"), inName.c_str(), "").toStdString();

  if(file.empty())
    return;
    
  outputLineEdit->setText(file.c_str()); 
}

void te::tools::rastermanager::RasterManagerDialog::copyPushButton_clicked()
{
  if(outputLineEdit->text().isEmpty() || inputLineEdit->text().isEmpty())
    return;

  setWaitCursor(this);

  std::string errorMessage;
  if(!m_rm->init(inputLineEdit->text().toStdString(), errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());    
    return;
  }
  
  std::vector<int> bands;
  size_t it = 0;
  while(it != copyBandsListWidget->selectedItems().count())
  {
    bands.push_back(atoi(copyBandsListWidget->selectedItems()[it]->text().toLatin1()));

    ++it;
  }

  if(!m_rm->copyRaster(outputLineEdit->text().toStdString(), bands, int(), errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }
  
  setReleaseCursor(this);
  QMessageBox::information(this, tr("Copy Executed"), tr("Copy Executed Satisfactorily!"));

}

void te::tools::rastermanager::RasterManagerDialog::reprojectPushButton_clicked()
{
  if(outputLineEdit->text().isEmpty() || inputLineEdit->text().isEmpty())
    return;

  setWaitCursor(this);

  std::string errorMessage;
  if(!m_rm->init(inputLineEdit->text().toStdString(), errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }
  
  if(!m_rm->reproject(outputLineEdit->text().toStdString(), atoi(sridLineEdit->text().toLatin1()), errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  setReleaseCursor(this);
  QMessageBox::information(this, tr("Reproject Executed"), tr("Reproject Executed Satisfactorily!"));
}

void te::tools::rastermanager::RasterManagerDialog::convertPushButton_clicked()
{
  if(inputLineEdit->text().isEmpty())
    return;

  setWaitCursor(this);

  std::string errorMessage;
  if(!m_rm->init(inputLineEdit->text().toStdString(), errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }  
    
  if(!m_rm->convert(outputLineEdit->text().toStdString(), extensionComboBox->currentText().toStdString(), 
    std::vector<int>(), errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  setReleaseCursor(this);
  QMessageBox::information(this, tr("Conversion Executed"), tr("Conversion Executed Satisfactorily!"));
}

void te::tools::rastermanager::RasterManagerDialog::resolutionPushButton_clicked()
{
  if(outputLineEdit->text().isEmpty() || inputLineEdit->text().isEmpty())
    return;

  setWaitCursor(this);

  if(methodComboBox->currentIndex() == 0)
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), tr("Select a Method!"));
    return;
  }

  std::string errorMessage;
  if(!m_rm->init(inputLineEdit->text().toStdString(), errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  if(!m_rm->changeResolution(outputLineEdit->text().toStdString(), methodComboBox->currentIndex(), 
    scaleSpinBox->value(), errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  setReleaseCursor(this);
  QMessageBox::information(this, tr("Change Resolution Executed"), tr("Resolution Changed Satisfactorily!"));

}

void te::tools::rastermanager::RasterManagerDialog::trimPushButton_clicked()
{
  if(outputLineEdit->text().isEmpty() || inputLineEdit->text().isEmpty())
    return;

  setWaitCursor(this);

  if(llxLineEdit->text().isEmpty() || llyLineEdit->text().isEmpty() 
    || urxLineEdit->text().isEmpty() || uryLineEdit->text().isEmpty())
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), tr("Complete the required parameters!"));
    return;
  }

  std::string errorMessage;
  if(!m_rm->init(inputLineEdit->text().toStdString(), errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  std::vector<double> env;
  env[0] = atof(llxLineEdit->text().toLatin1());
  env[1] = atof(llyLineEdit->text().toLatin1());
  env[2] = atof(urxLineEdit->text().toLatin1());
  env[3] = atof(uryLineEdit->text().toLatin1());

  if(!m_rm->trim(outputLineEdit->text().toStdString(), env, errorMessage))
  {
    setReleaseCursor(this);
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  setReleaseCursor(this);
  QMessageBox::information(this, tr("Trim Executed"), tr("Raster Trimmed Satisfactorily!"));

}

void te::tools::rastermanager::RasterManagerDialog::functionsTabWidget_currentChanged(int id)
{
  if(id == 3)
    outputGroupBox->setEnabled(false);
  else
    if(!outputGroupBox->isEnabled())
      outputGroupBox->setEnabled(true);
}

void te::tools::rastermanager::RasterManagerDialog::closeEvent(QCloseEvent* /*e*/)
{
}
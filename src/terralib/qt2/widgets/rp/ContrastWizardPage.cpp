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
  \file terralib/qt/widgets/rp/ContrastWizardPage.cpp

  \brief This file defines a class for a Contrast Wizard page.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess2/dataset/DataSet.h"
#include "../../../dataaccess2/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "ContrastWizardPage.h"
#include "RasterNavigatorWidget.h"
#include "ui_ContrastWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QCheckBox>

// Boost
#include <boost/lexical_cast.hpp>

te::qt::widgets::ContrastWizardPage::ContrastWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ContrastWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillContrastTypes();

//build form
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_frame);
  m_navigator.reset( new te::qt::widgets::RasterNavigatorWidget(m_ui->m_frame));
  m_navigator->showAsPreview(true);
  displayLayout->addWidget(m_navigator.get());
  displayLayout->setContentsMargins(0,0,0,0);

//connects
  connect(m_ui->m_histogramPushButton, SIGNAL(clicked()), this, SLOT(showHistogram()));
  connect(m_ui->m_contrastTypeComboBox, SIGNAL(activated(int)), this, SLOT(onContrastTypeComboBoxActivated(int)));

//configure page
  this->setTitle(tr("Contrast"));
  this->setSubTitle(tr("Select the type of contrast and set their specific parameters."));

  onContrastTypeComboBoxActivated(m_ui->m_contrastTypeComboBox->currentIndex());
}

te::qt::widgets::ContrastWizardPage::~ContrastWizardPage()
{
}

void te::qt::widgets::ContrastWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  m_navigator->set(m_layer);

  listBands();
}

te::rp::Contrast::InputParameters te::qt::widgets::ContrastWizardPage::getInputParams()
{
  //get contrast type
  int index = m_ui->m_contrastTypeComboBox->currentIndex();
  int contrastType = m_ui->m_contrastTypeComboBox->itemData(index).toInt();
  int nBands = m_ui->m_bandTableWidget->rowCount();

  te::rp::Contrast::InputParameters algoInputParams;

  if(contrastType == te::rp::Contrast::InputParameters::LinearContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::LinearContrastT;

    for(int i = 0; i < nBands; ++i)
    {
      QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
    
      if(checkBox->isChecked())
      {
        QString valueMin = m_ui->m_bandTableWidget->item(i, 1)->text();
        algoInputParams.m_lCMinInput.push_back(valueMin.toDouble());

        QString valueMax = m_ui->m_bandTableWidget->item(i, 2)->text();
        algoInputParams.m_lCMaxInput.push_back(valueMax.toDouble());
      }
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::HistogramEqualizationContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::HistogramEqualizationContrastT;

    for(int i = 0; i < nBands; ++i)
    {
      QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
    
      if(checkBox->isChecked())
      {
        QString valueMax = m_ui->m_bandTableWidget->item(i, 1)->text();
        algoInputParams.m_hECMaxInput.push_back(valueMax.toDouble());
      }
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::SetMeanAndStdContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::SetMeanAndStdContrastT;

    for(int i = 0; i < nBands; ++i)
    {
      QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
    
      if(checkBox->isChecked())
      {
        QString valueMean = m_ui->m_bandTableWidget->item(i, 1)->text();
        algoInputParams.m_sMASCMeanInput.push_back(valueMean.toDouble());

        QString valueStdDev = m_ui->m_bandTableWidget->item(i, 2)->text();
        algoInputParams.m_sMASCStdInput.push_back(valueStdDev.toDouble());
      }
    }
  }

  for(int i = 0; i < nBands; ++i)
  {
    QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
    
    if(checkBox->isChecked())
    {
      algoInputParams.m_inRasterBands.push_back(i);
    }
  }

  return algoInputParams;
}

void te::qt::widgets::ContrastWizardPage::apply()
{
  //get input raster
  te::da::DataSet* ds = m_layer->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);

  te::rst::Raster* inputRst = ds->getRaster(rpos);

  //get current box
  te::gm::Envelope extent = m_navigator->getCurrentExtent();

  //set contrast parameters
  te::rp::Contrast::InputParameters algoInputParams = getInputParams();

  algoInputParams.m_inRasterPtr = inputRst;

  te::rp::Contrast::OutputParameters algoOutputParams;

  std::map<std::string, std::string> rinfo;
  rinfo["MEM_RASTER_NROWS"] = "100";
  rinfo["MEM_RASTER_NCOLS"] = "100";
  rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>((int) te::dt::UCHAR_TYPE);
  rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfBands());

  algoOutputParams.m_createdOutRasterDSType = "MEM";
  algoOutputParams.m_createdOutRasterInfo = rinfo;

  //run contrast
  te::rp::Contrast algorithmInstance;

  if(algorithmInstance.initialize(algoInputParams))
  {
    if(algorithmInstance.execute(algoOutputParams))
    {
      QMessageBox::information(this, tr("Contrast"), tr("Contrast enhencement ended sucessfully"));
    }
    else
    {
      QMessageBox::critical(this, tr("Contrast"), tr("Contrast enhencement execution error"));
    }
  }
  else
  {
    QMessageBox::critical(this, tr("Contrast"), tr("Contrast enhencement initialization error"));
  }

  //delete input raster dataset
  delete ds;
}

void te::qt::widgets::ContrastWizardPage::preview()
{
  if(m_ui->m_previewGroupBox->isChecked())
  {
    apply();


  }
}

void te::qt::widgets::ContrastWizardPage::fillContrastTypes()
{
  m_ui->m_contrastTypeComboBox->clear();

  m_ui->m_contrastTypeComboBox->addItem(tr("Linear"), te::rp::Contrast::InputParameters::LinearContrastT);
  m_ui->m_contrastTypeComboBox->addItem(tr("Histogram Equalization"), te::rp::Contrast::InputParameters::HistogramEqualizationContrastT);
  m_ui->m_contrastTypeComboBox->addItem(tr("Mean and Standard Deviation"), te::rp::Contrast::InputParameters::SetMeanAndStdContrastT);
}

void te::qt::widgets::ContrastWizardPage::listBands()
{
  assert(m_layer.get());

  //get input raster
  te::da::DataSet* ds = m_layer->getData();

  if(ds)
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);

    te::rst::Raster* inputRst = ds->getRaster(rpos);

    if(inputRst)
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
         m_ui->m_bandTableWidget->setRowCount(0);

        // initializing the list of bands
        for(unsigned b = 0 ; b < inputRst->getNumberOfBands(); b++)
        {
          int newrow = m_ui->m_bandTableWidget->rowCount();
          m_ui->m_bandTableWidget->insertRow(newrow);

          QString bName(tr("Band "));
          bName.append(QString::number(b));
        
          QCheckBox* bandCheckBox = new QCheckBox(bName, this);

          m_ui->m_bandTableWidget->setCellWidget(newrow, 0, bandCheckBox);
        }
      }
    }
  }

  m_ui->m_bandTableWidget->resizeColumnsToContents();
  m_ui->m_bandTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

  onContrastTypeComboBoxActivated(m_ui->m_contrastTypeComboBox->currentIndex());

  delete ds;
}

void te::qt::widgets::ContrastWizardPage::showHistogram()
{
  assert(m_layer.get());


  //get input raster
  //te::da::DataSet* ds = m_layer->getData();

  //if(ds)
  //{

  //}

  //delete ds;
}

void te::qt::widgets::ContrastWizardPage::onContrastTypeComboBoxActivated(int index)
{
  int contrastType = m_ui->m_contrastTypeComboBox->itemData(index).toInt();

  if(contrastType == te::rp::Contrast::InputParameters::LinearContrastT)
  {
    QStringList list;
    list.append(tr("Band"));
    list.append(tr("Minimum"));
    list.append(tr("Maximum"));

    m_ui->m_bandTableWidget->setColumnCount(3);
    m_ui->m_bandTableWidget->setHorizontalHeaderLabels(list);

    int nBands = m_ui->m_bandTableWidget->rowCount();

    for(int i = 0; i < nBands; ++i)
    {
      QTableWidgetItem* itemMin = new QTableWidgetItem("0");
      itemMin->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
      m_ui->m_bandTableWidget->setItem(i, 1, itemMin);

      QTableWidgetItem* itemMax = new QTableWidgetItem("255");
      itemMax->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
      m_ui->m_bandTableWidget->setItem(i, 2, itemMax);
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::HistogramEqualizationContrastT)
  {
    QStringList list;
    list.append(tr("Band"));
    list.append(tr("Maximum"));

    m_ui->m_bandTableWidget->setColumnCount(2);
    m_ui->m_bandTableWidget->setHorizontalHeaderLabels(list);

    int nBands = m_ui->m_bandTableWidget->rowCount();

    for(int i = 0; i < nBands; ++i)
    {
      QTableWidgetItem* itemMax = new QTableWidgetItem("0");
      itemMax->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
      m_ui->m_bandTableWidget->setItem(i, 1, itemMax);
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::SetMeanAndStdContrastT)
  {
    QStringList list;
    list.append(tr("Band"));
    list.append(tr("Mean"));
    list.append(tr("Std Dev"));

    m_ui->m_bandTableWidget->setColumnCount(3);
    m_ui->m_bandTableWidget->setHorizontalHeaderLabels(list);

    int nBands = m_ui->m_bandTableWidget->rowCount();

    for(int i = 0; i < nBands; ++i)
    {
      QTableWidgetItem* itemMean = new QTableWidgetItem("0");
      itemMean->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
      m_ui->m_bandTableWidget->setItem(i, 1, itemMean);

      QTableWidgetItem* itemStdDev = new QTableWidgetItem("4");
      itemStdDev->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
      m_ui->m_bandTableWidget->setItem(i, 2, itemStdDev);
    }
  }

  m_ui->m_bandTableWidget->resizeColumnsToContents();
  m_ui->m_bandTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

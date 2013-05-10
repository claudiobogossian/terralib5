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
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
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

//configure page
  this->setTitle(tr("Contrast"));
  this->setSubTitle(tr("Select the type of contrast and set their specific parameters."));
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

  te::rp::Contrast::InputParameters algoInputParams;

  if(contrastType == te::rp::Contrast::InputParameters::LinearContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::LinearContrastT;
    algoInputParams.m_lCMinInput.resize( m_ui->m_rComboBox->count(),
      m_ui->m_lCMinInputLineEdit->text().toDouble() );
    algoInputParams.m_lCMaxInput.resize( m_ui->m_rComboBox->count(),
      m_ui->m_lCMaxInputLineEdit->text().toDouble() );
  }
  else if(contrastType == te::rp::Contrast::InputParameters::HistogramEqualizationContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::HistogramEqualizationContrastT;
    algoInputParams.m_hECMaxInput.resize( m_ui->m_rComboBox->count(),
      m_ui->m_hECMaxInputLineEdit->text().toDouble() );
  }
  else if(contrastType == te::rp::Contrast::InputParameters::SetMeanAndStdContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::SetMeanAndStdContrastT;
    algoInputParams.m_sMASCMeanInput.resize( m_ui->m_rComboBox->count(),
      m_ui->m_sMASCMeanInputLineEdit->text().toDouble() );
    algoInputParams.m_sMASCStdInput.resize( m_ui->m_rComboBox->count(),
      m_ui->m_sMASCStdInputLineEdit->text().toDouble() );
  }


 // algoInputParams.m_inRasterBands.push_back(m_ui->m_mComboBox->currentText().toUInt());


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
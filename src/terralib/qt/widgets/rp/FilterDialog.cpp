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
  \file terralib/qt/widgets/rp/FilterDialog.cpp

  \brief This file has the FilterDialog class.
*/

// TerraLib
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../rp/Filter.h"
#include "../../../rp/Module.h"
#include "../canvas/MapDisplay.h"
#include "../layer/search/LayerSelectorWidget.h"
#include "../progress/ProgressViewerDialog.h"
#include "FilterDialog.h"
#include "MaskDialog.h"
#include "OutputRasterParametersWidget.h"
#include "RasterNavigatorWidget.h"
#include "Utils.h"
#include "ui_FilterDialogForm.h"

//QT
#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>

//Boost
#include <boost/lexical_cast.hpp>

te::qt::widgets::FilterDialog::FilterDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::FilterDialogForm)
{
//build form
  m_ui->setupUi(this);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("raster-processing-filter-hint").pixmap(112,48));
  m_ui->m_maskToolButton->setIcon(QIcon::fromTheme("mask"));
  m_ui->m_loadMaskToolButton->setIcon(QIcon::fromTheme("mask-fill"));

  m_ui->m_maskDefaultValueLineEdit->setValidator(new QIntValidator(this));

  //layer selector
  QGridLayout* layoutLayer = new QGridLayout(m_ui->m_inputLayerWidget);
  m_layerSelector.reset( new te::qt::widgets::LayerSelectorWidget(m_ui->m_inputLayerWidget));
  layoutLayer->addWidget(m_layerSelector.get(), 0, 0);
  layoutLayer->setContentsMargins(0,0,0,0);

  //output raster info
  QGridLayout* layoutRaster = new QGridLayout(m_ui->m_ouputRasterInfoWidget);
  m_outputRaster.reset( new te::qt::widgets::OutputRasterParametersWidget(m_ui->m_ouputRasterInfoWidget));
  layoutRaster->addWidget(m_outputRaster.get(), 0, 0);
  layoutRaster->setContentsMargins(0,0,0,0);

  //preview
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_previewWidget);
  m_navigator.reset(new te::qt::widgets::RasterNavigatorWidget(m_ui->m_previewWidget));
  m_navigator->showAsPreview(true, false);
  m_navigator->hideColorCompositionTool(true);
  displayLayout->addWidget(m_navigator.get());
  displayLayout->setContentsMargins(0,0,0,0);

  //connects
  connect(m_ui->m_typeComboBox, SIGNAL(activated(int)), SLOT(onFilterTypeComboBoxActivated(int)));
  connect(m_ui->m_maskToolButton, SIGNAL(clicked()), SLOT(onMaskToolButtonClicked()));
  connect(m_ui->m_loadMaskToolButton, SIGNAL(clicked()), SLOT(onLoadMaskToolButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), SLOT(onOkPushButtonClicked()));
  connect(m_layerSelector.get(), SIGNAL(layerSelected(te::map::AbstractLayerPtr)), SLOT(onLayerSelected(te::map::AbstractLayerPtr)));
  connect(m_navigator.get(), SIGNAL(previewClicked()), this, SLOT(apply()));

  listFilterTypes();
}

te::qt::widgets::FilterDialog::~FilterDialog()
{
}

void te::qt::widgets::FilterDialog::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSelector->setList(layerList, te::qt::widgets::LayerSelectorWidget::FILTER_BY_RASTER);
}

te::map::AbstractLayerPtr te::qt::widgets::FilterDialog::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::FilterDialog::onFilterTypeComboBoxActivated(int index)
{
  int filterType = m_ui->m_typeComboBox->itemData(index).toInt();

  bool flag = filterType == te::rp::Filter::InputParameters::UserDefinedWindowT;

  m_ui->m_maskSizeSpinBox->setEnabled(flag);
  m_ui->m_maskToolButton->setEnabled(flag);
  m_ui->m_maskDefaultValueLineEdit->setEnabled(flag);
}

void te::qt::widgets::FilterDialog::onMaskToolButtonClicked()
{
  te::qt::widgets::MaskDialog dlg(this);

  dlg.setMaskSize(m_ui->m_maskSizeSpinBox->value(), 
    m_ui->m_maskDefaultValueLineEdit->text().isEmpty() ? 0 : m_ui->m_maskDefaultValueLineEdit->text().toInt());

  if(dlg.exec() == QDialog::Accepted)
  {
    m_window = dlg.getMatrix();

    m_ui->m_loadMaskToolButton->setEnabled(true);
  }
}

void te::qt::widgets::FilterDialog::onLoadMaskToolButtonClicked()
{
  te::qt::widgets::MaskDialog dlg(this);

  dlg.setMaskSize(m_window);

  m_ui->m_maskSizeSpinBox->setValue(m_window.size1());

  if(dlg.exec() == QDialog::Accepted)
  {
    m_window = dlg.getMatrix();
  }
}

void te::qt::widgets::FilterDialog::onLayerSelected(te::map::AbstractLayerPtr l)
{
  m_layer = l;

  m_navigator->set(m_layer, true);

  m_navigator->getDisplay()->refresh();

  listBands();
}

void te::qt::widgets::FilterDialog::apply()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  //get preview raster
  te::rst::Raster* inputRst = m_navigator->getExtentRaster(true);

  //set segmenters parameters
  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_inRasterPtr = inputRst;
  
  algoInputParams.m_iterationsNumber = m_ui->m_iterationsSpinBox->value();

  algoInputParams.m_enableProgress = true;

  int idx = m_ui->m_typeComboBox->currentIndex();
  algoInputParams.m_filterType = (te::rp::Filter::InputParameters::FilterType)m_ui->m_typeComboBox->itemData(idx).toInt();

  if(algoInputParams.m_filterType == te::rp::Filter::InputParameters::UserDefinedWindowT)
  {
    algoInputParams.m_windowH = m_window.size1();
    algoInputParams.m_windowW = m_window.size2();
    algoInputParams.m_window = m_window;
  }

  int nBands = m_ui->m_listWidget->count();
  for(int i = 0; i < nBands; ++i)
  {
    if(m_ui->m_listWidget->item(i)->isSelected())
      algoInputParams.m_inRasterBands.push_back(i);
  }

  te::rp::Filter::OutputParameters algoOutputParams;

  std::map<std::string, std::string> rinfo;
  rinfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfRows());
  rinfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfColumns());
  rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(inputRst->getBandDataType(0));
  rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfBands());

  algoOutputParams.m_rType = "MEM";
  algoOutputParams.m_rInfo = rinfo;

  //run contrast
  te::rp::Filter algorithmInstance;

  try
  {
    if(algorithmInstance.initialize(algoInputParams))
    {
      if(algorithmInstance.execute(algoOutputParams))
      {
        std::auto_ptr<te::rst::Raster> rst = algoOutputParams.m_outputRasterPtr;

        m_navigator->drawRaster(rst.get());
      }
    }
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Filter error."));
  }

  QApplication::restoreOverrideCursor();

  //delete input raster dataset
  delete inputRst;
}

void te::qt::widgets::FilterDialog::onOkPushButtonClicked()
{
  //get layer
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  //filter type
  int idx = m_ui->m_typeComboBox->currentIndex();

 //run contrast
  te::rp::Filter algorithmInstance;

  te::rp::Filter::InputParameters algoInputParams;
  algoInputParams.m_inRasterPtr = inputRst.get();
  algoInputParams.m_iterationsNumber = m_ui->m_iterationsSpinBox->value();
  algoInputParams.m_enableProgress = true;
  algoInputParams.m_filterType = (te::rp::Filter::InputParameters::FilterType)m_ui->m_typeComboBox->itemData(idx).toInt();

  if(algoInputParams.m_filterType == te::rp::Filter::InputParameters::UserDefinedWindowT)
  {
    algoInputParams.m_windowH = m_window.size1();
    algoInputParams.m_windowW = m_window.size2();
    algoInputParams.m_window = m_window;
  }

  int nBands = m_ui->m_listWidget->count();
  for(int i = 0; i < nBands; ++i)
  {
    if(m_ui->m_listWidget->item(i)->isSelected())
      algoInputParams.m_inRasterBands.push_back(i);
  }

  te::rp::Filter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = m_outputRaster->getInfo();
  algoOutputParams.m_rType = m_outputRaster->getType();

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  if(algorithmInstance.initialize(algoInputParams))
  {
    if(algorithmInstance.execute(algoOutputParams))
    {
      algoOutputParams.reset();

      //set output layer
      m_outputLayer = te::qt::widgets::createLayer(m_outputRaster->getType(), 
                                                   m_outputRaster->getInfo());
      
      QMessageBox::information(this, tr("Filter"), tr("Filter ended sucessfully"));
    }
    else
    {
      QMessageBox::critical(this, tr("Filter"), tr("Filter execution error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str());

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      return;
    }
  }
  else
  {
    QMessageBox::critical(this, tr("Filter"), tr("Filter initialization error.") +
      ( " " + te::rp::Module::getLastLogStr() ).c_str() );

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  accept();
}

void te::qt::widgets::FilterDialog::listBands()
{
  m_ui->m_listWidget->clear();

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_listWidget->addItem(QString::number(i));
      }
    }
  }
}

void te::qt::widgets::FilterDialog::listFilterTypes()
{
  m_ui->m_typeComboBox->clear();

  m_ui->m_typeComboBox->addItem(tr("Sobel"), te::rp::Filter::InputParameters::SobelFilterT);
  m_ui->m_typeComboBox->addItem(tr("Roberts"), te::rp::Filter::InputParameters::RobertsFilterT);
  m_ui->m_typeComboBox->addItem(tr("Mean"), te::rp::Filter::InputParameters::MeanFilterT);
  m_ui->m_typeComboBox->addItem(tr("Mode"), te::rp::Filter::InputParameters::ModeFilterT);
  m_ui->m_typeComboBox->addItem(tr("Median"), te::rp::Filter::InputParameters::MedianFilterT);
  m_ui->m_typeComboBox->addItem(tr("Dilation"), te::rp::Filter::InputParameters::DilationFilterT);
  m_ui->m_typeComboBox->addItem(tr("Erosion"), te::rp::Filter::InputParameters::ErosionFilterT);
  m_ui->m_typeComboBox->addItem(tr("User Defined"), te::rp::Filter::InputParameters::UserDefinedWindowT);

  onFilterTypeComboBoxActivated(0);
}

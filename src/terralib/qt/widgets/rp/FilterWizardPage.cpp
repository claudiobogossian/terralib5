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
  \file terralib/qt/widgets/rp/FilterWizardPage.h

  \brief This file has the FilterWizardPage class.
*/

// TerraLib
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../rp/Filter.h"
#include "../../../rp/Module.h"
#include "../canvas/MapDisplay.h"
#include "../progress/ProgressViewerDialog.h"
#include "FilterWizardPage.h"
#include "MaskDialog.h"
#include "RasterNavigatorWidget.h"
#include "Utils.h"
#include "ui_FilterWizardPageForm.h"

//QT
#include <QApplication>
#include <QGridLayout>
#include <QMessageBox>

//Boost
#include <boost/lexical_cast.hpp>

te::qt::widgets::FilterWizardPage::FilterWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::FilterWizardPageForm)
{
//build form
  m_ui->setupUi(this);

// configure page
  this->setTitle(tr("Filter"));
  this->setSubTitle(tr("Select the type of filter and set their specific parameters."));

  m_ui->m_maskToolButton->setIcon(QIcon::fromTheme("mask"));
  m_ui->m_loadMaskToolButton->setIcon(QIcon::fromTheme("mask-fill"));
  m_ui->m_maskDefaultValueLineEdit->setValidator(new QDoubleValidator(this));

  //preview
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_previewWidget);
  m_navigator.reset(new te::qt::widgets::RasterNavigatorWidget(m_ui->m_previewWidget));
  m_navigator->showAsPreview(true, false);
  m_navigator->hideColorCompositionTool(true);
  displayLayout->addWidget(m_navigator.get());
  displayLayout->setContentsMargins(0,0,0,0);

  //connects
  connect(m_ui->m_listWidget, SIGNAL(itemSelectionChanged()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_typeComboBox, SIGNAL(activated(int)), SLOT(onFilterTypeComboBoxActivated(int)));
  connect(m_ui->m_maskToolButton, SIGNAL(clicked()), SLOT(onMaskToolButtonClicked()));
  connect(m_ui->m_loadMaskToolButton, SIGNAL(clicked()), SLOT(onLoadMaskToolButtonClicked()));
  connect(m_navigator.get(), SIGNAL(previewClicked()), this, SLOT(apply()));

  listFilterTypes();
}

te::qt::widgets::FilterWizardPage::~FilterWizardPage()
{
}

bool te::qt::widgets::FilterWizardPage::isComplete() const
{
  if(m_ui->m_listWidget->selectedItems().isEmpty())
    return false;

  return true;
}

void te::qt::widgets::FilterWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  m_navigator->set(m_layer, true);

  m_navigator->getDisplay()->refresh();

  listBands();
}

te::rp::Filter::InputParameters  te::qt::widgets::FilterWizardPage::getInputParams()
{
  int idx = m_ui->m_typeComboBox->currentIndex();

  te::rp::Filter::InputParameters algoInputParams;
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

  return algoInputParams;
}

te::rp::Filter::OutputParameters te::qt::widgets::FilterWizardPage::getOutputParams()
{
  te::rp::Filter::OutputParameters algoOutputParams;

  return algoOutputParams;
}

void te::qt::widgets::FilterWizardPage::onFilterTypeComboBoxActivated(int index)
{
  int filterType = m_ui->m_typeComboBox->itemData(index).toInt();

  bool flag = filterType == te::rp::Filter::InputParameters::UserDefinedWindowT;

  m_ui->m_maskSizeSpinBox->setEnabled(flag);
  m_ui->m_maskToolButton->setEnabled(flag);
  m_ui->m_maskDefaultValueLineEdit->setEnabled(flag);
}

void te::qt::widgets::FilterWizardPage::onMaskToolButtonClicked()
{
  te::qt::widgets::MaskDialog dlg(this);

  dlg.setMaskSize(m_ui->m_maskSizeSpinBox->value(), m_ui->m_maskSizeSpinBox->value(),
    m_ui->m_maskDefaultValueLineEdit->text().isEmpty() ? 0 : m_ui->m_maskDefaultValueLineEdit->text().toDouble());

  if(dlg.exec() == QDialog::Accepted)
  {
    m_window = dlg.getMatrix();

    m_ui->m_loadMaskToolButton->setEnabled(true);
  }
}

void te::qt::widgets::FilterWizardPage::onLoadMaskToolButtonClicked()
{
  te::qt::widgets::MaskDialog dlg(this);

  dlg.setMaskSize(m_window);

  m_ui->m_maskSizeSpinBox->setValue(m_window.size1());

  if(dlg.exec() == QDialog::Accepted)
  {
    m_window = dlg.getMatrix();
  }
}

void te::qt::widgets::FilterWizardPage::apply()
{
  if(m_ui->m_listWidget->selectedItems().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("No band was selected."));
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  //get preview raster
  te::rst::Raster* inputRst = m_navigator->getExtentRaster(true);

  //set segmenters parameters
  te::rp::Filter::InputParameters algoInputParams = getInputParams();

  algoInputParams.m_inRasterPtr = inputRst;

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

void te::qt::widgets::FilterWizardPage::listBands()
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

void te::qt::widgets::FilterWizardPage::listFilterTypes()
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

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
#include "RasterHistogramWidget.h"
#include "RasterNavigatorWidget.h"
#include "ui_ContrastWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QMessageBox>

// Boost
#include <boost/lexical_cast.hpp>

//STL
#include <memory>

te::qt::widgets::ContrastWizardPage::ContrastWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ContrastWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillContrastTypes();

  //build preview
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_displayWidget);
  m_navigator.reset(new te::qt::widgets::RasterNavigatorWidget(m_ui->m_displayWidget));
  m_navigator->showAsPreview(true);
  m_navigator->hideColorCompositionTool(true);
  displayLayout->addWidget(m_navigator.get());
  displayLayout->setContentsMargins(0,0,0,0);

  //build histogram
  QGridLayout* histogramLayout = new QGridLayout(m_ui->m_tabWidget->widget(1));
  m_histogramWidget.reset(new te::qt::widgets::RasterHistogramWidget(m_ui->m_tabWidget->widget(1)));
  histogramLayout->addWidget(m_histogramWidget.get());
  histogramLayout->setContentsMargins(0,0,0,0);

//connects
  connect(m_ui->m_contrastTypeComboBox, SIGNAL(activated(int)), this, SLOT(onContrastTypeComboBoxActivated(int)));
  connect(m_ui->m_bandTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked(int, int)));
  connect(m_navigator.get(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(onPreviewChanged()));
  connect(m_navigator.get(), SIGNAL(previewClicked()), this, SLOT(apply()));

  connect(m_histogramWidget.get(), SIGNAL(minValueSelected(int, int)), this, SLOT(onMinValueSelected(int, int)));
  connect(m_histogramWidget.get(), SIGNAL(maxValueSelected(int, int)), this, SLOT(onMaxValueSelected(int, int)));

//configure page
  this->setTitle(tr("Contrast"));
  this->setSubTitle(tr("Select the type of contrast and set their specific parameters."));

  //onContrastTypeComboBoxActivated(m_ui->m_contrastTypeComboBox->currentIndex());
}

te::qt::widgets::ContrastWizardPage::~ContrastWizardPage()
{
}

bool te::qt::widgets::ContrastWizardPage::isComplete() const
{
  int nBands = m_ui->m_bandTableWidget->rowCount();

  for(int i = 0; i < nBands; ++i)
  {
    QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(i, 0);
    
    if(item->checkState() == Qt::Checked)
    {
      return true;
    }
  }

  return false;
}

void te::qt::widgets::ContrastWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  m_navigator->set(m_layer, true);

  listBands();
}

te::map::AbstractLayerPtr te::qt::widgets::ContrastWizardPage::get()
{
  return m_layer;
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
      QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(i, 0);
    
      if(item->checkState() == Qt::Checked)
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
      QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(i, 0);
    
      if(item->checkState() == Qt::Checked)
      {
        QString valueMax = m_ui->m_bandTableWidget->item(i, 1)->text();
        algoInputParams.m_hECMaxInput.push_back(valueMax.toDouble());
      }
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::MeanAndStdContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::MeanAndStdContrastT;

    for(int i = 0; i < nBands; ++i)
    {
      QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(i, 0);
    
      if(item->checkState() == Qt::Checked)
      {
        QString valueMean = m_ui->m_bandTableWidget->item(i, 1)->text();
        algoInputParams.m_sMASCMeanInput.push_back(valueMean.toDouble());

        QString valueStdDev = m_ui->m_bandTableWidget->item(i, 2)->text();
        algoInputParams.m_sMASCStdInput.push_back(valueStdDev.toDouble());
      }
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::SquareContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::SquareContrastT;

    for(int i = 0; i < nBands; ++i)
    {
      QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(i, 0);
    
      if(item->checkState() == Qt::Checked)
      {
        QString valueMin = m_ui->m_bandTableWidget->item(i, 1)->text();
        algoInputParams.m_squareCMinInput.push_back(valueMin.toDouble());

        QString valueMax = m_ui->m_bandTableWidget->item(i, 2)->text();
        algoInputParams.m_squareCMaxInput.push_back(valueMax.toDouble());
      }
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::SquareRootContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::SquareRootContrastT;

    for(int i = 0; i < nBands; ++i)
    {
      QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(i, 0);
    
      if(item->checkState() == Qt::Checked)
      {
        QString valueMin = m_ui->m_bandTableWidget->item(i, 1)->text();
        algoInputParams.m_squareRootCMinInput.push_back(valueMin.toDouble());

        QString valueMax = m_ui->m_bandTableWidget->item(i, 2)->text();
        algoInputParams.m_squareRootCMaxInput.push_back(valueMax.toDouble());
      }
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::LogContrastT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::LogContrastT;

    for(int i = 0; i < nBands; ++i)
    {
      QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(i, 0);
    
      if(item->checkState() == Qt::Checked)
      {
        QString valueMin = m_ui->m_bandTableWidget->item(i, 1)->text();
        algoInputParams.m_logCMinInput.push_back(valueMin.toDouble());

        QString valueMax = m_ui->m_bandTableWidget->item(i, 2)->text();
        algoInputParams.m_logCMaxInput.push_back(valueMax.toDouble());
      }
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::DecorrelationEnhancementT)
  {
    algoInputParams.m_type = te::rp::Contrast::InputParameters::DecorrelationEnhancementT;
  }

  for(int i = 0; i < nBands; ++i)
  {
    QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(i, 0);
    
    if(item->checkState() == Qt::Checked)
    {
      algoInputParams.m_inRasterBands.push_back(i);
    }
  }

  return algoInputParams;
}

void te::qt::widgets::ContrastWizardPage::apply()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  //get preview raster
  te::rst::Raster* inputRst = m_navigator->getExtentRaster();

  //set contrast parameters
  te::rp::Contrast::InputParameters algoInputParams = getInputParams();

  algoInputParams.m_inRasterPtr = inputRst;

  te::rp::Contrast::OutputParameters algoOutputParams;

  std::map<std::string, std::string> rinfo;
  rinfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfRows());
  rinfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfColumns());
  rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(inputRst->getBandDataType(0));
  rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfBands());

  algoOutputParams.m_createdOutRasterDSType = "MEM";
  algoOutputParams.m_createdOutRasterInfo = rinfo;

  //run contrast
  te::rp::Contrast algorithmInstance;

  try
  {
    if(algorithmInstance.initialize(algoInputParams))
    {
      if(algorithmInstance.execute(algoOutputParams))
      {
        m_navigator->drawRaster(algoOutputParams.m_outRasterPtr);

        //update histogram and replot
        m_histogramWidget->setOutputRaster(algoOutputParams.m_createdOutRasterPtr.release());

        drawHistogram();
      }
    }
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Constrast error."));
  }

  QApplication::restoreOverrideCursor();
}

void te::qt::widgets::ContrastWizardPage::fillContrastTypes()
{
  m_ui->m_contrastTypeComboBox->clear();

  m_ui->m_contrastTypeComboBox->addItem(tr("Linear"), te::rp::Contrast::InputParameters::LinearContrastT);
  m_ui->m_contrastTypeComboBox->addItem(tr("Histogram Equalization"), te::rp::Contrast::InputParameters::HistogramEqualizationContrastT);
  m_ui->m_contrastTypeComboBox->addItem(tr("Square Constrast"), te::rp::Contrast::InputParameters::SquareContrastT);
  m_ui->m_contrastTypeComboBox->addItem(tr("Square Root Constrast"), te::rp::Contrast::InputParameters::SquareRootContrastT);
  m_ui->m_contrastTypeComboBox->addItem(tr("Log Constrast"), te::rp::Contrast::InputParameters::LogContrastT);
  m_ui->m_contrastTypeComboBox->addItem(tr("Mean and Standard Deviation"), te::rp::Contrast::InputParameters::MeanAndStdContrastT);
  m_ui->m_contrastTypeComboBox->addItem(tr("Decorrelation Enhancement"), te::rp::Contrast::InputParameters::DecorrelationEnhancementT);
}

void te::qt::widgets::ContrastWizardPage::listBands()
{
  assert(m_layer.get());

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
         m_ui->m_bandTableWidget->setRowCount(0);

        // initializing the list of bands
        for(unsigned b = 0 ; b < inputRst->getNumberOfBands(); b++)
        {
          int newrow = m_ui->m_bandTableWidget->rowCount();
          m_ui->m_bandTableWidget->insertRow(newrow);

          QString bName(tr("Band "));
          bName.append(QString::number(b));

          QTableWidgetItem* itemBand = new QTableWidgetItem(bName);
          itemBand->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
          itemBand->setCheckState(Qt::Checked);
          m_ui->m_bandTableWidget->setItem(newrow, 0, itemBand);

        }
      }
    }
  }

  m_ui->m_bandTableWidget->resizeColumnsToContents();
#if (QT_VERSION >= 0x050000)
  m_ui->m_bandTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  m_ui->m_bandTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

  onContrastTypeComboBoxActivated(m_ui->m_contrastTypeComboBox->currentIndex());
}

void te::qt::widgets::ContrastWizardPage::onContrastTypeComboBoxActivated(int index)
{
  int contrastType = m_ui->m_contrastTypeComboBox->itemData(index).toInt();

  if(contrastType == te::rp::Contrast::InputParameters::LinearContrastT ||
     contrastType == te::rp::Contrast::InputParameters::SquareContrastT ||
     contrastType == te::rp::Contrast::InputParameters::SquareRootContrastT ||
     contrastType == te::rp::Contrast::InputParameters::LogContrastT)
  {
    bool reset = true;

    if(m_ui->m_bandTableWidget->horizontalHeaderItem(1) && m_ui->m_bandTableWidget->horizontalHeaderItem(1)->text() == tr("Minimum") &&
       m_ui->m_bandTableWidget->horizontalHeaderItem(2) && m_ui->m_bandTableWidget->horizontalHeaderItem(2)->text() == tr("Maximum"))
       reset = false;

    if(reset)
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
        itemMin->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
        m_ui->m_bandTableWidget->setItem(i, 1, itemMin);

        QTableWidgetItem* itemMax = new QTableWidgetItem("255");
        itemMax->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
        m_ui->m_bandTableWidget->setItem(i, 2, itemMax);
      }
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
      QTableWidgetItem* itemMax = new QTableWidgetItem("255");
      itemMax->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
      m_ui->m_bandTableWidget->setItem(i, 1, itemMax);
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::MeanAndStdContrastT)
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
      QTableWidgetItem* itemMean = new QTableWidgetItem("127");
      itemMean->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
      m_ui->m_bandTableWidget->setItem(i, 1, itemMean);

      QTableWidgetItem* itemStdDev = new QTableWidgetItem("50");
      itemStdDev->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
      m_ui->m_bandTableWidget->setItem(i, 2, itemStdDev);
    }
  }
  else if(contrastType == te::rp::Contrast::InputParameters::DecorrelationEnhancementT)
  {
    QStringList list;
    list.append(tr("Band"));

    m_ui->m_bandTableWidget->setColumnCount(1);
    m_ui->m_bandTableWidget->setHorizontalHeaderLabels(list);
  }

  m_ui->m_bandTableWidget->resizeColumnsToContents();
#if (QT_VERSION >= 0x050000)
  m_ui->m_bandTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  m_ui->m_bandTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
}

void te::qt::widgets::ContrastWizardPage::onCellClicked(int row, int column)
{
  QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(row, 0);

  if(item->checkState() == Qt::Checked)
    drawHistogram();
}

void te::qt::widgets::ContrastWizardPage::onMinValueSelected(int value, int band)
{
  int index = m_ui->m_contrastTypeComboBox->currentIndex();

  int contrastType = m_ui->m_contrastTypeComboBox->itemData(index).toInt();

  if(contrastType == te::rp::Contrast::InputParameters::LinearContrastT ||
     contrastType == te::rp::Contrast::InputParameters::SquareContrastT ||
     contrastType == te::rp::Contrast::InputParameters::SquareRootContrastT ||
     contrastType == te::rp::Contrast::InputParameters::LogContrastT)
  {
      m_ui->m_bandTableWidget->item(band, 1)->setText(QString::number(value));
      m_ui->m_bandTableWidget->setCurrentCell(band, 1);
  }
  else if(contrastType == te::rp::Contrast::InputParameters::HistogramEqualizationContrastT)
  {
    m_ui->m_bandTableWidget->item(band, 1)->setText(QString::number(value));
    m_ui->m_bandTableWidget->setCurrentCell(band, 1);
  }
  else if(contrastType == te::rp::Contrast::InputParameters::MeanAndStdContrastT)
  {
    m_ui->m_bandTableWidget->item(band, 1)->setText(QString::number(value));
    m_ui->m_bandTableWidget->setCurrentCell(band, 1);
  }
  else
  {
    return;
  }
}

void te::qt::widgets::ContrastWizardPage::onMaxValueSelected(int value, int band)
{
  int index = m_ui->m_contrastTypeComboBox->currentIndex();

  int contrastType = m_ui->m_contrastTypeComboBox->itemData(index).toInt();

  if(contrastType == te::rp::Contrast::InputParameters::LinearContrastT ||
     contrastType == te::rp::Contrast::InputParameters::SquareContrastT ||
     contrastType == te::rp::Contrast::InputParameters::SquareRootContrastT ||
     contrastType == te::rp::Contrast::InputParameters::LogContrastT)
  {
    m_ui->m_bandTableWidget->item(band, 2)->setText(QString::number(value));
    m_ui->m_bandTableWidget->setCurrentCell(band, 2);
  }
  else if(contrastType == te::rp::Contrast::InputParameters::MeanAndStdContrastT)
  {
    if(!m_ui->m_bandTableWidget->item(band, 1)->text().isEmpty())
    {
      double mean = m_ui->m_bandTableWidget->item(band, 1)->text().toDouble();

      double stdDev = abs(mean - value);

      m_ui->m_bandTableWidget->item(band, 2)->setText(QString::number(stdDev));
      m_ui->m_bandTableWidget->setCurrentCell(band, 2);
    }
  }
  else
  {
    return;
  }
}

void te::qt::widgets::ContrastWizardPage::onPreviewChanged()
{
  te::rst::Raster* inputRst = m_navigator->getExtentRaster();

  m_histogramWidget->setInputRaster(inputRst);

  drawHistogram();
}

void te::qt::widgets::ContrastWizardPage::drawHistogram()
{
  int curRow = 0;
  
  if(m_ui->m_bandTableWidget->currentRow() >= 0)
    curRow = m_ui->m_bandTableWidget->currentRow();

  int bandIdx = -1;

  for(int i = 0; i <= m_ui->m_bandTableWidget->currentRow(); ++i)
  {
    QTableWidgetItem* item =  m_ui->m_bandTableWidget->item(i, 0);
    
    if(item->checkState() == Qt::Checked)
    {
      ++bandIdx;
    }
  }

  if(bandIdx != -1)
    m_histogramWidget->drawHistogram(bandIdx);
}

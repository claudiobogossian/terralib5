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
  \file terralib/qt/widgets/rp/FusionWizardPage.cpp

  \brief This file defines a class for a Fusion Wizard page.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Interpolator.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Functions.h"
#include "../../../rp/SpectralResponseFunctions.h"
#include "../../../se/Utils.h"
#include "FusionWizardPage.h"
#include "ui_FusionWizardPageForm.h"

// Qt
#include <QApplication>
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>

// stl
#include <fstream>
#include <iosfwd>
#include <memory>
#include <stdio.h>

//QWT
#include <qwt_legend.h>
#include <qwt_plot.h>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::FusionWizardPage::FusionWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::FusionWizardPageForm),
    m_scatterHighRes(0),
    m_layerLower(0),
    m_layerHigher(0)
{
// setup controls
  m_ui->setupUi(this);

  fillFusionTypes();

  fillSensorTypes(m_ui->m_sensorTypeComboBox);

  fillWaveletTypes();

  //chart dialog
  m_chartDialog = new QDialog(this, Qt::Tool | Qt::WindowTitleHint);
  QGridLayout* dlgLayout = new QGridLayout(m_chartDialog);
  m_chartDisplay = new te::qt::widgets::ChartDisplay(m_chartDialog);
  dlgLayout->addWidget(m_chartDisplay);

  m_chartDisplay->setAxisTitle(QwtPlot::xBottom, tr("Wavelength in Microns"));
  m_chartDisplay->setAxisTitle(QwtPlot::yLeft, tr("Reflectance"));
  m_chartDisplay->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

  //connects
  connect(m_ui->m_scatterBoolButton, SIGNAL(toggled(bool)), m_chartDialog, SLOT(setVisible(bool)));
  connect(m_ui->m_sensorTypeComboBox, SIGNAL(activated(int)), this, SLOT(onHighResSensorTypeActivated(int)));
  connect(m_ui->m_csvToolButton, SIGNAL(clicked()), this, SLOT(onHighCsvToolButtonClicked()));
  

  //configure page
  this->setTitle(tr("Fusion"));
  this->setSubTitle(tr("Select the type of fusion and set their specific parameters."));

  m_ui->m_scatterBoolButton->setIcon(QIcon::fromTheme("chart-lines"));
}

te::qt::widgets::FusionWizardPage::~FusionWizardPage()
{

}

bool te::qt::widgets::FusionWizardPage::isComplete() const
{
  return true;
}

void te::qt::widgets::FusionWizardPage::setLower(te::map::AbstractLayerPtr layer)
{
  m_layerLower = layer;

  m_ui->m_ihsLowerLineEdit->setText(m_layerLower->getTitle().c_str());
  m_ui->m_pcaLowerLineEdit->setText(m_layerLower->getTitle().c_str());
  m_ui->m_wisperLowerLineEdit->setText(m_layerLower->getTitle().c_str());

  listBandsLower();
}

void te::qt::widgets::FusionWizardPage::setHigher(te::map::AbstractLayerPtr layer)
{
  m_layerHigher = layer;

  m_ui->m_higherLineEdit_IHS->setText(m_layerHigher->getTitle().c_str());
  m_ui->m_higherLineEdit_PCA->setText(m_layerHigher->getTitle().c_str());
  m_ui->m_higherLineEdit_Wisper->setText(m_layerHigher->getTitle().c_str());

  listBandsHigher();
}

bool te::qt::widgets::FusionWizardPage::isIHSFusion()
{
  int idx = m_ui->m_fusionTypeComboBox->currentIndex();

  int type = m_ui->m_fusionTypeComboBox->itemData(idx).toInt();

  if(type == FUSION_IHS)
    return true;

  return false;
}

bool te::qt::widgets::FusionWizardPage::isPCAFusion()
{
  int idx = m_ui->m_fusionTypeComboBox->currentIndex();

  int type = m_ui->m_fusionTypeComboBox->itemData(idx).toInt();

  if(type == FUSION_PCA)
    return true;

  return false;
}

bool te::qt::widgets::FusionWizardPage::isWisperFusion()
{
  int idx = m_ui->m_fusionTypeComboBox->currentIndex();

  int type = m_ui->m_fusionTypeComboBox->itemData(idx).toInt();

  if(type == FUSION_WISPER)
    return true;

  return false;
}

bool te::qt::widgets::FusionWizardPage::cropRasters()
{
  return m_ui->m_cropCheckBox->isChecked();
}

te::rp::IHSFusion::InputParameters te::qt::widgets::FusionWizardPage::getInputIHSParams()
{
  int interpolationIdx = m_ui->m_interpolatorComboBox->currentIndex();
  te::rst::Interpolator::Method method = (te::rst::Interpolator::Method)m_ui->m_interpolatorComboBox->itemData(interpolationIdx).toInt();

  te::rp::IHSFusion::InputParameters algoInputParams;

  algoInputParams.m_enableProgress = true;
  algoInputParams.m_interpMethod = method;

  algoInputParams.m_lowResRasterRedBandIndex = m_ui->m_ihsRComboBox->currentText().toInt();
  algoInputParams.m_lowResRasterGreenBandIndex = m_ui->m_ihsGComboBox->currentText().toInt();
  algoInputParams.m_lowResRasterBlueBandIndex = m_ui->m_ihsBComboBox->currentText().toInt();

  algoInputParams.m_highResRasterBand = m_ui->m_bandComboBox_IHS->currentText().toInt();

  return algoInputParams;
}

te::rp::IHSFusion::OutputParameters te::qt::widgets::FusionWizardPage::getOutputIHSParams()
{
  te::rp::IHSFusion::OutputParameters algoOutputParams;

  return algoOutputParams;
}

te::rp::PCAFusion::InputParameters te::qt::widgets::FusionWizardPage::getInputPCAParams()
{
  int interpolationIdx = m_ui->m_interpolatorComboBox->currentIndex();
  te::rst::Interpolator::Method method = (te::rst::Interpolator::Method)m_ui->m_interpolatorComboBox->itemData(interpolationIdx).toInt();

  te::rp::PCAFusion::InputParameters algoInputParams;

  algoInputParams.m_enableProgress = true;
  algoInputParams.m_interpMethod = method;

  int nBands = m_ui->m_listWidget->count();
  for(int i = 0; i < nBands; ++i)
  {
    if(m_ui->m_listWidget->item(i)->isSelected())
      algoInputParams.m_lowResRasterBands.push_back(i);
  }

  algoInputParams.m_highResRasterBand = m_ui->m_bandComboBox_PCA->currentText().toInt();

  return algoInputParams;
}

te::rp::PCAFusion::OutputParameters te::qt::widgets::FusionWizardPage::getOutputPCAParams()
{
  te::rp::PCAFusion::OutputParameters algoOutputParams;

  return algoOutputParams;
}

te::rp::WisperFusion::InputParameters te::qt::widgets::FusionWizardPage::getInputWisperParams()
{
  int idx = m_ui->m_interpolatorComboBox->currentIndex();
  te::rst::Interpolator::Method method = (te::rst::Interpolator::Method)m_ui->m_interpolatorComboBox->itemData(idx).toInt();

  te::rp::WisperFusion::InputParameters algoInputParams;

  algoInputParams.m_enableProgress = true;
  algoInputParams.m_interpMethod = method;

  int nBands = m_ui->m_wisperTableWidget->rowCount();
  for(int i = 0; i < nBands; ++i)
  {
    QTableWidgetItem* item =  m_ui->m_wisperTableWidget->item(i, 0);
    
    if(item->checkState() == Qt::Checked)
    {
      algoInputParams.m_lowResRasterBands.push_back(i);

      QComboBox* cmbBox = (QComboBox*)m_ui->m_wisperTableWidget->cellWidget(i, 2);

      idx = cmbBox->currentIndex();
      te::rp::srf::SensorType st = (te::rp::srf::SensorType)cmbBox->itemData(idx).toInt();
      std::string stName = cmbBox->itemText(idx).toStdString();
      algoInputParams.m_lowResRasterBandsSRFs.push_back(getSRFMap(st, stName));

      //algoInputParams.m_lowResRasterBandSensors.push_back((te::rp::srf::SensorType)cmbBox->itemData(idx).toInt());
    }
  }

  algoInputParams.m_highResRasterBand = m_ui->m_bandComboBox_Wisper->currentText().toInt();

  idx = m_ui->m_sensorTypeComboBox->currentIndex();
  te::rp::srf::SensorType st = (te::rp::srf::SensorType)m_ui->m_sensorTypeComboBox->itemData(idx).toInt();
  std::string stName = m_ui->m_sensorTypeComboBox->itemText(idx).toStdString();
  algoInputParams.m_hiResRasterBandsSRFs = getSRFMap(st, stName);

  //algoInputParams.m_hiResRasterBandSensor = (te::rp::srf::SensorType)m_ui->m_sensorTypeComboBox->itemData(idx).toInt();

  //wavelet
  idx = m_ui->m_waveletComboBox->currentIndex();
  algoInputParams.m_waveletFilterType = (te::rp::WaveletAtrousFilterType)m_ui->m_waveletComboBox->itemData(idx).toInt();
  algoInputParams.m_hiResRasterWaveletLevels = (unsigned int)m_ui->m_waveletLevelsSpinBox->value();

  return algoInputParams;
}

te::rp::WisperFusion::OutputParameters te::qt::widgets::FusionWizardPage::getOutputWisperParams()
{
  te::rp::WisperFusion::OutputParameters algoOutputParams;

  return algoOutputParams;
}

void te::qt::widgets::FusionWizardPage::onHighResSensorTypeActivated(int idx)
{
  //start scatter
  if(!m_scatterHighRes)
  {
    m_scatterHighRes = new te::qt::widgets::Scatter();
    m_scatterChartHighRes = new te::qt::widgets::ScatterChart(m_scatterHighRes);
    m_scatterChartHighRes->setPen(Qt::blue);
    m_scatterChartHighRes->setSymbol(0);
    m_scatterChartHighRes->setStyle(QwtPlotCurve::Lines);
    m_scatterChartHighRes->attach(m_chartDisplay);
    m_scatterChartHighRes->setTitle(tr("High Resolution"));
  }

  te::rp::srf::SensorType st = (te::rp::srf::SensorType)m_ui->m_sensorTypeComboBox->itemData(idx).toInt();
  std::string stName = m_ui->m_sensorTypeComboBox->itemText(idx).toStdString();

  std::map<double, double> srf = getSRFMap(st, stName);
  std::map<double, double>::iterator it;

  //plot model curve
  std::vector<double> wavelengthX;
  std::vector<double> reflectanceY;

  for(it = srf.begin(); it != srf.end(); ++it)
  {
    wavelengthX.push_back(it->first);
    reflectanceY.push_back(it->second);
  }

  m_scatterHighRes->setXValues(wavelengthX);
  m_scatterHighRes->setYValues(reflectanceY);
  m_scatterHighRes->calculateMinMaxValues();
  m_scatterChartHighRes->setData();

  //replot the chart display
  m_chartDisplay->replot();
  m_chartDisplay->updateLayout();
}

void te::qt::widgets::FusionWizardPage::onLowResSensorTypeActivated(int idx)
{
  QComboBox* cmbBox = dynamic_cast<QComboBox*>(sender());
  if(!cmbBox) 
    return;

  int row = m_comboMap[cmbBox];

  te::qt::widgets::Scatter* scatter = m_scatterLowResVec[row];
  te::qt::widgets::ScatterChart* scatterChart = m_scatterChartLowResVec[row];

 //start scatter
  if(!scatter)
  {
    scatter = new te::qt::widgets::Scatter();
    scatterChart = new te::qt::widgets::ScatterChart(scatter);

    //color
    QColor c(te::se::GenerateRandomColor().c_str());
    scatterChart->setPen(c);

    scatterChart->setSymbol(0);
    scatterChart->setStyle(QwtPlotCurve::Lines);
    scatterChart->attach(m_chartDisplay);
    
    //title
    QString title(tr("Low Resolution - Band "));
    title.append(QString::number(row));
    scatterChart->setTitle(title);

     m_scatterLowResVec[row] = scatter;
     m_scatterChartLowResVec[row] = scatterChart;
  }

  te::rp::srf::SensorType st = (te::rp::srf::SensorType)cmbBox->itemData(idx).toInt();
  std::string stName = cmbBox->itemText(idx).toStdString();

  std::map<double, double> srf = getSRFMap(st, stName);
  std::map<double, double>::iterator it;

  //plot model curve
  std::vector<double> wavelengthX;
  std::vector<double> reflectanceY;

  for(it = srf.begin(); it != srf.end(); ++it)
  {
    wavelengthX.push_back(it->first);
    reflectanceY.push_back(it->second);
  }

  scatter->setXValues(wavelengthX);
  scatter->setYValues(reflectanceY);
  scatter->calculateMinMaxValues();
  scatterChart->setData();

  //replot the chart display
  m_chartDisplay->replot();
  m_chartDisplay->updateLayout();
}

void te::qt::widgets::FusionWizardPage::onHighCsvToolButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Spectral Response Function File"), "", tr("CSV Files (*.csv *.CSV)"));

  if(fileName.isEmpty())
    return;

  int idx = m_ui->m_sensorTypeComboBox->count();
  m_ui->m_sensorTypeComboBox->addItem(fileName, QVariant(-1));
  m_ui->m_sensorTypeComboBox->setCurrentIndex(idx);
  onHighResSensorTypeActivated(idx);
}

void te::qt::widgets::FusionWizardPage::onLowCsvToolButtonClicked()
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if(!button) 
    return;

  int row = m_buttonMap[button];

  QComboBox* combo = (QComboBox*)m_ui->m_wisperTableWidget->cellWidget(row, 2);

  if(!combo)
    return;

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Spectral Response Function File"), "", tr("CSV Files (*.csv *.CSV)"));

  if(fileName.isEmpty())
    return;

  int idx = combo->count();
  combo->addItem(fileName, QVariant(-1));
  combo->setCurrentIndex(idx);
}

void te::qt::widgets::FusionWizardPage::fillFusionTypes()
{
  m_ui->m_fusionTypeComboBox->clear();

  m_ui->m_fusionTypeComboBox->addItem(tr("IHS"), FUSION_IHS);
  m_ui->m_fusionTypeComboBox->addItem(tr("PCA"), FUSION_PCA);
  m_ui->m_fusionTypeComboBox->addItem(tr("WISPER"), FUSION_WISPER);

  m_ui->m_interpolatorComboBox->clear();

  m_ui->m_interpolatorComboBox->addItem(tr("Nearest Neighbor"), te::rst::NearestNeighbor);
  m_ui->m_interpolatorComboBox->addItem(tr("Bilinear"), te::rst::Bilinear);
  m_ui->m_interpolatorComboBox->addItem(tr("Bicubic"), te::rst::Bicubic);
}

void te::qt::widgets::FusionWizardPage::fillSensorTypes(QComboBox* combo)
{
  combo->clear();

  combo->addItem("CBERS 2B Band 1", QVariant(te::rp::srf::CBERS2BCCDB1Sensor));
  combo->addItem("CBERS 2B Band 2", QVariant(te::rp::srf::CBERS2BCCDB2Sensor));
  combo->addItem("CBERS 2B Band 3", QVariant(te::rp::srf::CBERS2BCCDB3Sensor));
  combo->addItem("CBERS 2B Band 4", QVariant(te::rp::srf::CBERS2BCCDB4Sensor));
  combo->addItem("CBERS 2B Band 5", QVariant(te::rp::srf::CBERS2BCCDB5PANSensor));
  
  combo->addItem("CBERS 4 MUX Band 5", QVariant(te::rp::srf::CBERS4MUXB5Sensor));
  combo->addItem("CBERS 4 MUX Band 6", QVariant(te::rp::srf::CBERS4MUXB6Sensor));
  combo->addItem("CBERS 4 MUX Band 7", QVariant(te::rp::srf::CBERS4MUXB7Sensor));
  combo->addItem("CBERS 4 MUX Band 8", QVariant(te::rp::srf::CBERS4MUXB8Sensor));

  
  combo->addItem("LANDSAT 8 OLI Band 1", QVariant(te::rp::srf::LANDSAT8OLIB1Sensor));
  combo->addItem("LANDSAT 8 OLI Band 2", QVariant(te::rp::srf::LANDSAT8OLIB2Sensor));
  combo->addItem("LANDSAT 8 OLI Band 3", QVariant(te::rp::srf::LANDSAT8OLIB3Sensor));
  combo->addItem("LANDSAT 8 OLI Band 4", QVariant(te::rp::srf::LANDSAT8OLIB4Sensor));
  combo->addItem("LANDSAT 8 OLI Band 5", QVariant(te::rp::srf::LANDSAT8OLIB5Sensor));
  combo->addItem("LANDSAT 8 OLI Band 6", QVariant(te::rp::srf::LANDSAT8OLIB6Sensor));
  combo->addItem("LANDSAT 8 OLI Band 7", QVariant(te::rp::srf::LANDSAT8OLIB7Sensor));
  combo->addItem("LANDSAT 8 OLI Band 8", QVariant(te::rp::srf::LANDSAT8OLIB8Sensor));
  combo->addItem("LANDSAT 8 OLI Band 9", QVariant(te::rp::srf::LANDSAT8OLIB9Sensor));
  combo->addItem("LANDSAT 8 OLI Band 10", QVariant(te::rp::srf::LANDSAT8OLIB10Sensor));
  combo->addItem("LANDSAT 8 OLI Band 11", QVariant(te::rp::srf::LANDSAT8OLIB11Sensor));
  
  combo->addItem("LANDSAT 7 ETM+ Band 1", QVariant(te::rp::srf::LANDSAT7ETMB1Sensor));
  combo->addItem("LANDSAT 7 ETM+ Band 2", QVariant(te::rp::srf::LANDSAT7ETMB2Sensor));
  combo->addItem("LANDSAT 7 ETM+ Band 3", QVariant(te::rp::srf::LANDSAT7ETMB3Sensor));
  combo->addItem("LANDSAT 7 ETM+ Band 4", QVariant(te::rp::srf::LANDSAT7ETMB4Sensor));
  combo->addItem("LANDSAT 7 ETM+ Band 5", QVariant(te::rp::srf::LANDSAT7ETMB5Sensor));
  combo->addItem("LANDSAT 7 ETM+ Band 7", QVariant(te::rp::srf::LANDSAT7ETMB7Sensor));
  combo->addItem("LANDSAT 7 ETM+ Band 8 PAN", QVariant(te::rp::srf::LANDSAT7ETMB8PANSensor));
}

void te::qt::widgets::FusionWizardPage::fillWaveletTypes()
{
  m_ui->m_waveletComboBox->clear();

  m_ui->m_waveletComboBox->addItem(tr("Spline"), te::rp::B3SplineWAFilter);
  m_ui->m_waveletComboBox->addItem(tr("Triangle"), te::rp::TriangleWAFilter);
  m_ui->m_waveletComboBox->addItem(tr("Invalid"), te::rp::InvalidWAFilter);
}

void te::qt::widgets::FusionWizardPage::listBandsLower()
{
  m_ui->m_ihsRComboBox->clear();
  m_ui->m_ihsGComboBox->clear();
  m_ui->m_ihsBComboBox->clear();
  m_ui->m_listWidget->clear();
  m_ui->m_wisperTableWidget->setRowCount(0);

  m_comboMap.clear();

  assert(m_layerLower.get());

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layerLower->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_ihsRComboBox->addItem(QString::number(i));
        m_ui->m_ihsGComboBox->addItem(QString::number(i));
        m_ui->m_ihsBComboBox->addItem(QString::number(i));
        m_ui->m_listWidget->addItem(QString::number(i));

        //wisper table
        int newrow = m_ui->m_wisperTableWidget->rowCount();
        m_ui->m_wisperTableWidget->insertRow(newrow);

        QString bName(tr("Band "));
        bName.append(QString::number(i));

        QTableWidgetItem* itemBand = new QTableWidgetItem(bName);
        itemBand->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        itemBand->setCheckState(Qt::Checked);
        m_ui->m_wisperTableWidget->setItem(newrow, 0, itemBand);

        //csv toolbutton
        QToolButton* button = new QToolButton(this);
        button->setText("...");
        button->setToolTip(tr("Open Spectral Response Function File..."));
        button->resize(16,16);
        
        m_ui->m_wisperTableWidget->setCellWidget(newrow, 1, button);

        connect(button, SIGNAL(clicked()), this, SLOT(onLowCsvToolButtonClicked()));

        m_buttonMap.insert(std::map<QToolButton*, int>::value_type(button, (int)i));

        //combo
        QComboBox* cmbBox = new QComboBox(this);
        fillSensorTypes(cmbBox);
        m_ui->m_wisperTableWidget->setCellWidget(newrow, 2, cmbBox);

        connect(cmbBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLowResSensorTypeActivated(int)));

        m_comboMap.insert(std::map<QComboBox*, int>::value_type(cmbBox, (int)i));
      }

      m_scatterLowResVec.resize(inputRst->getNumberOfBands(), 0);
      m_scatterChartLowResVec.resize(inputRst->getNumberOfBands(), 0);
    }
  }

  m_ui->m_wisperTableWidget->resizeColumnsToContents();
}

void te::qt::widgets::FusionWizardPage::listBandsHigher()
{
  m_ui->m_bandComboBox_IHS->clear();
  m_ui->m_bandComboBox_PCA->clear();
  m_ui->m_bandComboBox_Wisper->clear();

  assert(m_layerHigher.get());

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layerHigher->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_bandComboBox_IHS->addItem(QString::number(i));
        m_ui->m_bandComboBox_PCA->addItem(QString::number(i));
        m_ui->m_bandComboBox_Wisper->addItem(QString::number(i));
      }
    }
  }
}

std::map<double, double> te::qt::widgets::FusionWizardPage::getSRFMap(te::rp::srf::SensorType st, std::string stName)
{
  std::map<double, double> srfMap;

  if(st == -1) //from csv file
  {
    std::ifstream file(stName.c_str());

    if(file.is_open())
    {
      //create boost tokenizer
      std::string buffer;

      //get header line (ignore)
      std::getline(file, buffer);

      while(std::getline(file, buffer))
      {
        std::vector<std::string> line;

        te::common::Tokenize(buffer, line, ",");

        if(line.size() >=2)
        {
          double wavelength = atof(line[0].c_str());
          double response = atof(line[1].c_str());

          srfMap.insert(std::map<double, double>::value_type(wavelength, response));
        }
      }
    }
  }
  else if(st != te::rp::srf::InvalidSensor) //hardcode defined
  {
    te::rp::srf::getSRF(st, srfMap);
  }

  return srfMap;
}

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
  \file terralib/sa/qt/GeostatisticalMethodsDialog.cpp

  \brief A dialog with geostatistical methods to measure the spatial variability of attribute of a dataset.
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../core/GeostatisticalFunctions.h"
#include "../core/GeostatisticalMethodSemivariogram.h"
#include "../core/GeostatisticalModel.h"
#include "../core/GeostatisticalModelExponential.h"
#include "../core/GeostatisticalModelGaussian.h"
#include "../core/GeostatisticalModelSpherical.h"
#include "../core/Utils.h"
#include "../Enums.h"
#include "../Exception.h"
#include "GeostatisticalMethodsDialog.h"
#include "ui_GeostatisticalMethodsDialogForm.h"

// Qt
#include <QGridLayout>
#include <QMessageBox>
#include <QValidator>

//QWT
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_symbol.h>

// STL
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::sa::GeostatisticalMethodsDialog::GeostatisticalMethodsDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::GeostatisticalMethodsDialogForm),
    m_method(0)
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_nLagsLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_lagsIncrementLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_angleDirLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_angleTolLineEdit->setValidator(new QDoubleValidator(this));

  m_ui->m_adjustGroupBox->setEnabled(false);
  m_ui->m_changeAttrToolButton->setEnabled(false);

  fillParameters();

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("sa-measurespatialvar-hint").pixmap(112,48));
  m_ui->m_changeAttrToolButton->setIcon(QIcon::fromTheme("view-refresh"));

// connectors
  connect(m_ui->m_inputLayerComboBox, SIGNAL(activated(int)), this, SLOT(onInputLayerComboBoxActivated(int)));
  connect(m_ui->m_changeAttrToolButton, SIGNAL(clicked()), this, SLOT(onChangeAttrToolButtonClicked()));
  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), this, SLOT(onApplyPushButtonClicked()));
  connect(m_ui->m_modelComboBox, SIGNAL(activated(int)), this, SLOT(calculate()));
  connect(m_ui->m_nuggetHorizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(calculate()));
  connect(m_ui->m_sillHorizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(calculate()));
  connect(m_ui->m_rangeHorizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(calculate()));

  //create chart
  QGridLayout* chartLayout = new QGridLayout(m_ui->m_chartWidget);
  m_chartDisplay = new te::qt::widgets::ChartDisplay(m_ui->m_chartWidget);
  chartLayout->addWidget(m_chartDisplay);


  m_chartDisplay->setAxisTitle(QwtPlot::xBottom, tr("h"));
  m_chartDisplay->setAxisTitle(QwtPlot::yLeft, tr("Y(h)"));

  //start plot objects
  m_scatterMethod = new te::qt::widgets::Scatter();
  m_scatterChartMethod = new te::qt::widgets::ScatterChart(m_scatterMethod);
  m_scatterChartMethod->setSymbol(new QwtSymbol( QwtSymbol::XCross, QBrush( Qt::red ), QPen( Qt::red, 3 ), QSize( 8, 8 )));
  m_scatterChartMethod->attach(m_chartDisplay);
  m_scatterChartMethod->setTitle(tr("Data"));

  m_scatterModel = new te::qt::widgets::Scatter();
  m_scatterChartModel = new te::qt::widgets::ScatterChart(m_scatterModel);
  m_scatterChartModel->setPen(Qt::blue);
  m_scatterChartModel->setSymbol(0);
  m_scatterChartModel->setStyle(QwtPlotCurve::Lines);
  m_scatterChartModel->attach(m_chartDisplay);
  m_scatterChartModel->setTitle(tr("Model"));

  m_chartDisplay->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

  // help info
  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/sa/sa_geostatisticalmethods.html");
}

te::sa::GeostatisticalMethodsDialog::~GeostatisticalMethodsDialog()
{
  delete m_method;

  m_methodMatrix.clear();

  m_modelMatrix.clear();
}

void te::sa::GeostatisticalMethodsDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while(it != layers.end())
  {
    te::map::AbstractLayerPtr l = *it;

    if(l->isValid())
    {
      std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

      m_ui->m_inputLayerComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));
    }

    ++it;
  }

// fill attributes combo
  if(m_ui->m_inputLayerComboBox->count() > 0)
    onInputLayerComboBoxActivated(0);
}

void te::sa::GeostatisticalMethodsDialog::onInputLayerComboBoxActivated(int index)
{
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(index, Qt::UserRole);
  
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

  std::vector<te::dt::Property*> propVec = dsType->getProperties();

  m_ui->m_attributeComboBox->clear();

  for(std::size_t t = 0; t < propVec.size(); ++t)
  {
    int dataType = propVec[t]->getType();

    if (dataType == te::dt::INT16_TYPE || dataType == te::dt::UINT16_TYPE ||
        dataType == te::dt::INT32_TYPE || dataType == te::dt::UINT32_TYPE ||
        dataType == te::dt::INT64_TYPE || dataType == te::dt::UINT64_TYPE ||
        dataType == te::dt::FLOAT_TYPE || dataType == te::dt::DOUBLE_TYPE)
    {
      m_ui->m_attributeComboBox->addItem(propVec[t]->getName().c_str(), dataType);
    }
  }

  //reset lag increment information
  std::auto_ptr<te::da::DataSet> ds = l->getData();
  double lagIncrement = l->getExtent().getWidth() / ds->size();
  
  QString strLagIncrement;
  strLagIncrement.setNum(lagIncrement);

  m_ui->m_lagsIncrementLineEdit->setText(strLagIncrement);
}

void te::sa::GeostatisticalMethodsDialog::onApplyPushButtonClicked()
{
  //disable adjust options
  m_ui->m_adjustGroupBox->setEnabled(false);
  m_ui->m_changeAttrToolButton->setEnabled(false);

  //check input interface parameters
  if(m_ui->m_nLagsLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Number of lags not defined."));
    return;
  }

  if(m_ui->m_lagsIncrementLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Lag increment not defined."));
    return;
  }

  if(m_ui->m_angleDirLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Angular direction not defined."));
    return;
  }

  if(m_ui->m_angleTolLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Angular tolerance not defined."));
    return;
  }

  //get selected layer
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  //get dataset and dataset type
  std::auto_ptr<te::da::DataSet> dataSet = l->getData();
  std::auto_ptr<te::da::DataSetType> dataSetType = l->getSchema();

  //get properties information
  std::string attrName = m_ui->m_attributeComboBox->currentText().toStdString();
  int attrIdx = te::da::GetPropertyIndex(dataSet.get(), attrName);

  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dataSetType.get());
  int geomIdx = te::da::GetPropertyIndex(dataSet.get(), geomProp->getName());

  //create geostatistical method
  delete m_method;

  int typeIdx = m_ui->m_typeComboBox->currentIndex();
  te::sa::GeostatisticalMethodType type = (te::sa::GeostatisticalMethodType)m_ui->m_typeComboBox->itemData(typeIdx).toInt();

  if(type == te::sa::Semivariogram)
  {
    m_method = new te::sa::GeostatisticalMethodSemivariogram();
  }
  else
  {
    m_method = 0;
    return;
  }

  //set geostatistical method parameters
  m_method->setNumberOfLags((std::size_t)m_ui->m_nLagsLineEdit->text().toInt());
  m_method->setLagIncrement(m_ui->m_lagsIncrementLineEdit->text().toDouble());
  m_method->setAngleDirection(m_ui->m_angleDirLineEdit->text().toDouble());
  m_method->setAngleTolerance(m_ui->m_angleTolLineEdit->text().toDouble());
  m_method->setMatrix(te::sa::CreateMatrixFromDataSet(dataSet.get(), attrIdx, geomIdx));

  //calculate moments
  double mean = 0.;
  double variance = 0.;

  te::sa::CalculateMoments(m_method->getMatrix(), mean, variance);

  //reset adjust values
  resetAdjustParameters(mean, variance);

  //run
  calculate();

  //enable adjust options
  m_ui->m_adjustGroupBox->setEnabled(true);
  m_ui->m_changeAttrToolButton->setEnabled(true);
}

void te::sa::GeostatisticalMethodsDialog::onChangeAttrToolButtonClicked()
{
  //get selected layer
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  //get dataset and dataset type
  std::auto_ptr<te::da::DataSet> dataSet = l->getData();

  //get properties information
  std::string attrName = m_ui->m_attributeComboBox->currentText().toStdString();
  int attrIdx = te::da::GetPropertyIndex(dataSet.get(), attrName);

  te::sa::SetMainDiagonal(m_method->getMatrix(), dataSet.get(), attrIdx);

  //calculate moments
  double mean = 0.;
  double variance = 0.;

  te::sa::CalculateMoments(m_method->getMatrix(), mean, variance);

  //reset adjust values
  resetAdjustParameters(mean, variance);

  //run
  calculate();
}

void te::sa::GeostatisticalMethodsDialog::calculate()
{
 //create geostatistical model
  int modelIdx = m_ui->m_modelComboBox->currentIndex();
  te::sa::GeostatisticalModelType type = (te::sa::GeostatisticalModelType)m_ui->m_modelComboBox->itemData(modelIdx).toInt();

  te::sa::GeostatisticalModel* model = 0;

  if(type == te::sa::Spherical)
    model = new te::sa::GeostatisticalModelSpherical();
  else if(type == te::sa::Exponential)
    model = new te::sa::GeostatisticalModelExponential();
  else if(type == te::sa::Gaussian)
    model = new te::sa::GeostatisticalModelGaussian();

  model->setNugget((double)m_ui->m_nuggetHorizontalSlider->value());
  model->setSill((double)m_ui->m_sillHorizontalSlider->value());
  model->setRange((double)m_ui->m_rangeHorizontalSlider->value());

  //generate output matrix
  m_methodMatrix = m_method->calculate();
  m_modelMatrix = model->calculate(m_methodMatrix);

  delete model;

  //plot
  plot();
}

void te::sa::GeostatisticalMethodsDialog::fillParameters()
{
  //fill msv types
  m_ui->m_typeComboBox->clear();

  m_ui->m_typeComboBox->addItem("Semivariogram", QVariant(te::sa::Semivariogram));

  //fill msv models
  m_ui->m_modelComboBox->clear();

  m_ui->m_modelComboBox->addItem("Spherical", QVariant(te::sa::Spherical));
  m_ui->m_modelComboBox->addItem("Exponential", QVariant(te::sa::Exponential));
  m_ui->m_modelComboBox->addItem("Gaussian", QVariant(te::sa::Gaussian));
}

void te::sa::GeostatisticalMethodsDialog::resetAdjustParameters(double mean, double variance)
{
  //set nugget value
  double min = 0.;
  double max = variance;
  double nuggetVar = (max - min)/mean;

  m_ui->m_nuggetHorizontalSlider->setRange(min - nuggetVar, max + nuggetVar);
  m_ui->m_nuggetHorizontalSlider->setValue(min);
  m_ui->m_nuggetLabel->setNum(min);

  //set sill value
  min = 0;
  max = max + variance;
  double sillVar = (max - min)/mean;

  m_ui->m_sillHorizontalSlider->setRange(min - sillVar, max + sillVar);
  m_ui->m_sillHorizontalSlider->setValue(min);
  m_ui->m_sillLabel->setNum(min);

  //set range value
  double rangeVar = m_ui->m_nLagsLineEdit->text().toDouble();
  min = m_ui->m_lagsIncrementLineEdit->text().toDouble();
  max = min * rangeVar;

  m_ui->m_rangeHorizontalSlider->setRange(min - rangeVar, max + rangeVar);
  m_ui->m_rangeHorizontalSlider->setValue(min);
  m_ui->m_rangeLabel->setNum(min);
}

void te::sa::GeostatisticalMethodsDialog::plot()
{
  m_chartDisplay->setTitle(m_ui->m_typeComboBox->currentText());

  //plot method curve
  std::vector<double> methodh;
  std::vector<double> methodyh;

  for(std::size_t t = 1; t < m_methodMatrix.size1(); ++t)
  {
    methodh.push_back(m_methodMatrix(t, 0));
    methodyh.push_back(m_methodMatrix(t, 1));
  }

  m_scatterMethod->setXValues(methodh);
  m_scatterMethod->setYValues(methodyh);
  m_scatterMethod->calculateMinMaxValues();
  m_scatterChartMethod->setData();

  //plot model curve
  std::vector<double> modelh;
  std::vector<double> modelyh;

  for(std::size_t t = 0; t < m_modelMatrix.size1(); ++t)
  {
    modelh.push_back(m_modelMatrix(t, 0));
    modelyh.push_back(m_modelMatrix(t, 1));
  }

  m_scatterModel->setXValues(modelh);
  m_scatterModel->setYValues(modelyh);
  m_scatterModel->calculateMinMaxValues();
  m_scatterChartModel->setData();

  //replot the chart display
  m_chartDisplay->replot();
}
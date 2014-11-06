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
  \file terralib/qt/widgets/rp/TiePointLocatorParametersWidget.cpp

  \brief This file has the TiePointLocatorParametersWidget class.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry/GTFactory.h"
#include "../../../geometry/GTParameters.h"
#include "TiePointLocatorParametersWidget.h"
#include "ui_TiePointLocatorParametersWidgetForm.h"

// Qt
#include <QMessageBox>

te::qt::widgets::TiePointLocatorParametersWidget::TiePointLocatorParametersWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::TiePointLocatorParametersWidgetForm)
{
  m_ui->setupUi(this);

  startAdvancedOptions();
}

te::qt::widgets::TiePointLocatorParametersWidget::~TiePointLocatorParametersWidget()
{
}

void te::qt::widgets::TiePointLocatorParametersWidget::setMosaicLayout()
{
  m_ui->m_glLabel->setVisible(false);
  m_ui->m_enableGeometryFilterCheckBox->setVisible(false);

  m_ui->m_mtLabel->setVisible(false);
  m_ui->m_enableMultiThreadCheckBox->setVisible(false);

  m_ui->m_gtLabel->setVisible(false);
  m_ui->m_geomTransfNameComboBox->setVisible(false);

  m_ui->m_interLabel->setVisible(false);
  m_ui->m_interpMethodComboBox->setVisible(false);
}

void te::qt::widgets::TiePointLocatorParametersWidget::setRescaleFactor(double value)
{
  m_ui->m_rescaleFactorLineEdit->setText(QString::number(value));
}

std::string te::qt::widgets::TiePointLocatorParametersWidget::getTransformationName()
{
  return  m_ui->m_geomTransfNameComboBox->currentText().toStdString();
}

te::rp::TiePointsLocator::InputParameters te::qt::widgets::TiePointLocatorParametersWidget::getTiePointInputParameters()
{
  updateAdvancedOptions();

  return m_inputParameters;
}

void te::qt::widgets::TiePointLocatorParametersWidget::startAdvancedOptions()
{
  m_ui->m_enableGeometryFilterCheckBox->setChecked(m_inputParameters.m_enableGeometryFilter);
  m_ui->m_enableMultiThreadCheckBox->setChecked(m_inputParameters.m_enableMultiThread);

  switch( m_inputParameters.m_interesPointsLocationStrategy )
  {
    case te::rp::TiePointsLocator::InputParameters::SurfStrategyT :
    {
      int idx = m_ui->m_interesPointsLocationStrategyComboBox->findText("Surf");
      m_ui->m_interesPointsLocationStrategyComboBox->setCurrentIndex(idx);
      break;
    }
    default:
    {
      int idx = m_ui->m_interesPointsLocationStrategyComboBox->findText("Moravec");
      m_ui->m_interesPointsLocationStrategyComboBox->setCurrentIndex(idx);
    }
  }

  te::gm::GTFactory::dictionary_type::const_iterator gtItB = te::gm::GTFactory::getDictionary().begin();
  const te::gm::GTFactory::dictionary_type::const_iterator gtItE = te::gm::GTFactory::getDictionary().end();

  while( gtItB != gtItE )
  {
    m_ui->m_geomTransfNameComboBox->addItem(QString(gtItB->first.c_str()));
    ++gtItB;
  }

  int idx = m_ui->m_geomTransfNameComboBox->findText(m_inputParameters.m_geomTransfName.c_str());
  m_ui->m_geomTransfNameComboBox->setCurrentIndex(idx);

  m_ui->m_geometryFilterAssuranceLineEdit->setText(QString::number(m_inputParameters.m_geometryFilterAssurance));

  m_ui->m_geomTransfMaxErrorLineEdit->setText(QString::number(m_inputParameters.m_geomTransfMaxError));

  switch(m_inputParameters.m_interpMethod)
  {
    case te::rst::Interpolator::Bilinear :
    {
      int idx = m_ui->m_interpMethodComboBox->findText("Bilinear");
      m_ui->m_interpMethodComboBox->setCurrentIndex(idx);
      break;
    }
    case te::rst::Interpolator::Bicubic :
    {
      int idx = m_ui->m_interpMethodComboBox->findText("Bicubic");
      m_ui->m_interpMethodComboBox->setCurrentIndex(idx);
      break;
    }
    default:
    {
      int idx = m_ui->m_interpMethodComboBox->findText("NearestNeighbor");
      m_ui->m_interpMethodComboBox->setCurrentIndex(idx);
    }
  }

  m_ui->m_maxTiePointsLineEdit->setText(QString::number(m_inputParameters.m_maxTiePoints));

  m_ui->m_correlationWindowWidthLineEdit->setText(QString::number(m_inputParameters.m_moravecCorrelationWindowWidth));

  m_ui->m_gaussianFilterIterationsLineEdit->setText(QString::number(m_inputParameters.m_moravecNoiseFilterIterations));

  m_ui->m_minAbsCorrelationLineEdit->setText(QString::number(m_inputParameters.m_moravecMinAbsCorrelation));

  m_ui->m_moravecWindowWidthLineEdit->setText(QString::number(m_inputParameters.m_moravecWindowWidth));

  m_ui->m_maxNormEuclideanDistLineEdit->setText(QString::number(m_inputParameters.m_surfMaxNormEuclideanDist));

  m_ui->m_octavesNumberLineEdit->setText(QString::number(m_inputParameters.m_surfOctavesNumber));

  m_ui->m_scalesNumberLineEdit->setText(QString::number(m_inputParameters.m_surfScalesNumber));

  m_ui->m_rescaleFactorLineEdit->setText(QString::number(m_inputParameters.m_subSampleOptimizationRescaleFactor));
}

void te::qt::widgets::TiePointLocatorParametersWidget::updateAdvancedOptions()
{
  m_inputParameters.m_enableGeometryFilter = m_ui->m_enableGeometryFilterCheckBox->isChecked();

  m_inputParameters.m_enableMultiThread = m_ui->m_enableMultiThreadCheckBox->isChecked();

  if(m_ui->m_interesPointsLocationStrategyComboBox->currentText() == "Surf")
    m_inputParameters.m_interesPointsLocationStrategy = te::rp::TiePointsLocator::InputParameters::SurfStrategyT;
  else
    m_inputParameters.m_interesPointsLocationStrategy = te::rp::TiePointsLocator::InputParameters::MoravecStrategyT;

  m_inputParameters.m_geomTransfName = m_ui->m_geomTransfNameComboBox->currentText().toStdString();

  if(m_ui->m_geometryFilterAssuranceLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Geometry assurance not defined."));
    return;
  }
  m_inputParameters.m_geometryFilterAssurance = m_ui->m_geometryFilterAssuranceLineEdit->text().toDouble();

  if(m_ui->m_geomTransfMaxErrorLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Transformation error not defined."));
    return;
  }
  m_inputParameters.m_geomTransfMaxError = m_ui->m_geomTransfMaxErrorLineEdit->text().toDouble();

  if(m_ui->m_interpMethodComboBox->currentText() == "Bilinear")
    m_inputParameters.m_interpMethod = te::rst::Interpolator::Bilinear;
  else if(m_ui->m_interpMethodComboBox->currentText() == "Bicubic")
    m_inputParameters.m_interpMethod = te::rst::Interpolator::Bicubic;
  else
    m_inputParameters.m_interpMethod = te::rst::Interpolator::NearestNeighbor;

  if(m_ui->m_maxTiePointsLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Maximum number of tie-points not defined."));
    return;
  }
  m_inputParameters.m_maxTiePoints =  m_ui->m_maxTiePointsLineEdit->text().toUInt();

  if(m_ui->m_rescaleFactorLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Sub-sampled search rescale factor not defined."));
    return;
  }
  m_inputParameters.m_subSampleOptimizationRescaleFactor = m_ui->m_rescaleFactorLineEdit->text().toDouble();

  if(m_ui->m_interesPointsLocationStrategyComboBox->currentText() == "Surf")
  {
    if(m_ui->m_maxNormEuclideanDistLineEdit->text().isEmpty())
    {
      QMessageBox::warning(this, tr("Warning"), tr("Maximum euclidean distance not defined."));
      return;
    }
    m_inputParameters.m_surfMaxNormEuclideanDist = m_ui->m_maxNormEuclideanDistLineEdit->text().toDouble();

    if(m_ui->m_octavesNumberLineEdit->text().isEmpty())
    {
      QMessageBox::warning(this, tr("Warning"), tr("Octaves number not defined."));
      return;
    }
    m_inputParameters.m_surfOctavesNumber = m_ui->m_octavesNumberLineEdit->text().toUInt();

    if(m_ui->m_scalesNumberLineEdit->text().isEmpty())
    {
      QMessageBox::warning(this, tr("Warning"), tr("Scales number not defined."));
      return;
    }
    m_inputParameters.m_surfScalesNumber = m_ui->m_scalesNumberLineEdit->text().toUInt();
  }
  else
  {
    if(m_ui->m_correlationWindowWidthLineEdit->text().isEmpty())
    {
      QMessageBox::warning(this, tr("Warning"), tr("Correlation window width not defined."));
      return;
    }
    m_inputParameters.m_moravecCorrelationWindowWidth = m_ui->m_correlationWindowWidthLineEdit->text().toUInt();

    if(m_ui->m_gaussianFilterIterationsLineEdit->text().isEmpty())
    {
      QMessageBox::warning(this, tr("Warning"), tr("Gaussian filter iterations not defined."));
      return;
    }
    m_inputParameters.m_moravecNoiseFilterIterations = m_ui->m_gaussianFilterIterationsLineEdit->text().toUInt();

    if(m_ui->m_minAbsCorrelationLineEdit->text().isEmpty())
    {
      QMessageBox::warning(this, tr("Warning"), tr("Minimum correlation value not defined."));
      return;
    }
    m_inputParameters.m_moravecMinAbsCorrelation = m_ui->m_minAbsCorrelationLineEdit->text().toDouble();

    if(m_ui->m_moravecWindowWidthLineEdit->text().isEmpty())
    {
      QMessageBox::warning(this, tr("Warning"), tr("Moravec window width not defined."));
      return;
    }
    m_inputParameters.m_moravecWindowWidth = m_ui->m_moravecWindowWidthLineEdit->text().toUInt();
  }
}

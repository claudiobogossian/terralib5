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
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Interpolator.h"
#include "../../../raster/Raster.h"
#include "FusionWizardPage.h"
#include "ui_FusionWizardPageForm.h"

// Qt
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>

// stl
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::FusionWizardPage::FusionWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::FusionWizardPageForm),
    m_layerLower(0),
    m_layerHigher(0)
{
// setup controls
  m_ui->setupUi(this);

  fillFusionTypes();

//configure page
  this->setTitle(tr("Fusion"));
  this->setSubTitle(tr("Select the type of fusion and set their specific parameters."));
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

  listBandsLower();
}

void te::qt::widgets::FusionWizardPage::setHigher(te::map::AbstractLayerPtr layer)
{
  m_layerHigher = layer;

  m_ui->m_higherLineEdit->setText(m_layerHigher->getTitle().c_str());

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

  algoInputParams.m_highResRasterBand = m_ui->m_bandComboBox->currentText().toInt();

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

  algoInputParams.m_highResRasterBand = m_ui->m_bandComboBox->currentText().toInt();

  return algoInputParams;
}

te::rp::PCAFusion::OutputParameters te::qt::widgets::FusionWizardPage::getOutputPCAParams()
{
  te::rp::PCAFusion::OutputParameters algoOutputParams;

  return algoOutputParams;
}

void te::qt::widgets::FusionWizardPage::fillFusionTypes()
{
  m_ui->m_fusionTypeComboBox->clear();

  m_ui->m_fusionTypeComboBox->addItem(tr("IHS"), FUSION_IHS);
  m_ui->m_fusionTypeComboBox->addItem(tr("PCA"), FUSION_PCA);

  m_ui->m_interpolatorComboBox->clear();

  m_ui->m_interpolatorComboBox->addItem(tr("Nearest Neighbor"), te::rst::Interpolator::NearestNeighbor);
  m_ui->m_interpolatorComboBox->addItem(tr("Bilinear"), te::rst::Interpolator::Bilinear);
  m_ui->m_interpolatorComboBox->addItem(tr("Bicubic"), te::rst::Interpolator::Bicubic);
}

void te::qt::widgets::FusionWizardPage::listBandsLower()
{
  m_ui->m_ihsRComboBox->clear();
  m_ui->m_ihsGComboBox->clear();
  m_ui->m_ihsBComboBox->clear();
  m_ui->m_listWidget->clear();

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
      }
    }
  }
}

void te::qt::widgets::FusionWizardPage::listBandsHigher()
{
  m_ui->m_bandComboBox->clear();

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
        m_ui->m_bandComboBox->addItem(QString::number(i));
      }
    }
  }
}

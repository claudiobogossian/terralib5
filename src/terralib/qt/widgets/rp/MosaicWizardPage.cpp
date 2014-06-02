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
  \file terralib/qt/widgets/rp/MosaicWizardPage.cpp

  \brief This file defines a class for a Mosaic Wizard page.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GTFactory.h"
#include "../../../raster/Interpolator.h"
#include "../../../rp/Blender.h"
#include "TiePointLocatorDialog.h"
#include "TiePointLocatorWidget.h"
#include "MosaicWizardPage.h"
#include "ui_MosaicWizardPageForm.h"

// Qt
#include <QApplication>
#include <QGridLayout>
#include <QMessageBox>

// stl
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::MosaicWizardPage::MosaicWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::MosaicWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

  fillMosaicTypes();

//configure page
  this->setTitle(tr("Mosaic"));
  this->setSubTitle(tr("Select the type of mosaic and set their specific parameters."));

  m_ui->m_noDataValueLineEdit->setValidator(new QDoubleValidator(this));

  m_ui->m_tpmAcquireToolButton->setIcon(QIcon::fromTheme("wand"));

  //connects
  connect(m_ui->m_tpmAcquireToolButton, SIGNAL(clicked()), this, SLOT(onTiePointsAcquiredToolButtonClicked()));
}

te::qt::widgets::MosaicWizardPage::~MosaicWizardPage()
{
  m_tiePoints.clear();
}

bool te::qt::widgets::MosaicWizardPage::isComplete() const
{
  return true;
}

bool te::qt::widgets::MosaicWizardPage::isGeoMosaic()
{
  int idx = m_ui->m_mosaicTypeComboBox->currentIndex();

  int type = m_ui->m_mosaicTypeComboBox->itemData(idx).toInt();

  return (type == MOSAIC_GEO);
}

bool te::qt::widgets::MosaicWizardPage::isTiePointMosaic()
{
  int idx = m_ui->m_mosaicTypeComboBox->currentIndex();

  int type = m_ui->m_mosaicTypeComboBox->itemData(idx).toInt();

  return (type == MOSAIC_TIEPOINT);
}

bool te::qt::widgets::MosaicWizardPage::isSequenceMosaic()
{
  int idx = m_ui->m_mosaicTypeComboBox->currentIndex();

  int type = m_ui->m_mosaicTypeComboBox->itemData(idx).toInt();

  return (type == MOSAIC_SEQUENCE);
}

void te::qt::widgets::MosaicWizardPage::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerList = layerList;

  //fill layer combos
  m_ui->m_tpmLayerAComboBox->clear();
  m_ui->m_tpmLayerBComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layerList.begin();
  std::vector<std::size_t> bands;

  while(it != m_layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSet> ds = l->getData();
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> rst = ds->getRaster(rpos);

    if(rst.get())
      bands.push_back(rst->getNumberOfBands());

    m_ui->m_tpmLayerAComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
    m_ui->m_tpmLayerBComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }

  //fill band info
  std::vector<std::size_t>::iterator itBand = std::min_element(bands.begin(), bands.end());

  for(std::size_t t = 0; t < *itBand; ++t)
  {
    m_ui->m_smRefBandComboBox->addItem(QString::number(t));
  }
}

te::rp::GeoMosaic::InputParameters te::qt::widgets::MosaicWizardPage::getInputGeoParams()
{
  int interpolationIdx = m_ui->m_interpolatorTypeComboBox->currentIndex();
  te::rst::Interpolator::Method interpolator = (te::rst::Interpolator::Method)m_ui->m_interpolatorTypeComboBox->itemData(interpolationIdx).toInt();

  int blenderIdx = m_ui->m_blenderTypeComboBox->currentIndex();
  te::rp::Blender::BlendMethod blender = (te::rp::Blender::BlendMethod)m_ui->m_blenderTypeComboBox->itemData(blenderIdx).toInt();

  te::rp::GeoMosaic::InputParameters algoInputParams;

  algoInputParams.m_interpMethod = interpolator;
  algoInputParams.m_blendMethod = blender;
  algoInputParams.m_noDataValue = m_ui->m_noDataValueLineEdit->text().isEmpty() ? 0 : m_ui->m_noDataValueLineEdit->text().toDouble();
  algoInputParams.m_forceInputNoDataValue = m_ui->m_noDataValueCheckBox->isChecked();
  algoInputParams.m_autoEqualize = m_ui->m_autoEqualizeCheckBox->isChecked();
  algoInputParams.m_useRasterCache = m_ui->m_rasterCacheCheckBox->isChecked();

  return algoInputParams;
}

te::rp::GeoMosaic::OutputParameters te::qt::widgets::MosaicWizardPage::getOutputGeoParams()
{
  te::rp::GeoMosaic::OutputParameters algoOutputParams;

  return algoOutputParams;
}

te::rp::TiePointsMosaic::InputParameters te::qt::widgets::MosaicWizardPage::getInputTPParams()
{
  te::rp::TiePointsMosaic::InputParameters algoInputParams;

  int interpolationIdx = m_ui->m_interpolatorTypeComboBox->currentIndex();
  te::rst::Interpolator::Method interpolator = (te::rst::Interpolator::Method)m_ui->m_interpolatorTypeComboBox->itemData(interpolationIdx).toInt();

  int blenderIdx = m_ui->m_blenderTypeComboBox->currentIndex();
  te::rp::Blender::BlendMethod blender = (te::rp::Blender::BlendMethod)m_ui->m_blenderTypeComboBox->itemData(blenderIdx).toInt();

  int linkerIdx = m_ui->m_tpmLinkTypeComboBox->currentIndex();
  te::rp::TiePointsMosaic::InputParameters::TiePointsLinkType tpLinkType = (te::rp::TiePointsMosaic::InputParameters::TiePointsLinkType)m_ui->m_tpmLinkTypeComboBox->itemData(linkerIdx).toInt();

  algoInputParams.m_interpMethod = interpolator;
  algoInputParams.m_blendMethod = blender;
  algoInputParams.m_noDataValue = m_ui->m_noDataValueLineEdit->text().isEmpty() ? 0 : m_ui->m_noDataValueLineEdit->text().toDouble();
  algoInputParams.m_forceInputNoDataValue = m_ui->m_noDataValueCheckBox->isChecked();
  algoInputParams.m_autoEqualize = m_ui->m_autoEqualizeCheckBox->isChecked();
  algoInputParams.m_useRasterCache = m_ui->m_rasterCacheCheckBox->isChecked();

  algoInputParams.m_geomTransfName = m_ui->m_smGeomTransformComboBox->currentText().toStdString();
  algoInputParams.m_tiePointsLinkType = tpLinkType;
  algoInputParams.m_tiePoints = m_tiePoints;


  return algoInputParams;
}

te::rp::TiePointsMosaic::OutputParameters te::qt::widgets::MosaicWizardPage::getOutputTPParams()
{
  te::rp::TiePointsMosaic::OutputParameters algoOutputParams;

  return algoOutputParams;
}

te::rp::SequenceMosaic::InputParameters te::qt::widgets::MosaicWizardPage::getInputSeqParams()
{
  te::rp::SequenceMosaic::InputParameters algoInputParams;

  int interpolationIdx = m_ui->m_interpolatorTypeComboBox->currentIndex();
  te::rst::Interpolator::Method interpolator = (te::rst::Interpolator::Method)m_ui->m_interpolatorTypeComboBox->itemData(interpolationIdx).toInt();

  int blenderIdx = m_ui->m_blenderTypeComboBox->currentIndex();
  te::rp::Blender::BlendMethod blender = (te::rp::Blender::BlendMethod)m_ui->m_blenderTypeComboBox->itemData(blenderIdx).toInt();

  algoInputParams.m_interpMethod = interpolator;
  algoInputParams.m_blendMethod = blender;
  algoInputParams.m_noDataValue = m_ui->m_noDataValueLineEdit->text().isEmpty() ? 0 : m_ui->m_noDataValueLineEdit->text().toDouble();
  algoInputParams.m_forceInputNoDataValue = m_ui->m_noDataValueCheckBox->isChecked();
  algoInputParams.m_autoEqualize = m_ui->m_autoEqualizeCheckBox->isChecked();
  algoInputParams.m_useRasterCache = m_ui->m_rasterCacheCheckBox->isChecked();

  algoInputParams.m_geomTransfName = m_ui->m_smGeomTransformComboBox->currentText().toStdString();
  algoInputParams.m_tiePointsLocationBandIndex = m_ui->m_smRefBandComboBox->currentText().toInt();
  algoInputParams.m_minRequiredTiePointsCoveredAreaPercent = m_ui->m_smMinTiePointsSpinBox->value();

  algoInputParams.m_enableMultiThread = true;
  algoInputParams.m_enableProgress = true;

  // The parameters used by the tie-points locator when processing each rasters pair was leaved untouched to use the default.

  return algoInputParams;
}

te::rp::SequenceMosaic::OutputParameters te::qt::widgets::MosaicWizardPage::getOutputSeqParams()
{
  te::rp::SequenceMosaic::OutputParameters algoOutputParams;

  return algoOutputParams;
}

te::map::AbstractLayerPtr te::qt::widgets::MosaicWizardPage::getTiePointMosaicLayerA()
{
  int aIdx = m_ui->m_tpmLayerAComboBox->currentIndex();
  QVariant aVarLayer = m_ui->m_tpmLayerAComboBox->itemData(aIdx, Qt::UserRole);
  te::map::AbstractLayerPtr aLayer = aVarLayer.value<te::map::AbstractLayerPtr>();

  return aLayer;
}

te::map::AbstractLayerPtr te::qt::widgets::MosaicWizardPage::getTiePointMosaicLayerB()
{
  int bIdx = m_ui->m_tpmLayerBComboBox->currentIndex();
  QVariant bVarLayer = m_ui->m_tpmLayerBComboBox->itemData(bIdx, Qt::UserRole);
  te::map::AbstractLayerPtr bLayer = bVarLayer.value<te::map::AbstractLayerPtr>();

  return bLayer;
}

void te::qt::widgets::MosaicWizardPage::onTiePointsAcquiredToolButtonClicked()
{
  int aIdx = m_ui->m_tpmLayerAComboBox->currentIndex();
  QVariant aVarLayer = m_ui->m_tpmLayerAComboBox->itemData(aIdx, Qt::UserRole);
  te::map::AbstractLayerPtr aLayer = aVarLayer.value<te::map::AbstractLayerPtr>();

  int bIdx = m_ui->m_tpmLayerBComboBox->currentIndex();
  QVariant bVarLayer = m_ui->m_tpmLayerBComboBox->itemData(bIdx, Qt::UserRole);
  te::map::AbstractLayerPtr bLayer = bVarLayer.value<te::map::AbstractLayerPtr>();

  te::qt::widgets::TiePointLocatorDialog dlg(this);

  dlg.setReferenceLayer(aLayer);
  dlg.setAdjustLayer(bLayer);

  dlg.exec();

  std::vector<te::gm::GTParameters::TiePoint> tiePoints;
 
  dlg.getWidget()->getTiePointsIdxCoords(tiePoints);

  m_tiePoints.clear();
  m_tiePoints.push_back(tiePoints);
}


void te::qt::widgets::MosaicWizardPage::fillMosaicTypes()
{
  //mosaic types
  m_ui->m_mosaicTypeComboBox->clear();

  m_ui->m_mosaicTypeComboBox->addItem(tr("Geo Mosaic"), MOSAIC_GEO);
  m_ui->m_mosaicTypeComboBox->addItem(tr("Tie Points Mosaic"), MOSAIC_TIEPOINT);
  m_ui->m_mosaicTypeComboBox->addItem(tr("Sequence Mosaic"), MOSAIC_SEQUENCE);

  //interpolator types
  m_ui->m_interpolatorTypeComboBox->clear();

  m_ui->m_interpolatorTypeComboBox->addItem(tr("Nearest Neighbor"), te::rst::Interpolator::NearestNeighbor);
  m_ui->m_interpolatorTypeComboBox->addItem(tr("Bilinear"), te::rst::Interpolator::Bilinear);
  m_ui->m_interpolatorTypeComboBox->addItem(tr("Bicubic"), te::rst::Interpolator::Bicubic);

  //blender types
  m_ui->m_blenderTypeComboBox->clear();

  m_ui->m_blenderTypeComboBox->addItem(tr("No blending performed"), te::rp::Blender::NoBlendMethod);
  m_ui->m_blenderTypeComboBox->addItem(tr("Euclidean distance method"), te::rp::Blender::EuclideanDistanceMethod);
  m_ui->m_blenderTypeComboBox->addItem(tr("Invalid blending method"), te::rp::Blender::InvalidBlendMethod);

  //tie points link types
  m_ui->m_tpmLinkTypeComboBox->clear();

  m_ui->m_tpmLinkTypeComboBox->addItem(tr("Linking adjacent raster pairs"), te::rp::TiePointsMosaic::InputParameters::AdjacentRastersLinkingTiePointsT);
  m_ui->m_tpmLinkTypeComboBox->addItem(tr("Linking any raster to the first raster"), te::rp::TiePointsMosaic::InputParameters::FirstRasterLinkingTiePointsT);
  m_ui->m_tpmLinkTypeComboBox->addItem(tr("Invalid linking type"), te::rp::TiePointsMosaic::InputParameters::InvalidTiePointsT);

  m_ui->m_tpmLinkTypeComboBox->setEnabled(false);

  //geometric transformations
  m_ui->m_tpmGeomTransformComboBox->clear();
  m_ui->m_smGeomTransformComboBox->clear();

  te::gm::GTFactory::dictionary_type::const_iterator gtItB = te::gm::GTFactory::getDictionary().begin();
  const te::gm::GTFactory::dictionary_type::const_iterator gtItE = te::gm::GTFactory::getDictionary().end();

  while( gtItB != gtItE )
  {
    m_ui->m_tpmGeomTransformComboBox->addItem(QString(gtItB->first.c_str()));
    m_ui->m_smGeomTransformComboBox->addItem(QString(gtItB->first.c_str()));
    ++gtItB;
  }
}


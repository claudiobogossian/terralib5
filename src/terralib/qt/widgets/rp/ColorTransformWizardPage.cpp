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
  \file terralib/qt/widgets/rp/ColorTransformWizardPage.cpp

  \brief This file defines a class for a ColorTransform Wizard page.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "ColorTransformWizardPage.h"
#include "ui_ColorTransformWizardPageForm.h"

// Qt
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>

// stl
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ColorTransformWizardPage::ColorTransformWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ColorTransformWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

  fillColorTransformTypes();

//configure page
  this->setTitle(tr("Color Transform"));
  this->setSubTitle(tr("Select the type of color transformation and set their specific parameters."));

  m_ui->m_rgbMinLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_rgbMaxLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_ihsMinLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_ihsMaxLineEdit->setValidator(new QDoubleValidator(this));

  //connects
  connect(m_ui->m_rgbRComboBox, SIGNAL(activated(int)), this, SLOT(rgbRComboBoxActivated(int)));
  connect(m_ui->m_rgbGComboBox, SIGNAL(activated(int)), this, SLOT(rgbGComboBoxActivated(int)));
  connect(m_ui->m_rgbBComboBox, SIGNAL(activated(int)), this, SLOT(rgbBComboBoxActivated(int)));

  connect(m_ui->m_ihsIComboBox, SIGNAL(activated(int)), this, SLOT(ihsIComboBoxActivated(int)));
  connect(m_ui->m_ihsHComboBox, SIGNAL(activated(int)), this, SLOT(ihsHComboBoxActivated(int)));
  connect(m_ui->m_ihsSComboBox, SIGNAL(activated(int)), this, SLOT(ihsSComboBoxActivated(int)));
}

te::qt::widgets::ColorTransformWizardPage::~ColorTransformWizardPage()
{

}

bool te::qt::widgets::ColorTransformWizardPage::isComplete() const
{
  return true;
}

bool te::qt::widgets::ColorTransformWizardPage::isRGB2IHS()
{
  int idx = m_ui->m_colorTransformTypeComboBox->currentIndex();

  int type = m_ui->m_colorTransformTypeComboBox->itemData(idx).toInt();

  return (type == COLORTRANSFORM_RGB2IHS);
}

bool te::qt::widgets::ColorTransformWizardPage::isIHS2RGB()
{
  int idx = m_ui->m_colorTransformTypeComboBox->currentIndex();

  int type = m_ui->m_colorTransformTypeComboBox->itemData(idx).toInt();

  return (type == COLORTRANSFORM_IHS2RGB);
}

te::rst::Raster* te::qt::widgets::ColorTransformWizardPage::getRGBRaster()
{
  //TEMPORARY USE ONLY ONE LAYER

  QVariant varLayer = m_ui->m_rgbRComboBox->itemData(0, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();
  std::auto_ptr<te::da::DataSet> ds = l->getData();
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

  return ds->getRaster(rpos).release();
}

unsigned int te::qt::widgets::ColorTransformWizardPage::getRGBRBand()
{
  return m_ui->m_rgbRBandComboBox->currentText().toUInt();
}

unsigned int te::qt::widgets::ColorTransformWizardPage::getRGBGBand()
{
  return m_ui->m_rgbGBandComboBox->currentText().toUInt();
}

unsigned int te::qt::widgets::ColorTransformWizardPage::getRGBBBand()
{
  return m_ui->m_rgbBBandComboBox->currentText().toUInt();
}

double te::qt::widgets::ColorTransformWizardPage::getRGBMinValue()
{
  return m_ui->m_rgbMinLineEdit->text().isEmpty() ? 0 : m_ui->m_rgbMinLineEdit->text().toDouble();
}

double te::qt::widgets::ColorTransformWizardPage::getRGBMaxValue()
{
  return m_ui->m_rgbMaxLineEdit->text().isEmpty() ? 255 : m_ui->m_rgbMaxLineEdit->text().toDouble();
}

te::rst::Raster* te::qt::widgets::ColorTransformWizardPage::getIHSRaster()
{
  //TEMPORARY USE ONLY ONE LAYER

  QVariant varLayer = m_ui->m_ihsIComboBox->itemData(0, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();
  std::auto_ptr<te::da::DataSet> ds = l->getData();
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

  return ds->getRaster(rpos).release();
}

unsigned int te::qt::widgets::ColorTransformWizardPage::getIHSIBand()
{
  return m_ui->m_ihsIBandComboBox->currentText().toUInt();
}

unsigned int te::qt::widgets::ColorTransformWizardPage::getIHSHBand()
{
  return m_ui->m_ihsHBandComboBox->currentText().toUInt();
}

unsigned int te::qt::widgets::ColorTransformWizardPage::getIHSSBand()
{
  return m_ui->m_ihsSBandComboBox->currentText().toUInt();
}

double te::qt::widgets::ColorTransformWizardPage::getIHSMinValue()
{
  return m_ui->m_ihsMinLineEdit->text().isEmpty() ? 0 : m_ui->m_ihsMinLineEdit->text().toDouble();
}

double te::qt::widgets::ColorTransformWizardPage::getIHSMaxValue()
{
  return m_ui->m_ihsMaxLineEdit->text().isEmpty() ? 255 : m_ui->m_ihsMaxLineEdit->text().toDouble();
}

void te::qt::widgets::ColorTransformWizardPage::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerList = layerList;

  //fill layer combos
  m_ui->m_rgbRComboBox->clear();
  m_ui->m_rgbGComboBox->clear();
  m_ui->m_rgbBComboBox->clear();

  m_ui->m_ihsIComboBox->clear();
  m_ui->m_ihsHComboBox->clear();
  m_ui->m_ihsSComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layerList.begin();
  std::vector<std::size_t> bands;

  while(it != m_layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dst = l->getSchema();

    if(dst.get() && dst->hasRaster())
    {
      m_ui->m_rgbRComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
      m_ui->m_rgbGComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
      m_ui->m_rgbBComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

      m_ui->m_ihsIComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
      m_ui->m_ihsHComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
      m_ui->m_ihsSComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
    }

    ++it;
  }

  rgbRComboBoxActivated(0);
  rgbGComboBoxActivated(0);
  rgbBComboBoxActivated(0);

  ihsIComboBoxActivated(0);
  ihsHComboBoxActivated(0);
  ihsSComboBoxActivated(0);
}

void te::qt::widgets::ColorTransformWizardPage::rgbRComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_rgbRComboBox, index, m_ui->m_rgbRBandComboBox);
}

void te::qt::widgets::ColorTransformWizardPage::rgbGComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_rgbGComboBox, index, m_ui->m_rgbGBandComboBox);
}

void te::qt::widgets::ColorTransformWizardPage::rgbBComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_rgbBComboBox, index, m_ui->m_rgbBBandComboBox);
}

void te::qt::widgets::ColorTransformWizardPage::ihsIComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_ihsIComboBox, index, m_ui->m_ihsIBandComboBox);
}

void te::qt::widgets::ColorTransformWizardPage::ihsHComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_ihsHComboBox, index, m_ui->m_ihsHBandComboBox);
}

void te::qt::widgets::ColorTransformWizardPage::ihsSComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_ihsSComboBox, index, m_ui->m_ihsSBandComboBox);
}

void te::qt::widgets::ColorTransformWizardPage::fillColorTransformTypes()
{
  //colorTransform types
  m_ui->m_colorTransformTypeComboBox->clear();

  m_ui->m_colorTransformTypeComboBox->addItem(tr("RGB - IHS"), COLORTRANSFORM_RGB2IHS);
  m_ui->m_colorTransformTypeComboBox->addItem(tr("IHS - RGB"), COLORTRANSFORM_IHS2RGB);
}

void te::qt::widgets::ColorTransformWizardPage::getRasterBands(QComboBox* layer, int index, QComboBox* band)
{
  //get layer
  QVariant varLayer = layer->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  //get raster
  std::auto_ptr<te::da::DataSet> ds = l->getData();
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rst = ds->getRaster(rpos);

  //fill band info
  if(rst.get())
  {
    band->clear();

    for(std::size_t t = 0; t < rst->getNumberOfBands(); ++t)
      band->addItem(QString::number(t));
  }

  //check data type range
  //double min, max;

  //te::rp::GetDataTypeRange(rst->getBandDataType(0), min, max);

  //m_ui->m_rgbMinLineEdit->setText(QString::number(min));
  //m_ui->m_ihsMinLineEdit->setText(QString::number(min));

  //m_ui->m_rgbMaxLineEdit->setText(QString::number(max));
  //m_ui->m_ihsMaxLineEdit->setText(QString::number(max));
}


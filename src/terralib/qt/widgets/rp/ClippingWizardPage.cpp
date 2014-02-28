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
  \file terralib/qt/widgets/rp/ClippingWizardPage.cpp

  \brief This file defines a class for a Clipping Wizard page.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Grid.h"
#include "../../../raster/Raster.h"
#include "RasterNavigatorWidget.h"
#include "ClippingWizardPage.h"
#include "ui_ClippingWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleValidator>
#include <QtGui/QIntValidator>
#include <QtGui/QMessageBox>

// STL
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ClippingWizardPage::ClippingWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ClippingWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillClippingTypes();

//build form
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_frame);
  m_navigator.reset( new te::qt::widgets::RasterNavigatorWidget(m_ui->m_frame));
  m_navigator->showAsPreview(false, true);
  m_navigator->hideColorCompositionTool(true);
  m_navigator->hideGeomTool(true);
  m_navigator->hideInfoTool(true);
  m_navigator->hidePickerTool(true);
  displayLayout->addWidget(m_navigator.get());
  displayLayout->setContentsMargins(0,0,0,0);

  connect(m_navigator.get(), SIGNAL(envelopeAcquired(te::gm::Envelope)), this, SLOT(onEnvelopeAcquired(te::gm::Envelope)));

//configure page
  this->setTitle(tr("Clipping"));
  this->setSubTitle(tr("Select the type of clipping and set their specific parameters."));

  m_ui->m_llxLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_llyLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_urxLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_uryLineEdit->setValidator(new QDoubleValidator(this));

  m_ui->m_startRowLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_startColumnLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_widthLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_heightLineEdit->setValidator(new QIntValidator(this));

//connects
  connect(m_ui->m_strategyTypeComboBox, SIGNAL(activated(int)), this, SLOT(onStrategyTypeComboBoxActivated(int)));
  connect(m_ui->m_layerComboBox, SIGNAL(activated(int)), this, SLOT(onLayerComboBoxActivated(int)));

  connect(m_ui->m_llxLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_llyLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_urxLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_uryLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));

  connect(m_ui->m_startRowLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_startColumnLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_widthLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_heightLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
}

te::qt::widgets::ClippingWizardPage::~ClippingWizardPage()
{
}

bool te::qt::widgets::ClippingWizardPage::isComplete() const
{
  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  if(type == CLIPPING_EXTENT)
  {
    if(m_ui->m_llxLineEdit->text().isEmpty() ||
       m_ui->m_llyLineEdit->text().isEmpty() ||
       m_ui->m_urxLineEdit->text().isEmpty() ||
       m_ui->m_uryLineEdit->text().isEmpty()
      )
      return false;
    
    return true;
  }
  else if(type == CLIPPING_DIMENSION)
  {
    if(m_ui->m_startRowLineEdit->text().isEmpty() ||
       m_ui->m_startColumnLineEdit->text().isEmpty() ||
       m_ui->m_widthLineEdit->text().isEmpty() ||
       m_ui->m_heightLineEdit->text().isEmpty()
      )
      return false;
    
    return true;
  }
  else if(type == CLIPPING_LAYER)
  {
    return true;
  }

  return false;
}

void te::qt::widgets::ClippingWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  m_navigator->set(m_layer);
}

void te::qt::widgets::ClippingWizardPage::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType->hasGeom())
      m_ui->m_layerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }

  if(m_ui->m_layerComboBox->count() > 0)
    onLayerComboBoxActivated(0);
}

te::map::AbstractLayerPtr te::qt::widgets::ClippingWizardPage::get()
{
  return m_layer;
}

bool te::qt::widgets::ClippingWizardPage::isExtentClipping()
{
  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  return type == CLIPPING_EXTENT;
}

bool te::qt::widgets::ClippingWizardPage::isDimensionClipping()
{
  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  return type == CLIPPING_DIMENSION;
}

bool te::qt::widgets::ClippingWizardPage::isLayerClipping()
{
  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  return type == CLIPPING_LAYER;
}

void te::qt::widgets::ClippingWizardPage::getExtentClipping(te::gm::Envelope& env)
{
  env.m_llx = m_ui->m_llxLineEdit->text().toDouble();
  env.m_lly = m_ui->m_llyLineEdit->text().toDouble();
  env.m_urx = m_ui->m_urxLineEdit->text().toDouble();
  env.m_ury = m_ui->m_uryLineEdit->text().toDouble();
}

void te::qt::widgets::ClippingWizardPage::getDimensionClipping(int& x, int& y, int& width, int& height)
{
  x = m_ui->m_startColumnLineEdit->text().toInt();
  y = m_ui->m_startRowLineEdit->text().toInt();
  width = m_ui->m_widthLineEdit->text().toInt();
  height = m_ui->m_heightLineEdit->text().toInt();
}

void te::qt::widgets::ClippingWizardPage::onStrategyTypeComboBoxActivated(int index)
{
  emit completeChanged();
}

void te::qt::widgets::ClippingWizardPage::onLayerComboBoxActivated(int index)
{
  m_ui->m_layerAttrComboBox->clear();

  QVariant varLayer = m_ui->m_layerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

  for(std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    m_ui->m_layerAttrComboBox->addItem(dsType->getProperties()[t]->getName().c_str());
  }
}

void te::qt::widgets::ClippingWizardPage::onEnvelopeAcquired(te::gm::Envelope env)
{
  if(!env.isValid())
    return;

  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  if(type == CLIPPING_EXTENT)
  {
    m_ui->m_llxLineEdit->setText(QString::number(env.getLowerLeftX(), 'f', 5));
    m_ui->m_llyLineEdit->setText(QString::number(env.getLowerLeftY(), 'f', 5));
    m_ui->m_urxLineEdit->setText(QString::number(env.getUpperRightX(), 'f', 5));
    m_ui->m_uryLineEdit->setText(QString::number(env.getUpperRightY(), 'f', 5));
  }
  else if(type == CLIPPING_DIMENSION)
  {
    std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

    if(ds.get())
    {
      std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
      std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

      if(inputRst.get())
      {
        // find the envelope coords in grid coordenates
        te::gm::Coord2D ul = inputRst->getGrid()->geoToGrid((double) env.getLowerLeftX(), (double) env.getUpperRightY());
        te::gm::Coord2D lr = inputRst->getGrid()->geoToGrid((double) env.getUpperRightX(), (double) env.getLowerLeftY());

        m_ui->m_startRowLineEdit->setText(QString::number((int)ul.y));
        m_ui->m_startColumnLineEdit->setText(QString::number((int)ul.x));
        m_ui->m_heightLineEdit->setText(QString::number((int)(lr.y - ul.y)));
        m_ui->m_widthLineEdit->setText(QString::number((int)(lr.x - ul.x)));
      }
    }
  }

  emit completeChanged();
}

void te::qt::widgets::ClippingWizardPage::fillClippingTypes()
{
  m_ui->m_strategyTypeComboBox->clear();

  m_ui->m_strategyTypeComboBox->addItem(tr("Extent"), CLIPPING_EXTENT);
  m_ui->m_strategyTypeComboBox->addItem(tr("Dimension"), CLIPPING_DIMENSION);
  //m_ui->m_strategyTypeComboBox->addItem(tr("Layer"), CLIPPING_LAYER);
}

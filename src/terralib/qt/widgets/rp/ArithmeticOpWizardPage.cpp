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
  \file terralib/qt/widgets/rp/ArithmeticOpWizardPage.cpp

  \brief This file defines a class for a ArithmeticOp Wizard page.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "ArithmeticOpWizard.h"
#include "ArithmeticOpWizardPage.h"
#include "ui_ArithmeticOpWizardPageForm.h"

// Qt
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>

// stl
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ArithmeticOpWizardPage::ArithmeticOpWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ArithmeticOpWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

//configure page
  this->setTitle(tr("Arithmetic Operations"));
  this->setSubTitle(tr("Select the type of arithmetic operation and set their specific parameters."));

  m_ui->m_gainLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_offsetLineEdit->setValidator(new QDoubleValidator(this));

  //connects
  connect(m_ui->m_layerAComboBox, SIGNAL(activated(int)), this, SLOT(layerAComboBoxActivated(int)));
  connect(m_ui->m_layerBComboBox, SIGNAL(activated(int)), this, SLOT(layerBComboBoxActivated(int)));
}

te::qt::widgets::ArithmeticOpWizardPage::~ArithmeticOpWizardPage()
{

}

bool te::qt::widgets::ArithmeticOpWizardPage::isComplete() const
{
  return true;
}


void te::qt::widgets::ArithmeticOpWizardPage::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerList = layerList;

  //fill layer combo
  m_ui->m_layerAComboBox->clear();
  m_ui->m_layerBComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layerList.begin();
  std::vector<std::size_t> bands;

  while(it != m_layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dst = l->getSchema();

    if(dst.get() && dst->hasRaster())
    {
      m_ui->m_layerAComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
      m_ui->m_layerBComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
    }

    ++it;
  }

  layerAComboBoxActivated(0);
  layerBComboBoxActivated(0);
}

int te::qt::widgets::ArithmeticOpWizardPage::getOperationType()
{
  int type = -1;

  if(m_ui->m_op1RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_1;
  else if(m_ui->m_op1RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_2;
  else if(m_ui->m_op1RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_3;
  else if(m_ui->m_op1RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_4;
  else if(m_ui->m_op1RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_5;
  else if(m_ui->m_op1RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_6;

  return type;
}

te::map::AbstractLayerPtr te::qt::widgets::ArithmeticOpWizardPage::getLayerRasterA()
{
  int curIdx = m_ui->m_layerAComboBox->currentIndex();
  QVariant varLayer = m_ui->m_layerAComboBox->itemData(curIdx, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  return l;
}

int te::qt::widgets::ArithmeticOpWizardPage::getLayerBandA()
{
  return m_ui->m_bandAComboBox->currentText().toInt();
}

te::map::AbstractLayerPtr te::qt::widgets::ArithmeticOpWizardPage::getLayerRasterB()
{
  int curIdx = m_ui->m_layerBComboBox->currentIndex();
  QVariant varLayer = m_ui->m_layerBComboBox->itemData(curIdx, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  return l;
}

int te::qt::widgets::ArithmeticOpWizardPage::getLayerBandB()
{
  return m_ui->m_bandBComboBox->currentText().toInt();
}

double te::qt::widgets::ArithmeticOpWizardPage::getGainValue()
{
  double val = 1.;

  if(!m_ui->m_gainLineEdit->text().isEmpty())
    val = m_ui->m_gainLineEdit->text().toDouble();

  return val;
}

double te::qt::widgets::ArithmeticOpWizardPage::getOffSetValue()
{
  double val = 0.;

  if(!m_ui->m_offsetLineEdit->text().isEmpty())
    val = m_ui->m_offsetLineEdit->text().toDouble();

  return val;
}

bool te::qt::widgets::ArithmeticOpWizardPage::normalize()
{
  return m_ui->m_normalizeCheckBox->isChecked();
}

void te::qt::widgets::ArithmeticOpWizardPage::layerAComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_layerAComboBox, index, m_ui->m_bandAComboBox);
}

void te::qt::widgets::ArithmeticOpWizardPage::layerBComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_layerBComboBox, index, m_ui->m_bandBComboBox);
}

void te::qt::widgets::ArithmeticOpWizardPage::getRasterBands(QComboBox* layer, int index, QComboBox* band)
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
}

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
  \file terralib/qt/widgets/rp/ComposeBandsWizardPage.cpp

  \brief This file defines a class for a Compose / Decompose Bands Wizard page.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Functions.h"
#include "ComposeBandsWizardPage.h"
#include "ui_ComposeBandsWizardPageForm.h"

// Qt
#include <QMessageBox>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ComposeBandsWizardPage::ComposeBandsWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ComposeBandsWizardPageForm)
{
  //setup controls
  m_ui->setupUi(this);

  //set interpolator types
  fillInterpolatorTypes();

  //configure page
  this->setTitle(tr("Compose / Decompose Bands"));
  this->setSubTitle(tr("Select the operation (Compose / Decompose) and set their specific parameters."));
}

te::qt::widgets::ComposeBandsWizardPage::~ComposeBandsWizardPage()
{

}

bool te::qt::widgets::ComposeBandsWizardPage::isComplete() const
{
  return true;
}

void te::qt::widgets::ComposeBandsWizardPage::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerList = layerList;

  m_ui->m_composeTableWidget->setRowCount(0);
  m_ui->m_decomposeTableWidget->setRowCount(0);

  std::list<te::map::AbstractLayerPtr>::iterator it;

  //set compose table info
  for(it = m_layerList.begin(); it!= m_layerList.end(); ++it)
  {
    te::map::AbstractLayerPtr l = *it;

    //get input raster
    std::auto_ptr<te::da::DataSet> ds = l->getData();

    if(ds.get())
    {
      std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

      std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

      if(inputRst.get())
      {
        int newrow = m_ui->m_composeTableWidget->rowCount();
        m_ui->m_composeTableWidget->insertRow(newrow);

        //layer name
        QTableWidgetItem* itemLayer = new QTableWidgetItem(l->getTitle().c_str());
        m_ui->m_composeTableWidget->setItem(newrow, 1, itemLayer);

        //band
        QComboBox* cmbBox = new QComboBox(this);

        for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
        {
          cmbBox->addItem(QString::number(i));
        }

        m_ui->m_composeTableWidget->setCellWidget(newrow, 0, cmbBox);
      }
    }
  }

  //set decompose table info
  if(!m_layerList.empty())
  {
    te::map::AbstractLayerPtr l = *m_layerList.begin();

    //get input raster
    std::auto_ptr<te::da::DataSet> ds = l->getData();

    m_ui->m_rasterLineEdit->setText(l->getTitle().c_str());

    if(ds.get())
    {
      std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

      std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

      if(inputRst.get())
      {
        for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
        {
          int newrow = m_ui->m_decomposeTableWidget->rowCount();
          m_ui->m_decomposeTableWidget->insertRow(newrow);

          //band item
          QString bName(tr("Band "));
          bName.append(QString::number(i));

          QTableWidgetItem* itemBand = new QTableWidgetItem(bName);
          itemBand->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
          itemBand->setCheckState(Qt::Checked);
          m_ui->m_decomposeTableWidget->setItem(newrow, 0, itemBand);
        }
      }
    }
  }
}

bool te::qt::widgets::ComposeBandsWizardPage::isCompose()
{
  if(m_ui->m_tabWidget->currentIndex() == 0)
    return true;

  return false;
}

bool te::qt::widgets::ComposeBandsWizardPage::isDecompose()
{
  if(m_ui->m_tabWidget->currentIndex() == 1)
    return true;

  return false;
}

void te::qt::widgets::ComposeBandsWizardPage::getComposeParameters(std::vector<const te::rst::Raster*>& rasters, std::vector<unsigned int>& bands, te::rst::Interpolator::Method& interpMethod)
{
  int count = 0;

  std::list<te::map::AbstractLayerPtr>::iterator it;

  for(it = m_layerList.begin(); it!= m_layerList.end(); ++it)
  {
    te::map::AbstractLayerPtr l = *it;

    //get input raster
    std::auto_ptr<te::da::DataSet> ds = l->getData();

    if(!ds.get())
      throw;

    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    rasters.push_back(ds->getRaster(rpos).release());

    //get band
    QComboBox* cmb = ( QComboBox*)m_ui->m_composeTableWidget->cellWidget(count, 0);

    if(!cmb)
      throw;

    bands.push_back((unsigned int)cmb->currentText().toInt());

    //interpolator method
    int idx = m_ui->m_interpolatorComboBox->currentIndex();
    interpMethod = (te::rst::Interpolator::Method)m_ui->m_interpolatorComboBox->itemData(idx).toInt();

    ++count;
  }
}

void te::qt::widgets::ComposeBandsWizardPage::getDecomposeParameters(te::rst::Raster*& raster, std::vector<unsigned int>& bands)
{
  te::map::AbstractLayerPtr l = *m_layerList.begin();

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = l->getData();

  if(!ds.get())
   throw;
  
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

  raster = ds->getRaster(rpos).release();

  //get bands
  for(int r = 0; r <  m_ui->m_decomposeTableWidget->rowCount(); ++r)
  {
    if(m_ui->m_decomposeTableWidget->item(r, 0)->checkState() == Qt::Checked)
    {
      bands.push_back((unsigned int)r);
    }
  }
}

void te::qt::widgets::ComposeBandsWizardPage::fillInterpolatorTypes()
{
  m_ui->m_interpolatorComboBox->clear();

  m_ui->m_interpolatorComboBox->addItem(tr("Nearest Neighbor"), te::rst::Interpolator::NearestNeighbor);
  m_ui->m_interpolatorComboBox->addItem(tr("Bilinear"), te::rst::Interpolator::Bilinear);
  m_ui->m_interpolatorComboBox->addItem(tr("Bicubic"), te::rst::Interpolator::Bicubic);
}

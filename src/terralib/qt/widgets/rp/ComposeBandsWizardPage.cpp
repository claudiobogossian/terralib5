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

  m_ui->m_addToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("list-remove"));

  //set interpolator types
  fillInterpolatorTypes();

  //connects
  connect(m_ui->m_addToolButton, SIGNAL(clicked()), this, SLOT(onAddToolButtonClicked()));
  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), this, SLOT(onRemoveToolButtonClicked()));

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

        //layer combo
        QComboBox* layerCmbBox = new QComboBox(this);

        connect(layerCmbBox, SIGNAL(activated(int)), this, SLOT(onLayerCmbActivated(int)));

        std::list<te::map::AbstractLayerPtr>::iterator itLayer;

        int curItem;
        int item = 0;

        for(itLayer = m_layerList.begin(); itLayer!= m_layerList.end(); ++itLayer)
        {
          te::map::AbstractLayerPtr lItem = *itLayer;

          layerCmbBox->addItem(lItem->getTitle().c_str(), QVariant::fromValue(lItem));

          if(lItem == l)
          {
            curItem = item;
          }

          ++item;
        }

        layerCmbBox->setCurrentIndex(curItem);

        m_ui->m_composeTableWidget->setCellWidget(newrow, 1, layerCmbBox);

        m_cmbMap.insert(std::map<QComboBox*, int>::value_type(layerCmbBox, newrow));

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
  int rowCount = m_ui->m_composeTableWidget->rowCount();

  for(int i = 0; i < rowCount; ++i)
  {
    QComboBox* layerCmb = (QComboBox*)m_ui->m_composeTableWidget->cellWidget(i, 1);

    int curIdx = layerCmb->currentIndex();
    QVariant varLayer = layerCmb->itemData(curIdx, Qt::UserRole);
    te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

    //get input raster
    std::auto_ptr<te::da::DataSet> ds = layer->getData();

    if(!ds.get())
      throw;

    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    rasters.push_back(ds->getRaster(rpos).release());


    QComboBox* bandCmb = (QComboBox*)m_ui->m_composeTableWidget->cellWidget(i, 0);

    bands.push_back((unsigned int)bandCmb->currentText().toInt());
  }

  //interpolator method
  int idx = m_ui->m_interpolatorComboBox->currentIndex();
  interpMethod = (te::rst::Interpolator::Method)m_ui->m_interpolatorComboBox->itemData(idx).toInt();
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

void te::qt::widgets::ComposeBandsWizardPage::onAddToolButtonClicked()
{
  int newrow = m_ui->m_composeTableWidget->rowCount();
  m_ui->m_composeTableWidget->insertRow(newrow);

  //layer combo
  QComboBox* layerCmbBox = new QComboBox(this);

  connect(layerCmbBox, SIGNAL(activated(int)), this, SLOT(onLayerCmbActivated(int)));

  std::list<te::map::AbstractLayerPtr>::iterator itLayer;

  for(itLayer = m_layerList.begin(); itLayer!= m_layerList.end(); ++itLayer)
  {
    te::map::AbstractLayerPtr lItem = *itLayer;

    layerCmbBox->addItem(lItem->getTitle().c_str(), QVariant::fromValue(lItem));
  }

  m_ui->m_composeTableWidget->setCellWidget(newrow, 1, layerCmbBox);

  m_cmbMap.insert(std::map<QComboBox*, int>::value_type(layerCmbBox, newrow));


  //band
  QComboBox* cmbBox = new QComboBox(this);

  m_ui->m_composeTableWidget->setCellWidget(newrow, 0, cmbBox);

  te::map::AbstractLayerPtr lFirst = *m_layerList.begin();

  if(lFirst.get())
  {
     std::auto_ptr<te::da::DataSet> ds = lFirst->getData();

    if(ds.get())
    {
      std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

      std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

      if(inputRst.get())
      {
        for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
        {
          cmbBox->addItem(QString::number(i));
        }
      }
    }
  }
}

void te::qt::widgets::ComposeBandsWizardPage::onRemoveToolButtonClicked()
{
  int currow = m_ui->m_composeTableWidget->currentRow();
  m_ui->m_composeTableWidget->removeRow(currow);
}

void te::qt::widgets::ComposeBandsWizardPage::onLayerCmbActivated(int index)
{
  QComboBox* cmb = dynamic_cast<QComboBox*>(sender());

  QVariant varLayer = cmb->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  int row = m_cmbMap[cmb];

  QComboBox* bandCmb = (QComboBox*)m_ui->m_composeTableWidget->cellWidget(row, 0);

  bandCmb->clear();

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        bandCmb->addItem(QString::number(i));
      }
    }
  }
}

void te::qt::widgets::ComposeBandsWizardPage::fillInterpolatorTypes()
{
  m_ui->m_interpolatorComboBox->clear();

  m_ui->m_interpolatorComboBox->addItem(tr("Nearest Neighbor"), te::rst::NearestNeighbor);
  m_ui->m_interpolatorComboBox->addItem(tr("Bilinear"), te::rst::Bilinear);
  m_ui->m_interpolatorComboBox->addItem(tr("Bicubic"), te::rst::Bicubic);
}

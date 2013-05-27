/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/rp/SegmenterWizard.cpp

  \brief A Qt dialog that allows users to run a segmenter operation defined by RP module.
*/

// TerraLib 
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Segmenter.h"
#include "../../../rp/SegmenterRegionGrowingStrategy.h"
#include "SegmenterWizard.h"
#include "SegmenterAdvancedOptionsWizardPage.h"
#include "SegmenterWizardPage.h"
#include "LayerSearchWidget.h"
#include "LayerSearchWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "ui_SegmenterAdvancedOptionsWizardPageForm.h"

// STL
#include <cassert>

// Qt
#include <QtGui/QMessageBox>


te::qt::widgets::SegmenterWizard::SegmenterWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Segmenter"));
  this->setFixedSize(640, 580);

  addPages();
}

te::qt::widgets::SegmenterWizard::~SegmenterWizard()
{

}

bool te::qt::widgets::SegmenterWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_segmenterPage->set(l);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_segmenterPage.get())
  {
    return m_segmenterPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::SegmenterWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
}

void te::qt::widgets::SegmenterWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_segmenterPage.reset(new te::qt::widgets::SegmenterWizardPage(this));
  m_segmenterAdvOptPage.reset(new te::qt::widgets::SegmenterAdvancedOptionsWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  addPage(m_layerSearchPage.get());
  addPage(m_segmenterPage.get());
  addPage(m_segmenterAdvOptPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::qt::widgets::SegmenterWizard::execute()
{
  //get layer
  std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr l = *list.begin();
  te::da::DataSet* ds = l->getData();

  //run contrast
  te::rp::Segmenter algorithmInstance;

  te::rp::Segmenter::InputParameters algoInputParams = m_segmenterPage->getInputParams();

  algoInputParams.m_enableThreadedProcessing = m_segmenterAdvOptPage->getForm()->m_enableThreadedProcessingcheckBox->isChecked();
  algoInputParams.m_maxSegThreads = m_segmenterAdvOptPage->getForm()->m_maximumThreadsNumberLineEdit->text().toUInt();
  algoInputParams.m_enableBlockProcessing = m_segmenterAdvOptPage->getForm()->m_enableBlockProcessingcheckBox->isChecked();
  algoInputParams.m_enableBlockMerging = m_segmenterAdvOptPage->getForm()->m_enableBlockMergingCheckBox->isChecked();
  algoInputParams.m_maxBlockSize = m_segmenterAdvOptPage->getForm()->m_maximumBlockSizeLineEdit->text().toUInt();

  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();
  algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();


  if(algorithmInstance.initialize(algoInputParams))
  {
    if(algorithmInstance.execute(algoOutputParams))
    {
      QMessageBox::information(this, tr("Segmenter"), tr("Segmenter ended sucessfully"));
    }
    else
    {
      QMessageBox::critical(this, tr("Segmenter"), tr("Segmenter execution error"));

      delete ds;
      return false;
    }
  }
  else
  {
    QMessageBox::critical(this, tr("Segmenter"), tr("Segmenter initialization error"));

    delete ds;
    return false;
  }

  delete ds;

  return true;
}

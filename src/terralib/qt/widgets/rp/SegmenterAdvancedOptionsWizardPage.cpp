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
  \file terralib/qt/widgets/rp/SegmenterAdvancedOptionsWizardPage.h

  \brief This file defines a class for a Segmenter Advanced Options Wizard page.
*/

// TerraLib
#include "../../../rp/Segmenter.h"
#include "SegmenterAdvancedOptionsWizardPage.h"
#include "ui_SegmenterAdvancedOptionsWizardPageForm.h"

te::qt::widgets::SegmenterAdvancedOptionsWizardPage::SegmenterAdvancedOptionsWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::SegmenterAdvancedOptionsWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

//configure page
  this->setTitle(tr("Segmenter Advanced Options"));
  this->setSubTitle(tr("Used to set the segmenter advanced options."));

  te::rp::Segmenter::InputParameters input;

  QString threadNumber;
  threadNumber.setNum(input.m_maxSegThreads);

  QString maxBlockSize;
  maxBlockSize.setNum(input.m_maxBlockSize);

  m_ui->m_enableThreadedProcessingcheckBox->setChecked(input.m_enableThreadedProcessing);
  m_ui->m_maximumThreadsNumberLineEdit->setText(threadNumber);
  m_ui->m_enableBlockProcessingcheckBox->setChecked(input.m_enableBlockProcessing);
  m_ui->m_blockOverlapPercentSpinBox->setValue(input.m_blocksOverlapPercent);
  m_ui->m_maximumBlockSizeLineEdit->setText(maxBlockSize);
}

te::qt::widgets::SegmenterAdvancedOptionsWizardPage::~SegmenterAdvancedOptionsWizardPage()
{
}

Ui::SegmenterAdvancedOptionsWizardPageForm* te::qt::widgets::SegmenterAdvancedOptionsWizardPage::getForm() const
{
  return m_ui.get();
}

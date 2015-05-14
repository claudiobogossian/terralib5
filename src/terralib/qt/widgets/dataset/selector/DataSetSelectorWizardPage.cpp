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
  \file terralib/qt/widgets/dataset/selector/DataSetSelectorWizardPage.cpp

  \brief ...
*/

// TerraLib
#include "../../datasource/explorer/DataSetItem.h"
#include "../explorer/DataSetTreeView.h"
#include "DataSetSelectorWidget.h"
#include "DataSetSelectorWizardPage.h"

// Qt
#include <QVBoxLayout>

te::qt::widgets::DataSetSelectorWizardPage::DataSetSelectorWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_completeMode(HAVING_CHECKED_ITEMS)
{
  m_selectorW.reset(new DataSetSelectorWidget(this));

  QVBoxLayout* layout = new QVBoxLayout(this);

  layout->addWidget(m_selectorW.get());

  connect(m_selectorW->getTreeView(), SIGNAL(toggled(DataSetItem*)), this, SLOT(onDataSetToggled(DataSetItem*)));
  connect(m_selectorW->getTreeView(), SIGNAL(clicked(DataSetItem*)), this, SLOT(onDataSetClicked(DataSetItem*)));
}

te::qt::widgets::DataSetSelectorWizardPage::~DataSetSelectorWizardPage()
{
}

void te::qt::widgets::DataSetSelectorWizardPage::initializePage()
{
}

bool te::qt::widgets::DataSetSelectorWizardPage::isComplete() const
{
  if(m_completeMode == HAVING_CHECKED_ITEMS)
    return m_selectorW->hasCheckedDataSets(); 
  else //if(m_completeMode == HAVING_SELECTED_ITEMS)
    return m_selectorW->hasSelectedDataSets();
}

void te::qt::widgets::DataSetSelectorWizardPage::set(const te::da::DataSourceInfoPtr& ds, bool useCheckableItems)
{
  m_selectorW->set(ds, useCheckableItems);
}

std::list<te::da::DataSetTypePtr> te::qt::widgets::DataSetSelectorWizardPage::getCheckedDataSets() const
{
  return m_selectorW->getCheckedDataSets();
}

std::list<te::da::DataSetTypePtr> te::qt::widgets::DataSetSelectorWizardPage::getSelectedDataSets() const
{
  return m_selectorW->getSelectedDataSets();
}

void te::qt::widgets::DataSetSelectorWizardPage::setCompleteMode(CompleteModeType m)
{
  m_completeMode = m;
}

te::qt::widgets::DataSetSelectorWidget* te::qt::widgets::DataSetSelectorWizardPage::getSelectorWidget() const
{
  return m_selectorW.get();
}

void te::qt::widgets::DataSetSelectorWizardPage::onDataSetToggled(DataSetItem* /*item*/)
{
  emit completeChanged();
}

void te::qt::widgets::DataSetSelectorWizardPage::onDataSetClicked(DataSetItem* /*item*/)
{
  emit completeChanged();
}


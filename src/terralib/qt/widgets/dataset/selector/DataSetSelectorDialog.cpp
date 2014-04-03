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
  \file terralib/qt/widgets/dataset/selector/DataSetSelectorDialog.cpp

  \brief ....
*/

// TerraLib
#include "../../datasource/core/DataSourceType.h"
#include "../../datasource/core/DataSourceTypeManager.h"
#include "../../datasource/explorer/DataSetItem.h"
#include "../explorer/DataSetTreeView.h"
#include "DataSetSelectorDialog.h"
#include "DataSetSelectorWidget.h"
#include "ui_DataSetSelectorDialogForm.h"

// Qt
#include <QMessageBox>

te::qt::widgets::DataSetSelectorDialog::DataSetSelectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::DataSetSelectorDialogForm),
    m_selectMode(CHECKED_ITEMS)
{
// add controls
  m_ui->setupUi(this);

// init controls
  m_selectorW.reset(new DataSetSelectorWidget(this));
  m_ui->m_selectorWidgetLayout->addWidget(m_selectorW.get(), 1);

// connect signals and slots
  connect(m_selectorW->getTreeView(), SIGNAL(toggled(DataSetItem*)), this, SLOT(onDataSetToggled(DataSetItem*)));
  connect(m_selectorW->getTreeView(), SIGNAL(clicked(DataSetItem*)), this, SLOT(onDataSetClicked(DataSetItem*)));
  connect(m_selectorW->getTreeView(), SIGNAL(toggled(DataSetGroupItem*)), this, SLOT(onDataSetGroupToggled(DataSetGroupItem*)));
  connect(m_selectorW->getTreeView(), SIGNAL(clicked(DataSetGroupItem*)), this, SLOT(onDataSetGroupClicked(DataSetGroupItem*)));

  m_ui->m_helpPushButton->setPageReference("widgets/dataset_selector/dataset_selector.html");
}

te::qt::widgets::DataSetSelectorDialog::~DataSetSelectorDialog()
{
}

void te::qt::widgets::DataSetSelectorDialog::set(const te::da::DataSourceInfoPtr& ds, bool useCheckableItems)
{
  m_selectorW->set(ds, useCheckableItems);

  if(ds.get() == 0)
    return;

  QString title(tr("Dataset Selection - %1"));
  title = title.arg(QString::fromStdString(ds->getTitle()));

  setWindowTitle(title);

  const DataSourceType* dsType = DataSourceTypeManager::getInstance().get(ds->getType());

  if(dsType == 0)
    return;

  title = tr("%1 - %2");
  title = title.arg(QString::fromStdString(dsType->getTitle())).arg(QString::fromStdString(ds->getTitle()));

  m_ui->m_titleLabel->setText(title);
}

std::list<te::da::DataSetTypePtr> te::qt::widgets::DataSetSelectorDialog::getCheckedDataSets() const
{
  return m_selectorW->getCheckedDataSets();
}

std::list<te::da::DataSetTypePtr> te::qt::widgets::DataSetSelectorDialog::getSelectedDataSets() const
{
  return m_selectorW->getSelectedDataSets();
}

void te::qt::widgets::DataSetSelectorDialog::setSelectMode(SelectModeType m)
{
  m_selectMode = m;
}

te::qt::widgets::DataSetSelectorWidget* te::qt::widgets::DataSetSelectorDialog::getSelectorWidget() const
{
  return m_selectorW.get();
}

void te::qt::widgets::DataSetSelectorDialog::onDataSetToggled(DataSetItem* /*item*/)
{
  updateSelectPushButton();
}

void te::qt::widgets::DataSetSelectorDialog::onDataSetClicked(DataSetItem* /*item*/)
{
  updateSelectPushButton();
}

void te::qt::widgets::DataSetSelectorDialog::onDataSetGroupToggled(DataSetGroupItem* /*item*/)
{
  updateSelectPushButton();
}

void te::qt::widgets::DataSetSelectorDialog::onDataSetGroupClicked(DataSetGroupItem* /*item*/)
{
  updateSelectPushButton();
}

void te::qt::widgets::DataSetSelectorDialog::updateSelectPushButton()
{
  if(m_selectMode == CHECKED_ITEMS)
    m_ui->m_selectPushButton->setEnabled(m_selectorW->hasCheckedDataSets());
  else //if(m_selectMode == SELECTED_ITEMS)
    m_ui->m_selectPushButton->setEnabled(m_selectorW->hasSelectedDataSets());
}

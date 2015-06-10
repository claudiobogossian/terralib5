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
  \file terralib/qt/widgets/dataset/selector/DataSetSelectorWidget.cpp

  \brief A wizard for selecting datasets from a data source.
*/

// TerraLib
#include "../../../../dataaccess/dataset/DataSet.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../memory/DataSet.h"
#include "../../datasource/explorer/AbstractDataSourceTreeItem.h"
#include "../../datasource/explorer/DataSetItem.h"
#include "../../datasource/explorer/DataSetGroupItem.h"
#include "../../mapdisplay/DataSetDisplay.h"
#include "../../table/DataSetTableView.h"
#include "../explorer/DataSetTreeView.h"
#include "../explorer/DataSetTreeModel.h"
#include "DataSetSelectorWidget.h"
#include "ui_DataSetSelectorWidgetForm.h"

// STL
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

// Qt
#include <QMessageBox>
#include <QVBoxLayout>

te::qt::widgets::DataSetSelectorWidget::DataSetSelectorWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::DataSetSelectorWidgetForm),
    m_nPreviewRows(10)
{
// add controls
  m_ui->setupUi(this);

// init controls
  m_datasetTreeView.reset(new DataSetTreeView(m_ui->m_datasetGroupBox));
  m_datasetTreeView->setHeaderHidden(true);
  m_datasetTreeView->setIndentation(TE_QTWIDGETS_DEFAULT_TREEVIEW_IDENTATION);

  QVBoxLayout* datasetGroupBoxLayout = new QVBoxLayout(m_ui->m_datasetGroupBox);
  datasetGroupBoxLayout->addWidget(m_datasetTreeView.get(), 1);

  m_mapPreview.reset(new te::qt::widgets::DataSetDisplay(m_ui->m_mapPreviewGroupBox));
  QVBoxLayout* mapPreviewGroupBoxLayout = new QVBoxLayout(m_ui->m_mapPreviewGroupBox);
  mapPreviewGroupBoxLayout->addWidget(m_mapPreview.get(), 1);
  m_mapPreview->show();

  m_ui->m_mapPreviewGroupBox->setChecked(false);

  m_tblView.reset(new DataSetTableView(m_ui->m_dataPreviewGroupBox));
  QVBoxLayout* dataPreviewGroupBoxLayout = new QVBoxLayout(m_ui->m_dataPreviewGroupBox);
  dataPreviewGroupBoxLayout->addWidget(m_tblView.get(), 1);

  QFont font;
  font.setPointSize(8);

  m_tblView->setFont(font);

  m_tblView->setAlternatingRowColors(true);
  m_tblView->verticalHeader()->setVisible(false);
  m_tblView->setSelectionMode(QAbstractItemView::NoSelection);
  m_ui->m_dataPreviewGroupBox->setChecked(false);

  m_tblView->show();

// connect signals and slots
  connect(m_datasetTreeView.get(), SIGNAL(clicked(DataSetItem*)), this, SLOT(onDataSetClicked(DataSetItem*)));
  connect(m_datasetTreeView.get(), SIGNAL(toggled(DataSetItem*)), this, SLOT(onDataSetToggled(DataSetItem*)));
  connect(m_datasetTreeView.get(), SIGNAL(toggled(DataSetGroupItem*)), this, SLOT(onDataSetGroupToggled(DataSetGroupItem*)));
  connect(m_ui->m_mapPreviewGroupBox, SIGNAL(toggled(bool)), this, SLOT(onMapPreviewToggled(bool)));
  connect(m_ui->m_dataPreviewGroupBox, SIGNAL(toggled(bool)), this, SLOT(onDataPreviewToggled(bool)));
}

te::qt::widgets::DataSetSelectorWidget::~DataSetSelectorWidget()
{
}

void te::qt::widgets::DataSetSelectorWidget::set(const te::da::DataSourceInfoPtr& ds, bool useCheckableItems)
{
  m_datasource = ds;

  if(ds.get() == 0)
    return;

  m_ui->m_datasetGroupBox->setTitle(QString::fromStdString(ds->getTitle()));

  try
  {
    m_datasetTreeView->set(m_datasource, useCheckableItems);

    QAbstractItemModel* nmodel = m_datasetTreeView->model();
    
    QModelIndex idx = nmodel->index(0, 0);

    m_datasetTreeView->expand(idx);
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error when showing datasets!"));
  }
}

void te::qt::widgets::DataSetSelectorWidget::setMapPreviewVisible(bool /*visible*/)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::widgets::DataSetSelectorWidget::setMapPreviewEnabled(bool enabled)
{
  m_ui->m_mapPreviewGroupBox->setChecked(false);
}

void te::qt::widgets::DataSetSelectorWidget::setDataPreviewVisible(bool /*visible*/)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::widgets::DataSetSelectorWidget::setDataPreviewEnabled(bool enabled)
{
  m_ui->m_dataPreviewGroupBox->setChecked(false);
}

void te::qt::widgets::DataSetSelectorWidget::setNumPreviewRows(int nrows)
{
  m_nPreviewRows = nrows;
}

std::list<te::da::DataSetTypePtr> te::qt::widgets::DataSetSelectorWidget::getCheckedDataSets() const
{
  std::list<te::da::DataSetTypePtr> datasets;

  std::map<te::da::DataSetTypePtr, std::string, DataSetComparer>::const_iterator it;
  for(it = m_checkedDatasets.begin(); it != m_checkedDatasets.end(); ++it)
    datasets.push_back(it->first);

  return datasets;
}

bool te::qt::widgets::DataSetSelectorWidget::hasCheckedDataSets() const
{
  return !m_checkedDatasets.empty();
}

std::list<std::string> te::qt::widgets::DataSetSelectorWidget::getCheckedGeomProperties() const
{
  std::list<std::string> gp;

  std::map<te::da::DataSetTypePtr, std::string, DataSetComparer>::const_iterator it;
  for(it = m_checkedDatasets.begin(); it != m_checkedDatasets.end(); ++it)
    gp.push_back(it->second);

  return gp;
}

std::list<te::da::DataSetTypePtr> te::qt::widgets::DataSetSelectorWidget::getSelectedDataSets() const
{
  std::list<te::da::DataSetTypePtr> datasets;

  std::list<DataSetItem*> items = m_datasetTreeView->getSelectedDataSets();

  for(std::list<DataSetItem*>::iterator it = items.begin(); it != items.end(); ++it)
  {
    DataSetItem* ditem = *it;

    if(ditem == 0)
      continue;

    if(ditem->getDataSet().get() == 0)
      continue;

    datasets.push_back(ditem->getDataSet());
  }

  return datasets;
}

bool te::qt::widgets::DataSetSelectorWidget::hasSelectedDataSets() const
{
  return m_datasetTreeView->hasSelectedDataSets();
}

std::list<std::string> te::qt::widgets::DataSetSelectorWidget::getSelectedGeomProperties() const
{
  std::list<std::string> gp;

  std::list<DataSetItem*> items = m_datasetTreeView->getSelectedDataSets();

  for(std::list<DataSetItem*>::iterator it = items.begin(); it != items.end(); ++it)
  {
    DataSetItem* ditem = *it;

    if(ditem == 0)
      continue;

    if(ditem->getDataSet().get() == 0)
      continue;

    gp.push_back(ditem->getGeomPropertyName());
  }

  return gp;
}

te::qt::widgets::DataSetTreeView* te::qt::widgets::DataSetSelectorWidget::getTreeView() const
{
  return m_datasetTreeView.get();
}

void te::qt::widgets::DataSetSelectorWidget::previewMap(const te::da::DataSetTypePtr& dataset)
{
  if(dataset.get() == 0)
    return;

  try
  {
    if(m_ui->m_mapPreviewGroupBox->isChecked())
    {
      m_mapPreview->clear();
      m_mapPreview->draw(dataset, m_datasource);
    }
  }
  catch(...)
  {
  }
}

void te::qt::widgets::DataSetSelectorWidget::previewData(const te::da::DataSetTypePtr& dataset)
{
  if((dataset.get() == 0) || (m_datasource.get() == 0))
    return;

  if(!m_ui->m_dataPreviewGroupBox->isChecked())
    return;

  try
  {
    te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(m_datasource->getId(), m_datasource->getAccessDriver(), m_datasource->getConnInfo());

    if(ds.get() == 0)
      return;

    std::auto_ptr<te::da::DataSet> feature(ds->getDataSet(dataset->getName()));

    std::vector<std::size_t> properties;
    for(std::size_t i = 0; i < feature->getNumProperties(); ++i)
      properties.push_back(i);

    std::size_t previewrows = m_nPreviewRows;  //Usually 10
    std::size_t dsetsize = feature->size();

    if(dsetsize < previewrows) 
      previewrows = dsetsize;

    std::auto_ptr<te::mem::DataSet> memFeature((new te::mem::DataSet(*feature.get(), properties, previewrows)));

    if(memFeature.get())
      m_tblView->setDataSet(memFeature.release(), ds->getEncoding());
  }
  catch(...)
  {
  }
}

void te::qt::widgets::DataSetSelectorWidget::onDataSetToggled(DataSetItem* item)
{
  if(item == 0)
    return;

  if(item->isChecked())
    //m_checkedDatasets.insert(std::make_pair<te::da::DataSetTypePtr, std::string>(item->getDataSet(), item->getGeomPropertyName()));
    m_checkedDatasets.insert(std::make_pair(item->getDataSet(), item->getGeomPropertyName()));
  else
    m_checkedDatasets.erase(item->getDataSet());
}

void te::qt::widgets::DataSetSelectorWidget::onDataSetClicked(DataSetItem* item)
{
  if(item == 0)
    return;

  previewMap(item->getDataSet());
  previewData(item->getDataSet());
}

void te::qt::widgets::DataSetSelectorWidget::onDataSetGroupToggled(DataSetGroupItem* item)
{
  if(item == 0)
    return;

  const std::vector<DataSetItem*>& items = item->getDataSetItems();

  for(std::size_t i = 0; i < items.size(); ++i)
    onDataSetToggled(items[i]);
}

void te::qt::widgets::DataSetSelectorWidget::onMapPreviewToggled(bool on)
{
  if(on)
  {
    QModelIndex current = m_datasetTreeView->currentIndex();

    AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(current.internalPointer());

    if(item == 0)
      return;

    DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

    if(ditem == 0)
      return;

    previewMap(ditem->getDataSet());
  }
  else
  {
    m_mapPreview->clear();
  }
}

void te::qt::widgets::DataSetSelectorWidget::onDataPreviewToggled(bool on)
{
  if(on)
  {
    QModelIndex current = m_datasetTreeView->currentIndex();

    AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(current.internalPointer());

    if(item == 0)
      return;

    DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

    if(ditem == 0)
      return;

    previewData(ditem->getDataSet());
  }
}

bool te::qt::widgets::DataSetSelectorWidget::DataSetComparer::operator()(const te::da::DataSetTypePtr& first, const te::da::DataSetTypePtr& second) const
{
  return first->getName() < second->getName();
}

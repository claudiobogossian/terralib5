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
  \file terralib/qt/widgets/layer/search/LayerSearchWidget.cpp

  \brief This file defines the LayerSearchWidget class.
*/

// TerraLib
#include "../../../../dataaccess/utils/Utils.h"
#include "LayerSearchWidget.h"
#include "ui_LayerSearchWidgetForm.h"


Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::LayerSearchWidget::LayerSearchWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::LayerSearchWidgetForm)
{
// add controls
  m_ui->setupUi(this);

  m_minRasterBandsDefined = false;
  m_maxRasterBandsDefined = false;

//connect
  connect(m_ui->m_nameLineEdit, SIGNAL(textChanged(const QString&)), SLOT(onFilter()));
  connect(m_ui->m_repAllRadioButton, SIGNAL(clicked(bool)), SLOT(onFilter()));
  connect(m_ui->m_repGeomRadioButton, SIGNAL(clicked(bool)), SLOT(onFilter()));
  connect(m_ui->m_repRstRadioButton, SIGNAL(clicked(bool)), SLOT(onFilter()));
}

te::qt::widgets::LayerSearchWidget::~LayerSearchWidget()
{
}

Ui::LayerSearchWidgetForm* te::qt::widgets::LayerSearchWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::LayerSearchWidget::enableMultiSelection(bool flag)
{
  if(flag == true)
  {
    m_ui->m_treeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
  }
  else
  {
    m_ui->m_treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  }
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerSearchWidget::getSelecteds() const
{
  std::list<te::map::AbstractLayerPtr> list;

  QList<QTreeWidgetItem*> sel = m_ui->m_treeWidget->selectedItems();

  QList<QTreeWidgetItem*>::iterator it = sel.begin();

  while(it != sel.end())
  {
    QTreeWidgetItem* item = *it;

    QVariant v = item->data(0, Qt::UserRole);

    te::map::AbstractLayerPtr l = v.value<te::map::AbstractLayerPtr>();

    list.push_back(l);

    ++it;
  }

  return list;
}

void te::qt::widgets::LayerSearchWidget::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerList = layerList;

  fillTreeView(m_layerList);
}

void te::qt::widgets::LayerSearchWidget::filterOnlyByRaster()
{
  m_ui->m_repRstRadioButton->setChecked(true);

  m_ui->m_filterByRepGroupBox->setEnabled(false);

  onFilter();
}

void te::qt::widgets::LayerSearchWidget::filterOnlyByGeom()
{
  m_ui->m_repGeomRadioButton->setChecked(true);

  m_ui->m_filterByRepGroupBox->setEnabled(false);

  onFilter();
}

void te::qt::widgets::LayerSearchWidget::filterAll()
{
  m_ui->m_repAllRadioButton->setChecked(true);

  m_ui->m_filterByRepGroupBox->setEnabled(false);

  onFilter();
}

void te::qt::widgets::LayerSearchWidget::setMinRasterBands(std::size_t value)
{
  m_minRasterBandsValue = value;

  m_minRasterBandsDefined = true;

  onFilter();
}

void te::qt::widgets::LayerSearchWidget::setMaxRasterBands(std::size_t value)
{
  m_maxRasterBandsValue = value;

  m_maxRasterBandsDefined = true;

  onFilter();
}

void te::qt::widgets::LayerSearchWidget::fillTreeView(std::list<te::map::AbstractLayerPtr>& layerList)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_treeWidget);
    //layer name
    item->setText(0, l->getTitle().c_str());
    item->setIcon(0, QIcon::fromTheme("layer"));
    item->setData(0, Qt::UserRole, QVariant::fromValue(l));

    //layer parent
    te::map::AbstractLayer* lParent = dynamic_cast<te::map::AbstractLayer*>(l->getParent());

    if(lParent)
      item->setText(1, lParent->getTitle().c_str());
    else
      item->setText(1, "");

    //layer type
    item->setText(2, l->getType().c_str());

    ++it;
  }

//fix columns sizes
  m_ui->m_treeWidget->resizeColumnToContents(0);
  m_ui->m_treeWidget->resizeColumnToContents(2);

  m_ui->m_treeWidget->expandAll();
}

void te::qt::widgets::LayerSearchWidget::filter(const QList<QTreeWidgetItem*>& items)
{
  QTreeWidgetItemIterator it(m_ui->m_treeWidget);

  while(*it)
  {
    QTreeWidgetItem* layerItem = *it;
    bool hide = items.indexOf(layerItem) == -1;
    layerItem->setHidden(hide);

    ++it;
  }

  update();
}

void te::qt::widgets::LayerSearchWidget::onFilter()
{
  QString str = m_ui->m_nameLineEdit->text();

  QList<QTreeWidgetItem*> items = m_ui->m_treeWidget->findItems(str, Qt::MatchContains | Qt::MatchRecursive, 0);

  QList<QTreeWidgetItem*> result;

  for(int i = 0; i < items.size(); ++i)
  {
    QTreeWidgetItem* item = items.at(i);

    QVariant v = item->data(0, Qt::UserRole);

    te::map::AbstractLayerPtr l = v.value<te::map::AbstractLayerPtr>();

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(m_ui->m_repAllRadioButton->isChecked())
    {
      result.push_back(item);
    }
    else if(m_ui->m_repGeomRadioButton->isChecked())
    {
      if(dsType.get() && dsType->hasGeom())
        result.push_back(item);
    }
    else if(m_ui->m_repRstRadioButton->isChecked())
    {
      if(dsType.get() && dsType->hasRaster())
      {
        //check if has band restriction
        if(m_minRasterBandsDefined || m_maxRasterBandsDefined)
        {
          std::auto_ptr<te::da::DataSet> ds = l->getData();

          std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

          std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

          if(m_minRasterBandsDefined && m_maxRasterBandsDefined)
          {
            if(inputRst->getNumberOfBands() >= m_minRasterBandsValue && inputRst->getNumberOfBands() <= m_maxRasterBandsValue)
            {
              result.push_back(item);
            }
          }
          else if(m_minRasterBandsDefined)
          {
            if(inputRst->getNumberOfBands() >= m_minRasterBandsValue)
            {
              result.push_back(item);
            }
          }
          else if(m_minRasterBandsDefined)
          {
            if(inputRst->getNumberOfBands() <= m_maxRasterBandsValue)
            {
              result.push_back(item);
            }
          }
        }
        else
        {
          result.push_back(item);
        }
      }
    }
  }

  filter(result);
}


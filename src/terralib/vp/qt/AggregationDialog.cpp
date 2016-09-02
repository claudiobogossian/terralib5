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
  \file terralib/vp/AggregationDialog.cpp

  \brief A dialog for aggregation operation
*/

// TerraLib
#include "../../core/logger/Logger.h"
#include "../../common/STLUtils.h"
#include "../../common/StringUtils.h"
#include "../../core/translator/Translator.h"


#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"

#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"

#include "../../dataaccess/utils/Utils.h"

#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"

#include "../../geometry/GeometryProperty.h"

#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"

#include "../../statistics/core/Utils.h"

#include "../Config.h"
#include "../Exception.h"
#include "../ComplexData.h"
#include "../Dissolve.h"
#include "../Utils.h"

#include "AggregationDialog.h"
#include "ui_AggregationDialogForm.h"
#include "Utils.h"

// Qt
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QString>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::vp::AggregationDialog::AggregationDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::AggregationDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_inputLayer(0),
    m_toFile(false)
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_objectTypeGroupBox->setVisible(false);
  m_ui->m_statisticalSummatyGroupBox->setVisible(false);
  this->onMultiGeometryChecked(false);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vp-aggregation-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  QSize iconSize(96, 48);

  m_ui->m_singleRadioButton->setIconSize(iconSize);
  m_ui->m_singleRadioButton->setIcon(QIcon::fromTheme("vp-single-objects"));

  m_ui->m_multiRadioButton->setIconSize(iconSize);
  m_ui->m_multiRadioButton->setIcon(QIcon::fromTheme("vp-multi-objects"));

  this->setStatisticalSummary();
  this->setStatisticalSummaryMap();

  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerComboBoxChanged(int)));
  connect(m_ui->m_filterLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onFilterLineEditTextChanged(const QString&)));
  connect(m_ui->m_calcStatCheckBox, SIGNAL(toggled(bool)), this, SLOT(onAdvanced(bool)));
  connect(m_ui->m_multiRadioButton, SIGNAL(toggled(bool)), this, SLOT(onMultiGeometryChecked(bool)));
  connect(m_ui->m_outputListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onOutputListWidgetClicked(QListWidgetItem *)));
  connect(m_ui->m_selectAllComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectAllComboBoxChanged(int)));
  connect(m_ui->m_rejectAllComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onRejectAllComboBoxChanged(int)));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/vp/vp_aggregation.html");

  m_outputDatasource = te::da::DataSourceInfoPtr();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
}

te::vp::AggregationDialog::~AggregationDialog()
{
}

void te::vp::AggregationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while (it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if (dsType->hasGeom())
      m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > te::vp::AggregationDialog::getStatisticalSummary()
{
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > outputStatisticalSummary;

  QList<QListWidgetItem*> itemList = m_ui->m_outputListWidget->selectedItems();
  
  te::stat::StatisticalSummary enumStatisticalSummary;
  std::map<std::string, std::vector<te::stat::StatisticalSummary> >  propname_stat;
  std::map<std::string, std::vector<te::stat::StatisticalSummary> >::iterator it;
  
  for(int i = 0; i < itemList.size(); ++i)
  {
    std::vector<std::string> tokens;
    std::string pname;
    std::string auxItem = itemList[i]->text().toUtf8().data();

    boost::split(tokens, auxItem, boost::is_any_of(":"));
    if(tokens[0] != "")
    {
      pname = tokens[0];
      pname.erase(pname.end() - 1);
      enumStatisticalSummary = (te::stat::StatisticalSummary)itemList[i]->data(Qt::UserRole).toInt();
      it = propname_stat.find(pname);
      if (it != propname_stat.end())
        it->second.push_back(enumStatisticalSummary);
      else
      {
        std::vector<te::stat::StatisticalSummary> nvec;
        nvec.push_back(enumStatisticalSummary);
        propname_stat.insert(std::make_pair(pname, nvec));
      }
    }
  }
  
  it = propname_stat.begin();
  while (it != propname_stat.end())
  {
    te::dt::Property* prop = getSelectedPropertyByName(it->first);
    outputStatisticalSummary.insert(std::make_pair(prop,it->second));
    ++it;
  }
  
  return outputStatisticalSummary;
}

te::dt::Property* te::vp::AggregationDialog::getSelectedPropertyByName(std::string propertyName)
{
  te::dt::Property* selProperty;
  if(propertyName == "")
    return 0;

  for(std::size_t i = 0; i < m_properties.size(); ++i)
  {
    if(propertyName == m_properties[i]->getName())
    {
      selProperty = m_properties[i];
      return selProperty;
    }
  }
  return 0;
}

std::vector<std::string> te::vp::AggregationDialog::getSelectedPropertyNames()
{
  std::vector<std::string> selectedPropertyNames;

  for(int i = 0; i != m_ui->m_propertieslistWidget->count(); ++i)
  {
    QListWidgetItem* item = m_ui->m_propertieslistWidget->item(i);

    if(m_ui->m_propertieslistWidget->isItemSelected(item))
    {
      std::string name = item->text().toUtf8().data();
      selectedPropertyNames.push_back(name);
    }
  }

  return selectedPropertyNames;
}

bool te::vp::AggregationDialog::isCollection()
{
  if (m_ui->m_multiRadioButton->isChecked())
    return true;

  return false;
}

te::map::AbstractLayerPtr te::vp::AggregationDialog::getLayer()
{
  return m_layer;
}

std::vector<std::string> te::vp::AggregationDialog::getWarnings()
{
  return m_warnings;
}

void te::vp::AggregationDialog::setStatisticalSummary()
{
    m_ui->m_selectAllComboBox->addItem("");
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MIN_VALUE).c_str()), te::stat::MIN_VALUE);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MAX_VALUE).c_str()), te::stat::MAX_VALUE);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEAN).c_str()), te::stat::MEAN);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::SUM).c_str()), te::stat::SUM);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::COUNT).c_str()), te::stat::COUNT);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VALID_COUNT).c_str()), te::stat::VALID_COUNT);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::STANDARD_DEVIATION).c_str()), te::stat::STANDARD_DEVIATION);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VARIANCE).c_str()), te::stat::VARIANCE);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::SKEWNESS).c_str()), te::stat::SKEWNESS);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::KURTOSIS).c_str()), te::stat::KURTOSIS);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::AMPLITUDE).c_str()), te::stat::AMPLITUDE);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEDIAN).c_str()), te::stat::MEDIAN);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VAR_COEFF).c_str()), te::stat::VAR_COEFF);
    m_ui->m_selectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MODE).c_str()), te::stat::MODE);

    m_ui->m_rejectAllComboBox->addItem("");
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MIN_VALUE).c_str()), te::stat::MIN_VALUE);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MAX_VALUE).c_str()), te::stat::MAX_VALUE);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEAN).c_str()), te::stat::MEAN);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::SUM).c_str()), te::stat::SUM);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::COUNT).c_str()), te::stat::COUNT);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VALID_COUNT).c_str()), te::stat::VALID_COUNT);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::STANDARD_DEVIATION).c_str()), te::stat::STANDARD_DEVIATION);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VARIANCE).c_str()), te::stat::VARIANCE);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::SKEWNESS).c_str()), te::stat::SKEWNESS);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::KURTOSIS).c_str()), te::stat::KURTOSIS);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::AMPLITUDE).c_str()), te::stat::AMPLITUDE);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEDIAN).c_str()), te::stat::MEDIAN);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VAR_COEFF).c_str()), te::stat::VAR_COEFF);
    m_ui->m_rejectAllComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MODE).c_str()), te::stat::MODE);
}

void te::vp::AggregationDialog::setStatisticalSummaryMap()
{
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MIN_VALUE, te::stat::GetStatSummaryFullName(te::stat::MIN_VALUE)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MAX_VALUE, te::stat::GetStatSummaryFullName(te::stat::MAX_VALUE)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MEAN, te::stat::GetStatSummaryFullName(te::stat::MEAN)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::SUM, te::stat::GetStatSummaryFullName(te::stat::SUM)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::COUNT, te::stat::GetStatSummaryFullName(te::stat::COUNT)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::VALID_COUNT, te::stat::GetStatSummaryFullName(te::stat::VALID_COUNT)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::STANDARD_DEVIATION, te::stat::GetStatSummaryFullName(te::stat::STANDARD_DEVIATION)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::VARIANCE, te::stat::GetStatSummaryFullName(te::stat::VARIANCE)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::SKEWNESS, te::stat::GetStatSummaryFullName(te::stat::SKEWNESS)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::KURTOSIS, te::stat::GetStatSummaryFullName(te::stat::KURTOSIS)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::AMPLITUDE, te::stat::GetStatSummaryFullName(te::stat::AMPLITUDE)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MEDIAN, te::stat::GetStatSummaryFullName(te::stat::MEDIAN)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::VAR_COEFF, te::stat::GetStatSummaryFullName(te::stat::VAR_COEFF)));
  m_StatisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MODE, te::stat::GetStatSummaryFullName(te::stat::MODE)));
}

void te::vp::AggregationDialog::setFunctionsByLayer(std::vector<te::dt::Property*> properties)
{
  QStringList propertyList;
  int propertyType;

  m_ui->m_selectAllComboBox->setCurrentIndex(0);
  m_ui->m_rejectAllComboBox->setCurrentIndex(0);
  m_ui->m_outputListWidget->clear();

  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
  te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
  const te::da::DataSourceCapabilities dsCapabilities = dataSource->getCapabilities();

  for(size_t i=0; i < properties.size(); ++i)
  {
    propertyType = properties[i]->getType();
    if (propertyType == te::dt::GEOMETRY_TYPE)
      continue;
    
    if(propertyType == te::dt::STRING_TYPE)
    {  
      QListWidgetItem* item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::MIN_VALUE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MIN_VALUE));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::MAX_VALUE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MAX_VALUE));
      m_ui->m_outputListWidget->addItem(item);
        
      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::COUNT].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::COUNT));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::VALID_COUNT].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::VALID_COUNT));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem("");
      m_ui->m_outputListWidget->addItem(item);
    }
    else
    {
      QListWidgetItem* item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::MIN_VALUE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MIN_VALUE));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::MAX_VALUE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MAX_VALUE));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::MEAN].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MEAN));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::SUM].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::SUM));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::COUNT].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::COUNT));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::VALID_COUNT].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::VALID_COUNT));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::STANDARD_DEVIATION].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::STANDARD_DEVIATION));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::VARIANCE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::VARIANCE));
      m_ui->m_outputListWidget->addItem(item);

      item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::AMPLITUDE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::AMPLITUDE));
      m_ui->m_outputListWidget->addItem(item);

// Does not implemented for Query Operation.
      if (!dsCapabilities.supportsPreparedQueryAPI())
      {
        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::SKEWNESS].c_str());
        item->setData(Qt::UserRole, QVariant(te::stat::SKEWNESS));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::KURTOSIS].c_str());
        item->setData(Qt::UserRole, QVariant(te::stat::KURTOSIS));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::MEDIAN].c_str());
        item->setData(Qt::UserRole, QVariant(te::stat::MEDIAN));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::VAR_COEFF].c_str());
        item->setData(Qt::UserRole, QVariant(te::stat::VAR_COEFF));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::MODE].c_str());
        item->setData(Qt::UserRole, QVariant(te::stat::MODE));
        m_ui->m_outputListWidget->addItem(item);
      }

      item = new QListWidgetItem("");
      m_ui->m_outputListWidget->addItem(item);
    }
  }

  int lastRow = m_ui->m_outputListWidget->count() - 1;
  delete m_ui->m_outputListWidget->item(lastRow);
}

void te::vp::AggregationDialog::onLayerComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toUtf8().data();

  m_ui->m_propertieslistWidget->clear();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      std::size_t type;
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_inputLayer = selectedLayer;
      std::auto_ptr<const te::map::LayerSchema> schema(selectedLayer->getSchema());

      if(schema->size() == 0)
        return;

      te::common::FreeContents(m_properties);
      m_properties.clear();

      const std::vector<te::dt::Property*>& properties = schema->getProperties();
      
      te::common::Clone(properties, m_properties);
      
      setFunctionsByLayer(m_properties);

      for(size_t i = 0; i < m_properties.size(); ++i)
      {
        type = m_properties[i]->getType();
        
        if(type != te::dt::GEOMETRY_TYPE)
          m_ui->m_propertieslistWidget->addItem(m_properties[i]->getName().c_str());
      }

      return;
    }
    ++it;
  }
}

void te::vp::AggregationDialog::onAdvanced(bool visible)
{
  m_ui->m_outputListWidget->reset();
  m_ui->m_objectTypeGroupBox->setVisible(visible);
  m_ui->m_statisticalSummatyGroupBox->setVisible(visible);
}

void te::vp::AggregationDialog::onMultiGeometryChecked(bool checked)
{
  m_ui->m_statisticalSummatyGroupBox->setEnabled(checked);
  m_ui->m_selectAllComboBox->setEnabled(checked);
  m_ui->m_rejectAllComboBox->setEnabled(checked);
  m_ui->m_outputListWidget->setEnabled(checked);

  if (!checked)
  {
    m_ui->m_selectAllComboBox->setCurrentIndex(0);
    m_ui->m_rejectAllComboBox->setCurrentIndex(0);
    m_ui->m_outputListWidget->clearSelection();
  }
}

void te::vp::AggregationDialog::onFilterLineEditTextChanged(const QString& text)
{
  QList<QListWidgetItem*> allItems;
  int count = m_ui->m_propertieslistWidget->count();
  for(int index = 0; index < count; ++index)
  {
    allItems.push_back(m_ui->m_propertieslistWidget->item(index));
  }

  QList<QListWidgetItem*> filteredItems = m_ui->m_propertieslistWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive);

  for(int i = 0; i < allItems.size(); ++i)
  {
    QListWidgetItem* item = allItems.at(i);
    bool hide = filteredItems.indexOf(item) == -1;
    item->setHidden(hide);
  }
  
  m_ui->m_propertieslistWidget->update();
}

void te::vp::AggregationDialog::onSelectAllComboBoxChanged(int index)
{
  QString text = m_ui->m_selectAllComboBox->itemText(index);
  Qt::MatchFlags flag = Qt::MatchEndsWith; //The search term matches the end of the item.
  
  if(text.isEmpty())
    return;

  QList<QListWidgetItem *> listFound;
  listFound = m_ui->m_outputListWidget->findItems(text, flag);
  
  for(int i=0; i < listFound.size(); ++i)
    listFound.at(i)->setSelected(true);

  m_ui->m_rejectAllComboBox->setCurrentIndex(0);
}

void te::vp::AggregationDialog::onRejectAllComboBoxChanged(int index)
{
  QString text = m_ui->m_selectAllComboBox->itemText(index);
  Qt::MatchFlags flag = Qt::MatchEndsWith; //The search term matches the end of the item.
  
  if(text=="")
    return;

  QList<QListWidgetItem *> listFound;
  listFound = m_ui->m_outputListWidget->findItems(text, flag);
  
  for(int i=0; i < listFound.size(); ++i)
    listFound.at(i)->setSelected(false);

  m_ui->m_selectAllComboBox->setCurrentIndex(0);
}

void te::vp::AggregationDialog::onOutputListWidgetClicked(QListWidgetItem * item)
{
  if(item->text().isEmpty())
  {
    item->setSelected(false);
  }
}

void te::vp::AggregationDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.empty())
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;
  
  m_toFile = false;
}

void te::vp::AggregationDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();
  
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                        QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;
  
  boost::filesystem::path outfile(fileName.toUtf8().data());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());
  
  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::vp::AggregationDialog::onOkPushButtonClicked()
{
// Validate Input Layer.
  if (m_ui->m_layersComboBox->count() == 0)
  {
    QMessageBox::information(this, tr("Dissolve"), tr("Select an input layer."));
    return;
  }

// Validate DataSource.
  te::da::DataSourcePtr inputDataSource = te::da::GetDataSource(m_inputLayer->getDataSourceId(), true);
  if (!inputDataSource.get())
  {
    QMessageBox::information(this, tr("Dissolve"), tr("The selected input data source can not be accessed."));
    return;
  }

// Verify selected properties to do Dissolve.
  std::vector<std::string> selProperties = getSelectedPropertyNames();
  if (selProperties.empty())
  {
    QMessageBox::information(this, tr("Dissolve"), tr("Select at least one grouping attribute."));
    return;
  }

// Set specific parameters.
  std::map<std::string, te::dt::AbstractData*> specificParams;
  specificParams["DISSOLVE"] = new te::vp::ComplexData<std::vector<std::string> >(selProperties);

  specificParams["IS_COLLECTION"] = new te::dt::SimpleData<bool, te::dt::BOOLEAN_TYPE>(this->isCollection());


  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > outputStatisticalSummary = getStatisticalSummary();
  specificParams["SUMMARY"] = new te::vp::ComplexData<std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > >(outputStatisticalSummary);


// Validade output repository.
  if (m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Dissolve"), tr("Define a repository for the result."));
    return;
  }

  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toUtf8().data();

  if (outputdataset.empty())
  {
    QMessageBox::information(this, tr("Dissolve"), tr("Define a name for the resulting layer."));
    return;
  }

// Verify if "Input Only Selected objects" is checked.
  bool inputIsChecked = false;

  if (m_ui->m_onlySelectedCheckBox->isChecked())
    inputIsChecked = true;

  try
  {
// Declare the input parameters
    te::vp::InputParams structInputParams;

// Set the inputLayer parameters
    structInputParams.m_inputDataSource = inputDataSource;
    structInputParams.m_inputDataSetType = m_inputLayer->getSchema().release();

    te::gm::GeometryProperty* geomInputProp = te::da::GetFirstGeomProperty(structInputParams.m_inputDataSetType);

    int inputSRID = 0;

    if (!geomInputProp)
    {
      QMessageBox::information(this, tr("Dissolve"), tr("Problem to get geometry property of input layer."));

      return;
    }

    inputSRID = m_inputLayer->getSRID();

// Select a strategy based on the capabilities of the input datasource
    const te::da::DataSourceCapabilities inputDSCapabilities = inputDataSource->getCapabilities();

    bool isQuery = false;

    te::da::Select* inputSelect = 0;
    te::da::DataSet* inputDataSet = 0;

    if (inputDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
    {
      isQuery = true;

// Get Select Query using AbstractLayerPtr to process by spatial database. 
      inputSelect = te::vp::GetSelectQueryFromLayer(m_inputLayer, inputIsChecked, inputSRID);

      if (inputSelect)
        structInputParams.m_inputQuery = inputSelect;
    }
    else
    {
// Get DataSet and DataSetType using AbstractLayerPtr to process by memory, using GEOS.
      te::vp::DataStruct inputData = te::vp::GetDataStructFromLayer(m_inputLayer, inputIsChecked, inputSRID);

      if (inputData.m_dataSet)
        structInputParams.m_inputDataSet = inputData.m_dataSet;

      if (inputData.m_dataSetType)
        structInputParams.m_inputDataSetType = inputData.m_dataSetType;
    }
    
    m_inputParams.push_back(structInputParams);


// Return of operation result.
    bool res = true;

    if (m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toUtf8().data());

      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, tr("Dissolve"), tr("Output file already exists. Remove it and try again. "));

        return;
      }

      std::size_t idx = outputdataset.find(".");
      if (idx != std::string::npos)
        outputdataset = outputdataset.substr(0, idx);

      te::da::DataSourcePtr dsOGR = te::vp::CreateOGRDataSource(m_ui->m_repositoryLineEdit->text().toUtf8().data());

      dsOGR->open();

      this->setCursor(Qt::WaitCursor);

// Set parameters (Input/Output).
      m_params = new te::vp::AlgorithmParams();
      m_params->setInputParams(m_inputParams);
      m_params->setOutputDataSource(dsOGR);
      m_params->setOutputDataSetName(outputdataset);
      m_params->setOutputSRID(inputSRID);
      m_params->setSpecificParams(specificParams);

      te::vp::Dissolve dissolve;

      const te::da::DataSourceCapabilities inputDSCapabilities = inputDataSource->getCapabilities();

      if (isQuery)
      {
        res = dissolve.executeQuery(m_params);
      }
      else
      {
        res = dissolve.executeMemory(m_params);
      }

      if (!res)
      {
        dsOGR->close();

        QMessageBox::information(this, tr("Dissolve"), tr("Error: could not generate the dissolve."));

        reject();
      }

      m_outputDatasource = te::da::DataSourceInfoManager::getInstance().get(dsOGR->getId());

      if (!m_outputDatasource)
      {
        QMessageBox::information(this, tr("Dissolve"), tr("The output data source can not be accessed."));

        return;
      }

      m_warnings = m_params->getWarnings();

      delete m_params;
    }
    else
    {
      te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
      if (!aux.get())
      {
        QMessageBox::information(this, tr("Dissolve"), tr("The output data source can not be accessed."));

        return;
      }

      std::string name = te::common::Convert2LCase(outputdataset);

      if (aux->dataSetExists(name))
      {
        QMessageBox::information(this, tr("Dissolve"), tr("Dataset already exists. Remove it or select a new name and try again."));

        return;
      }

      this->setCursor(Qt::WaitCursor);

// Set parameters (Input/Output).
      m_params = new te::vp::AlgorithmParams();
      m_params->setInputParams(m_inputParams);
      m_params->setOutputDataSource(aux);
      m_params->setOutputDataSetName(outputdataset);
      m_params->setOutputSRID(inputSRID);
      m_params->setSpecificParams(specificParams);

      te::vp::Dissolve dissolve;

// Select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities inputDSCapabilities = inputDataSource->getCapabilities();

      if (isQuery)
      {
        res = dissolve.executeQuery(m_params);
      }
      else
      {
        res = dissolve.executeMemory(m_params);
      }

      m_warnings = m_params->getWarnings();

      delete m_params;

      if (!res)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, tr("Dissolve"), tr("Error: could not generate the dissolve."));
        reject();
      }
    }

// creating a layer for the result
    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());

    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());

    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    m_layer = converter(dt);
  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);

    QMessageBox::information(this, "Aggregation", e.what());
    
#ifdef TERRALIB_LOGGER_ENABLED
    std::string str = "Aggregation - ";
    str += e.what();
    TE_CORE_LOG_DEBUG("vp", str.c_str());
#endif // TERRALIB_LOGGER_ENABLED

    return;
  }

  this->setCursor(Qt::ArrowCursor);
  
  accept();
}

void te::vp::AggregationDialog::onCancelPushButtonClicked()
{
  reject();
}


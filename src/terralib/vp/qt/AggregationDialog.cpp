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
  \file terralib/vp/AggregationDialog.cpp

  \brief A dialog for aggregation operation
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../statistics/core/Utils.h"
#include "../core/Config.h"
#include "../core/Exception.h"
#include "Aggregation.h"
#include "AggregationDialog.h"
#include "LayerTreeModel.h"
#include "ui_AggregationDialogForm.h"
#include "VectorProcessingConfig.h"
#include "Utils.h"

// Qt
#include <QtCore/QList>
#include <QtGui/QFileDialog>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeWidget>

te::vp::AggregationDialog::AggregationDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::AggregationDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_selectedLayer(0)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme(VP_IMAGES"/vp-aggregation-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  setStatisticalSummary();
  setStatisticalSummaryMap();

  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerComboBoxChanged(int)));
  connect(m_ui->m_filterLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onFilterLineEditTextChanged(const QString&)));
  connect(m_ui->m_outputListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onOutputListWidgetClicked(QListWidgetItem *)));
  connect(m_ui->m_selectAllComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectAllComboBoxChanged(int)));
  connect(m_ui->m_rejectAllComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onRejectAllComboBoxChanged(int)));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::vp::AggregationDialog::~AggregationDialog()
{
}

void te::vp::AggregationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {  
    m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

int te::vp::AggregationDialog::getMemoryUse()
{
  if(m_ui->m_advancedOptionsGroupBox->isChecked())
  {
    if(m_ui->m_wholeMemRadioButton->isChecked())
      return WHOLE_MEM;
    else if(m_ui->m_partiallyMemRadioButton->isChecked())
      return PARTIALLY_MEM;
    else
      return LOW_MEM;
  }
  else
    m_ui->m_wholeMemRadioButton->setChecked(true);
    return WHOLE_MEM;
}

std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > te::vp::AggregationDialog::getStatisticalSummary()
{
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > outputStatisticalSummary;

  QList<QListWidgetItem*> itemList = m_ui->m_outputListWidget->selectedItems();

  std::string propertyName = "";
  std::string aux = "";
  te::dt::Property* currentToken;
  te::dt::Property* auxProperty;
  te::stat::StatisticalSummary enumStatisticalSummary;
  std::vector<te::stat::StatisticalSummary> vectorStatisticalSummary;
  
  for(int i = 0; i < itemList.size(); ++i)
  {
    std::vector<std::string> tokens;

    te::common::Tokenize(itemList[i]->text().toStdString(), tokens, ":");
    
    if(tokens.empty())
    {
      propertyName = "";
      currentToken = getSelectedPropertyByName(propertyName);
    }
    else
    {
      propertyName = tokens[0];
      propertyName.erase(propertyName.end() - 1);
      currentToken = getSelectedPropertyByName(propertyName);
    }

    enumStatisticalSummary = (te::stat::StatisticalSummary)itemList[i]->data(Qt::UserRole).toInt();

    if(propertyName != aux && aux != "")
    {
      outputStatisticalSummary[auxProperty] = vectorStatisticalSummary;
      vectorStatisticalSummary.clear();
      
      if(!propertyName.empty())
        vectorStatisticalSummary.push_back(enumStatisticalSummary);
    }
    else
    {
        vectorStatisticalSummary.push_back(enumStatisticalSummary);

      if(i == itemList.size() - 1)
      {
        auxProperty = currentToken;
        outputStatisticalSummary[auxProperty] = vectorStatisticalSummary;
      }
    }

    aux = propertyName;
    auxProperty = currentToken;
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

std::vector<te::dt::Property*> te::vp::AggregationDialog::getSelectedProperties()
{
  std::vector<te::dt::Property*> selProperties;

  for(std::size_t i = 0; i != m_ui->m_propertieslistWidget->count(); ++i)
  {
    if(m_ui->m_propertieslistWidget->isItemSelected(m_ui->m_propertieslistWidget->item(i)))
    {
      selProperties.push_back(m_properties[i]);
    }
  }

  return selProperties;
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
    
  for(size_t i=0; i < properties.size(); ++i)
  {
    propertyType = properties[i]->getType();
    if(propertyType != te::dt::GEOMETRY_TYPE)
    {
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

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::SKEWNESS].c_str());
        item->setData(Qt::UserRole, QVariant(te::stat::SKEWNESS));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::KURTOSIS].c_str());
        item->setData(Qt::UserRole, QVariant(te::stat::KURTOSIS));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_StatisticalSummaryMap[te::stat::AMPLITUDE].c_str());
        item->setData(Qt::UserRole, QVariant(te::stat::AMPLITUDE));
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

        item = new QListWidgetItem("");
        m_ui->m_outputListWidget->addItem(item);
      }
    }
  }

  int lastRow = m_ui->m_outputListWidget->count() - 1;
  delete m_ui->m_outputListWidget->item(lastRow);
}

void te::vp::AggregationDialog::onLayerComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  m_ui->m_propertieslistWidget->clear();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      std::size_t type;
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_selectedLayer = selectedLayer;
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

void te::vp::AggregationDialog::onFilterLineEditTextChanged(const QString& text)
{
  std::list<te::map::AbstractLayerPtr> filteredLayers = te::vp::GetFilteredLayers(text.toStdString(), m_layers);

  m_ui->m_layersComboBox->clear();

  m_ui->m_selectAllComboBox->setCurrentIndex(0);
  m_ui->m_rejectAllComboBox->setCurrentIndex(0);
  m_ui->m_outputListWidget->clear();

  if(text.isEmpty())
    filteredLayers = m_layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = filteredLayers.begin();

  while(it != filteredLayers.end())
  {  
    m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
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
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;
}

void te::vp::AggregationDialog::onTargetFileToolButtonPressed()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Open Feature File"), QString(""), tr("Common Formats (*.shp *.SHP *.kml *.KML *.geojson *.GEOJSON *.gml *.GML);; Shapefile (*.shp *.SHP);; GML (*.gml *.GML);; Web Feature Service - WFS (*.xml *.XML *.wfs *.WFS);; All Files (*.*)"), 0, QFileDialog::ReadOnly);
  
  if(fileName.isEmpty())
    return;

  m_ui->m_repositoryLineEdit->setText(fileName);

  std::vector<te::da::DataSourceInfoPtr> datasources;
  te::da::DataSourceInfoManager::getInstance().getByType("OGR", datasources);

  m_outputDatasource = datasources[0];
}

void te::vp::AggregationDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::vp::AggregationDialog::onOkPushButtonClicked()
{
  if(m_ui->m_layersComboBox->count() == 0)
  {
    QMessageBox::information(this, "Aggregation", "Please, you must select a layer.");

    return;
  }
  
  std::vector<te::dt::Property*> selProperties = getSelectedProperties();

  if(selProperties.empty())
  {
    QMessageBox::information(this, "Aggregation", "Please, select at least one property.");

    return;
  }
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > outputStatisticalSummary = getStatisticalSummary();
      
  te::vp::MemoryUse memoryUse = (te::vp::MemoryUse)getMemoryUse();
      
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Aggregation", "Set a name for the new Layer.");

    return;
  }

  std::string outputLayerName = m_ui->m_newLayerNameLineEdit->text().toStdString();

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Aggregation", "Set a repository for the new Layer.");

    return;
  }

  try
  {
    te::vp::Aggregation(m_selectedLayer, selProperties, outputStatisticalSummary, memoryUse, outputLayerName, m_outputDatasource);
  }
  catch(const std::exception& e)
  {
    QString errMsg(tr("Error during map aggregation. The reported error is: %1"));

    errMsg = errMsg.arg(e.what());

    QMessageBox::information(this, "Aggregation", errMsg);
  }
}

void te::vp::AggregationDialog::onCancelPushButtonClicked()
{
  reject();
}
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

  \brief A dialog for intersection operation
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
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
    m_model(0)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme(VP_IMAGES"/vp-aggregation-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  setAttributes();
  setAttributesNameMap();

  connect(m_ui->m_layerTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onTreeViewClicked(const QModelIndex&)));
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
  delete m_model;
}

void te::vp::AggregationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  if(m_model != 0)
      delete m_model;

  m_model = new LayerTreeModel(m_layers, true);

  m_ui->m_layerTreeView->setModel(m_model);
  m_ui->m_layerTreeView->setSelectionMode(QAbstractItemView::NoSelection);
  m_ui->m_layerTreeView->resizeColumnToContents(0);
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

std::map<std::string, std::vector<te::vp::Attributes> > te::vp::AggregationDialog::getOutputAttributes()
{
  std::map<std::string, std::vector<te::vp::Attributes> > outputAttributes;

  QList<QListWidgetItem*> itemList = m_ui->m_outputListWidget->selectedItems();

  std::string aux = "";
  std::string currentToken = "";
  std::string attribute = "";
  te::vp::Attributes enumAttribute;
  std::vector<te::vp::Attributes> vectorAttributes;
  
  for(int i = 0; i < itemList.size(); ++i)
  {
    std::vector<std::string> tokens;
    te::common::Tokenize(itemList[i]->text().toStdString(), tokens, ":");

    currentToken = tokens[0];
    currentToken.erase(currentToken.end() - 1);

    enumAttribute = (te::vp::Attributes)itemList[i]->data(Qt::UserRole).toInt();

    if(currentToken != aux && aux != "")
    {
      outputAttributes[aux] = vectorAttributes;
      vectorAttributes.clear();
      vectorAttributes.push_back(enumAttribute);
    }
    else
    {
      vectorAttributes.push_back(enumAttribute);

      if(i == itemList.size() - 1)
      {
        outputAttributes[aux] = vectorAttributes;
      }
    }

    aux = currentToken;
  }

  return outputAttributes;
}

void te::vp::AggregationDialog::setAttributes()
{
  m_ui->m_selectAllComboBox->addItem("");
  m_ui->m_selectAllComboBox->addItem(TR_VP("Minimum value"), MIN_VALUE);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Maximum value"), MAX_VALUE);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Mean"), MEAN);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Sum of values"), SUM);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Total number of values"), COUNT);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Total not null values"), VALID_COUNT);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Standard deviation"), STANDARD_DEVIATION);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Kernel"), KERNEL);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Variance"), VARIANCE);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Skewness"), SKEWNESS);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Kurtosis"), KURTOSIS);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Amplitude"), AMPLITUDE);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Median"), MEDIAN);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Coefficient variation"), VAR_COEFF);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Mode"), MODE);

  m_ui->m_rejectAllComboBox->addItem("");
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Minimum value"), MIN_VALUE);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Maximum value"), MAX_VALUE);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Mean"), MEAN);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Sum of values"), SUM);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Total number of values"), COUNT);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Total not null values"), VALID_COUNT);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Standard deviation"), STANDARD_DEVIATION);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Kernel"), KERNEL);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Variance"), VARIANCE);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Skewness"), SKEWNESS);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Kurtosis"), KURTOSIS);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Amplitude"), AMPLITUDE);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Median"), MEDIAN);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Coefficient variation"), VAR_COEFF);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Mode"), MODE);

}

void te::vp::AggregationDialog::setAttributesNameMap()
{
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MIN_VALUE, TR_VP("Minimum value")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MAX_VALUE, TR_VP("Maximum value")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MEAN, TR_VP("Mean")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(SUM, TR_VP("Sum of values")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(COUNT, TR_VP("Total number of values")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(VALID_COUNT, TR_VP("Total not null values")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(STANDARD_DEVIATION, TR_VP("Standard deviation")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(KERNEL, TR_VP("Kernel")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(VARIANCE, TR_VP("Variance")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(SKEWNESS, TR_VP("Skewness")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(KURTOSIS, TR_VP("Kurtosis")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(AMPLITUDE, TR_VP("Amplitude")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MEDIAN, TR_VP("Median")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(VAR_COEFF, TR_VP("Coefficient variation")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MODE, TR_VP("Mode")));
}

void te::vp::AggregationDialog::onFilterLineEditTextChanged(const QString& text)
{
  std::list<te::map::AbstractLayerPtr> filteredLayers = te::vp::GetFilteredLayers(text.toStdString(), m_layers);

  delete m_model;
  m_ui->m_selectAllComboBox->setCurrentIndex(0);
  m_ui->m_rejectAllComboBox->setCurrentIndex(0);
  m_ui->m_outputListWidget->clear();

  if(text == "")
    filteredLayers = m_layers;

  m_model = new LayerTreeModel(filteredLayers);

  m_ui->m_layerTreeView->setModel(m_model);
}

void te::vp::AggregationDialog::onTreeViewClicked(const QModelIndex& index)
{
  QStringList propertyList;
  int propertyType;

  m_ui->m_selectAllComboBox->setCurrentIndex(0);
  m_ui->m_rejectAllComboBox->setCurrentIndex(0);

  std::map<te::map::AbstractLayerPtr, std::vector<te::dt::Property*> > selected = m_model->getSelected();
  
  if(selected.size() > 0)
  {
    m_ui->m_outputListWidget->clear();

    std::vector<te::dt::Property*> properties = selected.begin()->second;
    
    for(size_t i=0; i < properties.size(); ++i)
    {
      propertyType = properties[i]->getType();

      if(propertyType == te::dt::STRING_TYPE)
      {  
        QListWidgetItem* item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MIN_VALUE].c_str());
        item->setData(Qt::UserRole, QVariant(MIN_VALUE));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MAX_VALUE].c_str());
        item->setData(Qt::UserRole, QVariant(MAX_VALUE));
        m_ui->m_outputListWidget->addItem(item);
        
        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[COUNT].c_str());
        item->setData(Qt::UserRole, QVariant(COUNT));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[VALID_COUNT].c_str());
        item->setData(Qt::UserRole, QVariant(VALID_COUNT));
        m_ui->m_outputListWidget->addItem(item);
        
        if(i < properties.size()-1)
        {
          QListWidgetItem* item = new QListWidgetItem("");
          m_ui->m_outputListWidget->addItem(item);
        }
      }
      else
      {
        QListWidgetItem* item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MIN_VALUE].c_str());
        item->setData(Qt::UserRole, QVariant(MIN_VALUE));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MAX_VALUE].c_str());
        item->setData(Qt::UserRole, QVariant(MAX_VALUE));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MEAN].c_str());
        item->setData(Qt::UserRole, QVariant(MEAN));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[SUM].c_str());
        item->setData(Qt::UserRole, QVariant(SUM));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[COUNT].c_str());
        item->setData(Qt::UserRole, QVariant(COUNT));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[VALID_COUNT].c_str());
        item->setData(Qt::UserRole, QVariant(VALID_COUNT));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[STANDARD_DEVIATION].c_str());
        item->setData(Qt::UserRole, QVariant(STANDARD_DEVIATION));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[KERNEL].c_str());
        item->setData(Qt::UserRole, QVariant(KERNEL));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[VARIANCE].c_str());
        item->setData(Qt::UserRole, QVariant(VARIANCE));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[SKEWNESS].c_str());
        item->setData(Qt::UserRole, QVariant(SKEWNESS));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[KURTOSIS].c_str());
        item->setData(Qt::UserRole, QVariant(KURTOSIS));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[AMPLITUDE].c_str());
        item->setData(Qt::UserRole, QVariant(AMPLITUDE));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MEDIAN].c_str());
        item->setData(Qt::UserRole, QVariant(MEDIAN));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[VAR_COEFF].c_str());
        item->setData(Qt::UserRole, QVariant(VAR_COEFF));
        m_ui->m_outputListWidget->addItem(item);

        item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MODE].c_str());
        item->setData(Qt::UserRole, QVariant(MODE));
        m_ui->m_outputListWidget->addItem(item);
        
        if(i < properties.size()-1)
        {
          QListWidgetItem* item = new QListWidgetItem("");
          m_ui->m_outputListWidget->addItem(item);
        }
      }
    }

  }
  else
  {
    m_ui->m_outputListWidget->clear();
  }
}

void te::vp::AggregationDialog::onSelectAllComboBoxChanged(int index)
{
  QString text = m_ui->m_selectAllComboBox->itemText(index);
  Qt::MatchFlags flag = Qt::MatchEndsWith; //The search term matches the end of the item.
  
  if(text=="")
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
  if(item->text()=="")
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
  std::map<te::map::AbstractLayerPtr, std::vector<te::dt::Property*> > selectedLayer = m_model->getSelected();

  if(selectedLayer.size() > 0)
  {
    if(selectedLayer.begin()->second.size() > 0)
    {
      std::map<std::string, std::vector<te::vp::Attributes> > outputAttributes = getOutputAttributes();
      
      te::vp::MemoryUse memoryUse = (te::vp::MemoryUse)getMemoryUse();
      
      if(m_ui->m_newLayerNameLineEdit->text() != "")
      {
        std::string outputLayerName = m_ui->m_newLayerNameLineEdit->text().toStdString();

        if(m_ui->m_repositoryLineEdit->text() != "")
        {
          te::vp::Aggregation(selectedLayer, outputAttributes, outputLayerName, m_outputDatasource);
        }
        else
        {
          QMessageBox::information(this, "Aggregation", "Set a repository for the new Layer.");
        }
      }
      else
      {
        QMessageBox::information(this, "Aggregation", "Set a name for the new Layer.");
      }
    }
    else
    {
      QMessageBox::information(this, "Aggregation", "Select at least one property.");
    }
  }
  else
  {
    QMessageBox::information(this, "Aggregation", "Select one layer.");
  }
}

void te::vp::AggregationDialog::onCancelPushButtonClicked()
{
  reject();
}



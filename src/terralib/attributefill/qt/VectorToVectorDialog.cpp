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
  \file terralib/attributefill/VectorToVectorDialog.cpp

  \brief Raster to vector attributefill dialog.
*/

// TerraLib
#include "../../attributefill/VectorToVector.h"
#include "../../common/Exception.h"
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/af/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/Utils.h"
#include "../../statistics/core/Utils.h"
#include "../Config.h"
#include "../Exception.h"
#include "VectorToVectorDialog.h"
#include "ui_VectorToVectorDialogForm.h"

// Qt
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

//Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::attributefill::VectorToVectorDialog::VectorToVectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::VectorToVectorDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_path(""),
    m_toFile(false)
{
  // add controls
  m_ui->setupUi(this);

  setStatisticalSummary();
  setStatisticalSummaryMap();

  // add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("attributefill-vector2vector-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  connect(m_ui->m_fromLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFromLayerComboBoxCurrentIndexChanged(int)));
  connect(m_ui->m_toLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onToLayerComboBoxCurrentIndexChanged(int)));

  connect(m_ui->m_statisticsListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(onStatisticsListWidgetItemPressed(QListWidgetItem*)));

  connect(m_ui->m_selectAllComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectAllComboBoxChanged(int)));
  connect(m_ui->m_rejectAllComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onRejectAllComboBoxChanged(int)));
}

te::attributefill::VectorToVectorDialog::~VectorToVectorDialog()
{
}

void te::attributefill::VectorToVectorDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if(dsType->hasGeom())
    {
      std::string layerName = it->get()->getTitle();
      std::string layerId = it->get()->getId();

      m_ui->m_toLayerComboBox->addItem(QString(layerName.c_str()), QVariant(layerId.c_str()));
      m_ui->m_fromLayerComboBox->addItem(QString(layerName.c_str()), QVariant(layerId.c_str()));
    }
    ++it;
  }

  if(m_ui->m_fromLayerComboBox->count() > 1)
  {
    m_ui->m_fromLayerComboBox->setCurrentIndex(0);
    m_ui->m_toLayerComboBox->setCurrentIndex(1);
  }
}

te::map::AbstractLayerPtr te::attributefill::VectorToVectorDialog::getLayer()
{
  return 0;
}

std::vector<te::stat::StatisticalSummary> te::attributefill::VectorToVectorDialog::getSelectedStatistics()
{
  return std::vector<te::stat::StatisticalSummary>();
}

std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > te::attributefill::VectorToVectorDialog::getStatisticalSummary()
{
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > outputStatisticalSummary;

  QList<QListWidgetItem*> itemList = m_ui->m_statisticsListWidget->selectedItems();
  
  te::stat::StatisticalSummary enumStatisticalSummary;
  std::map<std::string, std::vector<te::stat::StatisticalSummary> >  propname_stat;
  std::map<std::string, std::vector<te::stat::StatisticalSummary> >::iterator it;
  
  for(int i = 0; i < itemList.size(); ++i)
  {
    if(itemList[i]->data(Qt::UserRole).toInt() == -1)
      continue;

    std::vector<std::string> tokens;
    std::string pname;
    std::string auxItem = itemList[i]->text().toStdString();

    boost::split(tokens, auxItem, boost::is_any_of(":"));
    boost::trim(tokens[0]);
    boost::trim(tokens[1]);
    if(tokens[0] != "")
    {
      pname = tokens[0];
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

te::dt::Property* te::attributefill::VectorToVectorDialog::getSelectedPropertyByName(std::string propertyName)
{
  te::dt::Property* selProperty;
  if(propertyName == "")
    return 0;

  te::map::AbstractLayerPtr fromLayer = getCurrentFromLayer();

  std::auto_ptr<te::da::DataSetType> fromSchema = fromLayer->getSchema();

  std::vector<te::dt::Property*> properties = fromSchema->getProperties();

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    if(propertyName == properties[i]->getName())
    {
      selProperty = properties[i];
      return selProperty;
    }
  }
  return 0;
}

void te::attributefill::VectorToVectorDialog::setOptions()
{
  m_ui->m_statisticsListWidget->clear();

  te::map::AbstractLayerPtr fromLayer = getCurrentFromLayer();

  std::auto_ptr<te::da::DataSetType> schema = fromLayer->getSchema();

  std::vector<te::dt::Property*> props = schema->getProperties();

  for(std::size_t i = 0; i < props.size(); ++i)
  {
    if(props[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      QListWidgetItem* itemValue = new QListWidgetItem(QString(props[i]->getName().c_str()) + " : " + "Value");
      itemValue->setData(Qt::UserRole, QVariant(-1));
      m_ui->m_statisticsListWidget->addItem(itemValue);

      setFunctionsByProperty(props[i]);
    }
  }

  int lastRow = m_ui->m_statisticsListWidget->count() - 1;
  delete m_ui->m_statisticsListWidget->item(lastRow);
}

void te::attributefill::VectorToVectorDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::attributefill::VectorToVectorDialog::onOkPushButtonClicked()
{
  te::map::DataSetLayerPtr fromLayer(dynamic_cast<te::map::DataSetLayer*>(getCurrentFromLayer().get()));

  if(!fromLayer)
  {
    // TODO
  }

  std::string fromDataSetName = fromLayer->getDataSetName();
  std::auto_ptr<te::da::DataSetType> fromSchema = fromLayer->getSchema();
  std::auto_ptr<te::da::DataSet> fromData = fromLayer->getData();
  std::size_t fromSpatialPropPos = te::da::GetFirstSpatialPropertyPos(fromData.get());
  te::da::DataSourcePtr fromSource = te::da::GetDataSource(fromLayer->getDataSourceId(), true);

  te::map::DataSetLayerPtr toLayer(dynamic_cast<te::map::DataSetLayer*>(getCurrentToLayer().get()));

  if(!toLayer)
  {
    // TODO
  }

  std::string toDataSetName = toLayer->getDataSetName();
  std::auto_ptr<te::da::DataSetType> toSchema = fromLayer->getSchema();
  std::auto_ptr<te::da::DataSet> toData = toLayer->getData();
  std::size_t toSpatialPropPos = te::da::GetFirstSpatialPropertyPos(toData.get());
  te::da::DataSourcePtr toSource = te::da::GetDataSource(toLayer->getDataSourceId(), true);

  std::auto_ptr<te::attributefill::VectorToVector> v2v(new te::attributefill::VectorToVector());

  v2v->setInput(fromLayer, toLayer);

  //Testes

  //v2v->setParams(getSelections(), getStatisticalSummary());
  //v2v->run();

  //return;

  //FIM testes

  std::string outDataSetName = m_ui->m_newLayerNameLineEdit->text().toStdString();
  te::da::DataSourcePtr outSource;

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("VectorToVector"), tr("Define a repository for the result."));
    return;
  }
       
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("VectorToVector"), tr("Define a name for the resulting layer."));
    return;
  }

  if(m_toFile)
  {
    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());
      
    if (boost::filesystem::exists(uri))
    {
      QMessageBox::information(this, tr("VectorToVector"), tr("Output file already exists. Remove it or select a new name and try again."));
      return;
    }
      
    std::size_t idx = outDataSetName.find(".");
    if (idx != std::string::npos)
      outDataSetName=outDataSetName.substr(0,idx);

    std::map<std::string, std::string> dsinfo;
    dsinfo["URI"] = uri.string();
      
    outSource.reset(te::da::DataSourceFactory::make("OGR").release());
    outSource->setConnectionInfo(dsinfo);
    outSource->open();
    if (outSource->dataSetExists(outDataSetName))
    {
      QMessageBox::information(this, tr("VectorToVector"), tr("There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again."));
      return;
    }

    v2v->setOutput(outSource, outDataSetName);

  }
  else
  {
    te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
    if (!aux)
    {
      QMessageBox::information(this, tr("VectorToVector"), tr("The selected output datasource can not be accessed."));
      return;
    }
      
    if (aux->dataSetExists(outDataSetName))
    {
      QMessageBox::information(this, tr("VectorToVector"), tr("Dataset already exists. Remove it or select a new name and try again."));
      return;
    }
    this->setCursor(Qt::WaitCursor);

    v2v->setOutput(aux, outDataSetName);

  }

  std::map<te::dt::Property*, std::vector<std::string> > selections = getSelections();

  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > summary = getStatisticalSummary();

  v2v->setParams(getSelections(), getStatisticalSummary());

  v2v->run();

  accept();
}

void te::attributefill::VectorToVectorDialog::onCancelPushButtonClicked()
{
  reject();
}

void te::attributefill::VectorToVectorDialog::onFromLayerComboBoxCurrentIndexChanged(int index)
{

  if(m_ui->m_toLayerComboBox->count() <= 1 || m_ui->m_fromLayerComboBox->count() <= 1 )
    return;

  int currentIndex = m_ui->m_fromLayerComboBox->currentIndex();

  if(currentIndex == m_ui->m_toLayerComboBox->currentIndex())
  {
    if(currentIndex == 0)
      m_ui->m_toLayerComboBox->setCurrentIndex(1);
    else
      m_ui->m_toLayerComboBox->setCurrentIndex(0);
  }

  setOptions();

  /*

  te::map::AbstractLayerPtr fromLayer = getCurrentFromLayer();

  std::auto_ptr<te::da::DataSetType> schema(fromLayer->getSchema());

  std::vector<te::dt::Property*> props = schema->getProperties();

  if(schema->size() == 0)
    return;

  setFunctionsByLayer(props);

  setFillCellOperations();*/
}

void te::attributefill::VectorToVectorDialog::onToLayerComboBoxCurrentIndexChanged(int index)
{
  if(m_ui->m_toLayerComboBox->count() <= 1 || m_ui->m_fromLayerComboBox->count() <= 1 )
    return;

  int currentIndex = m_ui->m_toLayerComboBox->currentIndex();
  
  if(m_ui->m_fromLayerComboBox->count() > 0 && currentIndex == m_ui->m_fromLayerComboBox->currentIndex())
  {
    if(currentIndex == 0)
      m_ui->m_fromLayerComboBox->setCurrentIndex(1);
    else
      m_ui->m_fromLayerComboBox->setCurrentIndex(0);
  }
}

void te::attributefill::VectorToVectorDialog::setFunctionsByProperty(te::dt::Property* prop)
{
  int propertyType = prop->getType();

  if(propertyType != te::dt::GEOMETRY_TYPE)
  {
    if(propertyType == te::dt::STRING_TYPE)
    {
      QListWidgetItem* item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MIN_VALUE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MIN_VALUE));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MAX_VALUE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MAX_VALUE));
      m_ui->m_statisticsListWidget->addItem(item);
        
      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::COUNT].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::COUNT));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VALID_COUNT].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::VALID_COUNT));
      m_ui->m_statisticsListWidget->addItem(item);

      //item = new QListWidgetItem("");
      //m_ui->m_statisticsListWidget->addItem(item);
    }
    else
    {
      QListWidgetItem* item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MIN_VALUE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MIN_VALUE));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MAX_VALUE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MAX_VALUE));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MEAN].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MEAN));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::SUM].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::SUM));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::COUNT].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::COUNT));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VALID_COUNT].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::VALID_COUNT));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::STANDARD_DEVIATION].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::STANDARD_DEVIATION));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VARIANCE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::VARIANCE));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::SKEWNESS].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::SKEWNESS));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::KURTOSIS].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::KURTOSIS));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::AMPLITUDE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::AMPLITUDE));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MEDIAN].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MEDIAN));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VAR_COEFF].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::VAR_COEFF));
      m_ui->m_statisticsListWidget->addItem(item);

      item = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MODE].c_str());
      item->setData(Qt::UserRole, QVariant(te::stat::MODE));
      m_ui->m_statisticsListWidget->addItem(item);

      //item = new QListWidgetItem("");
      //m_ui->m_statisticsListWidget->addItem(item);
    }
  }

  QListWidgetItem* item1 = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + "Major Class");
  item1->setData(Qt::UserRole, QVariant(-1));
  QListWidgetItem* item2 = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + "Percentage per Class");
  item2->setData(Qt::UserRole, QVariant(-1));
  QListWidgetItem* item3 = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + "Minimum Distance");
  item3->setData(Qt::UserRole, QVariant(-1));
  QListWidgetItem* item4 = new QListWidgetItem(QString(prop->getName().c_str()) + " : " + "Presence");
  item4->setData(Qt::UserRole, QVariant(-1));

  
  m_ui->m_statisticsListWidget->addItem(item1);
  m_ui->m_statisticsListWidget->addItem(item2);
  m_ui->m_statisticsListWidget->addItem(item3);
  m_ui->m_statisticsListWidget->addItem(item4);

  m_ui->m_statisticsListWidget->addItem("");

}

void te::attributefill::VectorToVectorDialog::setFunctionsByLayer(std::vector<te::dt::Property*> properties)
{
  int propertyType;

  m_ui->m_selectAllComboBox->setCurrentIndex(0);
  m_ui->m_rejectAllComboBox->setCurrentIndex(0);
  m_ui->m_statisticsListWidget->clear();

  te::map::AbstractLayerPtr fromLayer = getCurrentFromLayer();

  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(fromLayer.get());
  te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
  const te::da::DataSourceCapabilities dsCapabilities = dataSource->getCapabilities();

  if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
  {
    for(size_t i=0; i < properties.size(); ++i)
    {
      propertyType = properties[i]->getType();
      if(propertyType != te::dt::GEOMETRY_TYPE)
      {
        if(propertyType == te::dt::STRING_TYPE)
        {  
          QListWidgetItem* item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MIN_VALUE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MIN_VALUE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MAX_VALUE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MAX_VALUE));
          m_ui->m_statisticsListWidget->addItem(item);
        
          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::COUNT].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::COUNT));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VALID_COUNT].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::VALID_COUNT));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem("");
          m_ui->m_statisticsListWidget->addItem(item);
        }
        else
        {
          QListWidgetItem* item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MIN_VALUE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MIN_VALUE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MAX_VALUE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MAX_VALUE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MEAN].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MEAN));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::SUM].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::SUM));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::COUNT].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::COUNT));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VALID_COUNT].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::VALID_COUNT));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::STANDARD_DEVIATION].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::STANDARD_DEVIATION));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VARIANCE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::VARIANCE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::AMPLITUDE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::AMPLITUDE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem("");
          m_ui->m_statisticsListWidget->addItem(item);
        }
      }
    }
  }
  else
  {
    for(size_t i=0; i < properties.size(); ++i)
    {
      propertyType = properties[i]->getType();
      if(propertyType != te::dt::GEOMETRY_TYPE)
      {
        if(propertyType == te::dt::STRING_TYPE)
        {  
          QListWidgetItem* item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MIN_VALUE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MIN_VALUE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MAX_VALUE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MAX_VALUE));
          m_ui->m_statisticsListWidget->addItem(item);
        
          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::COUNT].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::COUNT));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VALID_COUNT].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::VALID_COUNT));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem("");
          m_ui->m_statisticsListWidget->addItem(item);
        }
        else
        {
          QListWidgetItem* item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MIN_VALUE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MIN_VALUE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MAX_VALUE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MAX_VALUE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MEAN].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MEAN));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::SUM].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::SUM));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::COUNT].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::COUNT));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VALID_COUNT].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::VALID_COUNT));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::STANDARD_DEVIATION].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::STANDARD_DEVIATION));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VARIANCE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::VARIANCE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::SKEWNESS].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::SKEWNESS));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::KURTOSIS].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::KURTOSIS));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::AMPLITUDE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::AMPLITUDE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MEDIAN].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MEDIAN));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::VAR_COEFF].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::VAR_COEFF));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem(QString(properties[i]->getName().c_str()) + " : " + m_statisticalSummaryMap[te::stat::MODE].c_str());
          item->setData(Qt::UserRole, QVariant(te::stat::MODE));
          m_ui->m_statisticsListWidget->addItem(item);

          item = new QListWidgetItem("");
          m_ui->m_statisticsListWidget->addItem(item);
        }
      }
    }
  }
  int lastRow = m_ui->m_statisticsListWidget->count() - 1;
  delete m_ui->m_statisticsListWidget->item(lastRow);
}

void te::attributefill::VectorToVectorDialog::setStatisticalSummary()
{
    m_ui->m_selectAllComboBox->addItem("");
    m_ui->m_selectAllComboBox->addItem("Value");
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
    m_ui->m_selectAllComboBox->addItem("Major Class");
    m_ui->m_selectAllComboBox->addItem("Percentage per Class");
    m_ui->m_selectAllComboBox->addItem("Minimum Distance");
    m_ui->m_selectAllComboBox->addItem("Presence");

    m_ui->m_rejectAllComboBox->addItem("");
    m_ui->m_rejectAllComboBox->addItem("Value");
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
    m_ui->m_rejectAllComboBox->addItem("Major Class");
    m_ui->m_rejectAllComboBox->addItem("Percentage per Class");
    m_ui->m_rejectAllComboBox->addItem("Minimum Distance");
    m_ui->m_rejectAllComboBox->addItem("Presence");
}

void te::attributefill::VectorToVectorDialog::setStatisticalSummaryMap()
{
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MIN_VALUE, te::stat::GetStatSummaryFullName(te::stat::MIN_VALUE)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MAX_VALUE, te::stat::GetStatSummaryFullName(te::stat::MAX_VALUE)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MEAN, te::stat::GetStatSummaryFullName(te::stat::MEAN)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::SUM, te::stat::GetStatSummaryFullName(te::stat::SUM)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::COUNT, te::stat::GetStatSummaryFullName(te::stat::COUNT)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::VALID_COUNT, te::stat::GetStatSummaryFullName(te::stat::VALID_COUNT)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::STANDARD_DEVIATION, te::stat::GetStatSummaryFullName(te::stat::STANDARD_DEVIATION)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::VARIANCE, te::stat::GetStatSummaryFullName(te::stat::VARIANCE)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::SKEWNESS, te::stat::GetStatSummaryFullName(te::stat::SKEWNESS)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::KURTOSIS, te::stat::GetStatSummaryFullName(te::stat::KURTOSIS)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::AMPLITUDE, te::stat::GetStatSummaryFullName(te::stat::AMPLITUDE)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MEDIAN, te::stat::GetStatSummaryFullName(te::stat::MEDIAN)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::VAR_COEFF, te::stat::GetStatSummaryFullName(te::stat::VAR_COEFF)));
  m_statisticalSummaryMap.insert(StaticalSummaryMap::value_type(te::stat::MODE, te::stat::GetStatSummaryFullName(te::stat::MODE)));
}

/*
void te::attributefill::VectorToVectorDialog::onOperationComboBoxCurrentIndexChanged(int index)
{
  m_ui->m_attributeComboBox->clear();

  te::map::AbstractLayerPtr fromLayer = getCurrentFromLayer();

  std::auto_ptr<te::da::DataSetType>schema = fromLayer->getSchema();

  std::vector<te::dt::Property*> props = schema->getProperties();

  for(std::size_t i = 0; i < props.size(); ++i)
  {
    std::string operation = m_ui->m_operationComboBox->currentText().toStdString();

    std::string propName = props[i]->getName();
    int propType = props[i]->getType();

    if(operation == "Major Class" || 
       operation == "Percentage per Class" ||
       operation == "Percentage of each Class by Area" )
    {
      if(propType == te::dt::STRING_TYPE ||
         propType == te::dt::INT16_TYPE ||
         propType == te::dt::INT32_TYPE ||
         propType == te::dt::INT64_TYPE)
      {
        m_ui->m_attributeComboBox->addItem(propName.c_str());
        m_ui->m_altAttributeComboBox->addItem(propName.c_str());
      }
    }
    else
    {
      if(propType == te::dt::DOUBLE_TYPE ||
         propType == te::dt::FLOAT_TYPE ||
         propType == te::dt::INT16_TYPE ||
         propType == te::dt::INT32_TYPE ||
         propType == te::dt::INT64_TYPE)
      {
        m_ui->m_attributeComboBox->addItem(propName.c_str());
        m_ui->m_altAttributeComboBox->addItem(propName.c_str());
      }
    }
  }
}
*/

void te::attributefill::VectorToVectorDialog::setFillCellOperations()
{
  /*
  m_ui->m_operationComboBox->clear();

  te::map::AbstractLayerPtr fromLayer = getCurrentFromLayer();

  std::auto_ptr<te::da::DataSetType> schema = fromLayer->getSchema();

  te::dt::Property* spatialProp = te::da::GetFirstSpatialProperty(schema.get());

  m_ui->m_operationComboBox->addItem("Minimum Value");// - POLYGONS | LINES | POINTS
  m_ui->m_operationComboBox->addItem("Maximum Value");
  m_ui->m_operationComboBox->addItem("Average Value");
  m_ui->m_operationComboBox->addItem("Sum of Values");
  m_ui->m_operationComboBox->addItem("Standard Deviation");
  m_ui->m_operationComboBox->addItem("Major Class");
  m_ui->m_operationComboBox->addItem("Percentage per Class");
  m_ui->m_operationComboBox->addItem("Minimum Distance");
  m_ui->m_operationComboBox->addItem("Presence");
  m_ui->m_operationComboBox->addItem("Count");

  if(spatialProp->getType() == te::gm::PolygonType)
  {
    m_ui->m_operationComboBox->addItem("Weighted by Area");// - TePOLYGONS
    m_ui->m_operationComboBox->addItem("Weighted Sum by Area");
    m_ui->m_operationComboBox->addItem("Percentage of each Class by Area");
    m_ui->m_operationComboBox->addItem("Percentage of Total Area");
  }
  */
}

te::map::AbstractLayerPtr te::attributefill::VectorToVectorDialog::getCurrentFromLayer()
{
  std::string layerID = m_ui->m_fromLayerComboBox->itemData(m_ui->m_fromLayerComboBox->currentIndex(), Qt::UserRole).toString().toStdString();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    if(it->get()->getId() == layerID)
      return *it;
    ++it;
  }

  return 0;
}

te::map::AbstractLayerPtr te::attributefill::VectorToVectorDialog::getCurrentToLayer()
{
  std::string layerID = m_ui->m_toLayerComboBox->itemData(m_ui->m_toLayerComboBox->currentIndex(), Qt::UserRole).toString().toStdString();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    if(it->get()->getId() == layerID)
      return *it;
    ++it;
  }

  return 0;
}

void te::attributefill::VectorToVectorDialog::onStatisticsListWidgetItemPressed(QListWidgetItem * item)
{
  if(item->text().isEmpty())
    item->setSelected(false);
}

void te::attributefill::VectorToVectorDialog::onSelectAllComboBoxChanged(int index)
{
  QString text = m_ui->m_selectAllComboBox->itemText(index);
  Qt::MatchFlags flag = Qt::MatchEndsWith; //The search term matches the end of the item.
  
  if(text.isEmpty())
    return;

  QList<QListWidgetItem *> listFound;
  listFound = m_ui->m_statisticsListWidget->findItems(text, flag);
  
  for(int i=0; i < listFound.size(); ++i)
  {
    std::vector<std::string> tokens;
    te::common::Tokenize(listFound.at(i)->text().toStdString(), tokens, ":");

    if(tokens.size() < 2)
      continue;

    QString token(tokens[1].c_str());

    if(token.trimmed() == text)
      listFound.at(i)->setSelected(true);
  }

  m_ui->m_rejectAllComboBox->setCurrentIndex(0);
}

void te::attributefill::VectorToVectorDialog::onRejectAllComboBoxChanged(int index)
{
  QString text = m_ui->m_selectAllComboBox->itemText(index);
  Qt::MatchFlags flag = Qt::MatchEndsWith; //The search term matches the end of the item.
  
  if(text=="")
    return;

  QList<QListWidgetItem *> listFound;
  listFound = m_ui->m_statisticsListWidget->findItems(text, flag);
  
  for(int i=0; i < listFound.size(); ++i)
  {
    std::vector<std::string> tokens;
    te::common::Tokenize(listFound.at(i)->text().toStdString(), tokens, ":");

    if(tokens.size() < 2)
      continue;

    QString token(tokens[1].c_str());

    if(token.trimmed() == text)
      listFound.at(i)->setSelected(false);
  }

  m_ui->m_selectAllComboBox->setCurrentIndex(0);
}

void te::attributefill::VectorToVectorDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;
  
  m_toFile = false;
}

void te::attributefill::VectorToVectorDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();
  
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                        QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;
  
  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());
  
  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

std::map<te::dt::Property*, std::vector<std::string> > te::attributefill::VectorToVectorDialog::getSelections()
{
  std::map<te::dt::Property*, std::vector<std::string> > result;

  std::auto_ptr<te::da::DataSetType> fromScheme = getCurrentFromLayer()->getSchema();

  std::vector<std::string> auxSelecteds;
  for(int i = 0; i < m_ui->m_statisticsListWidget->count(); ++i)
  {
    QListWidgetItem* item = m_ui->m_statisticsListWidget->item(i);

    if(!item->isSelected())
      continue;

    auxSelecteds.push_back(item->text().toStdString());
  }

  std::string auxPropName = "";
  std::vector<std::string> auxFuncsNames;
  for(std::size_t i = 0; i < auxSelecteds.size(); ++i)
  {
    std::vector<std::string> tokens;
    te::common::Tokenize(auxSelecteds[i], tokens, ":");

    std::string propName = tokens[0];
    std::string funcName = tokens[1];

    boost::trim(propName);
    boost::trim(funcName);

    if(auxPropName.empty())
      auxPropName = propName;

    if(propName != auxPropName)
    {
      result[fromScheme->getProperty(auxPropName)->clone()] = auxFuncsNames;
      auxFuncsNames.clear();
      auxPropName = propName;
      auxFuncsNames.push_back(funcName);

      if(i == auxSelecteds.size()-1)
        result[fromScheme->getProperty(auxPropName)->clone()] = auxFuncsNames;
    }
    else
    {
      auxFuncsNames.push_back(funcName);

      if(i == auxSelecteds.size()-1)
        result[fromScheme->getProperty(auxPropName)->clone()] = auxFuncsNames;
    }
  }

  return result;
}
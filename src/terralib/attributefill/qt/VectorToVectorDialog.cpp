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
#include "../../attributefill/VectorToVectorOp.h"
#include "../../attributefill/VectorToVectorMemory.h"
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
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/af/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
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
    onFromLayerComboBoxCurrentIndexChanged(0);
    m_ui->m_toLayerComboBox->setCurrentIndex(1);
  }
}

te::map::AbstractLayerPtr te::attributefill::VectorToVectorDialog::getLayer()
{
  return 0;
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

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  std::auto_ptr<te::attributefill::VectorToVectorOp> v2v;

//  if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
//  {
//    v2v.reset(new te::attributefill::VectorToVectorQuery());
//  }
//  else
//  {
    v2v.reset(new te::attributefill::VectorToVectorMemory());
//  }

  v2v->setInput(fromLayer, toLayer);

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

  v2v->setParams(getSelections());

  try
  {
    v2v->run();
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::warning(this, tr("Vector To Vector"), e.what());
    te::common::ProgressManager::getInstance().removeViewer(id);
    reject();
  }
  catch(std::exception& e)
  {
    QMessageBox::warning(this, tr("Vector To Vector"), e.what());
    te::common::ProgressManager::getInstance().removeViewer(id);
    reject();
  }

  te::common::ProgressManager::getInstance().removeViewer(id);
  this->setCursor(Qt::ArrowCursor);

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

  setFunctionsByLayer(getCurrentFromLayer());
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

  onFromLayerComboBoxCurrentIndexChanged(m_ui->m_fromLayerComboBox->currentIndex());
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
    m_ui->m_selectAllComboBox->addItem("Class with highest occurrence");
    m_ui->m_selectAllComboBox->addItem("Class with larger intersection area");
    m_ui->m_selectAllComboBox->addItem("Percentage per Class");
    m_ui->m_selectAllComboBox->addItem("Minimum Distance");
    m_ui->m_selectAllComboBox->addItem("Presence");
    m_ui->m_selectAllComboBox->addItem("Weighted by Area");
    m_ui->m_selectAllComboBox->addItem("Weighted Sum by Area");
    m_ui->m_selectAllComboBox->addItem("Percentage of each Class by Area");
    m_ui->m_selectAllComboBox->addItem("Percentage of Total Area");

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
    m_ui->m_rejectAllComboBox->addItem("Class with highest occurrence");
    m_ui->m_rejectAllComboBox->addItem("Class with larger intersection area");
    m_ui->m_rejectAllComboBox->addItem("Percentage per Class");
    m_ui->m_rejectAllComboBox->addItem("Minimum Distance");
    m_ui->m_rejectAllComboBox->addItem("Presence");
    m_ui->m_rejectAllComboBox->addItem("Weighted by Area");
    m_ui->m_rejectAllComboBox->addItem("Weighted Sum by Area");
    m_ui->m_rejectAllComboBox->addItem("Percentage of each Class by Area");
    m_ui->m_rejectAllComboBox->addItem("Percentage of Total Area");
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

void te::attributefill::VectorToVectorDialog::setFunctionsByLayer(te::map::AbstractLayerPtr layer)
{
  m_ui->m_selectAllComboBox->setCurrentIndex(0);
  m_ui->m_rejectAllComboBox->setCurrentIndex(0);
  m_ui->m_statisticsListWidget->clear();

  te::gm::GeomType toGeomType = getCurrentToLayerGeomType();

  std::auto_ptr<te::da::DataSetType> dst = layer->getSchema();
  std::auto_ptr<te::da::DataSet> ds = layer->getData();

  te::gm::GeometryProperty* geomProp = 0;

  if(dst->hasGeom())
  {
    std::string geomPropName = layer->getGeomPropertyName();

    if(geomPropName.empty())
      geomProp = te::da::GetFirstGeomProperty(dst.get());
    else
      geomProp = dynamic_cast<te::gm::GeometryProperty*>(dst->getProperty(geomPropName));
  }

  te::gm::GeomType geomType = geomProp->getGeometryType();

  std::vector<te::dt::Property*> props = dst->getProperties();

  for(std::size_t i = 0; i < props.size(); ++i)
  {
    te::dt::Property* prop = props[i];
    
    if(isValidPropertyType(prop->getType()))
    {
      int propertyType = prop->getType();

      m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Value");

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
      }

      if(isClassType(prop->getType()))
        m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Class with highest occurrence");

      // This function works only with polygon to polygon
      if(isClassType(prop->getType()) && isPolygon(geomType) && isPolygon(toGeomType))
        m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Class with larger intersection area");

      m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Percentage per Class");
      m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Minimum Distance");
      m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Presence");

      if(isPolygon(geomType))
      {
        m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Weighted by Area");
        m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Weighted Sum by Area");
        m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Percentage of each Class by Area");
        m_ui->m_statisticsListWidget->addItem(QString(props[i]->getName().c_str()) + " : " + "Percentage of Total Area");
      }
    }

    m_ui->m_statisticsListWidget->addItem("");
  }

  int lastRow = m_ui->m_statisticsListWidget->count() - 1;
  delete m_ui->m_statisticsListWidget->item(lastRow);
}

bool te::attributefill::VectorToVectorDialog::isPolygon(te::gm::GeomType type)
{
  if(type == te::gm::PolygonType ||
     type == te::gm::PolygonZType ||
     type == te::gm::PolygonMType ||
     type == te::gm::PolygonZMType ||
     type == te::gm::MultiPolygonType ||
     type == te::gm::MultiPolygonZType ||
     type == te::gm::MultiPolygonMType ||
     type == te::gm::MultiPolygonZMType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorDialog::isPoint(te::gm::GeomType type)
{
  if(type == te::gm::PointType ||
     type == te::gm::PointZType ||
     type == te::gm::PointMType ||
     type == te::gm::PointZMType ||
     type == te::gm::MultiPointType ||
     type == te::gm::MultiPointZType ||
     type == te::gm::MultiPointMType ||
     type == te::gm::MultiPointZMType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorDialog::isValidPropertyType(const int type)
{
  if(type == te::dt::CHAR_TYPE ||
     type == te::dt::UCHAR_TYPE ||
     type == te::dt::INT16_TYPE ||
     type == te::dt::UINT16_TYPE ||
     type == te::dt::INT32_TYPE ||
     type == te::dt::UINT32_TYPE ||
     type == te::dt::INT64_TYPE ||
     type == te::dt::UINT64_TYPE ||
     type == te::dt::FLOAT_TYPE ||
     type == te::dt::DOUBLE_TYPE ||
     type == te::dt::NUMERIC_TYPE ||
     type == te::dt::STRING_TYPE ||
     type == te::dt::CINT16_TYPE ||
     type == te::dt::CINT32_TYPE ||
     type == te::dt::CFLOAT_TYPE ||
     type == te::dt::CDOUBLE_TYPE)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorDialog::isClassType(const int type)
{
  if(type == te::dt::INT16_TYPE ||
     type == te::dt::UINT16_TYPE ||
     type == te::dt::INT32_TYPE ||
     type == te::dt::UINT32_TYPE ||
     type == te::dt::INT64_TYPE ||
     type == te::dt::UINT64_TYPE ||
     type == te::dt::STRING_TYPE)
  {
     return true;
  }

  return false;
}

te::gm::GeomType te::attributefill::VectorToVectorDialog::getCurrentToLayerGeomType()
{
  te::map::AbstractLayerPtr toLayer = getCurrentToLayer();
  std::auto_ptr<te::da::DataSetType> toSchema = toLayer->getSchema();
  te::dt::Property* p = te::da::GetFirstSpatialProperty(toSchema.get());
  te::gm::GeometryProperty* toGeomProp = dynamic_cast<te::gm::GeometryProperty*>(p);
  return toGeomProp->getGeometryType();
}
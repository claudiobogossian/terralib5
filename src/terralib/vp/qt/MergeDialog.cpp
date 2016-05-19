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
  \file terralib/vp/MergeDialog.cpp

  \brief A dialog for aggregation operation
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSetAdapter.h"
#include "../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"

#include "../../vp/Merge.h"

#include "MergeDialog.h"
#include "ui_MergeDialogForm.h"

#include "../../vp/AlgorithmParams.h"
#include "../../vp/ComplexData.h"
#include "../../vp/InputParams.h"

// Qt
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>

// Boost
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::vp::MergeDialog::MergeDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::MergeDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_toFile(false)
{
// add controls
  m_ui->setupUi(this);

  connect(m_ui->m_firstLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFirstLayerComboBoxChanged(int)));
  connect(m_ui->m_secondLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSecondLayerComboBoxChanged(int)));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));
  connect(m_ui->m_outputGroupBox, SIGNAL(toggled(bool)), this, SLOT(onOutputGroupBoxToggled(bool)));

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vp-merge-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/vp/vp_aggregation.html");

  m_outputDatasource = te::da::DataSourceInfoPtr();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
}

te::vp::MergeDialog::~MergeDialog()
{
}

void te::vp::MergeDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while (it != layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if (dsType->hasGeom())
    {
      m_layers.push_back(*it);
    }

    ++it;
  }

  updateFirstLayerComboBox();

  updateSecondLayerComboBox();

  updateAttrTableWidget();
}

void te::vp::MergeDialog::updateFirstLayerComboBox()
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  disconnect(m_ui->m_firstLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFirstLayerComboBoxChanged(int)));

  while (it != m_layers.end())
  {
    m_ui->m_firstLayerComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant::fromValue(*it));
    ++it;
  }

  connect(m_ui->m_firstLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFirstLayerComboBoxChanged(int)));

  QVariant varLayer = m_ui->m_firstLayerComboBox->itemData(m_ui->m_firstLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_firstSelectedLayer = layer;
}

void te::vp::MergeDialog::updateSecondLayerComboBox()
{
  int currIndex = m_ui->m_firstLayerComboBox->currentIndex();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  disconnect(m_ui->m_secondLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSecondLayerComboBoxChanged(int)));

  while (it != m_layers.end())
  {
    m_ui->m_secondLayerComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant::fromValue(*it));
    ++it;
  }

  m_ui->m_secondLayerComboBox->removeItem(currIndex);

  connect(m_ui->m_secondLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSecondLayerComboBoxChanged(int)));

  QVariant varLayer = m_ui->m_secondLayerComboBox->itemData(m_ui->m_secondLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_secondSelectedLayer = layer;
}

te::map::AbstractLayerPtr te::vp::MergeDialog::getLayer()
{
  return m_outputLayer;
}

void te::vp::MergeDialog::onTargetDatasourceToolButtonPressed()
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

void te::vp::MergeDialog::onTargetFileToolButtonPressed()
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

void te::vp::MergeDialog::onOkPushButtonClicked()
{
  bool isUpdate = !m_ui->m_outputGroupBox->isChecked();

  if (m_ui->m_firstLayerComboBox->currentText().isEmpty() || m_ui->m_secondLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::information(this, tr("Merge"), tr("It is necessary at least two layer to operat!"));
    return;
  }

  // Checking consistency of output paramenters
  if (!isUpdate && m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Merge"), tr("Select a repository for the resulting layer!"));

    return;
  }

  if (!isUpdate && m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Merge"), tr("Define a name for the resulting layer!"));
    return;
  }

  int firstSrid = m_firstSelectedLayer->getSRID();
  int secondSrid = m_secondSelectedLayer->getSRID();

  if (firstSrid <= 0 || secondSrid <= 0)
  {
    QMessageBox::information(this, tr("Merge"), tr("All layers must have SRID!"));
    return;
  }

  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

  std::string firstSourceId = m_firstSelectedLayer->getDataSourceId();
  std::string secondSourceId = m_secondSelectedLayer->getDataSourceId();

  te::da::DataSourcePtr firstSource = te::da::DataSourceManager::getInstance().find(firstSourceId);
  te::da::DataSourcePtr secondSource = te::da::DataSourceManager::getInstance().find(secondSourceId);

  std::auto_ptr<te::da::DataSetTypeConverter> firstConverter(new te::da::DataSetTypeConverter(m_firstSelectedLayer->getSchema().get(), firstSource->getCapabilities(), firstSource->getEncoding()));
  te::da::AssociateDataSetTypeConverterSRID(firstConverter.get(), m_firstSelectedLayer->getSRID());

  std::auto_ptr<te::da::DataSetTypeConverter> secondConverter(new te::da::DataSetTypeConverter(m_secondSelectedLayer->getSchema().get(), secondSource->getCapabilities(), secondSource->getEncoding()));
  te::da::AssociateDataSetTypeConverterSRID(secondConverter.get(), m_secondSelectedLayer->getSRID(), m_firstSelectedLayer->getSRID());

  te::da::DataSetType* firstDst(firstConverter->getResult());
  te::da::DataSetType* secondDst(secondConverter->getResult());

  te::da::DataSetAdapter* firstAdapter = te::da::CreateAdapter(m_firstSelectedLayer->getData().release(), firstConverter.get());
  te::da::DataSetAdapter* secondAdapter = te::da::CreateAdapter(m_secondSelectedLayer->getData().release(), secondConverter.get());

  std::auto_ptr<te::da::DataSet> firstDs(firstAdapter);
  std::auto_ptr<te::da::DataSet> secondDs(secondAdapter);
  
  try
  {

    te::vp::InputParams inputParam;
    inputParam.m_inputDataSet = firstDs.get();
    inputParam.m_inputDataSetType = firstDst;
    inputParam.m_inputDataSource = firstSource;

    te::vp::InputParams mergeParam;
    mergeParam.m_inputDataSet = secondDs.get();
    mergeParam.m_inputDataSetType = secondDst;
    mergeParam.m_inputDataSource = secondSource;

    std::vector<te::vp::InputParams> params;
    params.push_back(inputParam);
    params.push_back(mergeParam);

    te::vp::ComplexData<std::vector<std::pair<std::string, std::string> > >* attrs = new te::vp::ComplexData<std::vector<std::pair<std::string, std::string> > >(getTablePropertiesNames());

    te::vp::ComplexData<bool>* isUp = new te::vp::ComplexData<bool>(isUpdate);

    std::map<std::string, te::dt::AbstractData*> specificParams;
    specificParams["ATTRIBUTES"] = attrs;
    specificParams["ISUPDATE"] = isUp;

    te::vp::AlgorithmParams* aParams = new te::vp::AlgorithmParams();
    aParams->setInputParams(params);
    aParams->setOutputDataSetName(outputdataset);
    aParams->setSpecificParams(specificParams);

    te::da::DataSourcePtr auxSource;
    std::map<std::string, std::string> ogrDsinfo;
    boost::filesystem::path ogrUri;

    if (m_toFile)
    {
      ogrUri = m_ui->m_repositoryLineEdit->text().toStdString();

      if (boost::filesystem::exists(ogrUri))
      {
        QMessageBox::information(this, tr("Merge"), tr("Output file already exists. Remove it or select a new name and try again."));
        return;
      }

      std::size_t idx = outputdataset.find(".");
      if (idx != std::string::npos)
        outputdataset = outputdataset.substr(0, idx);

      ogrDsinfo["URI"] = ogrUri.string();

      auxSource.reset(te::da::DataSourceFactory::make("OGR").release());
      auxSource->setConnectionInfo(ogrDsinfo);
      auxSource->open();

      if (auxSource->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, tr("Merge"), tr("There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again."));
        return;
      }
    }
    else
    {
      if (!isUpdate)
      {
        auxSource = te::da::GetDataSource(m_outputDatasource->getId());
        if (!auxSource)
        {
          QMessageBox::information(this, tr("Merge"), tr("The selected output datasource can not be accessed."));
          return;
        }
        if (auxSource->dataSetExists(outputdataset))
        {
          QMessageBox::information(this, tr("Merge"), tr("Dataset already exists. Remove it or select a new name and try again."));
          return;
        }
      }
    }

  
    if (!isUpdate)
    {
      aParams->setOutputDataSource(auxSource);
    }
    else
    {
      aParams->setOutputDataSource(firstSource);
    }

    te::vp::Merge merge;
    merge.executeMemory(aParams);

    delete aParams;

    if (m_toFile)
    {
      // let's include the new datasource in the managers
      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string id = boost::uuids::to_string(u);

      te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
      ds->setConnInfo(ogrDsinfo);
      ds->setTitle(ogrUri.stem().string());
      ds->setAccessDriver("OGR");
      ds->setType("OGR");
      ds->setDescription(ogrUri.string());
      ds->setId(id);

      te::da::DataSourcePtr newds = te::da::DataSourceManager::getInstance().get(id, "OGR", ds->getConnInfo());
      newds->open();
      te::da::DataSourceInfoManager::getInstance().add(ds);
      m_outputDatasource = ds;
    }

    if (!isUpdate)
    {
      // creating a layer for the result
      te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());

      te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());

      te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
      m_outputLayer = converter(dt);
    }
    else
    {
      te::gm::Envelope env1 = m_firstSelectedLayer->getExtent();
      te::gm::Envelope env2 = m_secondSelectedLayer->getExtent();

      env1.Union(env2);

      m_firstSelectedLayer->setExtent(env1);
    }

  }
  catch (const te::common::Exception& e)
  {
    QMessageBox::warning(this, tr("Merge"), e.what());
    return;
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, tr("Merge"), e.what());
    return;
  }

  accept();
}

void te::vp::MergeDialog::onCancelPushButtonClicked()
{
  reject();
}

void te::vp::MergeDialog::onFirstLayerComboBoxChanged(int index)
{
  QVariant varLayer = m_ui->m_firstLayerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_ui->m_secondLayerComboBox->clear();

  m_firstSelectedLayer = layer;

  updateSecondLayerComboBox();

  updateAttrTableWidget();

  // The update only works with datasetlayer
  if (m_firstSelectedLayer->getType() != "DATASETLAYER")
  {
    m_ui->m_outputGroupBox->setChecked(true);
  }
}

void te::vp::MergeDialog::onSecondLayerComboBoxChanged(int index)
{
  QVariant varLayer = m_ui->m_secondLayerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_secondSelectedLayer = layer;

  updateAttrTableWidget();
}

void te::vp::MergeDialog::updateAttrTableWidget()
{
  m_ui->m_attributeTableWidget->setRowCount(0);

  std::auto_ptr<te::da::DataSetType> firstSchema;
  std::auto_ptr<te::da::DataSetType> secondSchema;

  if (m_firstSelectedLayer)
    firstSchema = m_firstSelectedLayer->getSchema();
  else
    return;

  if (m_secondSelectedLayer)
    secondSchema = m_secondSelectedLayer->getSchema();
  else
    return;

  std::vector<te::dt::Property*> secondProps = secondSchema->getProperties();

  std::vector<te::dt::Property*> firstProps = firstSchema->getProperties();

  int rowCount = -1;
  for (std::size_t i = 0; i < firstProps.size(); ++i)
  {
    if (firstProps[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      ++rowCount;
      m_ui->m_attributeTableWidget->insertRow(rowCount);

      QTableWidgetItem* item = new QTableWidgetItem(firstProps[i]->getName().c_str());
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_attributeTableWidget->setItem(rowCount, 0, item);

      QComboBox* cb = getPossibleAttributesComboBox(firstProps[i], secondProps);
      cb->setParent(m_ui->m_attributeTableWidget);

      m_ui->m_attributeTableWidget->setCellWidget(rowCount, 1, cb);
    }
  }

  std::size_t fSize = firstProps.size();
  std::size_t sSize = secondProps.size();
  if (firstProps.size() < secondProps.size())
  {
    int diference = (int)sSize - (int)fSize;

    for (std::size_t i = 0; i < diference; ++i)
    {
      ++rowCount;
      m_ui->m_attributeTableWidget->insertRow(rowCount);

      QTableWidgetItem* item = new QTableWidgetItem();
      m_ui->m_attributeTableWidget->setItem(rowCount, 0, item);

      QComboBox* cb = getAllAttributesComboBox(secondProps);
      cb->setParent(m_ui->m_attributeTableWidget);

      m_ui->m_attributeTableWidget->setCellWidget(rowCount, 1, cb);
    }
  }

  m_ui->m_attributeTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QComboBox* te::vp::MergeDialog::getPossibleAttributesComboBox(te::dt::Property* mainProp, std::vector<te::dt::Property*> props)
{
  QComboBox* result = new QComboBox();
  result->addItem("");

  int sameName = -1;
  int count = 1;

  for (std::size_t i = 0; i < props.size(); ++i)
  {
    if (mainProp->getType() == props[i]->getType())
    {
      if (te::common::Convert2LCase(mainProp->getName()) == te::common::Convert2LCase(props[i]->getName()))
      {
        sameName = count;
      }

      result->addItem(props[i]->getName().c_str());
      ++count;
    }
  }

  if (sameName > -1)
  {
    result->setCurrentIndex(sameName);
  }

  return result;
}

QComboBox* te::vp::MergeDialog::getAllAttributesComboBox(std::vector<te::dt::Property*> props)
{
  QComboBox* result = new QComboBox();
  result->addItem("");

  for (std::size_t i = 0; i < props.size(); ++i)
  {
    if (props[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      result->addItem(props[i]->getName().c_str());
    }
  }

  return result;
}

std::vector<std::pair<std::string, std::string> > te::vp::MergeDialog::getTablePropertiesNames()
{
  std::vector<std::pair<std::string, std::string> > result;

  int rowCount = m_ui->m_attributeTableWidget->rowCount();

  for (std::size_t i = 0; i < rowCount; ++i)
  {
    std::string fp = m_ui->m_attributeTableWidget->item((int)i, 0)->text().toStdString();;

    QComboBox* cb = dynamic_cast<QComboBox*>(m_ui->m_attributeTableWidget->cellWidget((int)i, 1));

    std::string sp = cb->currentText().toStdString();

    result.push_back(std::pair<std::string, std::string>(fp, sp));
  }

  return result;
}

void te::vp::MergeDialog::onOutputGroupBoxToggled(bool on)
{
  if (!on)
  {
    if (m_firstSelectedLayer->getType() != "DATASETLAYER")
    {
      QMessageBox::warning(this, tr("Merge"), tr("To use first layer as output (update), it must be a DataSet Layer!"));
      m_ui->m_outputGroupBox->setChecked(true);
      return;
    }
  }
}
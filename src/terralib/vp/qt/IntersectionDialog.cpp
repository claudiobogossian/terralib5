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
  \file terralib/vp/qt/IntersectionDialog.cpp

  \brief A dialog for intersection operation
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/query/And.h"
#include "../../dataaccess/query/Expression.h"
#include "../../dataaccess/query/In.h"
#include "../../dataaccess/query/Where.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Property.h"
#include "../../maptools/QueryLayer.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "../../srs/Config.h"
#include "../Exception.h"
#include "../IntersectionMemory.h"
#include "../IntersectionOp.h"
#include "../IntersectionQuery.h"
#include "IntersectionDialog.h"
//#include "LayerTreeModel.h"
#include "ui_IntersectionDialogForm.h"
#include "Utils.h"

// Qt
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QTreeWidget>

// BOOST
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::vp::IntersectionDialog::IntersectionDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::IntersectionDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>())
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vp-intersection-hint").pixmap(48,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  //add double list widget to this form
  m_doubleListWidget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_attrSelectionGroupBox));
  m_doubleListWidget->setLeftLabel("");
  m_doubleListWidget->setRightLabel("");

  QGridLayout* layout = new QGridLayout(m_ui->m_attrSelectionGroupBox);
  layout->addWidget(m_doubleListWidget.get());
  layout->setContentsMargins(0, 0, 0, 0);

  connect(m_ui->m_firstLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFirstLayerComboBoxChanged(int)));
  connect(m_ui->m_secondLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSecondLayerComboBoxChanged(int)));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/vp/vp_intersection.html");
}

te::vp::IntersectionDialog::~IntersectionDialog()
{
}

void te::vp::IntersectionDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
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

  updateDoubleListWidget();
}

void te::vp::IntersectionDialog::updateFirstLayerComboBox()
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

void te::vp::IntersectionDialog::updateSecondLayerComboBox()
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

te::map::AbstractLayerPtr te::vp::IntersectionDialog::getLayer()
{
  return m_layerResult;
}

void te::vp::IntersectionDialog::onFirstLayerComboBoxChanged(int index)
{
  QVariant varLayer = m_ui->m_firstLayerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_ui->m_secondLayerComboBox->clear();
  
  m_firstSelectedLayer = layer;

  updateSecondLayerComboBox();

  updateDoubleListWidget();
}

void te::vp::IntersectionDialog::onSecondLayerComboBoxChanged(int index)
{
  QVariant varLayer = m_ui->m_secondLayerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_secondSelectedLayer = layer;

  updateDoubleListWidget();
}

void te::vp::IntersectionDialog::updateDoubleListWidget()
{
  std::vector<std::string> inputValues;

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

  std::vector<te::dt::Property*> firstProps = firstSchema->getProperties();
  for (std::size_t i = 0; i < firstProps.size(); ++i)
  {
    if (firstProps[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      std::string name = firstSchema->getTitle();

      if (name.empty())
        name = firstSchema->getName();

      inputValues.push_back(name + ": " + firstProps[i]->getName());
    }
  }

  std::vector<te::dt::Property*> secondProps = secondSchema->getProperties();
  for (std::size_t i = 0; i < secondProps.size(); ++i)
  {
    if (secondProps[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      std::string name = secondSchema->getTitle();

      if (name.empty())
        name = secondSchema->getName();

      inputValues.push_back(secondSchema->getTitle() + ": " + secondProps[i]->getName());
    }
  }

  m_doubleListWidget->setInputValues(inputValues);
}

std::vector<std::pair<std::string, std::string> > te::vp::IntersectionDialog::getSelectedProperties()
{
  std::vector<std::string> outVec = m_doubleListWidget->getOutputValues();
  std::vector<std::pair<std::string, std::string> > result;

  for (std::size_t i = 0; i < outVec.size(); ++i)
  {
    std::vector<std::string> tok;
    te::common::Tokenize(outVec[i], tok, ": ");

    std::pair<std::string, std::string> p;
    p.first = tok[0];
    p.second = tok[1];

    result.push_back(p);
  }

  return result;
}

void te::vp::IntersectionDialog::onOkPushButtonClicked()
{
  if(m_ui->m_firstLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Intersection"), TE_TR("Select a first input layer."));
    return;
  }

  const te::da::ObjectIdSet* firstOidSet = 0;
  if(m_ui->m_firstSelectedCheckBox->isChecked())
  {
    firstOidSet = m_firstSelectedLayer->getSelected();
    if(!firstOidSet)
    {
      QMessageBox::information(this, "Intersection", "Select the layer objects to perform the intersection operation.");
      return;
    }
  }

  te::da::DataSourcePtr firstDataSource = te::da::GetDataSource(m_firstSelectedLayer->getDataSourceId(), true);
  if (!firstDataSource.get())
  {
    QMessageBox::information(this, "Intersection", "The selected first input data source can not be accessed.");
    return;
  }

  if(m_ui->m_secondLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Intersection"), TE_TR("Select a second input layer."));
    return;
  }

  const te::da::ObjectIdSet* secondOidSet = 0;
  if(m_ui->m_secondSelectedCheckBox->isChecked())
  {
    secondOidSet = m_secondSelectedLayer->getSelected();
    if(!secondOidSet)
    {
      QMessageBox::information(this, "Intersection", "Select the layer objects to perform the intersection operation.");
      return;
    }
  }

  te::da::DataSourcePtr secondDataSource = te::da::GetDataSource(m_secondSelectedLayer->getDataSourceId(), true);
  if (!secondDataSource.get())
  {
    QMessageBox::information(this, "Intersection", "The selected second input data source can not be accessed.");
    return;
  }

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Intersection"), TE_TR("Select a repository for the resulting layer."));
    return;
  }
  
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Intersection"), TE_TR("Define a name for the resulting layer."));
    return;
  }
  
  if ((m_firstSelectedLayer->getSRID() == TE_UNKNOWN_SRS && m_secondSelectedLayer->getSRID() != TE_UNKNOWN_SRS) ||
      (m_firstSelectedLayer->getSRID() != TE_UNKNOWN_SRS && m_secondSelectedLayer->getSRID() == TE_UNKNOWN_SRS))
  {
    int ret = QMessageBox::question(this, "Intersection", "The two layers have incompatible SRS. The result might be incorrect. Do you wish to continue?", QMessageBox::No, QMessageBox::Yes);
    if (ret == QMessageBox::No)
      return;
  }

  te::map::DataSetLayer* firstDataSetLayer = 0;
  te::map::QueryLayer* firstQueryLayer = 0;

  te::map::DataSetLayer* secondDataSetLayer = 0;
  te::map::QueryLayer* secondQueryLayer = 0;

  if (m_firstSelectedLayer->getType() == "DATASETLAYER")
  {
    firstDataSetLayer = dynamic_cast<te::map::DataSetLayer*>(m_firstSelectedLayer.get());
  }
  else if (m_firstSelectedLayer->getType() == "QUERYLAYER")
  {
    firstQueryLayer = dynamic_cast<te::map::QueryLayer*>(m_firstSelectedLayer.get());
  }
  else
  {
    QMessageBox::information(this, "Intersection", "Can not execute this operation on this type of first layer.");
    return;
  }

  if (m_secondSelectedLayer->getType() == "DATASETLAYER")
  {
    secondDataSetLayer = dynamic_cast<te::map::DataSetLayer*>(m_secondSelectedLayer.get());
  }
  else if (m_secondSelectedLayer->getType() == "QUERYLAYER")
  {
    secondQueryLayer = dynamic_cast<te::map::QueryLayer*>(m_secondSelectedLayer.get());
  }
  else
  {
    QMessageBox::information(this, "Intersection", "Can not execute this operation on this type of second layer.");
    return;
  }

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);
  
  try
  {
    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

    bool res;
    if (m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());
      
      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "Intersection", "Output file already exists. Remove it and try again. ");
        return;
      }

      std::size_t idx = outputdataset.find(".");
      if(idx != std::string::npos)
        outputdataset = outputdataset.substr(0, idx);

      std::map<std::string, std::string> dsinfo;
      dsinfo["URI"] = uri.string();

      te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
      dsOGR->setConnectionInfo(dsinfo);
      dsOGR->open();
      if(dsOGR->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Intersection", "Output file already exists. Remove it or select a new name and try again.");
        return;
      }
      
      std::auto_ptr<te::da::DataSetTypeConverter> firstConverter(new te::da::DataSetTypeConverter(m_firstSelectedLayer->getSchema().get(), dsOGR->getCapabilities(), dsOGR->getEncoding()));
      te::da::AssociateDataSetTypeConverterSRID(firstConverter.get(), m_firstSelectedLayer->getSRID());

      std::auto_ptr<te::da::DataSetTypeConverter> secondConverter(new te::da::DataSetTypeConverter(m_secondSelectedLayer->getSchema().get(), dsOGR->getCapabilities(), dsOGR->getEncoding()));
      te::da::AssociateDataSetTypeConverterSRID(secondConverter.get(), m_firstSelectedLayer->getSRID());

      this->setCursor(Qt::WaitCursor);

      te::vp::IntersectionOp* intersectionOp = 0;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities firstDSCapabilities = firstDataSource->getCapabilities();
      const te::da::DataSourceCapabilities secondDSCapabilities = secondDataSource->getCapabilities();

      if( (firstDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
          secondDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() ) && 
          (firstDataSource->getId() == secondDataSource->getId()) && (firstDataSetLayer && secondDataSetLayer))
      {
        intersectionOp = new te::vp::IntersectionQuery();
      }
      else
      {
        intersectionOp = new te::vp::IntersectionMemory();
      }

      /*intersectionOp->setInput( firstDataSource, firstDataSetLayer->getDataSetName(), firstConverter,
                                secondDataSource, secondDataSetLayer->getDataSetName(), secondConverter,
                                firstOidSet, secondOidSet);*/

      std::string firstName;
      std::string secondName;

      if (firstDataSetLayer)
      {
        firstName = firstDataSetLayer->getDataSetName();
      }
      else if (firstQueryLayer)
      {
        firstName = firstQueryLayer->getTitle();
        intersectionOp->setIsFirstQuery();
      }

      if (secondDataSetLayer)
      {
        secondName = secondDataSetLayer->getDataSetName();
      }
      else if (secondQueryLayer)
      {
        secondName = secondQueryLayer->getTitle();
        intersectionOp->setIsSecondQuery();
      }

      if (firstQueryLayer && (firstOidSet != 0))
      {
        te::da::Select* query = firstQueryLayer->getQuery();
        te::da::Where* where = query->getWhere();

        te::da::Expression* originalExp = where->getExp();

        te::da::Expression* in = firstOidSet->getExpressionByInClause();

        te::da::Expression* cloneWhere = originalExp->clone();

        te::da::And* newAnd = new te::da::And(cloneWhere, in);

        te::da::Where* newWhere = new te::da::Where(newAnd);

        query->setWhere(newWhere);
      }

      if (secondQueryLayer && (secondOidSet != 0))
      {
        te::da::Select* query = secondQueryLayer->getQuery();
        te::da::Where* where = query->getWhere();

        te::da::Expression* originalExp = where->getExp();

        te::da::Expression* in = secondOidSet->getExpressionByInClause();

        te::da::Expression* cloneWhere = originalExp->clone();

        te::da::And* newAnd = new te::da::And(cloneWhere, in);

        te::da::Where* newWhere = new te::da::Where(newAnd);

        query->setWhere(newWhere);
      }

      intersectionOp->setInput(firstDataSource, firstName, m_firstSelectedLayer->getSchema(), m_firstSelectedLayer->getData(), firstConverter,
                               secondDataSource, secondName, m_secondSelectedLayer->getSchema(), m_secondSelectedLayer->getData(), secondConverter,
                               firstOidSet, secondOidSet);

      intersectionOp->setOutput(dsOGR, outputdataset);
      intersectionOp->setParams(getSelectedProperties());

      if (!intersectionOp->paramsAreValid())
        res = false;
      else
        res = intersectionOp->run();

      if(!res)
      {
        dsOGR->close();
        QMessageBox::information(this, "Intersection", "Error: could not generate the intersection.");
        reject();
      }
      dsOGR->close();

      delete intersectionOp;

      // let's include the new datasource in the managers
      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string id = boost::uuids::to_string(u);
      
      te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
      ds->setConnInfo(dsinfo);
      ds->setTitle(uri.stem().string());
      ds->setAccessDriver("OGR");
      ds->setType("OGR");
      ds->setDescription(uri.string());
      ds->setId(id);
      
      te::da::DataSourcePtr newds = te::da::DataSourceManager::getInstance().get(id, "OGR", ds->getConnInfo());
      newds->open();
      te::da::DataSourceInfoManager::getInstance().add(ds);
      m_outputDatasource = ds;
    }
    else
    {
      te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
      if (!aux.get())
      {
        QMessageBox::information(this, "Intersection", "The output data source can not be accessed.");
        return;
      }
      if (aux->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Intersection", "Dataset already exists. Remove it or select a new name and try again. ");
        return;
      }

      std::auto_ptr<te::da::DataSetTypeConverter> firstConverter(new te::da::DataSetTypeConverter(m_firstSelectedLayer->getSchema().get(), aux->getCapabilities(), aux->getEncoding()));
      te::da::AssociateDataSetTypeConverterSRID(firstConverter.get(), m_firstSelectedLayer->getSRID());

      std::auto_ptr<te::da::DataSetTypeConverter> secondConverter(new te::da::DataSetTypeConverter(m_secondSelectedLayer->getSchema().get(), aux->getCapabilities(), aux->getEncoding()));
      te::da::AssociateDataSetTypeConverterSRID(secondConverter.get(), m_firstSelectedLayer->getSRID());
      
      this->setCursor(Qt::WaitCursor);

      te::vp::IntersectionOp* intersectionOp = 0;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities firstDSCapabilities = firstDataSource->getCapabilities();
      const te::da::DataSourceCapabilities secondDSCapabilities = secondDataSource->getCapabilities();

      if( firstDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
          secondDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
          (firstDataSource->getId() == secondDataSource->getId()) &&
          (m_firstSelectedLayer->getSRID() == m_secondSelectedLayer->getSRID()) && (firstDataSetLayer && secondDataSetLayer) )
      {
        intersectionOp = new te::vp::IntersectionQuery();
      }
      else
      {
        intersectionOp = new te::vp::IntersectionMemory();
      }

      //intersectionOp->setInput( firstDataSource, firstDataSetLayer->getDataSetName(), firstConverter,
      //                          secondDataSource, secondDataSetLayer->getDataSetName(), secondConverter,
      //                          firstOidSet, secondOidSet);

      intersectionOp->setInput(firstDataSource, firstDataSetLayer->getDataSetName(), m_firstSelectedLayer->getSchema(), m_firstSelectedLayer->getData(), firstConverter,
        secondDataSource, secondDataSetLayer->getDataSetName(), m_secondSelectedLayer->getSchema(), m_secondSelectedLayer->getData(), secondConverter,
        firstOidSet, secondOidSet);

      intersectionOp->setOutput(aux, outputdataset);
      intersectionOp->setParams(getSelectedProperties());

      if (!intersectionOp->paramsAreValid())
        res = false;
      else
        res = intersectionOp->run();

      delete intersectionOp;

      if(!res)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Intersection", "Error: could not generate the intersection.");
        reject();
      }
    }

    // creating a layer for the result
    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());
    
    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());
      
    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    m_layerResult = converter(dt);
  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);
    QMessageBox::warning(this, TE_TR("Intersection"), e.what());

#ifdef TERRALIB_LOGGER_ENABLED
    std::string str = "Intersection - ";
    str += e.what();
    te::common::Logger::logDebug("vp", str.c_str());
#endif //TERRALIB_LOGGER_ENABLED

    te::common::ProgressManager::getInstance().removeViewer(id);
    return;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);
  this->setCursor(Qt::ArrowCursor);
  accept();
}

void te::vp::IntersectionDialog::onTargetDatasourceToolButtonPressed()
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

void te::vp::IntersectionDialog::onTargetFileToolButtonPressed()
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

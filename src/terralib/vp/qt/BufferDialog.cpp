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
  \file terralib/vp/BufferDialog.cpp

  \brief A dialog for buffer operation
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/AbstractLayer.h"
#include "../../maptools/DataSetLayer.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../srs/Config.h"
#include "../Exception.h"
#include "../BufferMemory.h"
#include "BufferDialog.h"
#include "../BufferOp.h"
#include "../BufferQuery.h"
#include "ui_BufferDialogForm.h"
#include "Utils.h"

// Qt
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QSize>

// STL
#include <map>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::vp::BufferDialog::BufferDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::BufferDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_selectedLayer(0)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vp-buffer-hint").pixmap(112,48));
  
  QSize iconSize(96, 48);

  m_ui->m_withoutBoundRadioButton->setIconSize(iconSize);
  m_ui->m_withoutBoundRadioButton->setIcon(QIcon::fromTheme("buffer-without-boundaries"));

  m_ui->m_withBoundRadioButton->setIconSize(iconSize);
  m_ui->m_withBoundRadioButton->setIcon(QIcon::fromTheme("buffer-with-boundaries"));

  m_ui->m_ruleImgLabel->setPixmap(QIcon::fromTheme("buffer-inside-outside").pixmap(150,60));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));  

//add controls
  m_ui->m_fixedDistanceLineEdit->setEnabled(true);
  m_ui->m_fixedDistanceLineEdit->setValidator(new QDoubleValidator(this));

  setPossibleLevels();

//signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerComboBoxChanged(int)));
  connect(m_ui->m_fixedRadioButton, SIGNAL(toggled(bool)), this, SLOT(onFixedDistanceToggled()));
  connect(m_ui->m_fromAttRadioButton, SIGNAL(toggled(bool)), this, SLOT(onAttDistanceToggled()));
  connect(m_ui->m_ruleInOutRadioButton, SIGNAL(toggled(bool)), this, SLOT(onRuleInOutToggled()));
  connect(m_ui->m_ruleOnlyOutRadioButton, SIGNAL(toggled(bool)), this, SLOT(onRuleOutToggled()));
  connect(m_ui->m_ruleOnlyInRadioButton, SIGNAL(toggled(bool)), this, SLOT(onRuleInToggled()));
  connect(m_ui->m_withoutBoundRadioButton, SIGNAL(toggled(bool)), this, SLOT(onWithoutBoundToggled()));
  connect(m_ui->m_withBoundRadioButton, SIGNAL(toggled(bool)), this, SLOT(onWithBoundToggled()));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
  
  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/vp/vp_buffer.html");
}

te::vp::BufferDialog::~BufferDialog()
{
}

void te::vp::BufferDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if(dsType->hasGeom())
      m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

void te::vp::BufferDialog::setPossibleLevels()
{
  for(int i=1; i <= 10; ++i)
    m_ui->m_levelsNumComboBox->addItem(QString::number(i));
}

void te::vp::BufferDialog::setAttributesForDistance(std::vector<te::dt::Property*> properties)
{
  m_ui->m_fromAttDistanceComboBox->clear();

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    if(properties[i]->getType() == te::dt::CDOUBLE_TYPE ||
       properties[i]->getType() == te::dt::CFLOAT_TYPE ||
       properties[i]->getType() == te::dt::CINT16_TYPE ||
       properties[i]->getType() == te::dt::CINT32_TYPE ||
       properties[i]->getType() == te::dt::DOUBLE_TYPE ||
       properties[i]->getType() == te::dt::INT16_TYPE ||
       properties[i]->getType() == te::dt::INT32_TYPE ||
       properties[i]->getType() == te::dt::INT64_TYPE ||
       properties[i]->getType() == te::dt::FLOAT_TYPE ||
       properties[i]->getType() == te::dt::NUMERIC_TYPE ||
       properties[i]->getType() == te::dt::UINT16_TYPE ||
       properties[i]->getType() == te::dt::UINT32_TYPE ||
       properties[i]->getType() == te::dt::UINT64_TYPE)
      m_ui->m_fromAttDistanceComboBox->addItem(properties[i]->getName().c_str());
  }

  if(m_ui->m_fromAttDistanceComboBox->count() > 0)
  {
    //m_ui->m_fromAttRadioButton->setEnabled(true);
  }
  else
    m_ui->m_fromAttRadioButton->setEnabled(false);
}

int te::vp::BufferDialog::getPolygonRule()
{
  if(m_ui->m_ruleInOutRadioButton->isChecked())
    return te::vp::INSIDE_OUTSIDE;
  else if(m_ui->m_ruleOnlyOutRadioButton->isChecked())
    return te::vp::ONLY_OUTSIDE;
  else
    return te::vp::ONLY_INSIDE;
}

int te::vp::BufferDialog::getBoundariesRule()
{
  if(m_ui->m_withoutBoundRadioButton->isChecked())
    return te::vp::DISSOLVE;
  else
    return te::vp::NOT_DISSOLVE;
}

te::map::AbstractLayerPtr te::vp::BufferDialog::getLayer()
{
  return m_outputLayer;
}

void te::vp::BufferDialog::onLayerComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_selectedLayer = selectedLayer;
      std::auto_ptr<const te::map::LayerSchema> schema(selectedLayer->getSchema());

      if(schema->size() == 0)
        return;

      te::common::FreeContents(m_properties);
      m_properties.clear();

      const std::vector<te::dt::Property*>& properties = schema->getProperties();
      te::common::Clone(properties, m_properties);

      setAttributesForDistance(m_properties);

      te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_selectedLayer.get());
      te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
      te::da::DataSetType* dsType = te::da::GetDataSetType(dsLayer->getDataSetName(), inDataSource->getId());
      std::auto_ptr<te::gm::GeometryProperty>geomProp(te::da::GetFirstGeomProperty(dsType));

      te::gm::GeomType gmType = geomProp->getGeometryType();
      if(gmType == te::gm::PointType || gmType == te::gm::MultiPointType ||
        gmType == te::gm::LineStringType || gmType == te::gm::MultiLineStringType)
      {
        m_ui->m_ruleInOutRadioButton->setDisabled(true);
        m_ui->m_ruleOnlyInRadioButton->setDisabled(true);
        m_ui->m_ruleOnlyOutRadioButton->setChecked(true);
      }
      else
      {
        m_ui->m_ruleInOutRadioButton->setEnabled(true);
        m_ui->m_ruleOnlyInRadioButton->setEnabled(true);
        m_ui->m_ruleInOutRadioButton->setChecked(true);
      }

      return;
    }
    ++it;
  }
}

void te::vp::BufferDialog::onFixedDistanceToggled()
{
  m_ui->m_fixedDistanceLineEdit->setEnabled(true);
  m_ui->m_fromAttDistanceComboBox->setEnabled(false);
}

void te::vp::BufferDialog::onAttDistanceToggled()
{
  m_ui->m_fromAttDistanceComboBox->setEnabled(true);
  m_ui->m_fixedDistanceLineEdit->setEnabled(false);
}

void te::vp::BufferDialog::onRuleInOutToggled()
{
  m_ui->m_ruleImgLabel->setPixmap(QIcon::fromTheme("buffer-inside-outside").pixmap(150,60));
}

void te::vp::BufferDialog::onRuleOutToggled()
{
  m_ui->m_ruleImgLabel->setPixmap(QIcon::fromTheme("buffer_only-outside").pixmap(150,60));
}

void te::vp::BufferDialog::onRuleInToggled()
{
  m_ui->m_ruleImgLabel->setPixmap(QIcon::fromTheme("buffer-only-inside").pixmap(150,60));
}

void te::vp::BufferDialog::onWithoutBoundToggled()
{
  m_ui->m_copyColumnsCheckBox->setChecked(false);
  m_ui->m_copyColumnsCheckBox->setEnabled(false);
}

void te::vp::BufferDialog::onWithBoundToggled()
{
  m_ui->m_copyColumnsCheckBox->setEnabled(true);
}

void te::vp::BufferDialog::onTargetDatasourceToolButtonPressed()
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

void te::vp::BufferDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                      QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);

  if(fileName.isEmpty())
    return;

  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());
  
  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::vp::BufferDialog::onOkPushButtonClicked()
{
  if(m_ui->m_layersComboBox->currentText().isEmpty())
  {
    QMessageBox::information(this, "Buffer", "Select an input layer.");
    return;
  }
  
  // Checking consistency of the input layer where the buffer will executed
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_selectedLayer.get());
  if(!dsLayer)
  {
    QMessageBox::information(this, "Buffer", "Can not execute this operation on this type of layer.");
    return;
  }
  
  const te::da::ObjectIdSet* oidSet = 0;

  if(m_ui->m_onlySelectedCheckBox->isChecked())
  {
    oidSet = m_selectedLayer->getSelected();
    if(!oidSet)
    {
      QMessageBox::information(this, "Buffer", "Select the layer objects to perform the buffer operation.");
      return;
    }
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
  if (!inDataSource.get())
  {
    QMessageBox::information(this, "Buffer", "The selected input data source can not be accessed.");
    return;
  }
  
  // Check consistency of buffer parameters
  double fixedDistance;
  std::string propDistance = "";
  if(m_ui->m_fixedRadioButton->isChecked())
  {
    fixedDistance = m_ui->m_fixedDistanceLineEdit->text().toDouble();
    if (fixedDistance <= 0)
    {
      QMessageBox::information(this, "Buffer", "Fixed distance value should be greater than 0.");
      return;
    }
  }
  else
  {
    fixedDistance = 0;
    int i = m_ui->m_fromAttDistanceComboBox->currentIndex();
    propDistance = m_ui->m_fromAttDistanceComboBox->itemText(i).toStdString();
  }
  
  // Checking consistency of output paramenters
  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Buffer", "Select a repository for the resulting layer.");
    
    return;
  }
  
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Buffer", "Define a name for the resulting layer.");
    return;
  }

  int bufferPolygonRule = getPolygonRule();
  int bufferBoundariesRule = getBoundariesRule();
  bool copyInputColumns = m_ui->m_copyColumnsCheckBox->isChecked();
  int levels = m_ui->m_levelsNumComboBox->currentText().toInt();
  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();
  
  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    bool res;

    if(m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

      if(boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "Buffer", "Output file already exists. Remove it or select a new name and try again.");
        return;
      }

      std::size_t idx = outputdataset.find(".");
      if(idx != std::string::npos)
        outputdataset = outputdataset.substr(0,idx);

      std::map<std::string, std::string> dsinfo;
      dsinfo["URI"] = uri.string();

      te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
      dsOGR->setConnectionInfo(dsinfo);
      dsOGR->open();

      if(dsOGR->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Buffer", "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.");
        return;
      }

      std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(dsLayer->getSchema().get(), dsOGR->getCapabilities(), dsOGR->getEncoding()));

      te::da::AssociateDataSetTypeConverterSRID(converter.get(), dsLayer->getSRID());

      this->setCursor(Qt::WaitCursor);

      te::vp::BufferOp* bufferOp = 0;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities dsCapabilities = inDataSource->getCapabilities();

      if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
      {
        bufferOp = new te::vp::BufferQuery();
      }
      else
      {
        bufferOp = new te::vp::BufferMemory();
      }

      bufferOp->setInput(inDataSource, dsLayer->getDataSetName(), converter, oidSet);
      bufferOp->setOutput(dsOGR, outputdataset);
      bufferOp->setParams(fixedDistance, bufferPolygonRule, bufferBoundariesRule, copyInputColumns, levels);

      if (!bufferOp->paramsAreValid())
        res = false;
      else
        res = bufferOp->run();

      if(!res)
      {
        dsOGR->close();
        QMessageBox::information(this, "Buffer", "Error: could not generate the buffer.");
      }
      dsOGR->close();

      delete bufferOp;

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
      if (!aux)
      {
        QMessageBox::information(this, "Buffer", "The selected output datasource can not be accessed.");
        return;
      }
      if (aux->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Buffer", "Dataset already exists. Remove it or select a new name and try again. ");
        return;
      }
      this->setCursor(Qt::WaitCursor);


      std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(dsLayer->getSchema().get(), aux->getCapabilities(), aux->getEncoding()));

      te::da::AssociateDataSetTypeConverterSRID(converter.get(), dsLayer->getSRID());


      te::vp::BufferOp* bufferOp = 0;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities dsCapabilities = inDataSource->getCapabilities();

      if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
      {
        bufferOp = new te::vp::BufferQuery();
      }
      else
      {
        bufferOp = new te::vp::BufferMemory();
      }

      bufferOp->setInput(inDataSource, dsLayer->getDataSetName(), converter);
      bufferOp->setOutput(aux, outputdataset);
      bufferOp->setParams(fixedDistance, bufferPolygonRule, bufferBoundariesRule, copyInputColumns, levels);

      if (!bufferOp->paramsAreValid())
        res = false;
      else
        res = bufferOp->run();

      if(!res)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Buffer", "Error: could not generate the buffer.");
        reject();
      }

      delete bufferOp;
    }

    // creating a layer for the result
    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());
    
    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());
      
    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    m_outputLayer = converter(dt);
  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);
    QMessageBox::information(this, "Buffer", e.what());

#ifdef TERRALIB_LOGGER_ENABLED
    std::string str = "Buffer - ";
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

void te::vp::BufferDialog::onCancelPushButtonClicked()
{
  reject();
}

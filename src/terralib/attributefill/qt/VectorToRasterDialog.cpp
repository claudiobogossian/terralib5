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
  \file terralib/vp/VectorToRasterDialog.cpp

  \brief A dialog for vector to raster operation
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
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
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/rp/Utils.h"
#include "../../qt/widgets/Utils.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "../../statistics/core/Utils.h"
#include "../../srs/SpatialReferenceSystemManager.h"
#include "../Config.h"
#include "../Exception.h"
#include "VectorToRasterDialog.h"
#include "../VectorToRaster.h"
#include "ui_VectorToRasterDialogForm.h"

// Qt
#include <QDoubleValidator>
#include <QFileDialog>
#include <QIntValidator>
#include <QLineEdit>
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

te::attributefill::VectorToRasterDialog::VectorToRasterDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::VectorToRasterDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>())
{
// add controls
  m_ui->setupUi(this);

  // add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vector-raster-hint").pixmap(112,48));

  m_widget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_widget));
  m_widget->setLeftLabel("Available Properties");
  m_widget->setRightLabel("Used Properties");

  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  displayLayout->addWidget(m_widget.get());

  m_ui->m_colsLineEdit->setValidator( new QIntValidator(this) );
  m_ui->m_rowsLineEdit->setValidator( new QIntValidator(this) );
  m_ui->m_resXLineEdit->setValidator( new QDoubleValidator(0, 100, 4, this) );
  m_ui->m_resYLineEdit->setValidator( new QDoubleValidator(0, 100, 4, this) );
  m_ui->m_dummyLineEdit->setValidator( new QIntValidator(this) );

  connect(m_ui->m_resXLineEdit, SIGNAL(editingFinished()), this, SLOT(onResXLineEditEditingFinished()));
  connect(m_ui->m_resYLineEdit, SIGNAL(editingFinished()), this, SLOT(onResYLineEditEditingFinished()));
  connect(m_ui->m_colsLineEdit, SIGNAL(editingFinished()), this, SLOT(onColsLineEditEditingFinished()));
  connect(m_ui->m_rowsLineEdit, SIGNAL(editingFinished()), this, SLOT(onRowsLineEditEditingFinished()));

  connect(m_ui->m_inVectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onVectorComboBoxChanged(int)));

  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::attributefill::VectorToRasterDialog::~VectorToRasterDialog()
{
}

void te::attributefill::VectorToRasterDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if(dsType->hasGeom())
      m_ui->m_inVectorComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

te::map::AbstractLayerPtr te::attributefill::VectorToRasterDialog::getLayer()
{
  return m_outLayer;
}

void te::attributefill::VectorToRasterDialog::showSRS()
{
  std::string name = te::srs::SpatialReferenceSystemManager::getInstance().getName(m_srid);
  name += " - " + boost::lexical_cast<std::string>(m_srid);

  m_ui->m_sridLabel->setText(name.c_str());
}

void te::attributefill::VectorToRasterDialog::onVectorComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  
  std::string layerID = m_ui->m_inVectorComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  m_widget->clearInputValues();
  m_widget->clearOutputValues();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      m_selectedLayer = it->get();
      std::auto_ptr<const te::map::LayerSchema> schema(m_selectedLayer->getSchema());

      const std::vector<te::dt::Property*> vecProperties = schema->getProperties();
      std::vector<std::string> propNameVec;

      for(std::size_t i = 0; i < vecProperties.size(); ++i)
      {
        int type = vecProperties[i]->getType();
        if( type == te::dt::CDOUBLE_TYPE ||
            type == te::dt::CFLOAT_TYPE ||
            type == te::dt::CINT16_TYPE ||
            type == te::dt::CINT32_TYPE ||
            type == te::dt::DOUBLE_TYPE ||
            type == te::dt::FLOAT_TYPE || 
            type == te::dt::INT16_TYPE ||
            type == te::dt::INT32_TYPE ||
            type == te::dt::INT64_TYPE ||
            type == te::dt::UINT16_TYPE ||
            type == te::dt::UINT32_TYPE ||
            type == te::dt::UINT64_TYPE)
        {
          propNameVec.push_back(vecProperties[i]->getName().c_str());
          m_properties.push_back(vecProperties[i]);
        }
      }
      m_widget->setInputValues(propNameVec);

      m_srid = m_selectedLayer->getSRID();
      showSRS();
    }
    ++it;
  }
}


void te::attributefill::VectorToRasterDialog::onResXLineEditEditingFinished()
{
  double resX = m_ui->m_resXLineEdit->text().toDouble();

  te::gm::Envelope env = m_selectedLayer->getExtent();

  if(!env.isValid())
  {
    QMessageBox::warning(this, tr("Attribute Fill"), tr("Invalid envelope!"));
    return;
  }

  int maxCols = (int)ceil((env.m_urx - env.m_llx)/resX);

  m_ui->m_colsLineEdit->setText(QString::number(maxCols));
}

void te::attributefill::VectorToRasterDialog::onResYLineEditEditingFinished()
{
  double resY = m_ui->m_resYLineEdit->text().toDouble();

  te::gm::Envelope env = m_selectedLayer->getExtent();

  if(!env.isValid())
  {
    QMessageBox::warning(this, tr("Attribute Fill"), tr("Invalid envelope!"));
    return;
  }

  int maxRows = (int)ceil((env.m_ury - env.m_lly)/resY);

  m_ui->m_rowsLineEdit->setText(QString::number(maxRows));
}

void te::attributefill::VectorToRasterDialog::onColsLineEditEditingFinished()
{
  int cols = m_ui->m_colsLineEdit->text().toInt();

  te::gm::Envelope env = m_selectedLayer->getExtent();

  if(!env.isValid())
  {
    QMessageBox::warning(this, tr("Attribute Fill"), tr("Invalid envelope!"));
    return;
  }

  double resX = (env.m_urx - env.m_llx)/cols;

  m_ui->m_resXLineEdit->setText(QString::number(resX));
}

void te::attributefill::VectorToRasterDialog::onRowsLineEditEditingFinished()
{
  int rows = m_ui->m_rowsLineEdit->text().toInt();

  te::gm::Envelope env = m_selectedLayer->getExtent();

  if(!env.isValid())
  {
    QMessageBox::warning(this, tr("Attribute Fill"), tr("Invalid envelope!"));
    return;
  }

  double resY = (env.m_ury - env.m_lly)/rows;

  m_ui->m_resYLineEdit->setText(QString::number(resY));
}

void te::attributefill::VectorToRasterDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();
  
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                        QString(), tr("TIFF (*.tif *.TIF);;"),0, QFileDialog::DontConfirmOverwrite);

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

void te::attributefill::VectorToRasterDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::attributefill::VectorToRasterDialog::onOkPushButtonClicked()
{
  if(m_ui->m_inVectorComboBox->count() == 0)
  {
    QMessageBox::information(this, "Fill", "Select an input vector layer.");
    return;
  }

  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_selectedLayer.get());

  if(!dsLayer)
  {
    QMessageBox::information(this, "Fill", "Can not execute this operation on this type of layer.");
    return;
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
  if (!inDataSource.get())
  {
    QMessageBox::information(this, "Fill", "The selected input data source can not be accessed.");
    m_ui->m_targetFileToolButton->setFocus();
    return;
  }

  if(m_widget->getOutputValues().size() == 0)
  {
    QMessageBox::information(this, "Fill", "Select at least one output attribute to represent the band of the raster.");
    return;
  }

  if(m_ui->m_resXLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a resolution for the output raster.");
    m_ui->m_resXLineEdit->setFocus();
    return;
  }

  if(m_ui->m_resYLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a resolution for the output raster.");
    m_ui->m_resYLineEdit->setFocus();
    return;
  }

  if(m_ui->m_colsLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a resolution for the output raster.");
    m_ui->m_colsLineEdit->setFocus();
    return;
  }

  if(m_ui->m_rowsLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a resolution for the output raster.");
    m_ui->m_rowsLineEdit->setFocus();
    return;
  }

  if(m_ui->m_dummyCheckBox->isChecked())
  {
    if(m_ui->m_dummyLineEdit->text().isEmpty())
    {
      QMessageBox::information(this, "Fill", "Define a dummy value for the output raster.");
      m_ui->m_dummyLineEdit->setFocus();
      return;
    }
  }

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a repository for the result.");
    m_ui->m_targetFileToolButton->setFocus();
    return;
  }
       
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a name for the resulting layer.");
    m_ui->m_newLayerNameLineEdit->setFocus();
    return;
  }
  
  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    bool res;

    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

    if (boost::filesystem::exists(uri))
    {
      QMessageBox::information(this, "Fill", "Output file already exists. Remove it or select a new name and try again.");
      return;
    }

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset=outputdataset.substr(0,idx);

    std::map<std::string, std::string> dsinfo;
    dsinfo["URI"] = uri.string();
      
    te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
    dsOGR->setConnectionInfo(dsinfo);
    dsOGR->open();
    if (dsOGR->dataSetExists(outputdataset))
    {
      QMessageBox::information(this, "Fill", "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.");
      return;
    }

    this->setCursor(Qt::WaitCursor);

    te::attributefill::VectorToRaster* vec2rst = new te::attributefill::VectorToRaster();
    vec2rst->setInput(inDataSource, dsLayer->getTitle(), dsLayer->getSchema());
    vec2rst->setParams( m_widget->getOutputValues(), 
                        m_ui->m_resXLineEdit->text().toDouble(),
                        m_ui->m_resYLineEdit->text().toDouble(),
                        m_ui->m_colsLineEdit->text().toInt(),
                        m_ui->m_rowsLineEdit->text().toInt(),
                        m_ui->m_dummyCheckBox->isChecked(),
                        m_ui->m_dummyLineEdit->text().toInt());
    vec2rst->setOutput(dsOGR, outputdataset);

    if (!vec2rst->paramsAreValid())
      res = false;
    else
      res = vec2rst->run();

    if(!res)
    {
      this->setCursor(Qt::ArrowCursor);
      dsOGR->close();
      QMessageBox::information(this, "Fill", "Error: could not generate the operation.");
      reject();
    }
    dsOGR->close();

    delete vec2rst;

// let's include the new datasource in the managers
    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id_ds = boost::uuids::to_string(u);

    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
    ds->setConnInfo(dsinfo);
    ds->setTitle(uri.stem().string());
    ds->setAccessDriver("GDAL");
    ds->setType("GDAL");
    ds->setDescription(uri.string());
    ds->setId(id_ds);

    te::da::DataSourcePtr newds = te::da::DataSourceManager::getInstance().get(id_ds, "GDAL", ds->getConnInfo());
    newds->open();
    te::da::DataSourceInfoManager::getInstance().add(ds);
    m_outputDatasource = ds;

  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);

    QMessageBox::information(this, "Fill", e.what());
    te::common::Logger::logDebug("fill", e.what());
    te::common::ProgressManager::getInstance().removeViewer(id);

    return;
  }

  m_outLayer = te::qt::widgets::createLayer("GDAL", 
                                            m_outputDatasource->getConnInfo());

  te::common::ProgressManager::getInstance().removeViewer(id);
  this->setCursor(Qt::ArrowCursor);

  accept();

}

void te::attributefill::VectorToRasterDialog::onCancelPushButtonClicked()
{
  reject();
}
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
#include "../../qt/widgets/Utils.h"
#include "../../statistics/core/Utils.h"
#include "../Config.h"
#include "../Exception.h"
#include "VectorToRasterDialog.h"
#include "../VectorToRaster.h"
#include "ui_VectorToRasterDialogForm.h"

// Qt
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QLineEdit>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
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
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  m_ui->m_xLineEdit->setValidator(new QDoubleValidator(0, 100, 2, this));
  m_ui->m_yLineEdit->setValidator(new QDoubleValidator(0, 100, 2, this));

  connect(m_ui->m_inVectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onVectorComboBoxChanged(int)));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
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

std::vector<te::dt::Property*> te::attributefill::VectorToRasterDialog::getSelectedProperties()
{
  std::vector<te::dt::Property*> selProperties;

  for(std::size_t i = 0; i != m_ui->m_attributesListWidget->count(); ++i)
  {
    if(m_ui->m_attributesListWidget->isItemSelected(m_ui->m_attributesListWidget->item(i)))
    {
      selProperties.push_back(m_properties[i]);
    }
  }

  return selProperties;
}

void te::attributefill::VectorToRasterDialog::onVectorComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  
  std::string layerID = m_ui->m_inVectorComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  m_ui->m_attributesListWidget->clear();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      m_selectedLayer = it->get();
      std::auto_ptr<const te::map::LayerSchema> schema(m_selectedLayer->getSchema());

      const std::vector<te::dt::Property*> vecProperties = schema->getProperties();
      for(std::size_t i = 0; i < vecProperties.size(); ++i)
      {
        int type = vecProperties[i]->getType();
        if(type != te::dt::GEOMETRY_TYPE)
        {
          m_ui->m_attributesListWidget->addItem(vecProperties[i]->getName().c_str());
          m_properties.push_back(vecProperties[i]);
        }
      }
    }
    ++it;
  }


}

void te::attributefill::VectorToRasterDialog::onTargetDatasourceToolButtonPressed()
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

void te::attributefill::VectorToRasterDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();
  
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), te::qt::widgets::GetFilePathFromSettings("attributefill_vec2raster"), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  
  if(dir.isEmpty() == false)
  {
    m_path = dir.replace(QRegExp("\\\\"), "/").toStdString();

    m_ui->m_repositoryLineEdit->setText(m_path.c_str());

    te::qt::widgets::AddFilePathToSettings(m_path.c_str(), "attributefill_vec2raster");

    m_toFile = true;
  }
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
    return;
  }

  std::vector<te::dt::Property*> selProperties = getSelectedProperties();
  if(selProperties.empty())
  {
    QMessageBox::information(this, "Fill", "Select at least one attribute.");
    return;
  }
  
  if(m_ui->m_xLineEdit->text().isEmpty() || m_ui->m_yLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a resolution.");
    return;
  }

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a repository for the result.");
    return;
  }
       
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a name for the resulting layer.");
    return;
  }
  
  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();


  try
  {
    if (m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());


    }
    else
    {

    }
  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);

    QMessageBox::information(this, "Fill", e.what());
    te::common::Logger::logDebug("fill", e.what());

    return;
  }

}

void te::attributefill::VectorToRasterDialog::onCancelPushButtonClicked()
{
  reject();
}
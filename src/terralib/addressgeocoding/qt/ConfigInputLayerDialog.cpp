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
  \file terralib/addressgeocoding/ConfigInputLayerDialog.cpp

  \brief A dialog configure the input layer to address geocoding operation
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
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/af/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "../../statistics/core/Utils.h"
#include "../Config.h"
#include "../Exception.h"
#include "ConfigInputLayerDialog.h"
#include "ui_ConfigInputLayerDialogForm.h"

// Qt
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::addressgeocoding::ConfigInputLayerDialog::ConfigInputLayerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ConfigInputLayerDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_toFile(false)
{
// add controls
  m_ui->setupUi(this);
  
  m_widget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_widget));
  
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  displayLayout->addWidget(m_widget.get());

  m_widget->setLeftLabel("Available Properties");
  m_widget->setRightLabel("Used Properties");

  connect(m_ui->m_inputLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputLayerComboBoxChanged(int)));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::addressgeocoding::ConfigInputLayerDialog::~ConfigInputLayerDialog()
{
}

void te::addressgeocoding::ConfigInputLayerDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if(dsType->hasGeom())
		{
      te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dsType.get());
      int type = geomProp->getGeometryType();

      if((type == te::gm::LineStringType) || (type == te::gm::MultiLineStringType))
			  m_ui->m_inputLayerComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
		}
      
    ++it;
  }
}

void te::addressgeocoding::ConfigInputLayerDialog::onInputLayerComboBoxChanged(int index)
{
  m_widget->clearInputValues();
  m_widget->clearOutputValues();
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_inputLayerComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_selectedLayer = selectedLayer;
      std::auto_ptr<const te::map::LayerSchema> schema(selectedLayer->getSchema());
      const std::vector<te::dt::Property*>& properties = schema->getProperties();
      std::vector<std::string> propNames;

      for(std::size_t i=0; i < properties.size(); ++i)
      {
        if(properties[i]->getType() != te::dt::GEOMETRY_TYPE)
          propNames.push_back(properties[i]->getName());
      }
      m_widget->setInputValues(propNames);
    }
      ++it;
  }
}

void te::addressgeocoding::ConfigInputLayerDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::addressgeocoding::ConfigInputLayerDialog::onOkPushButtonClicked()
{
  m_selectedProps = m_widget->getOutputValues();
  this->close();
}

void te::addressgeocoding::ConfigInputLayerDialog::onCancelPushButtonClicked()
{
  reject();
}


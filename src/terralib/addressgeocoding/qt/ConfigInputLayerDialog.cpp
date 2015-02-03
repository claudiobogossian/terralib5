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
#include "../../postgis/Transactor.h"
#include "../../qt/af/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "../../statistics/core/Utils.h"
#include "../Config.h"
#include "../Exception.h"
#include "ConfigInputLayerDialog.h"
#include "ConfigNumberDialog.h"
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

#include <iostream>
#include <string>

te::addressgeocoding::ConfigInputLayerDialog::ConfigInputLayerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ConfigInputLayerDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>())
{
// add controls
  m_ui->setupUi(this);
  
  m_widget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_widget));
  
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  displayLayout->addWidget(m_widget.get());

  m_widget->setLeftLabel("Available Attributes");
  m_widget->setRightLabel("Selected Attributes");

  connect(m_ui->m_inputLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputLayerComboBoxChanged(int)));

//  connect(m_ui->m_numberPushButton, SIGNAL(clicked()), this, SLOT(onNumberPushButtonClicked()));

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

te::map::AbstractLayerPtr te::addressgeocoding::ConfigInputLayerDialog::getLayer()
{
  return m_selectedLayer;
}

te::da::DataSourcePtr te::addressgeocoding::ConfigInputLayerDialog::getDataSource()
{
  return m_dataSource;
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

void te::addressgeocoding::ConfigInputLayerDialog::onNumberPushButtonClicked()
{
  te::addressgeocoding::ConfigNumberDialog dlg(this);
  dlg.setLayer(m_selectedLayer);

  if(dlg.exec()!=QDialog::Accepted)
  {
    //m_initialLeft = dlg.getInitialLeft();
    //m_finalLeft = dlg.getFinalLeft();
    //m_initialRight = dlg.getInitialRight();
    //m_finalRight = dlg.getFinalRight();
  }

  return;
}

void te::addressgeocoding::ConfigInputLayerDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::addressgeocoding::ConfigInputLayerDialog::onOkPushButtonClicked()
{
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_selectedLayer.get());
  if(!dsLayer)
  {
    QMessageBox::information(this, "Address Geocoding", "Can not execute this operation on this type of layer.");
    return;
  }

  m_dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
  if(!m_dataSource.get())
  {
    QMessageBox::information(this, "Address Geocoding", "The selected input data source can not be accessed.");
  }

  m_selectedProps = m_widget->getOutputValues();
  if(m_selectedProps.empty())
  {
    QMessageBox::information(this, "Address Geocoding", "Select at least one attribute.");
    return;
  }
  
//Checks whether the table already contains a column called tsvector.
  std::auto_ptr<te::da::DataSetType> schema = m_selectedLayer->getSchema();
  const std::vector<te::dt::Property*>& properties = schema->getProperties();

  bool addNewColumn = true;
  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    std::string name = properties[i]->getName();
    if(name == "tsvector")
      addNewColumn = false;
  }

// ALTER TABLE adding a new columns of tsvector type.  
  if(addNewColumn == true)
  {
    std::auto_ptr<te::da::DataSourceTransactor> trans = m_dataSource->getTransactor();
    std::string alterTable = "ALTER TABLE "+ m_selectedLayer->getTitle() + " ADD tsvector tsvector";
    trans->execute(alterTable);
  }


  
// UPDATE values in tsvector column.
  std::string updateTable = "UPDATE " + m_selectedLayer->getTitle() + " SET tsvector = to_tsvector('english', ";

  for(std::size_t selProps = 0; selProps < m_selectedProps.size(); ++selProps)
  {
    if(selProps == 0)
      updateTable += " "+ m_selectedProps[selProps];
    else
      updateTable += "||' '||"+ m_selectedProps[selProps];
  }
  
  updateTable += ")";

  m_dataSource->execute(updateTable);

  
  unsigned dot = m_selectedLayer->getTitle().find_last_of(".");
  std::string table = m_selectedLayer->getTitle().substr(dot+1);

//DROP INDEX if exists.
  std::string dropIndex = "DROP INDEX IF EXISTS " + table +"_idx";
  m_dataSource->execute(dropIndex);

//CREATE INDEX to speed up the text search.
  std::string createIndex = "CREATE INDEX "+ table +"_idx ON "+ m_selectedLayer->getTitle() + "  USING GIN(tsvector)";

  m_dataSource->execute(createIndex);

  this->close();
}

void te::addressgeocoding::ConfigInputLayerDialog::onCancelPushButtonClicked()
{
  reject();
}


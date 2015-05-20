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
  \file terralib/layout/qt/outside/MapLayerChoiceOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "../../../common/Logger.h"
#include "../../../common/progress/ProgressManager.h"
#include "../../../common/Translator.h"
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/Enums.h"
#include "../../../datatype/Property.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../postgis/Transactor.h"
#include "../../../qt/af/Utils.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../../qt/widgets/utils/DoubleListWidget.h"
#include "../../../statistics/core/Utils.h"
#include "MapLayerChoiceOutside.h"
#include "ui_MapLayerChoice.h"

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

te::layout::MapLayerChoiceOutside::MapLayerChoiceOutside(OutsideController* controller, Observable* o)
  : QDialog(0),
    OutsideObserver(controller, o),
    m_ui(new Ui::MapLayerChoice),
    m_layers(std::list<te::map::AbstractLayerPtr>())
{
// add controls
  m_ui->setupUi(this);
  
  m_widget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_widget));
  
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  displayLayout->addWidget(m_widget.get());

  m_widget->setLeftLabel("Available Attributes");
  m_widget->setRightLabel("Selected Attributes");

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::layout::MapLayerChoiceOutside::~MapLayerChoiceOutside()
{
}

void te::layout::MapLayerChoiceOutside::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  std::vector <std::string> names;

  while(it != m_layers.end())
  {
    te::map::AbstractLayerPtr layer = it->get();
    names.push_back(layer->getTitle());
    ++it;
  }

  m_widget->setInputValues(names);      
}

te::map::AbstractLayerPtr te::layout::MapLayerChoiceOutside::getLayer()
{
  return m_selectedLayer;
}

te::da::DataSourcePtr te::layout::MapLayerChoiceOutside::getDataSource()
{
  return m_dataSource;
}

void te::layout::MapLayerChoiceOutside::onOkPushButtonClicked()
{
  m_layersOnTheRight = m_widget->getOutputValues();

  if(m_layersOnTheRight.empty())
  {  
    return;
  }

  std::vector<std::string>::iterator itString = m_layersOnTheRight.begin();
  for ( ; itString != m_layersOnTheRight.end() ; ++itString)
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
    for( ; it != m_layers.end() ; ++it)
    {
      te::map::AbstractLayerPtr layer = it->get();
      std::string nameLayer = layer->getTitle();

      std::string name = (*itString);
      if(nameLayer.compare(name) == 0)
      {
        m_layersSelected.push_back(layer);
      }
    }
  }    

  m_layersSelected.clear();
  
  QMessageBox::information(this, "Address Geocoding", "Test button OK.");
}

void te::layout::MapLayerChoiceOutside::onCancelPushButtonClicked()
{
  reject();
}

void te::layout::MapLayerChoiceOutside::updateObserver( ContextItem context )
{
  setVisible(context.isShow());
  if(context.isShow() == true)
    show();
  else
    hide();
}

void te::layout::MapLayerChoiceOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::MapLayerChoiceOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}


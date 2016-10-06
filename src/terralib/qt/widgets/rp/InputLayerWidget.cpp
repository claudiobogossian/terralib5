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
  \file terralib/qt/widgets/rp/InputLayerWidget.cpp

  \brief This file has the InputLayerWidget class.
*/

// TerraLib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../datasource/selector/DataSourceSelectorDialog.h"
#include "../layer/search/LayerSearchDialog.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../Utils.h"
#include "InputLayerWidget.h"
#include "ui_InputLayerWidgetForm.h"

// BOOST Includes
#include <boost/filesystem.hpp>

// QT
#include <QFileDialog>


te::qt::widgets::InputLayerWidget::InputLayerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
  m_ui(new Ui::InputLayerWidgetForm)
{
  m_ui->setupUi(this);

  m_layer = 0;

  connect(m_ui->m_searchLayerToolButton, SIGNAL(clicked()), this, SLOT(onSearchLayerToolButtonClicked()));
  connect(m_ui->m_layersComboBox, SIGNAL(activated(int)), this, SLOT(onLayersComboBoxActivated(int)));
}

te::qt::widgets::InputLayerWidget::~InputLayerWidget()
{
}

Ui::InputLayerWidgetForm* te::qt::widgets::InputLayerWidget::getForm() const
{
  return m_ui.get();
}

te::map::AbstractLayerPtr te::qt::widgets::InputLayerWidget::getInputLayer()
{
  return m_layer;
}

std::string te::qt::widgets::InputLayerWidget::getType() const
{
  return "GDAL";
}

std::map<std::string, std::string> te::qt::widgets::InputLayerWidget::getInfo() const
{
  std::map<std::string, std::string> rinfo;

  //rinfo["URI"] = m_ui->m_repositoryLineEdit->text().toUtf8().data();

  return rinfo;
}

void te::qt::widgets::InputLayerWidget::setInputLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;
}

void te::qt::widgets::InputLayerWidget::setLayerList(std::list<te::map::AbstractLayerPtr> list)
{
  m_layerList = list;

  int i = 0;

  std::list<te::map::AbstractLayerPtr>::iterator it = list.begin();
  while (it != list.end())
  {
    m_ui->m_layersComboBox->insertItem(i, it->get()->getTitle().c_str());

    it++;
    i++;
  }
}

void te::qt::widgets::InputLayerWidget::onSearchLayerToolButtonClicked()
{
  te::qt::widgets::LayerSearchDialog* dlg = new te::qt::widgets::LayerSearchDialog(this);
  dlg->getWidget()->filterOnlyByRaster();
  dlg->getWidget()->setList(m_layerList);
  if (dlg->exec())
  {
    std::list<te::map::AbstractLayerPtr> layerList = dlg->getWidget()->getSelecteds();

    std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

    m_layer = *it;

    std::list<te::map::AbstractLayerPtr>::iterator itLayer = std::find(m_layerList.begin(), m_layerList.end(), *it);
    const int pos = std::distance(m_layerList.begin(), itLayer);
    m_ui->m_layersComboBox->setCurrentIndex(pos);

    emit layerChanged();
  }
}

void te::qt::widgets::InputLayerWidget::onLayersComboBoxActivated(int index)
{
  auto l_front = m_layerList.begin();

  std::advance(l_front, index);
  m_layer = *l_front;

  emit layerChanged();
}

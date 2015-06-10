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
  \file terralib/qt/widgets/vector/FixGeometryWidget.cpp

  \brief A widget to fix geometries.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../Exception.h"
#include "FixGeometryWidget.h"
#include "ui_FixGeometryWidgetForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::FixGeometryWidget::FixGeometryWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::FixGeometryWidgetForm)
{
// add controls
  m_ui->setupUi(this);

// set button icons
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

//connects
  connect(m_ui->m_targetFileToolButton, SIGNAL(clicked()), this, SLOT(onTargetFileToolButtonPressed()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(clicked()), this, SLOT(onTargetDatasourceToolButtonPressed()));

 }

te::qt::widgets::FixGeometryWidget::~FixGeometryWidget()
{
}

Ui::FixGeometryWidgetForm* te::qt::widgets::FixGeometryWidget::getForm() const
{
  return m_ui.get();
}

bool te::qt::widgets::FixGeometryWidget::fixGeometry()
{
  return false;
}

void te::qt::widgets::FixGeometryWidget::setLayerList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layers = layerList;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while (it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if (dsType->hasGeom())
      m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

void te::qt::widgets::FixGeometryWidget::onTargetFileToolButtonPressed()
{

}

void te::qt::widgets::FixGeometryWidget::onTargetDatasourceToolButtonPressed()
{

}



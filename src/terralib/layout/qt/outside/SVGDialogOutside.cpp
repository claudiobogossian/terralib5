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
  \file terralib/layout/qt/outside/SVGDialogOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "SVGDialogOutside.h"
#include "../../../common/Logger.h"
#include "../../../qt/widgets/utils/DoubleListWidget.h"
#include "../../outside/SVGDialogModel.h"
#include "../../core/enum/Enums.h"
#include "../../core/property/GenericVariant.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "ui_SVGView.h"

// STL
#include <algorithm>

// Qt
#include <QGridLayout>
#include <QMessageBox>
#include <QString>
#include <QImage>
#include <QPainter>
#include <QIcon>

te::layout::SVGDialogOutside::SVGDialogOutside(AbstractOutsideController* controller)
  : QDialog(0),
    AbstractOutsideView(controller),
    m_ui(new Ui::SVGView), 
    m_property(0)
{
  m_ui->setupUi(this);

  connect(m_ui->OK, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->CANCEL, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::layout::SVGDialogOutside::~SVGDialogOutside()
{
}

void te::layout::SVGDialogOutside::init()
{
  m_initFile = "";

  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  SVGDialogModel* model = dynamic_cast<SVGDialogModel*>(abstractModel);
  if(!model)
  {
    return;
  }

  std::vector<std::string> paths = model->getPaths();

  QListWidgetItem* item = new QListWidgetItem(m_ui->listWidget);

  item->setData(Qt::DecorationRole, QIcon(QString::fromStdString(m_initFile)));

  m_ui->listWidget->addItem(item);
}

void te::layout::SVGDialogOutside::onOkPushButtonClicked()
{
  
}

void te::layout::SVGDialogOutside::onCancelPushButtonClicked()
{
  reject();
}

void te::layout::SVGDialogOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::SVGDialogOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::SVGDialogOutside::setPathsProperty( Property prop )
{
  m_property = prop;
}


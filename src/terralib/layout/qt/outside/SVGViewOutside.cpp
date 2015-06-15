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
  \file terralib/layout/qt/outside/SVGViewOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "../../../common/Logger.h"
#include "../../../qt/widgets/utils/DoubleListWidget.h"
#include "SVGViewOutside.h"
#include "../../outside/SVGViewModel.h"
#include "ui_SVGView.h"

// STL
#include <algorithm>

// Qt
#include <QGridLayout>
#include <QMessageBox>
#include "../../core/enum/Enums.h"
#include "../../core/property/GenericVariant.h"

te::layout::SVGViewOutside::SVGViewOutside(OutsideController* controller, Observable* o)
  : QDialog(0),
    OutsideObserver(controller, o),
    m_ui(new Ui::SVGView)
{
  m_ui->setupUi(this);
}

te::layout::SVGViewOutside::~SVGViewOutside()
{
}

void te::layout::SVGViewOutside::init()
{
  SVGViewOutside* model = dynamic_cast<SVGViewOutside*>(m_model);
  if(!model)
  {
    return;
  }  
}

void te::layout::SVGViewOutside::onOkPushButtonClicked()
{
  
}

void te::layout::SVGViewOutside::onCancelPushButtonClicked()
{
  reject();
}

void te::layout::SVGViewOutside::updateObserver( ContextItem context )
{
  setVisible(context.isShow());
  if(context.isShow() == true)
    show();
  else
    hide();
}

void te::layout::SVGViewOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::SVGViewOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}


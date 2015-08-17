/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ColorDialogOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ColorDialogOutside.h"
#include "../../outside/ColorDialogModel.h"
#include "../../../color/RGBAColor.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"

// Qt
#include <QColor>

te::layout::ColorDialogOutside::ColorDialogOutside(AbstractOutsideController* controller) :
  QColorDialog(0),
	AbstractOutsideView(controller),
  m_okClicked(false)
{
	connect(this, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(onCurrentColorChanged(const QColor&)));
}

te::layout::ColorDialogOutside::~ColorDialogOutside()
{
  
}

void te::layout::ColorDialogOutside::init()
{
	AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
	ColorDialogModel* model = dynamic_cast<ColorDialogModel*>(abstractModel);
  if(!model)
    return;

  Property prop_color = model->getColorProperty();

  te::color::RGBAColor rgbColor = prop_color.getValue().toColor();

  QColor color;
  color.setRed(rgbColor.getRed());
  color.setGreen(rgbColor.getGreen());
  color.setBlue(rgbColor.getBlue());
  color.setAlpha(rgbColor.getAlpha());

  setCurrentColor(color);
}

void te::layout::ColorDialogOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::ColorDialogOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::ColorDialogOutside::accept()
{
  m_okClicked = true;

	AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
	ColorDialogModel* model = dynamic_cast<ColorDialogModel*>(abstractModel);
  if(!model)
    return;

  Property prop_color = model->getColorProperty();

  te::color::RGBAColor rgbColor = prop_color.getValue().toColor();

  QColor color;
  color.setRed(rgbColor.getRed());
  color.setGreen(rgbColor.getGreen());
  color.setBlue(rgbColor.getBlue());
  color.setAlpha(rgbColor.getAlpha());
  
  if(color == m_currentColor)
  {
    return;
  }

  rgbColor.setColor(m_currentColor.red(), m_currentColor.green(), m_currentColor.blue(), m_currentColor.alpha());

  prop_color.setValue(rgbColor, prop_color.getType());

  model->setColorProperty(prop_color); // refresh property value

  emit updateProperty(prop_color);
}

void te::layout::ColorDialogOutside::onCurrentColorChanged( const QColor & color )
{
  m_currentColor = color;
}

bool te::layout::ColorDialogOutside::event( QEvent * e )
{
  return QColorDialog::event(e);
}

void te::layout::ColorDialogOutside::closeEvent( QCloseEvent * event )
{
  if(m_okClicked)
  {
    m_okClicked = false;
    event->accept();
  }
  else
  {
    QColorDialog::closeEvent(event);
  }
}






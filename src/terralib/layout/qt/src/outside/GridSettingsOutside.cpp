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
  \file GridSettingsOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridSettingsOutside.h"
#include "ui_GridSettings.h"
#include "OutsideObserver.h"
#include "OutsideController.h"

// STL
#include <string>

// Qt
#include <QColorDialog> 
#include <QFrame>
#include <QLineEdit>
#include <QValidator>
#include <QComboBox>
#include <QCheckBox> 
#include <QMessageBox>

te::layout::GridSettingsOutside::GridSettingsOutside( OutsideController* controller, Observable* o ) :
  QDialog(0),
  OutsideObserver(controller, o),
  m_ui(new Ui::GridSettings)
{
  m_ui->setupUi(this);

  init();
}

te::layout::GridSettingsOutside::~GridSettingsOutside()
{

}

void te::layout::GridSettingsOutside::init()
{
  m_ui->lneVrtPlanarDisplacement->setValidator(new  QDoubleValidator(this));
  m_ui->lneHrzPlanarDisplacement->setValidator(new  QDoubleValidator(this));

  m_ui->lneVrtGeoDisplacement->setValidator(new  QDoubleValidator(this));
  m_ui->lneHrzGeoDisplacement->setValidator(new  QDoubleValidator(this));

  m_ui->lneCornerHrzGeoDisplacement->setValidator(new  QDoubleValidator(this));
  m_ui->lneCornerVrtGeoDisplacement->setValidator(new  QDoubleValidator(this));

  m_ui->lneHrzPlanarGap->setValidator(new  QDoubleValidator(this));
  m_ui->lneVrtPlanarGap->setValidator(new  QDoubleValidator(this));

  m_ui->lneX1->setValidator(new  QDoubleValidator(this));
  m_ui->lneY1->setValidator(new  QDoubleValidator(this));
  m_ui->lneX2->setValidator(new  QDoubleValidator(this));
  m_ui->lneY2->setValidator(new  QDoubleValidator(this));
  m_ui->lneX3->setValidator(new  QDoubleValidator(this));
  m_ui->lneY3->setValidator(new  QDoubleValidator(this));
  m_ui->lneX4->setValidator(new  QDoubleValidator(this));
  m_ui->lneY4->setValidator(new  QDoubleValidator(this));
  m_ui->cmbScale->hide();
  m_ui->ckDefineScale->hide();
}

void te::layout::GridSettingsOutside::updateObserver( ContextItem context )
{
  setVisible(context.getShow());
  if(context.getShow() == true)
    show();
  else
    hide();
}

void te::layout::GridSettingsOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::GridSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::GridSettingsOutside::changeLineColor()
{
  configColor(m_ui->fraLineColor);
}

void te::layout::GridSettingsOutside::pbPlanarLineColor_clicked()
{
  configColor(m_ui->fraPlanarLineColor);
}

void te::layout::GridSettingsOutside::changeVerticalFontColor()
{
	 
}  

void te::layout::GridSettingsOutside::cmdGridTextPlanarColor_clicked()
{
  configColor(m_ui->fraGridTextPlanarColor);
}

void te::layout::GridSettingsOutside::cmdGridTextGeoColor_clicked()
{
  configColor(m_ui->fraGridTextGeoColor);
}

void te::layout::GridSettingsOutside::cmdCornerTextGeoColor_clicked()
{
  configColor(m_ui->fraCornerTextGeoColor);
}

 void te::layout::GridSettingsOutside::changeHorizontalFontColor()
 {
	
 }

 bool te::layout::GridSettingsOutside::checkValidDegreeValue(const QString &value)
 {
	int									degree = 0, minute = 0;
	float								second = 0;
	int									status = 0;
	std::basic_string <char>::size_type		index;
	std::string							strDegree = "";
	double								ret = 0;

	strDegree=value.toLatin1();
	if((index=strDegree.find("º")) !=std::string::npos)	
	{
		strDegree.replace(index,1,"");
	}
	if((index=strDegree.find("°")) !=std::string::npos)
	{
		strDegree.replace(index,1,"");
	}

	if((index=strDegree.find("'")) !=std::string::npos)	
	{
		strDegree.replace(index,1,"");
	}
	if((index=strDegree.find("'")) !=std::string::npos)	
	{
		strDegree.replace(index,1,"");
	}
	
	status=sscanf(strDegree.c_str(),"%d %d %f",&degree,&minute,&second);
	if(status!=3)	return false;

	return true;

 }

 void te::layout::GridSettingsOutside::okClicked()
 {
	 if(checkValidDegreeValue(m_ui->lneHorizontalGap->text()) == false)
	 {
		QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
		m_ui->lneHorizontalGap->setFocus();
		return;
	 }

	 if(checkValidDegreeValue(m_ui->lneVerticalGap->text()) == false)
	 {
		QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
		m_ui->lneVerticalGap->setFocus();
		return;
	 }
	 if(checkValidDegreeValue(m_ui->xGridInitialPoint_geo_textField->text()) == false)
	 {
		 QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
		 m_ui->lneVerticalGap->setFocus();
		 return;
	 }
	 if(checkValidDegreeValue(m_ui->yGridInitialPoint_geo_textField->text()) == false)
	 {
		 QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
		 m_ui->lneVerticalGap->setFocus();
		 return;
	 }
	 accept();
 }

 void te::layout::GridSettingsOutside::helpClicked()
 {

 }

 void te::layout::GridSettingsOutside::ckDefineScale_clicked()
 {
	 m_ui->cmbScale->setEnabled(m_ui->ckDefineScale->isChecked());
 }

 void te::layout::GridSettingsOutside::lneHorizontalGap_textChanged(const QString& value)
 {
	 
 }

 void te::layout::GridSettingsOutside::configColor( QWidget* widget )
 {
   QPalette ptt(widget->palette());
   QBrush brush = ptt.brush(widget->backgroundRole());

   QColor color = QColorDialog::getColor(brush.color(),this, "Line Color" );

   if(!color.isValid())	
     return;

   QPalette paltt(widget->palette());
   paltt.setColor(widget->backgroundRole(), color);
   widget->setPalette(paltt);
 }

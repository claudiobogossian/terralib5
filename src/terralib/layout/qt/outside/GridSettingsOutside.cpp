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
  \file GridSettingsOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridSettingsOutside.h"
#include "ui_GridSettings.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/pattern/mvc/OutsideController.h"
#include "../../outside/GridSettingsController.h"
#include "../../core/property/Variant.h"
#include "../../core/property/PlanarGridSettingsConfigProperties.h"
#include "../../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../../core/enum/Enums.h"
#include "../../outside/GridSettingsModel.h"

// STL
#include <string>
#include <sstream> 

// Qt
#include <QColorDialog> 
#include <QFrame>
#include <QLineEdit>
#include <QValidator>
#include <QComboBox>
#include <QCheckBox> 
#include <QMessageBox>
#include <QObjectList>

te::layout::GridSettingsOutside::GridSettingsOutside( OutsideController* controller, Observable* o ) :
  QDialog(0),
  OutsideObserver(controller, o),
  m_ui(new Ui::GridSettings)
{
  m_planarGridSettings = new PlanarGridSettingsConfigProperties;
  m_geodesicGridSettings = new GeodesicGridSettingsConfigProperties;

  EnumObjectType* objType = Enums::getInstance().getEnumObjectType();

  if(objType)
  {
    m_planarType = objType->getGridPlanarItem();
    m_geodesicType = objType->getGridGeodesicItem();
  }

  m_ui->setupUi(this);

  setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint 
    | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );

  init();
}

te::layout::GridSettingsOutside::~GridSettingsOutside()
{
  if(m_planarGridSettings)
  {
    delete m_planarGridSettings;
    m_planarGridSettings = 0;
  }

  if(m_geodesicGridSettings)
  {
    delete m_geodesicGridSettings;
    m_geodesicGridSettings = 0;
  }
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

  m_ui->fraCornerTextGeoColor->setAutoFillBackground(true);
  m_ui->fraGridTextGeoColor->setAutoFillBackground(true);
  m_ui->fraGridTextPlanarColor->setAutoFillBackground(true);
  m_ui->fraPlanarLineColor->setAutoFillBackground(true);  
}

void te::layout::GridSettingsOutside::updateObserver( ContextItem context )
{
  setVisible(context.isShow());
  if(context.isShow() == true)
    show();
  else
    hide();

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;
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

void te::layout::GridSettingsOutside::blockComponents()
{
  QObjectList list = children();

  QList<QObject*>::Iterator it;
  for(it = list.begin() ; it != list.end() ; ++it)
  {
    QObject* obj = (*it);
    QWidget* w = dynamic_cast<QWidget*>(obj);
    if(w)
    {
      if(w->isVisible())
      {
        w->setEnabled(false);
      }
    }
  }
}

void te::layout::GridSettingsOutside::unblockComponents()
{
  QObjectList list = children();

  QList<QObject*>::Iterator it;
  for(it = list.begin() ; it != list.end() ; ++it)
  {
    QObject* obj = (*it);
    QWidget* w = dynamic_cast<QWidget*>(obj);
    if(w)
    {
      if(w->isVisible())
      {
        w->setEnabled(true);
      }
    }
  }
}

bool te::layout::GridSettingsOutside::checkValidDegreeValue(const QString &value)
{
  int									degree = 0, minute = 0;
  float								second = 0;
  int									status = 0;
  std::basic_string <char>::size_type		index;
  std::string							strDegree = "";

  strDegree = std::string(value.toLatin1());
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

te::color::RGBAColor te::layout::GridSettingsOutside::configColor( QWidget* widget )
{
  te::color::RGBAColor rgbaColor;

  QPalette ptt(widget->palette());
  QBrush brush = ptt.brush(widget->backgroundRole());

  QColor bColor = brush.color();
  rgbaColor.setColor(bColor.red(), bColor.green(), bColor.blue());

  QColor color = QColorDialog::getColor(brush.color(),this, "Color" );

  if(!color.isValid())	
    return rgbaColor;

  QPalette paltt(widget->palette());
  paltt.setColor(widget->backgroundRole(), color);
  widget->setPalette(paltt);
  widget->setAutoFillBackground(true);

  rgbaColor.setColor(color.red(), color.green(), color.blue());

  return rgbaColor;
}

void te::layout::GridSettingsOutside::load()
{
  GridSettingsModel* model = dynamic_cast<GridSettingsModel*>(m_model);
  if(!model)
    return;

  if(!model->containsGrid(m_planarType))
  {
    m_ui->tabType->setTabEnabled(0, false);
  }
  
  if(!model->containsGrid(m_geodesicType))
  {
    m_ui->tabType->setTabEnabled(1, false);
  }
  else
  {
    m_ui->tbwSettings->setTabEnabled(2, false);
  }

  /* Grid */
  
  initCombo(m_ui->cmbUnit, m_planarGridSettings->getUnit(), m_planarType);
  
  initBool(m_ui->chkShowPlanar, m_planarGridSettings->getVisible(), m_planarType);

  initCombo(m_ui->cmbPlanarStyle, m_planarGridSettings->getStyle(), m_planarType);

  initCombo(m_ui->cmbGridStyle, m_geodesicGridSettings->getStyle(), m_geodesicType);
  
  initBool(m_ui->chkShowGeodesic, m_geodesicGridSettings->getVisible(), m_geodesicType);

  ///* Line */
  
  initDouble(m_ui->lneHrzPlanarGap, m_planarGridSettings->getLneHrzGap(), m_planarType);
  
  initDouble(m_ui->lneVrtPlanarGap, m_planarGridSettings->getLneVrtGap(), m_planarType);
  
  initDouble(m_ui->lneHorizontalGap, m_geodesicGridSettings->getLneHrzGap(), m_geodesicType);
  
  initDouble(m_ui->lneVerticalGap, m_geodesicGridSettings->getLneVrtGap(), m_geodesicType);

  initColor(m_ui->fraPlanarLineColor, m_planarGridSettings->getLineColor(), m_planarType);

  initCombo(m_ui->cmbPlanarLineType, m_planarGridSettings->getLineStyle(), m_planarType);

  initCombo(m_ui->cmbPlanarLineWidth, m_planarGridSettings->getLineWidth(), m_planarType);

  initCombo(m_ui->cmbLineType, m_geodesicGridSettings->getStyle(), m_geodesicType);

  initCombo(m_ui->cmbLineWidth, m_geodesicGridSettings->getLineWidth(), m_geodesicType);

  initColor(m_ui->fraLineColor, m_geodesicGridSettings->getLineColor(), m_geodesicType);

  ///*Text: Basic Configuration*/

  initCombo(m_ui->cmbPlanarTextSize, m_planarGridSettings->getPointTextSize(), m_planarType);

  initCombo(m_ui->cmbPlanarFont, m_planarGridSettings->getFontText(), m_planarType);

  initColor(m_ui->fraGridTextPlanarColor, m_planarGridSettings->getTextColor(), m_planarType);

  initBool(m_ui->chkSuperscriptPlanarText, m_planarGridSettings->getSuperscriptText(), m_planarType);

  initCombo(m_ui->cmbGeoFont, m_geodesicGridSettings->getFontText(), m_geodesicType);

  initCombo(m_ui->cmbGeoTextSize, m_geodesicGridSettings->getPointTextSize(), m_geodesicType);

  initColor(m_ui->fraGridTextGeoColor, m_geodesicGridSettings->getTextColor(), m_geodesicType);

  initBool(m_ui->chkSuperscriptGeoText, m_geodesicGridSettings->getSuperscriptText(), m_geodesicType);

  ///*Text: Advanced configuration*/

  initDouble(m_ui->xGridInitialPoint_planar_textField, m_planarGridSettings->getInitialGridPointX(), m_planarType);

  initDouble(m_ui->yGridInitialPoint_planar_textField, m_planarGridSettings->getInitialGridPointY(), m_planarType);

  initDouble(m_ui->xGridInitialPoint_geo_textField, m_geodesicGridSettings->getInitialGridPointX(), m_geodesicType);

  initDouble(m_ui->yGridInitialPoint_geo_textField, m_geodesicGridSettings->getInitialGridPointY(), m_geodesicType);

  initBool(m_ui->chkBottomPlanarText, m_planarGridSettings->getBottomText(), m_planarType);

  initBool(m_ui->chkLeftPlanarText, m_planarGridSettings->getLeftText(), m_planarType);

  initBool(m_ui->chkRightPlanarText, m_planarGridSettings->getRightText(), m_planarType);

  initBool(m_ui->chkTopPlanarText, m_planarGridSettings->getTopText(), m_planarType);

  initBool(m_ui->chkBottomGeoText, m_geodesicGridSettings->getBottomText(), m_geodesicType);

  initBool(m_ui->chkLeftGeoText, m_geodesicGridSettings->getLeftText(), m_geodesicType);

  initBool(m_ui->chkRightGeoText, m_geodesicGridSettings->getRightText(), m_geodesicType);

  initBool(m_ui->chkTopGeoText, m_geodesicGridSettings->getTopText(), m_geodesicType);

  initBool(m_ui->chkBottomRotatePlanarText, m_planarGridSettings->getBottomRotateText(), m_planarType);

  initBool(m_ui->chkLeftRotatePlanarText, m_planarGridSettings->getLeftRotateText(), m_planarType);

  initBool(m_ui->chkRightRotatePlanarText, m_planarGridSettings->getRightRotateText(), m_planarType);

  initBool(m_ui->chkTopRotatePlanarText, m_planarGridSettings->getTopRotateText(), m_planarType);

  initBool(m_ui->chkBottomRotateGeoText, m_geodesicGridSettings->getBottomRotateText(), m_geodesicType);

  initBool(m_ui->chkLeftRotateGeoText, m_geodesicGridSettings->getLeftRotateText(), m_geodesicType);

  initBool(m_ui->chkRightRotateGeoText, m_geodesicGridSettings->getRightRotateText(), m_geodesicType);

  initBool(m_ui->chkTopRotateGeoText, m_geodesicGridSettings->getTopRotateText(), m_geodesicType);

  initDouble(m_ui->lneVrtPlanarDisplacement, m_planarGridSettings->getLneVrtDisplacement(), m_planarType);

  initBool(m_ui->chkVisibleTextsPlanarText, m_planarGridSettings->getVisibleAllTexts(), m_planarType);

  initDouble(m_ui->lneHrzPlanarDisplacement, m_planarGridSettings->getLneHrzDisplacement(), m_planarType);

  initDouble(m_ui->lneVrtGeoDisplacement, m_geodesicGridSettings->getLneVrtDisplacement(), m_geodesicType);

  initDouble(m_ui->lneHrzGeoDisplacement, m_geodesicGridSettings->getLneHrzDisplacement(), m_geodesicType);

  initBool(m_ui->chkDegreesGeoText, m_geodesicGridSettings->getDegreesText(), m_geodesicType);

  initBool(m_ui->chkMinutesGeoText, m_geodesicGridSettings->getMinutesText(), m_geodesicType);

  initBool(m_ui->chkSecondsGeoText, m_geodesicGridSettings->getSecondsText(), m_geodesicType);

  initBool(m_ui->chkVisibleTextsGeoText, m_geodesicGridSettings->getVisibleAllTexts(), m_geodesicType);

  initCombo(m_ui->cmbCornerGeoFont, m_geodesicGridSettings->getFontTextCorner(), m_geodesicType);

  initCombo(m_ui->cmbCornerGeoTextSize, m_geodesicGridSettings->getPointTextSize(), m_geodesicType);
  
  initColor(m_ui->fraCornerTextGeoColor, m_geodesicGridSettings->getTextColorCorner(), m_geodesicType);
  
  initDouble(m_ui->lneCornerHrzGeoDisplacement, m_geodesicGridSettings->getLneCornerHrzDisplacement(), m_geodesicType);

  initDouble(m_ui->lneCornerVrtGeoDisplacement, m_geodesicGridSettings->getLneCornerVrtDisplacement(), m_geodesicType);
  
  initBool(m_ui->chkLowerRightCornerGeoText, m_geodesicGridSettings->getLowerRightCornerText(), m_geodesicType);

  initBool(m_ui->chkUpperRightCornerGeoText, m_geodesicGridSettings->getUpperRightCornerText(), m_geodesicType);

  initBool(m_ui->chkLowerLeftCornerGeoText, m_geodesicGridSettings->getLowerLeftCornerText(), m_geodesicType);

  initBool(m_ui->chkUpperLeftCornerGeoText, m_geodesicGridSettings->getUpperLeftCornerText(), m_geodesicType);
  
  /*Geodesic: Topographic Map*/
  
  initBool(m_ui->ckDefineScale, m_geodesicGridSettings->getDefineScale(), m_geodesicType);

  initCombo(m_ui->cmbScale, m_geodesicGridSettings->getScale(), m_geodesicType);

  initBool(m_ui->ckbClip, m_geodesicGridSettings->getClip(), m_geodesicType);

  initDouble(m_ui->lneX1, m_geodesicGridSettings->getLneX1(), m_geodesicType);

  initDouble(m_ui->lneX2, m_geodesicGridSettings->getLneX2(), m_geodesicType);

  initDouble(m_ui->lneY1, m_geodesicGridSettings->getLneY1(), m_geodesicType);

  initDouble(m_ui->lneY2, m_geodesicGridSettings->getLneY2(), m_geodesicType);

  initDouble(m_ui->lneX3, m_geodesicGridSettings->getLneX3(), m_geodesicType);

  initDouble(m_ui->lneX4, m_geodesicGridSettings->getLneX4(), m_geodesicType);

  initDouble(m_ui->lneY3, m_geodesicGridSettings->getLneY3(), m_geodesicType);

  initDouble(m_ui->lneY4, m_geodesicGridSettings->getLneY4(), m_geodesicType);
}

void te::layout::GridSettingsOutside::on_pbClose_clicked()
{
  accept();
}

void te::layout::GridSettingsOutside::on_helpPushButton_clicked()
{

}

void te::layout::GridSettingsOutside::on_cmbUnit_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_planarGridSettings->getUnit(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkShowPlanar_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkShowPlanar->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getVisible(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarStyle_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_planarGridSettings->getStyle(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbGridStyle_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getStyle(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkShowGeodesic_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkShowGeodesic->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getVisible(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneHrzPlanarGap_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneHrzPlanarGap->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_planarGridSettings->getLneHrzGap(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneVrtPlanarGap_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneVrtPlanarGap->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_planarGridSettings->getLneVrtGap(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneHorizontalGap_editingFinished()
{
  /*if(checkValidDegreeValue(m_ui->lneHorizontalGap->text()) == false)
  {
    QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
    m_ui->lneHorizontalGap->setFocus();
    return;
  }*/

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneHorizontalGap->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneHrzGap(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneVerticalGap_editingFinished()
{
  /*if(checkValidDegreeValue(m_ui->lneVerticalGap->text()) == false)
  {
    QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
    m_ui->lneVerticalGap->setFocus();
    return;
  }*/

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneVerticalGap->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneVrtGap(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_pbPlanarLineColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraPlanarLineColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(color, dataType->getDataTypeColor());
    Property prop = controller->updateProperty(m_planarGridSettings->getLineColor(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarLineType_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_planarGridSettings->getLineStyle(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarLineWidth_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toInt(), dataType->getDataTypeInt());
    Property prop = controller->updateProperty(m_planarGridSettings->getLineWidth(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbLineType_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLineStyle(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbLineWidth_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLineWidth(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_pbLineColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraLineColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(color, dataType->getDataTypeColor());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLineColor(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarTextSize_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toInt(), dataType->getDataTypeInt());
    Property prop = controller->updateProperty(m_planarGridSettings->getPointTextSize(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarFont_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_planarGridSettings->getFontText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_pbGridTextPlanarColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraGridTextPlanarColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(color, dataType->getDataTypeColor());
    Property prop = controller->updateProperty(m_planarGridSettings->getTextColor(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkSuperscriptPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkSuperscriptPlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getSuperscriptText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbGeoFont_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getFontText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbGeoTextSize_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toInt(), dataType->getDataTypeInt());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getPointTextSize(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_pbGridTextGeoColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraGridTextGeoColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(color, dataType->getDataTypeColor());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getTextColor(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkSuperscriptGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkSuperscriptGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getSuperscriptText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_xGridInitialPoint_planar_textField_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->xGridInitialPoint_planar_textField->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_planarGridSettings->getInitialGridPointX(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_yGridInitialPoint_planar_textField_editingFinished()
{
  /*  
  if(checkValidDegreeValue(m_ui->yGridInitialPoint_geo_textField->text()) == false)
  {
    QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
    m_ui->lneVerticalGap->setFocus();
    return;
  }*/

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->yGridInitialPoint_planar_textField->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_planarGridSettings->getInitialGridPointY(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_xGridInitialPoint_geo_textField_editingFinished()
{
  /*if(checkValidDegreeValue(m_ui->xGridInitialPoint_geo_textField->text()) == false)
  {
    QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
    m_ui->lneVerticalGap->setFocus();
    return;
  }*/

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->xGridInitialPoint_geo_textField->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getInitialGridPointX(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_yGridInitialPoint_geo_textField_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->yGridInitialPoint_geo_textField->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getInitialGridPointY(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkBottomPlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getBottomText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkLeftPlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getLeftText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkRightPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkRightPlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getRightText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkTopPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkTopPlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getTopText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkBottomGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getBottomText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkLeftGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLeftText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkRightGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkRightGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getRightText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkTopGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkTopGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getTopText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkBottomRotatePlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getBottomRotateText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkLeftRotatePlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getLeftRotateText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkRightRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkRightRotatePlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getRightRotateText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkTopRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkTopRotatePlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getTopRotateText(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkBottomRotateGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getBottomRotateText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkLeftRotateGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLeftRotateText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkRightRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkRightRotateGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getRightRotateText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkTopRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkTopRotateGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getTopRotateText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneVrtPlanarDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneVrtPlanarDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_planarGridSettings->getLneVrtDisplacement(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkVisibleTextsPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkVisibleTextsPlanarText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_planarGridSettings->getVisibleAllTexts(), variant, m_planarType);

    bool opt = m_ui->chkVisibleTextsPlanarText->isChecked();
    m_ui->chkBottomPlanarText->setChecked(opt);
    m_ui->chkLeftPlanarText->setChecked(opt);
    m_ui->chkRightPlanarText->setChecked(opt);
    m_ui->chkTopPlanarText->setChecked(opt);

    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneHrzPlanarDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneHrzPlanarDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_planarGridSettings->getLneHrzDisplacement(), variant, m_planarType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneVrtGeoDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneVrtGeoDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneHrzDisplacement(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneHrzGeoDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneHrzGeoDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneHrzDisplacement(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkDegreesGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkDegreesGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getDegreesText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkMinutesGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkMinutesGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getMinutesText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkSecondsGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkSecondsGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getSecondsText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkVisibleTextsGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkVisibleTextsGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getVisibleAllTexts(), variant, m_geodesicType);

    bool opt = m_ui->chkVisibleTextsGeoText->isChecked();
    m_ui->chkBottomGeoText->setChecked(opt);
    m_ui->chkLeftGeoText->setChecked(opt);
    m_ui->chkRightGeoText->setChecked(opt);
    m_ui->chkTopGeoText->setChecked(opt);

    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbCornerGeoFont_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getFontTextCorner(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbCornerGeoTextSize_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toInt(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getPointTextSizeCorner(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_pbCornerTextGeoColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraCornerTextGeoColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(color, dataType->getDataTypeColor());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getTextColorCorner(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneCornerHrzGeoDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneCornerHrzGeoDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneCornerHrzDisplacement(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneCornerVrtGeoDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneCornerVrtGeoDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneCornerVrtDisplacement(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLowerRightCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkLowerRightCornerGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLowerRightCornerText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkUpperRightCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkUpperRightCornerGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getUpperRightCornerText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLowerLeftCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkLowerLeftCornerGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLowerLeftCornerText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkUpperLeftCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkUpperLeftCornerGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getUpperLeftCornerText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkVisibleCornerTextsGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkVisibleCornerTextsGeoText->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getVisibleCornerTextsText(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_ckDefineScale_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->ckDefineScale->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getDefineScale(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbScale_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toInt(), dataType->getDataTypeString());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getScale(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_ckbClip_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->ckbClip->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getClip(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneX1_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneX1->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneX1(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneX2_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneX2->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneX2(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneY1_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneY1->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneY1(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneY2_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneY2->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneY2(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneX3_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneX3->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneX3(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneX4_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneX4->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneX4(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneY3_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneY3->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneY3(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneY4_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneY4->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->updateProperty(m_geodesicGridSettings->getLneY4(), variant, m_geodesicType);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::initString( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent, gridType);

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    edit->setText(prop.getValue().toString().c_str());
    return;
  }
}

void te::layout::GridSettingsOutside::initInt( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  std::ostringstream convert;
  Property prop = controller->getProperty(nameComponent, gridType);
  convert << prop.getValue().toInt();

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    edit->setText(convert.str().c_str());
  }
}

void te::layout::GridSettingsOutside::initDouble( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  std::ostringstream convert;
  convert.precision(15);
  Property prop = controller->getProperty(nameComponent, gridType);
  double number = prop.getValue().toDouble();
  convert << number;

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    edit->setText(convert.str().c_str());
  }
}

void te::layout::GridSettingsOutside::initBool( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent, gridType);

  QCheckBox* chk = dynamic_cast<QCheckBox*>(widget);
  
  if(chk)
  {
    chk->setChecked(prop.getValue().toBool());
  }
}

void te::layout::GridSettingsOutside::initColor( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent, gridType);
    
  te::color::RGBAColor color = prop.getValue().toColor();
  QColor qcolor(color.getRed(), color.getGreen(), color.getBlue());

  if(!qcolor.isValid())	
    return;

  if(!widget)
    return;

  QPalette paltt(widget->palette());
  paltt.setColor(widget->backgroundRole(), qcolor);
  widget->setPalette(paltt);
  widget->setAutoFillBackground(true);
}

void te::layout::GridSettingsOutside::initCombo( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent, gridType);

  QComboBox* combo = dynamic_cast<QComboBox*>(widget);

  if(!combo)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int index = -1;
  QVariant variant;

  if(prop.getType() == dataType->getDataTypeBool())
  {
    variant.setValue(prop.getValue().toBool());
  }
  else if(prop.getType() == dataType->getDataTypeDouble())
  {
    variant.setValue(prop.getValue().toDouble());
  }
  else if(prop.getType() == dataType->getDataTypeInt())
  {
    variant.setValue(prop.getValue().toInt());
  }
  else if(prop.getType() == dataType->getDataTypeString())
  {
    variant.setValue(QString(prop.getValue().toString().c_str()));
  }
  
  index = combo->findData(variant);
  if ( index != -1 ) 
  { 
    combo->setCurrentIndex(index);
  }
}




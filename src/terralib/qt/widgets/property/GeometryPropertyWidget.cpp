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
  \file terralib/qt/widgets/property/GeometryPropertyWidget.h

  \brief Defines the GeometryPropertyWidget class.
*/

// Terralib
#include "../../../geometry/Enums.h"
#include "../../../geometry/GeometryProperty.h"
#include "GeometryPropertyWidget.h"
#include "ui_GeometryPropertyWidgetForm.h"


te::qt::widgets::GeometryPropertyWidget::GeometryPropertyWidget(QWidget* parent)
  : SimplePropertyWidget(parent),
    m_ui(new Ui::GeometryPropertyWidgetForm)
{
  m_ui->setupUi(this);

  //fill combo box 
  m_ui->m_typeComboBox->addItem(tr("Geometry"), te::gm::GeometryType);
  m_ui->m_typeComboBox->addItem(tr("Geometry Z"), te::gm::GeometryZType);
  m_ui->m_typeComboBox->addItem(tr("Geometry M"), te::gm::GeometryMType);
  m_ui->m_typeComboBox->addItem(tr("Geometry ZM"), te::gm::GeometryZMType);

  m_ui->m_typeComboBox->addItem(tr("Point"), te::gm::PointType);
  m_ui->m_typeComboBox->addItem(tr("Point Z"), te::gm::PointZType);
  m_ui->m_typeComboBox->addItem(tr("Point M"), te::gm::PointMType);
  m_ui->m_typeComboBox->addItem(tr("Point ZM"), te::gm::PointZMType);
  m_ui->m_typeComboBox->addItem(tr("Point K dimensions"), te::gm::PointKdType);

  m_ui->m_typeComboBox->addItem(tr("Line String"), te::gm::LineStringType);
  m_ui->m_typeComboBox->addItem(tr("Line String Z"), te::gm::LineStringZType);
  m_ui->m_typeComboBox->addItem(tr("Line String M"), te::gm::LineStringMType);
  m_ui->m_typeComboBox->addItem(tr("Line String ZM"), te::gm::LineStringZMType);

  m_ui->m_typeComboBox->addItem(tr("Circular String"), te::gm::CircularStringType);
  m_ui->m_typeComboBox->addItem(tr("Circular String Z"), te::gm::CircularStringZType);
  m_ui->m_typeComboBox->addItem(tr("Circular String M"), te::gm::CircularStringMType);
  m_ui->m_typeComboBox->addItem(tr("Circular String ZM"), te::gm::CircularStringZMType);

  m_ui->m_typeComboBox->addItem(tr("Compound Curve"), te::gm::CompoundCurveType);
  m_ui->m_typeComboBox->addItem(tr("Compound Curve Z"), te::gm::CompoundCurveZType);
  m_ui->m_typeComboBox->addItem(tr("Compound Curve M"), te::gm::CompoundCurveMType);
  m_ui->m_typeComboBox->addItem(tr("Compound Curve ZM"), te::gm::CompoundCurveZMType);

  m_ui->m_typeComboBox->addItem(tr("Polygon"), te::gm::PolygonType);
  m_ui->m_typeComboBox->addItem(tr("Polygon Z"), te::gm::PolygonZType);
  m_ui->m_typeComboBox->addItem(tr("Polygon M"), te::gm::PolygonMType);
  m_ui->m_typeComboBox->addItem(tr("Polygon ZM"), te::gm::PolygonZMType);

  m_ui->m_typeComboBox->addItem(tr("Curve Polygon"), te::gm::CurvePolygonType);
  m_ui->m_typeComboBox->addItem(tr("Curve Polygon Z"), te::gm::CurvePolygonZType);
  m_ui->m_typeComboBox->addItem(tr("Curve Polygon M"), te::gm::CurvePolygonMType);
  m_ui->m_typeComboBox->addItem(tr("Curve Polygon ZM"), te::gm::CurvePolygonZMType);

  m_ui->m_typeComboBox->addItem(tr("Geometry Collection"), te::gm::GeometryCollectionType);
  m_ui->m_typeComboBox->addItem(tr("Geometry Collection Z"), te::gm::GeometryCollectionZType);
  m_ui->m_typeComboBox->addItem(tr("Geometry Collection M"), te::gm::GeometryCollectionMType);
  m_ui->m_typeComboBox->addItem(tr("Geometry Collection ZM"), te::gm::GeometryCollectionZMType);

  m_ui->m_typeComboBox->addItem(tr("Multi Point"), te::gm::MultiPointType);
  m_ui->m_typeComboBox->addItem(tr("Multi Point Z"), te::gm::MultiPointZType);
  m_ui->m_typeComboBox->addItem(tr("Multi Point M"), te::gm::MultiPointMType);
  m_ui->m_typeComboBox->addItem(tr("Multi Point ZM"), te::gm::MultiPointZMType);

  m_ui->m_typeComboBox->addItem(tr("Multi Line String"), te::gm::MultiLineStringType);
  m_ui->m_typeComboBox->addItem(tr("Multi Line String Z"), te::gm::MultiLineStringZType);
  m_ui->m_typeComboBox->addItem(tr("Multi Line String M"), te::gm::MultiLineStringMType);
  m_ui->m_typeComboBox->addItem(tr("Multi Line String ZM"), te::gm::MultiLineStringZMType);

  m_ui->m_typeComboBox->addItem(tr("Multi Polygon"), te::gm::MultiPolygonType);
  m_ui->m_typeComboBox->addItem(tr("Multi Polygon Z"), te::gm::MultiPolygonZType);
  m_ui->m_typeComboBox->addItem(tr("Multi Polygon M"), te::gm::MultiPolygonMType);
  m_ui->m_typeComboBox->addItem(tr("Multi Polygon ZM"), te::gm::MultiPolygonZMType);

  m_ui->m_typeComboBox->addItem(tr("Polyhedral Surface"), te::gm::PolyhedralSurfaceType);
  m_ui->m_typeComboBox->addItem(tr("Polyhedral Surface Z"), te::gm::PolyhedralSurfaceZType);
  m_ui->m_typeComboBox->addItem(tr("Polyhedral Surface M"), te::gm::PolyhedralSurfaceMType);
  m_ui->m_typeComboBox->addItem(tr("Polyhedral Surface ZM"), te::gm::PolyhedralSurfaceZMType);

  m_ui->m_typeComboBox->addItem(tr("TIN"), te::gm::TINType);
  m_ui->m_typeComboBox->addItem(tr("TIN Z"), te::gm::TINZType);
  m_ui->m_typeComboBox->addItem(tr("TIN M"), te::gm::TINMType);
  m_ui->m_typeComboBox->addItem(tr("TIN ZM"), te::gm::TINZMType);

  m_ui->m_typeComboBox->addItem(tr("Triangle"), te::gm::TriangleType);
  m_ui->m_typeComboBox->addItem(tr("Triangle Z"), te::gm::TriangleZType);
  m_ui->m_typeComboBox->addItem(tr("Triangle M"), te::gm::TriangleMType);
  m_ui->m_typeComboBox->addItem(tr("Triangle ZM"), te::gm::TriangleZMType);

  m_ui->m_typeComboBox->addItem(tr("Unknown"), te::gm::UnknownGeometryType);

}

te::qt::widgets::GeometryPropertyWidget::~GeometryPropertyWidget()
{

}

void te::qt::widgets::GeometryPropertyWidget::fillProperty(te::dt::SimpleProperty* sp)
{
  int index = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(index).toInt();

  te::gm::GeometryProperty* gp = dynamic_cast<te::gm::GeometryProperty*>(sp);

  if(gp)
  {
    gp->setGeometryType((te::gm::GeomType)type);

    if(m_ui->m_sridLineEdit->text().isEmpty() == false)
    {
      bool ok = false;
      int srid = m_ui->m_sridLineEdit->text().toInt(&ok);
      
      if(ok)
        gp->setSRID(srid);
    }
  }
}

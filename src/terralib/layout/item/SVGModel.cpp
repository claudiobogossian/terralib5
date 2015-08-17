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
  \file SVCModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SVGModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../core/pattern/mvc/AbstractItemModel.h"

te::layout::SVGModel::SVGModel()
  : AbstractItemModel()
{
  /*m_type = Enums::getInstance().getEnumObjectType()->getSVGItem();
  m_box = te::gm::Envelope(0., 0., 20., 20.);*/
}

te::layout::SVGModel::~SVGModel()
{
  
}

//te::layout::Properties* te::layout::SVGModel::getProperties() const
//{
//  ItemModelObservable::getProperties();
//
//  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
//
//  if(!dataType)
//    return m_properties;
//
//  std::string name = "Choice";
//
//  Property pro_svgchoice(m_hashCode);
//  pro_svgchoice.setName("sgvChoice");
//  pro_svgchoice.setValue(name, dataType->getDataTypeSVGView());
//  pro_svgchoice.setMenu(true);
//  
//  Property pro_svgPaths = getSVGPathsProperty();
//  pro_svgchoice.addSubProperty(pro_svgPaths);
//  
//  m_properties->addProperty(pro_svgchoice);
//
//  return m_properties;
//}
//
//void te::layout::SVGModel::updateProperties( te::layout::Properties* properties, bool notify )
//{
//  ItemModelObservable::updateProperties(properties, false);
//
//  Properties* vectorProps = const_cast<Properties*>(properties);  
//
//  //updateVisitors();
//
//  if(notify)
//  {
//    ContextItem context;
//    notifyAll(context);
//  }  
//}
//
//
//te::layout::Property te::layout::SVGModel::getSVGPathsProperty() const
//{
//  Property prop;
//  prop.setName("paths");
//
//  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
//
//  GenericVariant gv;
//
//  gv.setVector(m_pathsName, dataType->getDataTypeStringVector());
//
//  prop.setValue(gv, dataType->getDataTypeGenericVariant());
//
//  prop.setEditable(false);
//  prop.setVisible(false);
//  return prop;
//}
//
//void te::layout::SVGModel::setPathsName( std::vector<std::string> pathVector )
//{
//  m_pathsName = pathVector;
//}
//
//

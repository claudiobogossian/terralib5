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
  \file ImageModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ImageModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/property/Property.h"
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/property/Properties.h"
#include "../core/enum/Enums.h"

te::layout::ImageModel::ImageModel() :
  m_fileName(""),
  m_imgType(te::map::JPEG)
{
  m_borderColor = te::color::RGBAColor(0, 0, 0, 255);
  m_backgroundColor = te::color::RGBAColor(0, 0, 255, 255);

  m_box = te::gm::Envelope(0., 0., 90., 90.);

  m_properties->setHasWindows(true);
}

te::layout::ImageModel::~ImageModel()
{

}

void te::layout::ImageModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);
  
  drawBackground(context);

  if(m_fileName.compare("") != 0)
  {
    m_imgType = utils->getFileExtensionType(m_fileName);
    m_fileExtension = utils->getFileExtension(m_fileName);
    utils->drawImage(m_fileName, m_box);
  }
  
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::ImageModel::setFileName( std::string fileName )
{
  m_fileName = fileName;
}

std::string te::layout::ImageModel::getFileName()
{
  return m_fileName;
}

te::layout::Properties* te::layout::ImageModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_fileName;
  pro_fileName.setName("fileName");
  pro_fileName.setId("");
  pro_fileName.setValue(m_fileName, dataType->getDataTypeImage());
  m_properties->addProperty(pro_fileName);
  
  return m_properties;
}

void te::layout::ImageModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_fileName = vectorProps->contains("fileName");

  if(!pro_fileName.isNull())
  {
    m_fileName = pro_fileName.getValue().toString();
  }
}

std::string te::layout::ImageModel::getFileExtension()
{
  return m_fileExtension;
}

te::map::ImageType te::layout::ImageModel::getFileType()
{
  return m_imgType;
}

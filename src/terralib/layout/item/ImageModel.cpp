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
#include "../core/pattern/singleton/Context.h"

te::layout::ImageModel::ImageModel() :
  m_fileName(""),
  m_imgType(te::map::JPEG)
{
  m_type = Enums::getInstance().getEnumObjectType()->getImageItem();

  m_backgroundColor = te::color::RGBAColor(0, 0, 255, 0);

  m_box = te::gm::Envelope(0., 0., 90., 90.);

  m_properties->setHasWindows(true);

  m_border = true;
}

te::layout::ImageModel::~ImageModel()
{

}

te::layout::Properties* te::layout::ImageModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_fileName(m_hashCode);
  pro_fileName.setName("fileName");
  pro_fileName.setValue(m_fileName, dataType->getDataTypeImage());
  pro_fileName.setMenu(true);
  m_properties->addProperty(pro_fileName);
  
  return m_properties;
}

void te::layout::ImageModel::updateProperties( te::layout::Properties* properties, bool notify )
{
  ItemModelObservable::updateProperties(properties, false);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_fileName = vectorProps->getProperty("fileName");

  if(!pro_fileName.isNull())
  {
    m_fileName = pro_fileName.getValue().toString();
  }

  if(notify)
  {
    ContextItem context;
    notifyAll(context);
  }
}

void te::layout::ImageModel::setFileName( const std::string& fileName )
{
  m_fileName = fileName;
}

const std::string& te::layout::ImageModel::getFileName() const
{
  return m_fileName;
}

const std::string& te::layout::ImageModel::getFileExtension() const
{
  return m_fileExtension;
}

te::map::ImageType te::layout::ImageModel::getFileType()
{
  return m_imgType;
}


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
#include "../../common/Translator.h"
#include "../../common/Exception.h"

// STL
#include <iostream>

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
  
  canvas->setPolygonContourWidth(2);
  canvas->setPolygonContourColor(m_borderColor);
  canvas->setPolygonFillColor(m_backgroundColor);
  
  utils->drawRectW(m_box);

  std::ifstream::pos_type size;
  char* img = imageToChar(size);
  if(img)
  {
    canvas->drawImage((int)m_box.getLowerLeftX(), (int)m_box.getLowerLeftY(), img, size, m_imgType);
    delete[] img;
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

  Property pro_fileName;
  pro_fileName.setName("fileName");
  pro_fileName.setId("");
  pro_fileName.setValue(m_fileName, DataTypeImage);
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
    changeExtension();
  }
}

char* te::layout::ImageModel::imageToChar(std::ifstream::pos_type &size)
{
  char* memblock = 0;

  if(m_fileName.compare("") == 0)
    return memblock;

  try 
  { 
    std::ifstream file (m_fileName, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
      size = file.tellg();
      memblock = new char[size]; 
      file.seekg (0, ios::beg);
      file.read((char*)memblock, size); // cast to a char* to give to file.read

      file.close();
    }
  }
  catch (std::ifstream::failure &e) 
  {
    std::cerr << e.what() << std::endl;
    std::string errmsg = "Exception opening/reading/closing file: \n ";
    te::common::Exception ex(TE_TR(errmsg));
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return memblock;
}

std::string te::layout::ImageModel::getFileExtension()
{
  std::string fileName = m_fileName;
  std::string extension = fileName.substr(fileName.find_last_of("/\\.") + 1);
  return extension;
}

void te::layout::ImageModel::changeExtension()
{
  std::string extension = getFileExtension();

  if(extension.compare("") == 0)
    return;
  
  if(extension.compare("png") == 0)
  {
    m_imgType = te::map::PNG;
  }
  else if(extension.compare("bmp") == 0)
  {
    m_imgType = te::map::BMP;
  }
  else if(extension.compare("jpeg") == 0 || extension.compare("jpg") == 0)
  {
    m_imgType = te::map::JPEG;
  }
  else if(extension.compare("gif") == 0)
  {
    m_imgType = te::map::GIF;
  }
  else if(extension.compare("tiff") == 0)
  {
    m_imgType = te::map::TIFF;
  }
}

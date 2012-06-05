/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/CharacterMarkFactory.cpp

  \brief A concrete factory based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../maptools/Utils.h"
#include "../../../se/Mark.h"
#include "../Utils.h"
#include "CharacterMarkFactory.h"
#include "Utils.h"

// STL
#include <algorithm>

// Factory key
std::string te::qt::widgets::CharacterMarkFactory::sm_factoryKey("ttf");

// Global factory
te::qt::widgets::CharacterMarkFactory* te::qt::widgets::CharacterMarkFactory::sm_factory(0);

void te::qt::widgets::CharacterMarkFactory::initialize()
{
  finalize();
  sm_factory = new CharacterMarkFactory;
}

void te::qt::widgets::CharacterMarkFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

QString te::qt::widgets::CharacterMarkFactory::encode(const QString& font, const int& charCode)
{
  QString result = QString::fromStdString(sm_factoryKey);
  result += "://" + font + "#0x" + QString::number(charCode, 16);
  return result;
}

void te::qt::widgets::CharacterMarkFactory::decode(QString& name, QString& font, QChar& charCode)
{
  // Extract the part important to this factory!
  QString pattern = name.remove(0, sm_factoryKey.size());
  if(!pattern.contains("#"))
    return; // TODO: Exception: Bad format.

  // Tokenizes based on '#" separator. The first value is the font and the second is the char code...
  QStringList myParams = pattern.split("#", QString::SkipEmptyParts);
  if(myParams.size() < 2)
    return; // TODO: Exception: Bad format.

  // Getting char
  bool wasConverted = false;
  // Base 0: if the string begins with "0x", base 16 is used; if the string begins with "0", base 8 is used; otherwise, base 10 is used.
  charCode = myParams[1].toInt(&wasConverted, 0);
  if(!wasConverted)
    return; // TODO: Exception: Invalid char code.

  // The font name
  font = myParams[0];
}

te::qt::widgets::CharacterMarkFactory::~CharacterMarkFactory()
{
}

te::map::AbstractMarkFactory* te::qt::widgets::CharacterMarkFactory::build()
{
  return sm_factory;
}

te::color::RGBAColor** te::qt::widgets::CharacterMarkFactory::create(const te::se::Mark* mark, std::size_t size)
{
  // Decoding...
  QChar ch;
  QString fontName;
  QString name(mark->getWellKnownName()->c_str());
  te::qt::widgets::CharacterMarkFactory::decode(name, fontName, ch); // Can throw exceptions!

  // Configuring the font
  QFont font;
  font.setFamily(fontName);
  font.setPixelSize(size);

  // Bulding the character path
  QPainterPath charPath;
  charPath.addText(0.0, 0.0, font, ch);
   // Adjusting...
  QRectF bounds = charPath.boundingRect();
  double invMax =  1 / std::max(bounds.width(), bounds.height());
  QTransform t;
  t.scale(invMax, invMax);
  t.translate(-bounds.center().x(), -bounds.center().y());
  charPath = t.map(charPath);

  // Creates the image that will represent the graphical mark pattern
  QImage* img = new QImage(size, size, QImage::Format_ARGB32_Premultiplied);
  img->fill(Qt::transparent);

  // Configuring visual...
  te::qt::widgets::Config(m_pen, mark->getStroke());
  te::qt::widgets::Config(m_brush, mark->getFill());

  // Let's draw the char path!
  draw(img, charPath);

  // Converts QImage to te::color::RGBA**
  te::color::RGBAColor** rgba = te::qt::widgets::GetImage(img);

  delete img;

  return rgba;
}

void te::qt::widgets::CharacterMarkFactory::getSupportedMarks(std::vector<std::string>& marks) const
{
}

void te::qt::widgets::CharacterMarkFactory::setup(QImage* img)
{
  m_painter.begin(img);
  m_painter.setRenderHints(QPainter::Antialiasing);
  m_painter.setPen(m_pen);
  m_painter.setBrush(m_brush);
}

void te::qt::widgets::CharacterMarkFactory::end()
{
  m_painter.end();
  m_pen = QPen(QColor(TE_SE_DEFAULT_STROKE_BASIC_COLOR));
  m_brush = QBrush(QColor(TE_SE_DEFAULT_FILL_BASIC_COLOR), Qt::SolidPattern);
}

void te::qt::widgets::CharacterMarkFactory::draw(QImage* img, QPainterPath& path)
{
  setup(img);

  // Transformation parameters
  double s = img->width() - m_pen.width() - 1;
  double t = img->width() * 0.5;

  QTransform transform;
  QPainterPath transformedPath = transform.translate(t, t).scale(s, s).map(path);

  m_painter.drawPath(transformedPath);

  end();
}

bool te::qt::widgets::CharacterMarkFactory::getChar(QString& charCode, QChar& ch)
{
  bool isOk = false;
  // Base 0: if the string begins with "0x", base 16 is used; if the string begins with "0", base 8 is used; otherwise, base 10 is used.
  ch = charCode.toInt(&isOk, 0);
  return isOk;
}

te::qt::widgets::CharacterMarkFactory::CharacterMarkFactory()
  : te::map::AbstractMarkFactory(sm_factoryKey)
{
  m_brush.setStyle(Qt::SolidPattern);
  m_brush.setColor(QColor(TE_SE_DEFAULT_FILL_BASIC_COLOR));
}

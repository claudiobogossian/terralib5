/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/Utils.cpp
   
  \brief This file contains utility functions for dealing with Symbology Enconding and Qt concepts.
 */

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../../../maptools/Utils.h"
#include "../../../se/Stroke.h"
#include "../../../se/SvgParameter.h"
#include "../../../xml/Reader.h"
#include "../../../xml/ReaderFactory.h"
#include "../Exception.h"
#include "Symbol.h"
#include "SymbolLibrary.h"
#include "SymbolLibraryManager.h"
#include "Utils.h"

// Boost
#include <boost/format.hpp>

// Qt
#include <QtCore/QVector>
#include <QtGui/QBrush>
#include <QtGui/QPen>

// STL
#include <cassert>

void te::qt::widgets::Config(QPen& pen, const te::se::Stroke* stroke)
{
  if(stroke == 0)
  {
    pen.setColor(Qt::transparent);
    return;
  }

  // Color
  te::color::RGBAColor rgba(TE_SE_DEFAULT_STROKE_BASIC_COLOR, TE_OPAQUE);
  te::map::GetColor(stroke, rgba);
  QColor qrgba(rgba.getRgba());
  qrgba.setAlpha(rgba.getAlpha());
  pen.setColor(qrgba);

  // Width
  const te::se::SvgParameter* width = stroke->getWidth();
  if(width)
    pen.setWidth(te::map::GetInt(width));

  // Cap Style
  const te::se::SvgParameter* linecap = stroke->getLineCap();
  if(linecap)
  {
    std::string capValue = te::map::GetString(linecap);

    Qt::PenCapStyle capStyle = Qt::FlatCap;
    capValue == TE_SE_ROUND_CAP ? capStyle = Qt::RoundCap : capStyle = Qt::SquareCap;

    pen.setCapStyle(capStyle);
  }
  
  // Join Style
  const te::se::SvgParameter* linejoin = stroke->getLineJoin();
  if(linejoin)
  {
    std::string joinValue = te::map::GetString(linejoin);

    Qt::PenJoinStyle joinStyle = Qt::MiterJoin;
    joinValue == TE_SE_ROUND_JOIN ? joinStyle = Qt::RoundJoin : joinStyle = Qt::BevelJoin;

    pen.setJoinStyle(joinStyle);
  }

  // Dash Style
  const te::se::SvgParameter* dasharray = stroke->getDashArray();
  if(dasharray)
  {
    std::string value = te::map::GetString(dasharray);
    
    std::vector<double> pattern;
    te::map::GetDashStyle(value, pattern);
    
    pen.setDashPattern(QVector<qreal>::fromStdVector(pattern));
  }

  /* TODO: Is necessary verify stroke-dashoffset, <GraphicStroke>, and <GraphicFill> elements here ?! */
}

void te::qt::widgets::Config(QBrush& brush, const te::se::Fill* fill)
{
  if(fill == 0)
  {
    brush.setColor(Qt::transparent);
    return;
  }

  te::color::RGBAColor rgba(TE_SE_DEFAULT_FILL_BASIC_COLOR, TE_OPAQUE);
  te::map::GetColor(fill, rgba);
  QColor qrgba(rgba.getRgba());
  qrgba.setAlpha(rgba.getAlpha());
  brush.setColor(qrgba);

  /* TODO: Is necessary verify <GraphicFill> element here ?! */
}

void te::qt::widgets::ReadSymbolLibrary(const std::string& path)
{
  std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));
  reader->read(path);

  if(!reader->next())
    throw Exception((boost::format(TR_QT_WIDGETS("Could not read symbol library information in file: %1%.")) % path).str());

  if(reader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception((boost::format(TR_QT_WIDGETS("Error reading the document %1%, the start element wasn't found.")) % path).str());

  if(reader->getElementLocalName() != "SymbolLibrary")
    throw Exception((boost::format(TR_QT_WIDGETS("The first tag in the document %1% is not 'SymbolLibrary'.")) % path).str());

  std::string name = reader->getAttr("name");

  std::auto_ptr<SymbolLibrary> library(new SymbolLibrary(name));

  reader->next();

  while((reader->getNodeType() == te::xml::START_ELEMENT) &&
        (reader->getElementLocalName() == "Symbol"))
  {
    Symbol* symbol = ReadSymbol(*reader);
    library->insert(symbol);
  }

  SymbolLibraryManager::getInstance().insert(library.release());

  assert(reader->getNodeType() == te::xml::END_DOCUMENT);
}

te::qt::widgets::Symbol* te::qt::widgets::ReadSymbol(te::xml::Reader& reader)
{
  std::auto_ptr<te::qt::widgets::Symbol> symbol(new te::qt::widgets::Symbol);

  te::qt::widgets::SymbolInfo info;

  // Symbol Id
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Id");
  reader.next();
  info.m_id = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  // Symbol Name
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Name");
  reader.next();
  info.m_name = reader.getElementValue();
  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  // Symbol Author
  if(reader.getElementLocalName() == "Author")
  {
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    reader.next();
    info.m_author = reader.getElementValue();
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // Symbol Tags
  if(reader.getElementLocalName() == "Tags")
  {
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    reader.next();
    info.m_tags = reader.getElementValue();
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // Symbol Description
  if(reader.getElementLocalName() == "Description")
  {
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    reader.next();
    info.m_description = reader.getElementValue();
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  symbol->setInfo(info);

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName().find("Symbolizer") != std::string::npos);

  // TODO: *** Need move the OGC Symbology Enconding serialization methods from <te::serialization> module to <te::se> module! ***
  while((reader.getNodeType() == te::xml::START_ELEMENT) &&
        (reader.getElementLocalName().find("Symbolizer") != std::string::npos))
    //symbol->addSymbolizer(te::serialize::Symbolizer::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return symbol.release();
}

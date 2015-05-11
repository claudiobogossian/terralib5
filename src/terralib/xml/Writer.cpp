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

// TerraLib
#include "Writer.h"

// STL
#include <iosfwd>
#include <iostream>

// Boost
#include <boost/lexical_cast.hpp>

te::xml::Writer::Writer()
  : m_ostr(0),
    m_isOpened(false)
{
  //m_ostr = new std::ofstream();//m_uri.c_str(), std::ios_base::trunc);
  /*
  std::ofstream fout(uri.c_str(), std::ios_base::trunc);

  te::xml::Writer w(fout);

  Project p(project);

  XMLFormatter::format(&p, true);

  Save(p, w);

  XMLFormatter::format(&p, false);

  fout.close();*/
}

/*
te::xml::Writer::Writer(std::ostream& ostr)
  : m_ostr(ostr),
    m_isOpened(false)
{
}
*/

te::xml::Writer::~Writer()
{
  if(m_ostr)
    delete m_ostr;
}

void te::xml::Writer::writeStartDocument(const std::string& encoding, const std::string& standalone)
{
  *m_ostr << "<?xml version=\"1.0\" encoding=\"" << encoding << "\" standalone=\"" << standalone << "\"?>";
}

void te::xml::Writer::writeStartElement(const std::string& qName)
{
  if(m_isOpened)
    *m_ostr << ">";

  *m_ostr << "<" << qName;

  m_isOpened = true;
}

std::string Function2Ascii(std::string funcName)
{
  if(funcName == "<")
    return "&#60;";
  else if(funcName == ">")
    return "&#62;";
  else if(funcName == "<>")
    return "&#60;&#62;";
  else if(funcName == "<=")
    return "&#60;=";
  else if(funcName == ">=")
    return "&#62;=";
  else
    return funcName;
}

void te::xml::Writer::writeElement(const std::string& qName, const std::string& value)
{
  std::string v = value;

  v = Function2Ascii(v);

  if(m_isOpened)
  {
    *m_ostr << ">";
    m_isOpened = false;
  }

  *m_ostr << "<" << qName << ">" << v << "</" << qName << ">";
}

void te::xml::Writer::writeElement(const std::string& qName, const double& value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeElement(const std::string& qName, boost::int32_t value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeElement(const std::string& qName, boost::uint32_t value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeElement(const std::string& qName, boost::int64_t value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeElement(const std::string& qName, boost::uint64_t value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeAttribute(const std::string& attName, const std::string& value)
{
  *m_ostr << " " << attName << "=\"" << value << "\"";
}

void te::xml::Writer::writeAttribute(const std::string& attName, const double& value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeAttribute(const std::string& attName, boost::int32_t value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeAttribute(const std::string& attName, boost::uint32_t value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeAttribute(const std::string& attName, boost::int64_t value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeAttribute(const std::string& attName, boost::uint64_t value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeValue(const std::string& value)
{
  if(m_isOpened)
  {
    *m_ostr << ">";
    m_isOpened = false;
  }

  *m_ostr << value;
}

void te::xml::Writer::writeValue(const double& value)
{
  writeValue(boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeValue(boost::int32_t value)
{
  writeValue(boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeValue(boost::uint32_t value)
{
  writeValue(boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeValue(boost::int64_t value)
{
  writeValue(boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeValue(boost::uint64_t value)
{
  writeValue(boost::lexical_cast<std::string>(value));
}

void te::xml::Writer::writeEndElement(const std::string& qName)
{
  if(m_isOpened)
  {
    *m_ostr << ">";
    m_isOpened = false;
  }

  *m_ostr << "</" << qName << ">";
}

void te::xml::Writer::writeToFile()
{

}
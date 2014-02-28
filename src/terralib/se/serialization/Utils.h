/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/serialization/Utils.h
 
  \brief Utility methods for the Symbology serialization.
*/

#ifndef __TERRALIB_SE_SERIALIZATION_INTERNAL_UTILS_H
#define __TERRALIB_SE_SERIALIZATION_INTERNAL_UTILS_H

// STL
#include <string>

namespace te
{
  namespace fe { class PropertyName; } 

  namespace xl { class SimpleLink; }

  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace se
  {
    class ParameterValue;
    class SelectedChannel;
    class Symbolizer;

    namespace serialize
    {
      void WriteStringPtrHelper(const std::string& elementName, const std::string* s, te::xml::Writer& writer);

      void WriteParameterValuePtrHelper(const std::string& elementName, const te::se::ParameterValue* p, te::xml::Writer& writer);

      void WriteBaseSymbolizerHelper(const te::xl::SimpleLink* link, te::xml::Writer& writer);
    
      void WriteOnlineResourceHelper(const te::xl::SimpleLink* link, te::xml::Writer& writer);

      void WriteSymbolizerHelper(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer);

      void ReadSymbolizerHelper(te::se::Symbolizer* symbolizer, te::xml::Reader& reader);

      void WriteSelectedChannelHelper(const std::string& elementName, const te::se::SelectedChannel* sc, te::xml::Writer& writer);

      void WriteGeometryPropertyHelper(const te::fe::PropertyName* p, te::xml::Writer& writer);

      te::fe::PropertyName* ReadGeometryPropertyHelper(te::xml::Reader& reader);
    }  // end namespace serialize
  }    // end namespace se
}      // end namespace te

#endif  // __TERRALIB_SE_SERIALIZATION_INTERNAL_UTILS_H

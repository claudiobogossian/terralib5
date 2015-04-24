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
  \file terralib/xlink/serialization/xml/XLinkSerializer.h

  \brief Data serialization for the XLink module.
*/

#ifndef __TERRALIB_XLINK_INTERNAL_XLINKSERIALIZER_H
#define __TERRALIB_XLINK_INTERNAL_XLINKSERIALIZER_H

// TerraLib
#include "terralib_config.h"
#include "../../Config.h"

#ifdef TERRALIB_MOD_XML_ENABLED

namespace te
{
  namespace xml
  {
    class AbstractWriter;
    class Reader;
  }

  namespace xl
  {
    class SimpleLink;

    namespace serialize
    {
      TEXLEXPORT SimpleLink* ReadSimpleLink(te::xml::Reader& reader);

      TEXLEXPORT void Save(const SimpleLink* link, te::xml::AbstractWriter& writer);
    }  // end namespace serialize
  }    // end namespace xl
}      // end namespace te

#endif // TERRALIB_MOD_XML_ENABLED

#endif  // __TERRALIB_XLINK_INTERNAL_XLINKSERIALIZER_H

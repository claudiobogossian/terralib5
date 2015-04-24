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
  \file terralib/fe/serialization/xml/Filter.h
 
  \brief Support for the Filter serialization.
*/

#ifndef __TERRALIB_FE_SERIALIZATION_XML_INTERNAL_FILTER_H
#define __TERRALIB_FE_SERIALIZATION_XML_INTERNAL_FILTER_H

// TerraLib
#include "../../Config.h"

namespace te
{
  namespace fe { class Filter; }

  namespace xml
  {
    class AbstractWriter;
    class Reader;
  }

  namespace fe
  {
    class Filter;

    namespace serialize
    {
      TEFEEXPORT te::fe::Filter* ReadFilter(te::xml::Reader& reader);

      TEFEEXPORT void Save(const te::fe::Filter* filter, te::xml::AbstractWriter& writer);

    } // end namespace serialize
  }   // end namespace fe
}     // end namespace te

#endif  // __TERRALIB_FE_SERIALIZATION_XML_INTERNAL_FILTER_H

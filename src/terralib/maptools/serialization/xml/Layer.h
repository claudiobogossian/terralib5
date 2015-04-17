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
  \file src/terralib/maptools/serialization/xml/Layer.h
   
  \brief Auxiliary classes and functions to read layer information from a XML document.
*/

#ifndef __TERRALIB_MAPTOOLS_SERIALIZATION_XML_INTERNAL_LAYER_H
#define __TERRALIB_MAPTOOLS_SERIALIZATION_XML_INTERNAL_LAYER_H

// TerraLib
#include "../../../common/Singleton.h"
#include "../../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/function.hpp>

namespace te
{
  namespace xml
  {
    class AbstractWriter;
    class Reader;
  }

  namespace map
  {
    class AbstractLayer;

    namespace serialize
    {
      class TEMAPEXPORT Layer : public te::common::Singleton<Layer>
      {
        friend class te::common::Singleton<Layer>;

        public:

          typedef boost::function<te::map::AbstractLayer* (te::xml::Reader&)> LayerReadFnctType;
          typedef boost::function<void (const te::map::AbstractLayer*, te::xml::AbstractWriter&)> LayerWriteFnctType;
          typedef std::pair<LayerReadFnctType, LayerWriteFnctType> LayerFnctSerializeType;
          typedef std::map<std::string, LayerFnctSerializeType> LayerFnctIdxType;

          void reg(const std::string& layerType, const LayerFnctSerializeType& fncts);

          te::map::AbstractLayer* read(te::xml::Reader& reader) const;

          void write(const te::map::AbstractLayer* alayer, te::xml::AbstractWriter& writer) const;

          ~Layer();

        protected:

          Layer();

        private:

          LayerFnctIdxType m_fncts;
      };

    }  //end namespace serialize
  }    // end namespace map
}      // end namespace te

#endif  // __TERRALIB_MAPTOOLS_SERIALIZATION_XML_INTERNAL_LAYER_H

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
  \file terralib/se/serialization/Symbolizer.h
   
  \brief Auxiliary classes and functions to serialize Symbolizer informations from a XML document.
*/

#ifndef __TERRALIB_SE_SERIALIZATION_INTERNAL_SYMBOLIZER_H
#define __TERRALIB_SE_SERIALIZATION_INTERNAL_SYMBOLIZER_H

// TerraLib
#include "../../common/Singleton.h"
#include "../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/function.hpp>

namespace te
{
  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace se
  {
    class Symbolizer;

    namespace serialize
    {
      class TESEEXPORT Symbolizer : public te::common::Singleton<Symbolizer>
      {
        friend class te::common::Singleton<Symbolizer>;

        public:

          typedef boost::function<te::se::Symbolizer* (te::xml::Reader&)> SymbolizerReadFnctType;
          typedef boost::function<void (const te::se::Symbolizer*, te::xml::Writer&)> SymbolizerWriteFnctType;
          typedef std::pair<SymbolizerReadFnctType, SymbolizerWriteFnctType> SymbolizerFnctSerializeType;
          typedef std::map<std::string, SymbolizerFnctSerializeType> SymbolizerFnctIdxType;

          void reg(const std::string& symbolizerType, const SymbolizerFnctSerializeType& fncts);

          te::se::Symbolizer* read(te::xml::Reader& reader) const;

          void write(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer) const;

          ~Symbolizer();

        protected:

          Symbolizer();

        private:

          SymbolizerFnctIdxType m_fncts;
      };
    }  //end namespace serialize
  }    // end namespace se
}      // end namespace te

#endif  // __TERRALIB_SE_SERIALIZATION_INTERNAL_SYMBOLIZER_H

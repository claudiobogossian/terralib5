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
  \file terralib/fe/serialization/xml/AbstractOp.h
   
  \brief Auxiliary classes and functions to serialize filter operations from a XML document.
*/

#ifndef __TERRALIB_FE_SERIALIZATION_XML_INTERNAL_ABSTRACTOP_H
#define __TERRALIB_FE_SERIALIZATION_XML_INTERNAL_ABSTRACTOP_H

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
    class Reader;
    class Writer;
  }

  namespace fe
  {
    class AbstractOp;

    namespace serialize
    {
      class TEFEEXPORT AbstractOp : public te::common::Singleton<AbstractOp>
      {
        friend class te::common::Singleton<AbstractOp>;

        public:

          typedef boost::function<te::fe::AbstractOp* (const char* opName, te::xml::Reader&)> AbstractOpReadFnctType;
          typedef boost::function<void (const te::fe::AbstractOp*, te::xml::Writer&)> AbstractOpWriteFnctType;
          typedef std::pair<AbstractOpReadFnctType, AbstractOpWriteFnctType> AbstractOpFnctSerializeType;
          typedef std::map<std::string, AbstractOpFnctSerializeType> AbstractOpFnctIdxType;

          void reg(const std::string& opName, const AbstractOpFnctSerializeType& fncts);

          te::fe::AbstractOp* read(te::xml::Reader& reader) const;

          void write(const te::fe::AbstractOp* op, te::xml::Writer& writer) const;

          ~AbstractOp();

        protected:

          AbstractOp();

        private:

          AbstractOpFnctIdxType m_fncts;
          std::map<std::string, const char*> m_names; //!< Auxiliary map of string to te::fe::Global names pointers.
      };
    } // end namespace serialize
  }   // end namespace fe
}     // end namespace te

#endif  // __TERRALIB_FE_SERIALIZATION_XML_INTERNAL_ABSTRACTOP_H

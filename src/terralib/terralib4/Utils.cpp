/*  Copyright (C) 2009-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/terralib4/Utils.cpp

  \brief  Utilitary functions for dealing with TerraLib 5 and 4.x conversion.
*/

// TerraLib 5
#include "../datatype/NumericProperty.h"
#include "../datatype/Property.h"
#include "../datatype/StringProperty.h"
#include "Utils.h"

std::auto_ptr<te::dt::Property> terralib4::Convert2T5(const TeAttributeRep& attRep)
{
  std::string* defaultValue = attRep.defaultValue_.empty() ? 0 : new std::string(attRep.defaultValue_);

  bool isRequired = !attRep.null_;

  switch(attRep.type_)
  {
    //case TeSTRING:
    //  if(attRep.numChar_ == 0)
    //    return new te::dt::StringProperty(attRep.name_, te::dt::STRING, 0, isRequired, defaultValue);
    //  else
    //    return new te::dt::StringProperty(attRep.name_, te::dt::VAR_STRING, attRep.numChar_, isRequired, defaultValue);

    //case TeCHARACTER:
    //  return new te::dt::SimpleProperty(attRep.name_, te::dt::PropertyType::CHAR, isRequired, defaultValue);

    //case TeBOOLEAN:
    //  return new te::dt::SimpleProperty(attRep.name_, te::dt::PropertyType::BOOLEAN, isRequired, defaultValue);

    //case TeINT:
    //  return new te::dt::SimpleProperty(attRep.name_, te::dt::PropertyType::INT32, isRequired, defaultValue);

    //case TeUNSIGNEDINT:
    //  return new te::dt::SimpleProperty(attRep.name_, te::dt::PropertyType::INT32, isRequired, defaultValue);

    //case TeREAL:
    //  return new te::dt::NumericProperty(attRep.name_, 0, 0, isRequired, defaultValue);

    //case TeDATETIME:
    //  return new te::dt::SimpleProperty(attRep.name_, te::dt::TIME_STAMP_TZ, isRequired, defaultValue);

    //case TeBLOB:
    //  return new te::dt::SimpleProperty(attRep.name_, te::dt::PropertyType::BYTE_ARRAY, isRequired, defaultValue);

    //case TePOINTTYPE:
    //  return new te::dt::GeometryType(attRep.name_, 0, te::gm::PointType, isRequired, defaultValue);

    //case TeLINE2DTYPE:
    //  return new te::dt::GeometryType(attRep.name_, 0, te::gm::LineStringType, isRequired, defaultValue);

    //case TePOLYGONTYPE:
    //  return new te::dt::GeometryType(attRep.name_, 0, te::gm::PolygonType, isRequired, defaultValue);

    //case TeCELLTYPE:
    //  return new te::dt::GeometryType(attRep.name_, 0, te::gm::PolygonType, isRequired, defaultValue);

    //case TePOINTSETTYPE:
    //  return new te::dt::GeometryType(attRep.name_, 0, te::gm::MultiPointType, isRequired, defaultValue);

    //case TeLINESETTYPE:
    //  return new te::dt::GeometryType(attRep.name_, 0, te::gm::MultiLineStringType, isRequired, defaultValue);

    //case TePOLYGONSETTYPE:
    //  return new te::dt::GeometryType(attRep.name_, 0, te::gm::MultiPolygonType, isRequired, defaultValue);

    //case TeCELLSETTYPE:   
    //  return new te::dt::GeometryType(attRep.name_, 0, te::gm::MultiPolygonType, isRequired, defaultValue);
    //
    //case TeRASTERTYPE:
    //  {
    //    std::vector<std::pair<std::string, std::string> > params;
    //    return new te::dt::CoverageType(attRep.name_, 0, 0, params);
    //  }

    case TeNODETYPE:
    case TeNODESETTYPE:
    case TeTEXTTYPE:
    case TeTEXTSETTYPE:
    case TeOBJECT:
    case TeUNKNOWN:
    default:
      throw Exception(TR_COMMON("The informed attribute representation is not supported!"));
  }
}


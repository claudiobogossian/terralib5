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
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../datatype/NumericProperty.h"
#include "../datatype/Property.h"
#include "../datatype/StringProperty.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/RasterProperty.h"
#include "Utils.h"

// TerraLib 4.x
#include <terralib/kernel/TeDatabaseFactoryParams.h>

std::auto_ptr<te::dt::Property> terralib4::Convert2T5(const TeAttributeRep& attRep)
{
  std::string* defaultValue = attRep.defaultValue_.empty() ? 0 : new std::string(attRep.defaultValue_);

  bool isRequired = !attRep.null_;

  switch(attRep.type_)
  {
    case TeSTRING:
      if(attRep.numChar_ == 0)
        return std::auto_ptr<te::dt::Property>(new te::dt::StringProperty(attRep.name_, te::dt::STRING, 0, isRequired, defaultValue));
      else
        return std::auto_ptr<te::dt::Property>(new te::dt::StringProperty(attRep.name_, te::dt::VAR_STRING, attRep.numChar_, isRequired, defaultValue));

    case TeCHARACTER:
      return std::auto_ptr<te::dt::Property>(new te::dt::SimpleProperty(attRep.name_, te::dt::CHAR_TYPE, isRequired, defaultValue));

    case TeBOOLEAN:
      return std::auto_ptr<te::dt::Property>(new te::dt::SimpleProperty(attRep.name_, te::dt::BOOLEAN_TYPE, isRequired, defaultValue));

    case TeINT:
      return std::auto_ptr<te::dt::Property>(new te::dt::SimpleProperty(attRep.name_, te::dt::INT32_TYPE, isRequired, defaultValue));

    case TeUNSIGNEDINT:
      return std::auto_ptr<te::dt::Property>(new te::dt::SimpleProperty(attRep.name_, te::dt::UINT32_TYPE, isRequired, defaultValue));

    case TeREAL:
      return std::auto_ptr<te::dt::Property>(new te::dt::NumericProperty(attRep.name_, 0, 0, isRequired, defaultValue));

    case TeDATETIME:
      return std::auto_ptr<te::dt::Property>(new te::dt::SimpleProperty(attRep.name_, te::dt::TIME_INSTANT, isRequired, defaultValue));

    case TeBLOB:
      return std::auto_ptr<te::dt::Property>(new te::dt::SimpleProperty(attRep.name_, te::dt::BYTE_ARRAY_TYPE, isRequired, defaultValue));

    case TePOINTTYPE:
      return std::auto_ptr<te::dt::Property>(new te::gm::GeometryProperty(attRep.name_, 0, te::gm::PointType, isRequired, defaultValue));

    case TeLINE2DTYPE:
      return std::auto_ptr<te::dt::Property>(new te::gm::GeometryProperty(attRep.name_, 0, te::gm::LineStringType, isRequired, defaultValue));

    case TePOLYGONTYPE:
      return std::auto_ptr<te::dt::Property>(new te::gm::GeometryProperty(attRep.name_, 0, te::gm::PolygonType, isRequired, defaultValue));

    case TeCELLTYPE:
      return std::auto_ptr<te::dt::Property>(new te::gm::GeometryProperty(attRep.name_, 0, te::gm::PolygonType, isRequired, defaultValue));

    case TePOINTSETTYPE:
      return std::auto_ptr<te::dt::Property>(new te::gm::GeometryProperty(attRep.name_, 0, te::gm::MultiPointType, isRequired, defaultValue));

    case TeLINESETTYPE:
      return std::auto_ptr<te::dt::Property>(new te::gm::GeometryProperty(attRep.name_, 0, te::gm::MultiLineStringType, isRequired, defaultValue));

    case TePOLYGONSETTYPE:
      return std::auto_ptr<te::dt::Property>(new te::gm::GeometryProperty(attRep.name_, 0, te::gm::MultiPolygonType, isRequired, defaultValue));

    case TeCELLSETTYPE:
      return std::auto_ptr<te::dt::Property>(new te::gm::GeometryProperty(attRep.name_, 0, te::gm::MultiPolygonType, isRequired, defaultValue));

    case TeRASTERTYPE:
      return std::auto_ptr<te::dt::Property>(new te::rst::RasterProperty(attRep.name_, isRequired));

    case TeUNKNOWN:
      return std::auto_ptr<te::dt::Property>(new te::dt::SimpleProperty(attRep.name_, te::dt::UNKNOWN_TYPE, isRequired));

    case TeNODETYPE:
    case TeNODESETTYPE:
    case TeTEXTTYPE:
    case TeTEXTSETTYPE:
    case TeOBJECT:
    
    default:
      throw te::common::Exception(TR_COMMON("The informed attribute representation is not supported!"));
  }
}

std::auto_ptr<TeDatabaseFactoryParams> terralib4::Convert2T4DatabaseParams(const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<TeDatabaseFactoryParams> fparams(new TeDatabaseFactoryParams());

  fparams->dbms_name_ = dsInfo.at("T4_DRIVER");
  fparams->database_ = dsInfo.at("T4_DATABASE_NAME");
  fparams->host_ = dsInfo.at("T4_HOST");
  //....

  return fparams;
}


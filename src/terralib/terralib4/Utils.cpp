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
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/NumericProperty.h"
#include "../datatype/Property.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Enums.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "Utils.h"

// TerraLib 4.x
#include <terralib4/kernel/TeBox.h>
#include <terralib4/kernel/TeDatabaseFactoryParams.h>
#include <terralib4/kernel/TeDecoderDatabase.h>
#include <terralib4/kernel/TeProjection.h>
#include <terralib4/kernel/TeTable.h>
#include <terralib4/kernel/TeRasterParams.h>

// Boost
#include <boost/lexical_cast.hpp>

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
      return std::auto_ptr<te::dt::Property>(new te::dt::SimpleProperty(attRep.name_, te::dt::DOUBLE_TYPE, isRequired, defaultValue));

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
      throw te::common::Exception(TE_TR("The informed attribute representation is not supported!"));
  }
}

std::auto_ptr<TeDatabaseFactoryParams> terralib4::Convert2T4DatabaseParams(const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<TeDatabaseFactoryParams> fparams(new TeDatabaseFactoryParams());

  fparams->dbms_name_ = dsInfo.at("T4_DRIVER");
  fparams->database_ = dsInfo.at("T4_DB_NAME");
  fparams->host_ = dsInfo.at("T4_HOST");
  //fparams->port_ = boost::lexical_cast<int>(dsInfo.at("T4_PORT"));
  fparams->user_ = dsInfo.at("T4_USER");
  fparams->password_ = dsInfo.at("T4_PASSWORD");

  return fparams;
}

int terralib4::Convert2T5(TeAttrDataType type)
{
  switch(type)
  {
    case TeSTRING:
      return te::dt::STRING_TYPE;

    case TeREAL:
      return te::dt::NUMERIC_TYPE;

    case TeINT:
      return te::dt::INT32_TYPE;

    case TeDATETIME:
      return te::dt::DATETIME_TYPE;

    case TeBLOB:
      return te::dt::BYTE_ARRAY_TYPE;

    case TeCHARACTER:
      return te::dt::CHAR_TYPE;

    case TeUNSIGNEDINT:
      return te::dt::UINT32_TYPE;

    case TePOINTTYPE:
    case TeNODETYPE:
    case TeLINE2DTYPE:
    case TePOLYGONTYPE:
    case TeCELLTYPE:
    case TePOINTSETTYPE:
    case TeNODESETTYPE:
    case TeLINESETTYPE:
    case TePOLYGONSETTYPE:
    case TeCELLSETTYPE:
      return te::dt::GEOMETRY_TYPE;

    case TeRASTERTYPE:
      return te::dt::RASTER_TYPE;

    case TeBOOLEAN:
      return te::dt::BOOLEAN_TYPE;

    case TeUNKNOWN:
    case TeOBJECT:
    case TeTEXTTYPE:
    case TeTEXTSETTYPE:
    default:
      return te::dt::UNKNOWN_TYPE;
  }
}

te::gm::GeomType terralib4::Convert2T5GeomType(TeAttrDataType type)
{
  switch(type)
  {
    case TePOINTTYPE:
    case TeNODETYPE:
      return te::gm::PointType;

    case TeLINE2DTYPE:
      return te::gm::LineStringType;

    case TePOLYGONTYPE:
    case TeCELLTYPE:
      return te::gm::PolygonType;

    case TePOINTSETTYPE:
    case TeNODESETTYPE:
      return te::gm::MultiPointType;

    case TeLINESETTYPE:
      return te::gm::MultiLineStringType;

    case TePOLYGONSETTYPE:
    case TeCELLSETTYPE:
      return te::gm::MultiPolygonType;

    default:
      return te::gm::UnknownGeometryType;
  }
}

te::gm::GeomType terralib4::Convert2T5GeomType(TeGeomRep type)
{
  switch(type)
  {
    case TePOINTS:
    case TeNODES:
      return te::gm::PointType;

    case TeLINES:
      return te::gm::LineStringType;

    case TePOLYGONS:
    case TeCELLS:
      return te::gm::PolygonType;

    default:
      return te::gm::UnknownGeometryType;
  }
}

int terralib4::Convert2T5(TeDataType dt)
{
  switch(dt)
  {
    case TeBIT:
      return te::dt::BIT_TYPE;

    case TeUNSIGNEDCHAR:
      return te::dt::UCHAR_TYPE;

    case TeCHAR:
      return te::dt::CHAR_TYPE;

    case TeUNSIGNEDSHORT:
      return te::dt::UINT16_TYPE;

    case TeSHORT:
      return te::dt::INT16_TYPE;

    case TeINTEGER:
      return te::dt::INT32_TYPE;

    case TeUNSIGNEDLONG:
      return te::dt::UINT32_TYPE;

    case TeLONG:
      return te::dt::INT32_TYPE;  // see TerraLib 4.x => TeRasterParams.cpp elementSize method => sizeof(long) => 4bytes!

    case TeFLOAT:
      return te::dt::FLOAT_TYPE;

    case TeDOUBLE:
      return te::dt::DOUBLE_TYPE;

    default:
      return te::dt::UNKNOWN_TYPE;
  }
}

TeAttrDataType terralib4::Convert2T4(int type)
{
  switch(type)
  {
    case te::dt::STRING_TYPE:
      return TeSTRING;

    case te::dt::DOUBLE_TYPE:
    case te::dt::NUMERIC_TYPE:
      return TeREAL;

    case te::dt::INT16_TYPE:
    case te::dt::INT32_TYPE:
    case te::dt::INT64_TYPE:
      return TeINT;

    case te::dt::DATETIME_TYPE:
      return TeDATETIME;

    case te::dt::BYTE_ARRAY_TYPE:
      return TeBLOB;

    case te::dt::CHAR_TYPE:
      return TeCHARACTER;

    case te::dt::UINT16_TYPE:
    case te::dt::UINT32_TYPE:
    case te::dt::UINT64_TYPE:
      return TeUNSIGNEDINT;

    case te::dt::RASTER_TYPE:
      return TeRASTERTYPE;

    case te::dt::BOOLEAN_TYPE:
      return TeBOOLEAN;

    case te::dt::UNKNOWN_TYPE:
    default:
      return TeUNKNOWN;
  }
}

TeAttrDataType terralib4::Convert2T4GeomType(te::gm::GeomType type)
{
  switch(type)
  {
    case te::gm::PointType:
      return TePOINTTYPE;

    case te::gm::LineStringType:
      return TeLINE2DTYPE;

    case te::gm::PolygonType:
      return TePOLYGONTYPE;

    case te::gm::MultiPointType:
      return TePOINTSETTYPE;

    case te::gm::MultiLineStringType:
      return TeLINESETTYPE;

    case te::gm::MultiPolygonType:
      return TePOLYGONSETTYPE;

    default:
      return TeUNKNOWN;
  }
}

std::auto_ptr<te::gm::Envelope> terralib4::Convert2T5(TeBox box)
{
  std::auto_ptr<te::gm::Envelope> env(new te::gm::Envelope(box.x1(), box.y1(), box.x2(), box.y2()));

  return env;
}

std::auto_ptr<te::da::DataSetType> terralib4::Convert2T5(TeTable table)
{
  TeAttributeList attrList = table.attributeList();

  std::auto_ptr<te::da::DataSetType> newDst(new te::da::DataSetType(table.name()));

  for(std::size_t i = 0; i < attrList.size(); ++i)
  {
    TeAttributeRep attr = attrList[i].rep_;

    std::auto_ptr<te::dt::Property> prop = terralib4::Convert2T5(attr);

    newDst->add(prop.release());
  }

  return newDst;
}

te::rst::RasterProperty* terralib4::Convert2T5(TeRasterParams& rparams)
{
  std::auto_ptr<te::rst::RasterProperty> rproperty(new te::rst::RasterProperty("raster"));

  unsigned int ncols = rparams.ncols_;
  unsigned int nrows = rparams.nlines_;
  std::auto_ptr<te::gm::Envelope> mbr(Convert2T5(rparams.boundingBox()));

  int srid = rparams.projection()->epsgCode();
  if(srid == 4979)
    srid = 4326;

  std::auto_ptr<te::rst::Grid> grid(new te::rst::Grid(ncols, nrows, mbr.release(), srid));

  rproperty->set(grid.release());

  std::vector<te::rst::BandProperty::ColorEntry> palette;
  if(rparams.photometric_[0] == TeRasterParams::TePallete)
  {
    std::size_t size = rparams.lutr_.size();

    for(std::size_t j = 0; j < size; ++j)
    {
      te::rst::BandProperty::ColorEntry c;
      c.c1 = rparams.lutr_[j];
      c.c2 = rparams.lutg_[j];
      c.c3 = rparams.lutb_[j];
      palette.push_back(c);
    }
  }

  for(int i = 0; i != rparams.nBands(); ++i)
  {
    te::rst::BandProperty* bp = new te::rst::BandProperty(i, Convert2T5(rparams.dataType_[i]));

    bp->m_blkh = rparams.blockHeight_;
    bp->m_blkw = rparams.blockWidth_;
    bp->m_noDataValue = rparams.dummy_[i];
    bp->m_valuesOffset = rparams.offset_;

    if(!palette.empty())
      bp->m_palette.assign(palette.begin(), palette.end());

    bp->m_nblocksx = (ncols + bp->m_blkw - 1) / bp->m_blkw;
    bp->m_nblocksy = (nrows + bp->m_blkh - 1) / bp->m_blkh;

    rproperty->add(bp);
  }

  return rproperty.release();
}


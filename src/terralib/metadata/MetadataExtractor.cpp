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

/*!
 \file terralib/metadata/MetadataExtractor.cpp
 */

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/utils/Utils.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "Config.h"
#include "iso19115/CI_Citation.h"
#include "iso19115/MD_Metadata.h"
#include "MetadataExtractor.h"


// STL
#include <cassert>
#include <string>

// Boost
#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

te::md::MD_Metadata* te::md::Extract(const te::da::DataSet* d)
{
  assert(d);

  te::md::MD_Metadata* md = new te::md::MD_Metadata();
  md->setStandardInfo("iso19115", "2003");
  md->setDateStamp(boost::gregorian::day_clock::local_day());
  
  //const te::da::DataSetType* dst = d->getType();
  //md->setFileId(boost::lexical_cast<std::string>(dst->getId()));
  
  //te::md::CI_Citation* ct = new te::md::CI_Citation(dst->getTitle(),boost::gregorian::day_clock::local_day(),te::md::CI_creation); 
  te::md::CI_Citation* ct = new te::md::CI_Citation("dst->getTitle()",boost::gregorian::day_clock::local_day(),te::md::CI_creation); 
   
  te::md::MD_DataIdentification* id = new te::md::MD_DataIdentification(ct,"",te::md::MD_completed, "pt; BR");
  
  int pos = static_cast<int>(te::da::GetFirstPropertyPos(d, te::dt::GEOMETRY_TYPE));

  std::auto_ptr<te::gm::Geometry> g = d->getGeometry(pos);
  std::auto_ptr<te::gm::Envelope> env = const_cast<te::da::DataSet*>(d)->getExtent(pos); 
  md->setReferenceSystem(g->getSRID());
  id->addGeographicBBoxExt(env.get());

  return md;
}



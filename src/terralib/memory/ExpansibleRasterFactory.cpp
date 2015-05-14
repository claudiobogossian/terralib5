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
  \file terralib/memory/ExpansibleRasterFactory.cpp

  \brief This is the concrete factory for In-Memory driver.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "Exception.h"
#include "ExpansibleRaster.h"
#include "ExpansibleRasterFactory.h"

// STL
#include <memory>

// Boost
#include <boost/lexical_cast.hpp>

static std::string sg_expansibleRasterFactoryId("EXPANSIBLE");

te::mem::ExpansibleRasterFactory* te::mem::ExpansibleRasterFactory::sm_factoryPointer( 0 );

te::mem::ExpansibleRasterFactory::~ExpansibleRasterFactory()
{
}

const std::string& te::mem::ExpansibleRasterFactory::getType() const
{
  return sg_expansibleRasterFactoryId;
}

void te::mem::ExpansibleRasterFactory::getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const
{
  params.push_back(std::pair<std::string, std::string>("MAXMEMPERCENTUSED", "40"));  
  params.push_back(std::pair<std::string, std::string>("MAXNUMBEROFRAMBLOCKS", "1"));  
}

te::mem::ExpansibleRasterFactory::ExpansibleRasterFactory()
  : te::rst::RasterFactory(sg_expansibleRasterFactoryId)
{
}

te::rst::Raster* te::mem::ExpansibleRasterFactory::create(te::rst::Grid* g,
                                                const std::vector<te::rst::BandProperty*> bands,
                                                const std::map<std::string, std::string>& rinfo,
                                                void* h,
                                                void (*deleter)(void*))
{
  std::auto_ptr<te::rst::Raster> rasterPtr;
  
  if( rinfo.find( "MAXNUMBEROFRAMBLOCKS" ) != rinfo.end() )
  {
    unsigned int maxNumberOfRAMBlocks = boost::lexical_cast< unsigned int >( rinfo.find( "MAXNUMBEROFRAMBLOCKS" )->second );
    rasterPtr.reset( new ExpansibleRaster( g, bands, maxNumberOfRAMBlocks ) );
  }
  else if( rinfo.find( "MAXMEMPERCENTUSED" ) != rinfo.end() )
  {
    unsigned int maxMemPercentUsed = boost::lexical_cast< unsigned int>( rinfo.find( "MAXMEMPERCENTUSED" )->second );
    if( maxMemPercentUsed > 100 )
    {
      throw Exception(TE_TR("Invalid parameter: MAXMEMPERCENTUSED") );
    }
    
    rasterPtr.reset( new ExpansibleRaster( (unsigned char)maxMemPercentUsed, g, bands ) );
  }
  else
  {
    rasterPtr.reset( new ExpansibleRaster( 40, g, bands ) );
  }
  
  return rasterPtr.release();
}

te::rst::Raster* te::mem::ExpansibleRasterFactory::build()
{
  return new ExpansibleRaster;
}

std::map<std::string, std::string> te::mem::ExpansibleRasterFactory::getCapabilities() const
{
  return std::map<std::string, std::string>();
}

void te::mem::ExpansibleRasterFactory::initialize()
{
  finalize();
  sm_factoryPointer = new ExpansibleRasterFactory;
}

void te::mem::ExpansibleRasterFactory::finalize()
{
  delete sm_factoryPointer;
  sm_factoryPointer = 0;
}

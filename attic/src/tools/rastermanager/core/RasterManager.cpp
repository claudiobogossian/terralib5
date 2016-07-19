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
  \file terralib/tools/rastermanager/core/RasterManager.cpp

  \brief An utility class to provide methods to manage rasters
 */

// Terralib Raster Manager
#include "RasterManager.h"
#include "Utils.h"

// Terralib
#include <terralib/common.h>
#include <terralib/raster.h>
#include <terralib/geometry/Envelope.h>
#include <terralib/raster/Reprojection.h>

// Boost
#include <boost/filesystem.hpp>

// GDAL
#include <gdal_priv.h>

namespace fs = boost::filesystem;

te::tools::rastermanager::RasterManager::RasterManager()
{
  m_loadPlugin = true;
}

bool te::tools::rastermanager::RasterManager::init(std::string input, std::string & errorMessage)
{
  
  if(!Utils::checkInputPath(input, errorMessage))
    return false;
  
  m_input = input;

  if(m_loadPlugin)
    if(!Utils::loadModules(errorMessage))
      return false;

  m_loadPlugin = false;

  return true;
   
}

bool te::tools::rastermanager::RasterManager::copyRaster(std::string output, std::vector<int> bandVec, int bandType, std::string & errorMessage)
{

  if(!Utils::checkOutputPath(output, errorMessage))
    return false; 
  
  te::rst::Raster* raster = 0;
  if(!Utils::getRaster(m_input, raster, errorMessage))
    return false;
  
  // Start the copy
  if(bandVec.size() > raster->getNumberOfBands())
  {
    errorMessage = "Error in the number of bands!";
    return false;
  }
  
  std::vector<te::rst::BandProperty*> bandProps;

  te::rst::Grid* newGrid = new te::rst::Grid(*raster->getGrid());
  
  if(bandVec.empty())
  {
    for(int unsigned i = 0; i < raster->getNumberOfBands(); i++)
    {
      bandProps.push_back(raster->getBand(i)->getProperty());
    }
  }
  else
  {
    for(int unsigned i = 0; i < bandVec.size(); i++)
    {
      if(bandVec[i] > raster->getNumberOfBands())
      {
        errorMessage = "The band " + te::common::Convert2String(bandVec[i]) + " not exist in the input raster!";
        return false;
      }
      bandProps.push_back(raster->getBand(bandVec[i])->getProperty());
    }
  }

  std::map<std::string, std::string> newRasterInfo;
  newRasterInfo["URI"] = output;

  te::rst::Raster* newRaster = te::rst::RasterFactory::make(newGrid, bandProps, newRasterInfo);
  
  if(bandVec.empty())
  {
    for(int unsigned r = 0; r < raster->getNumberOfRows(); r++)
    {
      for(int unsigned c = 0; c < raster->getNumberOfColumns(); c++)
      {
        std::vector<std::complex<double>> values;
        raster->getValues(c, r, values);

        newRaster->setValues(c, r, values);

      }
    }
  }
  else
  {
    for(int unsigned r = 0; r < raster->getNumberOfRows(); r++)
    {
      for(int unsigned c = 0; c < raster->getNumberOfColumns(); c++)
      {
        for(int unsigned b = 0; b < bandVec.size(); b++)
        {
          std::complex<double> value;
          raster->getValue(c, r, value, bandVec[b]);

          newRaster->setValue(c, r, value, bandVec[b]);
        }
      }
    }
  } 

  delete raster;
  delete newRaster;

  return true;

}

bool te::tools::rastermanager::RasterManager::reproject(std::string output, int srid, std::string & errorMessage)
{
  te::rst::Raster* raster = 0;
  if(!Utils::getRaster(m_input, raster, errorMessage))
    return false;

  // The raster file will be overwriting if output empty
  if(output.empty())
    output = m_input;
  else
    if(!Utils::checkOutputPath(output, errorMessage))
      return false;

  te::rst::Raster* outraster = 0;
  std::map<std::string, std::string> outInfo;
   
  outInfo["URI"] = output;
  outraster = te::rst::Reproject(raster, srid, outInfo);
 
  delete outraster;

  return true;
  
}

bool te::tools::rastermanager::RasterManager::getRasterInfo(std::ostringstream & output, std::string & errorMessage)
{

  te::rst::Raster* raster = 0;
  if(!Utils::getRaster(m_input, raster, errorMessage))
    return false;

  output << raster->toString() << std::endl << std::endl;

  output << "Bands Type Informations:" << std::endl << std::endl;

  for(unsigned int i = 0; i < raster->getNumberOfBands(); i++)
  {
    te::rst::BandProperty* currPorperty = raster->getBand(i)->getProperty();  
    
    output << "Band " << te::common::Convert2String(i) << ": " << te::common::Convert2String(currPorperty->getType());
    output << std::endl;   
  }

  output << std::endl;

  output << "Block Informations:" << std::endl << std::endl;

  te::rst::BandProperty* currPorperty = raster->getBand(0)->getProperty();

  output << "Block Width: " << te::common::Convert2String(currPorperty->m_blkw) << std::endl;
  output << "Block Height: " << te::common::Convert2String(currPorperty->m_blkh) << std::endl;    

  output << "Blocks in X: " << te::common::Convert2String(currPorperty->m_nblocksx) << std::endl;  
  output << "Blocks in Y: " << te::common::Convert2String(currPorperty->m_nblocksy) << std::endl;
   
  return true;
}

bool te::tools::rastermanager::RasterManager::convert(std::string output, std::string extension, std::vector<int> bandVec, std::string & errorMessage)
{
  std::string finalOutput = "";
  
 
  if(!output.empty())
  {
    if(output == m_input)
    {
      errorMessage = "Con not convert the raster for the same path and same extension!";
      return false;
    }
    else
    {
      finalOutput = output;
    }
  }
  else if(!extension.empty())
  {
    if(extension[0] != '.')
      extension = "." + extension;

    fs::path temp(m_input);
    temp.replace_extension(extension);

    finalOutput = temp.string();

    if(finalOutput == m_input)
    {
      errorMessage = "Con not convert the raster for the same path and same extension!";
      return false;
    }
  }
  else
  {
    errorMessage = "For this operations you must enter with the output path (\"--output\") or the output extension (\"--extension\")!";
    return false;
  }  

 te::rst::Raster* raster = 0;
  if(!Utils::getRaster(m_input, raster, errorMessage))
    return false;  

  if(!copyRaster(finalOutput, bandVec, int(),errorMessage))
    return false;

  return true;
}

bool te::tools::rastermanager::RasterManager::changeResolution(std::string output, int method, int scale, std::string & errorMessage)
{

  // The raster file will be overwriting if output empty
  if(output.empty())
    output = m_input;
  else
    if(!Utils::checkOutputPath(output, errorMessage))
      return false;


  if(method < 1 || method > 3)
  {
    errorMessage = "Method invalid!";
    return false;
  }
  
  te::rst::Raster* raster = 0;
  if(!Utils::getRaster(m_input, raster, errorMessage))
    return false; 

  std::map<std::string, std::string> newRasterInfo;
  newRasterInfo["URI"] = output;

  raster->resample(method, scale, newRasterInfo);
  
  delete raster;

  return true;
}

bool te::tools::rastermanager::RasterManager::trim(std::string output, std::vector<double> env, std::string & errorMessage)
{
  // The raster file will be overwriting if output empty
  if(output.empty())
    output = m_input;
  else
    if(!Utils::checkOutputPath(output, errorMessage))
      return false;

  te::rst::Raster* raster = 0;
  if(!Utils::getRaster(m_input, raster, errorMessage))
    return false;
    
  te::gm::Envelope* rasterEnv = raster->getExtent();  

  te::gm::Envelope* newRasterEnv = new te::gm::Envelope(env[0], env[1], env[2], env[3]);  

  std::map<std::string, std::string> outInfo;
  outInfo["URI"] = output;
  raster->trim(newRasterEnv, outInfo);
    
  delete raster;

  return true;
}

bool te::tools::rastermanager::RasterManager::showSupportedExtensions(std::string & errorMessage)
{

  Utils::loadModules(errorMessage);

  std::map<std::string, std::string> capabilities;

  te::common::AbstractFactory<te::rst::Raster, std::string>::dictionary_type& d = te::common::AbstractFactory<te::rst::Raster, std::string>::getDictionary();

  te::common::AbstractFactory<te::rst::Raster, std::string>::dictionary_type::const_iterator it = d.begin();
  te::common::AbstractFactory<te::rst::Raster, std::string>::dictionary_type::const_iterator itend = d.end();

  std::cout << "Supported Extensions" << std::endl;
  std::cout << "--------------------" << std::endl << std::endl;

  while(it != itend)
  {
    std::cout << it->first << std::endl;
    
    std::map<std::string, std::string> map = static_cast<te::rst::RasterFactory*>(it->second)->getCapabilities();
    
    std::cout << "  " << map["supported_formats"] << std::endl << std::endl;

    ++it;
  }

  return true;

}

bool te::tools::rastermanager::RasterManager::getSupportedExtensions(std::vector<std::string> & extents, std::string & errorMessage)
{
  std::map<std::string, std::string> capabilities;

  te::common::AbstractFactory<te::rst::Raster, std::string>::dictionary_type& d = te::common::AbstractFactory<te::rst::Raster, std::string>::getDictionary();

  te::common::AbstractFactory<te::rst::Raster, std::string>::dictionary_type::const_iterator it = d.begin();
  te::common::AbstractFactory<te::rst::Raster, std::string>::dictionary_type::const_iterator itend = d.end();

  std::map<std::string, std::string> map;

  while(it != itend)
  {
    map = static_cast<te::rst::RasterFactory*>(it->second)->getCapabilities();

    ++it;
  }

  te::common::Tokenize(map["supported_formats"], extents, ",");

  return true;
}
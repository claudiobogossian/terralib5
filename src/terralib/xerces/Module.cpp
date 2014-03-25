/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Module.cpp
   
  \brief The TerraLib Xerces module is a plugin.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"

#include "Exception.h"
#include "Module.h"
#include "ReaderFactory.h"

// Xerces-C++ 
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

bool te::xerces::Module::m_initialized = false;

te::xerces::Module::Module(const te::plugin::PluginInfo& pInfo)
  : te::plugin::CppPlugin(pInfo)
{
}

te::xerces::Module::~Module()
{
}

void te::xerces::Module::startup()
{
  if(m_initialized)
    return;

  try
  {
    xercesc::XMLPlatformUtils::Initialize();
  }
  catch(const xercesc::XMLException& e)
  {
    char* exceptMesg = xercesc::XMLString::transcode(e.getMessage());
    
    std::string m  = TE_TR("Error during Xerces initialization. Exception messages is: ");
    m += exceptMesg;
    
    xercesc::XMLString::release(&exceptMesg);
    
    throw te::xerces::Exception(m);
  }
  
    // it initializes the reader factory based on Xerces
  ReaderFactory::initialize();
  
  TE_LOG_TRACE(TE_TR("TerraLib Xerces driver startup!"));
  
  m_initialized = true;
}

void te::xerces::Module::shutdown()
{
  if(!m_initialized)
    return;
    
  // it finalizes  the reader factory based on Xerces
  ReaderFactory::finalize();
  
  try
  {
    xercesc::XMLPlatformUtils::Terminate();
  }
  catch(const xercesc::XMLException& e)
  {
    char* exceptMesg = xercesc::XMLString::transcode(e.getMessage());
    
    std::string m  = TE_TR("Error during Xerces finalization. Exception messages is: ");
    m += exceptMesg;
    
    xercesc::XMLString::release(&exceptMesg);
    
    throw te::xerces::Exception(m);
  }

  TE_LOG_TRACE(TE_TR("TerraLib Xerces driver shutdown!"));
  
  m_initialized = true;
}

PLUGIN_CALL_BACK_IMPL(te::xerces::Module)




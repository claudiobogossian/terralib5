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
  \file terralib/xml/Module.cpp
   
  \brief This singleton defines the TerraLib XML module entry.
 */

// TerraLib
#include "../core/logger/Logger.h"
#include "../common/TerraLib.h"
#include "../core/translator/Translator.h"
#include "../xerces/ReaderFactory.h"
#include "../xerces/WriterFactory.h"
#include "Config.h"
#include "Exception.h"
#include "Module.h"

// Xerces-C++
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

const te::xml::Module& sm_module = te::xml::Module::getInstance();
static bool m_initialized(false);


te::xml::Module::Module()
{
  TerraLib::Module m = { TE_XML_MODULE_NAME,
                         te::xml::Module::initialize,
                         te::xml::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::xml::Module::~Module()
{
  TerraLib::getInstance().remove(TE_XML_MODULE_NAME);
}

void te::xml::Module::initialize()
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

    throw Exception(m);
  }

  // it initializes the reader factory based on Xerces
  te::xerces::ReaderFactory::initialize();
  te::xerces::WriterFactory::initialize();

  TE_LOG_INFO(TE_TR("TerraLib XML module initialized!"));

  m_initialized = true;
}

void te::xml::Module::finalize()
{
  if(!m_initialized)
    return;

  // it finalizes  the reader factory based on Xerces
  te::xerces::ReaderFactory::finalize();
  te::xerces::WriterFactory::finalize();

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

    throw Exception(m);
  }

  TE_LOG_INFO(TE_TR("TerraLib XML module finalized!"));

  m_initialized = false;
}

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
  \file Metadata.cpp
 */

#include <terralib/dataaccess.h>
#include <terralib/metadata.h>

//Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

void 
ExMetadataManager()
{
  // Open a data source
  std::string ogrInfo("connection_string="TE_DATA_EXAMPLE_LOCALE"/data/shp");
  
  te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
  ds->setConnectionStr(ogrInfo);
  ds->open();
  
  te::da::DataSourceTransactor* transactor = ds->getTransactor();
  
  te::da::DataSourceCatalogLoader* cloader = transactor->getCatalogLoader();
  
  boost::ptr_vector<std::string> datasets;
  
  cloader->getDataSets(datasets);
  
  // Create an annotation for each dataset in the datasourc
  for (unsigned int i=0; i<datasets.size(); ++i)
  {
    te::da::DataSetType* dt = cloader->getDataSetType(datasets[i], true);
    
    te::md::MetadataAnnotation* anot = new te::md::MetadataAnnotation();
    anot->setElementId(boost::lexical_cast<std::string>(i));
    anot->setElementName(dt->getTitle());
    
    boost::format fname("%1%/%2%.xml");
    fname = fname % TE_DATA_EXAMPLE_LOCALE"/data/shp" % dt->getTitle();
    anot->setAnnotationURI(fname.str());    
    te::md::MetadataManager::getInstance().insert(anot); 
  }
  
  // Print manager contents
  const std::map<std::string,te::md::MetadataAnnotation*>& mm = te::md::MetadataManager::getInstance().getAnnotations();
  
  std::map<std::string,te::md::MetadataAnnotation*>::const_iterator it = mm.begin();
  
  std::string aid;
  while (it != mm.end())  
  {
    std::cout << "Annt id: " << it->second->getAnnotationtId() 
              << " Elem id: "  << it->second->getElementId() 
              << " Elem name: " << it->second->getElementName()
              << " Annt URI: " << it->second->getAnnotationURI() << std::endl;
    aid = it->second->getAnnotationtId();
    ++it;
  }
  
  // Find an annotation
  const te::md::MetadataAnnotation* aannot = te::md::MetadataManager::getInstance().find(aid);
  if (aannot)
    std::cout << "Annotation " << aid << " refers to element " << aannot->getElementName() << std::endl; 
  
  te::md::MetadataManager::getInstance().clear();
}
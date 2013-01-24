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

void 
ExMetadataAnnotate()
{
  // Create the metadata root element: this basically describes the metadata record
  
  te::md::CI_ResponsibleParty* creator = new te::md::CI_ResponsibleParty(te::md::CI_originator);
  creator->setOrganizationName("Fundação Instituto Brasileiro de Geografia e Estatística - IBGE/ Diretoria de Geociências - DGC/ Coordenação de Cartografia - CCAR");
  
  te::md::CI_ResponsibleParty* distributor = new te::md::CI_ResponsibleParty(te::md::CI_distributor);
  distributor->setOrganizationName("Instituto Brasileiro de Geografia e Estatística - IBGE, Centro de Documentação e Disseminação - CDDI");
  
  te::md::MD_Metadata* md = new te::md::MD_Metadata(creator);
  md->addContact(distributor);
  
  md->setFileId("ibge.xml");
  md->setLanguage("pt; BR");
  md->setCharset(te::md::MD_utf8);
  md->setDateStamp(boost::gregorian::date(boost::gregorian::from_undelimited_string("20041210")));
  md->setStandardInfo("ISO 19115/IBGE", "2003");
  
  //  Citation
  te::md::CI_Citation* dt_ct = new te::md::CI_Citation("Carta Topográfica 1:25.000 SD24-X-A-VI-1-NO",
                                                       boost::gregorian::date(boost::gregorian::from_undelimited_string("19931201")),
                                                       te::md::CI_creation);
  
  //  Data identification 
  std::string abstract("A série de carta topográfica na escala 1: 25 000 representa parte do território nacional, de forma articulada, por folhas (segundo a convenção internacional da Carta CIM), e que abrange um quadrilátero geográfico de 7,5' de latitude por 7,5' de longitude. O IBGE desenvolve a produção dessa série, em conjunto com a DSG - Diretoria de Serviço Geográfico do Exército. Esta base vetorial está estruturada em categorias de informação: Hidrografia, Hipsografia, Localidades, Limites, Sistemas de Transporte, Pontos de Referência, Vegetação e Obras e Edificações, segundo modelagem da Mapoteca Topográfica Digital do IBGE.");
  
  te::md::MD_DataIdentification* dt_id = new te::md::MD_DataIdentification(dt_ct, abstract, te::md::MD_completed, "pt; BR");
  dt_id->addCharacterSet(te::md::MD_utf8);  

  dt_id->setSpatialRepType(te::md::MD_vector);

  dt_id->addGeographicBBoxExt(-38, -37.83, -12.63, -12.5);
  
  dt_id->addTopicCategory(te::md::MD_imageryBaseMapsEarthCover);
  
  // Insert the data idenfiticaion in the metadata record
  md->addIdentification(dt_id);
  
  delete md;

}
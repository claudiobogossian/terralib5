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

// TerraLib
#include <terralib/metadata.h> 
#include <terralib/dataaccess.h>
#include <terralib/metadata/serialization/MD_Metadata.h>
#include <terralib/srs/Config.h>
#include <terralib/xml/Writer.h>

//Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

//STL
#include <fstream>
#include <iostream>

void 
ExMetadataManager()
{
  // Open a data source
  std::string ogrInfo("connection_string="TE_DATA_EXAMPLE_DIR"/data/shp");
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/shp";
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  ds->setConnectionInfo(connInfo);
  ds->open();
  
  std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();
  
  std::vector<std::string> datasets = transactor->getDataSetNames();
  
  // Create an annotation for each dataset in the datasourc
  for (unsigned int i=0; i<datasets.size(); ++i)
  {
    std::auto_ptr<te::da::DataSetType> dt = transactor->getDataSetType(datasets[i]);
    
    te::md::MetadataAnnotation* anot = new te::md::MetadataAnnotation();
    anot->setElementId(boost::lexical_cast<std::string>(i));
    anot->setElementName(dt->getTitle());
    
    boost::format fname("%1%/%2%.xml");
    fname = fname % TE_DATA_EXAMPLE_DIR"/data/shp" % dt->getTitle();
    anot->setAnnotationURI(fname.str());    
    te::md::MetadataManager::getInstance().insert(anot); 
  }
  
  // Print manager contents
  const std::map<std::string,te::md::MetadataAnnotation*>& mm = te::md::MetadataManager::getInstance().getAnnotations();
  
  std::map<std::string,te::md::MetadataAnnotation*>::const_iterator it = mm.begin();
  
  std::string aid;
  while (it != mm.end())  
  {
    std::cout << "Annt id: " << it->second->getAnnotationtId() << std::endl 
              << " Elem id: "  << it->second->getElementId() << std::endl 
              << " Elem name: " << it->second->getElementName() << std::endl 
              << " Annt URI: " << it->second->getAnnotationURI() << std::endl << std::endl ;
    aid = it->second->getAnnotationtId();
    ++it;
  }
  
  // Find an annotation
  const te::md::MetadataAnnotation* aannot = te::md::MetadataManager::getInstance().find(aid);
  if (aannot)
    std::cout << "Annotation " << aid << " refers to element " << aannot->getElementName() << std::endl; 
  
  te::md::MetadataManager::getInstance().clear();
  
  ds->close();
}


// This example shows how to manually annotate a resource with the minimum ISO19115 core metadata elements
void 
ExMetadataISO19115()
{
  const std::string ibge("Fundação Instituto Brasileiro de Geografia e Estatística - IBGE/ Diretoria de Geociências - DGC/ Coordenação de Cartografia - CCAR");
  
  // Create the metadata root element
  
  // this basically describes the metadata record
  te::md::MD_Metadata* md = new te::md::MD_Metadata(new te::md::CI_ResponsibleParty("",ibge,"",te::md::CI_originator));
  md->setFileId("ibge_topo");
  md->setLanguage("por; BRA");
  md->setCharset(te::md::MD_utf8);
  md->setStandardInfo("ISO 19115", "2003");  
  md->setDateStamp(boost::gregorian::date(boost::gregorian::from_undelimited_string("20041210")));
  md->setMetadataURI(""TE_DATA_EXAMPLE_DIR"/data/ibge_topo.xml");
  // now comes the identification section, with information about one or more resources
  
  // resource citation - part of data identification
  te::md::CI_Citation* dt_ct = new te::md::CI_Citation("Carta Topográfica 1:25.000 SD24-X-A-VI-1-NO",
                                                       boost::gregorian::date(boost::gregorian::from_undelimited_string("19931201")),
                                                       te::md::CI_creation);
  
  // resource abstract - part of data identification
  std::string abstract("A série de carta topográfica na escala 1: 25 000 representa parte do território nacional, de forma articulada, por folhas (segundo a convenção internacional da Carta CIM), e que abrange um quadrilátero geográfico de 7,5' de latitude por 7,5' de longitude. O IBGE desenvolve a produção dessa série, em conjunto com a DSG - Diretoria de Serviço Geográfico do Exército. Esta base vetorial está estruturada em categorias de informação: Hidrografia, Hipsografia, Localidades, Limites, Sistemas de Transporte, Pontos de Referência, Vegetação e Obras e Edificações, segundo modelagem da Mapoteca Topográfica Digital do IBGE.");
  
  // the dataset identification
  te::md::MD_DataIdentification* dt_id = new te::md::MD_DataIdentification(dt_ct, abstract, te::md::MD_completed, "pt; BR");
  
  dt_id->addCharacterSet(te::md::MD_utf8);   
  
  te::md::CI_ResponsibleParty* ds_poc = new te::md::CI_ResponsibleParty("",ibge,"",te::md::CI_distributor);
  dt_id->addPointOfContact(ds_poc);
  
  dt_id->addTopicCategory(te::md::MD_imageryBaseMapsEarthCover);
  
  dt_id->setSpatialRepType(te::md::MD_vector);

  dt_id->addGeographicBBoxExt(-45, -25, -40, -20); 
  
  dt_id->setScale(25000);
  
  // associates the data identification to the metadata record
  md->addIdentification(dt_id);
  
  // distribution information
  te::md::MD_Distribution* dist = new te::md::MD_Distribution();
  dist->addDistrOption(new te::md::MD_Format("Microstation DGN","Windows NT"));
  dist->setURL("http://www.ibge.gov.br/home/geociencias/default_prod.shtm#TOPO");
  
  md->setDistribution(dist);
  
  // reference system
  md->setReferenceSystem(TE_SRS_SAD69_UTM_ZONE_24S);
  
  // lineage
  md->setLineage("folha topográfica obtida a partir de levantamento aerofotogramétrico, na escala de 1:25.000, com apoio de campo por GPS e restituída em ambiente digital, através de equipamento restituidor fotogramétrico convencional (B9) repotencializado com aplicativo dedicado, elaborado em ambiente MicroStation");
  
  // encode the metadata record and persist it to its location.
  std::ofstream ofs(md->getMetadataURI().c_str());
  te::xml::Writer writer(ofs);
  te::md::Save(md, writer);
  
  delete md;
}

void ExMetadataExtractor()
{
  std::string ogrInfo("connection_string="TE_DATA_EXAMPLE_DIR"/data/shp");
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/shp";
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  ds->setConnectionInfo(connInfo);
  ds->open();
  
  std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();
  
  std::vector<std::string> datasets = transactor->getDataSetNames();
   
  if (datasets.empty())
  {
    ds->close();
    throw te::common::Exception("Couldn't find the datasets in the datasource");
  }
  
  std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet(datasets[0]);
  bool isbb = dataset->isBeforeBegin();

  if (dataset->isBeforeBegin()) dataset->moveFirst();

  if(!dataset.get())
    return;
  
  te::md::MD_Metadata* md = te::md::Extract(dataset.get());
  
  delete md;
  ds->close();
}

void ExMetadataCycle()
{
  // Open a data source
  std::string ogrInfo("connection_string="TE_DATA_EXAMPLE_DIR"/data/shp");
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/shp";
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  ds->setConnectionInfo(connInfo);
  ds->open();
  
  std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();
  
  std::vector<std::string> datasets = transactor->getDataSetNames();
  
  
  // Create an annotation for each dataset in the datasourc
  for (unsigned int i=0; i<datasets.size(); ++i)
  {
    std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet(datasets[i]);
    std::auto_ptr<te::da::DataSetType> dt = transactor->getDataSetType(datasets[i]);
 
    if (dataset->isEmpty()) continue; //soilmeasure is empty.

    if (dataset->isBeforeBegin()) dataset->moveFirst(); //otherwise m_currentFeature is invalid in getWKB()

    te::md::MD_Metadata* md = te::md::Extract(dataset.get());
        
    // include manually other metadata elements
    // ...
    boost::format fname("%1%/%2%.xml");
    fname = fname % TE_DATA_EXAMPLE_DIR"/data/shp" % dt->getTitle();
    std::string ffname = fname.str();
    md->setMetadataURI(ffname);
    // serialize it
    std::ofstream ofs(md->getMetadataURI().c_str());
    te::xml::Writer writer(ofs);
    te::md::Save(md, writer);
    
    
    // feed the manager
    te::md::MetadataAnnotation* anot = new te::md::MetadataAnnotation();
    anot->setElementId(boost::lexical_cast<std::string>(i));
    anot->setElementName(dt->getTitle());
 
    anot->setAnnotationURI(md->getMetadataURI());    
    te::md::MetadataManager::getInstance().insert(anot);  
    
    delete md;
  }
  
  // Print manager contents
  const std::map<std::string,te::md::MetadataAnnotation*>& mm = te::md::MetadataManager::getInstance().getAnnotations();
  std::map<std::string,te::md::MetadataAnnotation*>::const_iterator it = mm.begin();
  std::string aid;
  while (it != mm.end())  
  {
    std::cout << "Annt id: " << it->second->getAnnotationtId()  << std::endl
    << " Elem id: "  << it->second->getElementId() << std::endl
    << " Elem name: " << it->second->getElementName() << std::endl
    << " Annt URI: " << it->second->getAnnotationURI() << std::endl << std::endl;
    aid = it->second->getAnnotationtId();
    ++it;
  }
  
  // manager should be persisted
  // ...
  // 
  
  te::md::MetadataManager::getInstance().clear();  
  ds->close();
}

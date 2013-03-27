// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void MovingObjectExamples()
{
  try
  {
    std::vector<te::st::MovingObject*> output;
    
    std::string XMLFileName = TE_DATA_EXAMPLE_DIR"\\data\\kml\\t_40_41_metadata.xml";
  
    MovingObjectsFromKMLAndMetadata(output, XMLFileName);

    //PrintMovingObjects(output);
    
    PrintMovingObjectInfo(output); //temporal extent, spatial extent!

    //Calculate the distance between two moving objects
    //if(output.size()>1)
    //{
    //  PrintMovingObjectDistance(output[0], output[1]);
    //}

    //==== extrair trajetorias
    te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
    std::string ogrInfo("connection_string="TE_DATA_EXAMPLE_DIR"\\data\\kml\\t41_region.shp");
    ds->setConnectionStr(ogrInfo);
    ds->open();
    te::da::DataSourceTransactor* t = ds->getTransactor();
    te::da::DataSet* dSet = t->getDataSet("t41_region");
    te::gm::Geometry* geom = 0;
    
    if(dSet->moveNext())
    {
      geom = dSet->getGeometry();
       
      PrintMovingObjectIntersection(output[1], geom);
      PrintMovingObjectDifference(output[1], geom);
    }

    delete geom;
    delete dSet;
    delete t;
    delete ds;
    
    //PrintMovingObjectByTemporalResolution();

    te::common::FreeContents(output);
  }
  
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in MovingObjectExamples: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in MovingObjectExamples!" << std::endl;
  } 
}


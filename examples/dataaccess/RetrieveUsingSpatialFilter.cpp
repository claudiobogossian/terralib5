// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools/DataSetLayer.h>

// STL
#include <iostream>

void RetrieveUsingSpatialFilter(te::da::DataSourceTransactor* transactor)
{
// Let's get a DataSet called 'br_munic_2001' in the 'public' schema
// and retrieve all its data
  {
    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001");

    //const te::da::DataSetType* dType = dataset->getType();

    std::size_t pos = te::da::GetFirstPropertyPos(dataset, te::dt::GEOMETRY_TYPE);
    //int pos = static_cast<int>(dType->getDefaultGeomPropertyPos());

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001 ===" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);

      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = dataset->getGeometry(pos);

      //std::cout << dType->getProperty(pos)->getName() << ": " << g->asText() << std::endl;
      //std::cout << dType->getProperty(pos)->getName() << ": " ; //<< g->asText() << std::endl;
      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }

    delete dataset;

    std::cout << std::endl;
  }

  // 2 variables used in other examples...
  te::gm::Geometry* geomMunic;
  int srid;

// now we will retrieve all cities that contains a given point
  {
    te::gm::Point pt( -43.6107606714293, -20.3913548070123, 4291, 0);

    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001", &pt, te::gm::INTERSECTS);

    //const te::da::DataSetType* dType = dataset->getType();

    std::size_t pos = te::da::GetFirstPropertyPos(dataset, te::dt::GEOMETRY_TYPE);
    //int pos = static_cast<int>(dType->getDefaultGeomPropertyPos());

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001 ===" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = dataset->getGeometry(pos);
      srid = g->getSRID();

      geomMunic =  static_cast<te::gm::Geometry*>(dataset->getGeometry(pos)->clone());

      //std::cout << dType->getProperty(pos)->getName() << ": " << g->asText() << std::endl;
      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }

    delete dataset;

    std::cout << std::endl;
  }

  geomMunic->computeMBR(true);

// now we will retrieve all cities that intercepts a given geometry (OuroPreto Geometry recovered above)
  {
    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001", geomMunic, te::gm::INTERSECTS);

    //const te::da::DataSetType* dType = dataset->getType();
    
    std::size_t pos = te::da::GetFirstPropertyPos(dataset, te::dt::GEOMETRY_TYPE);
    //int pos = static_cast<int>(dType->getDefaultGeomPropertyPos());

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001 ===" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = dataset->getGeometry(pos);

      //std::cout << dType->getProperty(pos)->getName() << ": " << g->asText() << std::endl;
      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }

    delete dataset;

    std::cout << std::endl;
  }

  delete geomMunic;

// now we will retrieve all cities that intercepts a given Polygon formed by OuroPreto_box with one inserted point
  {
	  te::gm::LinearRing* s = new te::gm::LinearRing(6, te::gm::LineStringType);
	  s->setPoint(0, -43.932979522347395, -20.632799968306863);  
	  s->setPoint(1, -43.932979522347395, -20.161208868097958);
	  s->setPoint(2, -43.600000000000000, -19.500000000000000); //inserted point
	  s->setPoint(3, -43.403643659752738, -20.161208868097958); 
	  s->setPoint(4, -43.403643659752738, -20.632799968306863);
    s->setPoint(5, -43.932979522347395, -20.632799968306863); // closing

	  te::gm::Polygon* pol = new te::gm::Polygon(0, te::gm::PolygonType);
	  pol->push_back(s);
    pol->setSRID(srid); //srid =  4291

    std::cout << std::endl << "Polygon NPoints: " << pol->getNPoints() << std::endl; 

    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001", pol, te::gm::INTERSECTS);

    //const te::da::DataSetType* dType= dataset->getType();

    //int pos = static_cast<int>(dType->getDefaultGeomPropertyPos());
    std::size_t pos = te::da::GetFirstPropertyPos(dataset, te::dt::GEOMETRY_TYPE);

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001 ===" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = dataset->getGeometry(pos);

      //std::cout << dType->getProperty(pos)->getName() << ": " << g->asText() << std::endl;
      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }

    delete dataset;

    std::cout << std::endl;
  }

// now we will retrieve all cities and then we will refine with cities that contains a given point
  {
    //te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001");
    std::auto_ptr<te::map::DataSetLayer> layer(new te::map::DataSetLayer("public.br_munic_2001"));

    te::gm::Point pt(-43.6107606714293, -20.3913548070123 , 4291, 0);

    //dataset->setFilter(&pt, te::gm::INTERSECTS);
    te::da::DataSet* dataset = layer->getData(pt, te::gm::INTERSECTS);

    //const te::da::DataSetType* dType = dataset->getType();

    //int pos = static_cast<int>(dType->getDefaultGeomPropertyPos());
    std::size_t pos = te::da::GetFirstPropertyPos(dataset, te::dt::GEOMETRY_TYPE);

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001 ===" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = dataset->getGeometry(pos);

      //std::cout << dType->getProperty(pos)->getName() << ": " << g->asText() << std::endl;
      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }

    delete dataset;

    std::cout << std::endl;
  }   

// now we will retrieve all cities that intersects a given box
  {
    te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);

    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001", &box, te::gm::INTERSECTS);

    //const te::da::DataSetType* dType = dataset->getType();

    //int pos = static_cast<int>(dType->getDefaultGeomPropertyPos());
    std::size_t pos = te::da::GetFirstPropertyPos(dataset, te::dt::GEOMETRY_TYPE);

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001 ===" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = dataset->getGeometry(pos);

      //std::cout << dType->getProperty(pos)->getName() << ": " << g->asText() << std::endl;
      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }

    delete dataset;

    std::cout << std::endl;
  }   

// now we will retrieve all cities and then refine with a box filter
  {
     te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);

    //te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001");
    std::auto_ptr<te::map::DataSetLayer> layer(new te::map::DataSetLayer("public.br_munic_2001"));
    te::da::DataSet* dataset = layer->getData(box, te::gm::INTERSECTS);

    //dataset->setFilter(&box, te::gm::INTERSECTS);

    //const te::da::DataSetType* dType = dataset->getType();

    //int pos = static_cast<int>(dType->getDefaultGeomPropertyPos());
    std::size_t pos = te::da::GetFirstPropertyPos(dataset, te::dt::GEOMETRY_TYPE);

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001 ===" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = dataset->getGeometry(pos);

      //std::cout << dType->getProperty(pos)->getName() << ": " << g->asText() << std::endl;
      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }

    delete dataset;

    std::cout << std::endl;
  }
}

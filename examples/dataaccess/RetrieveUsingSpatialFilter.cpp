// Examples
#include "DataAccessExamples.h"
#include <iostream>

void RetrieveUsingSpatialFilter(te::da::DataSource* ds)
{
// Let's get a DataSet called 'br_munic_2001' in the 'public' schema
// and retrieve all its data

  std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();
  {
    std::auto_ptr<te::da::DataSet> dataset = ds->getDataSet("public.br_munic_2001");

    std::size_t pos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001: 5 first rows" << std::endl;

    while(dataset->moveNext() && row<5)
    {
      std::string municName = dataset->getString(2);

      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = (dataset->getGeometry(pos)).release();

      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }

    std::cout << std::endl;
  }

  // Defining 2 variables used in other examples...
  te::gm::Geometry* geomMunic;
  int srid;

// now we will retrieve all cities that contains a given point
  {
    te::gm::Point pt( -43.6107606714293, -20.3913548070123, 4291, 0);

    std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet("public.br_munic_2001", "geom", &pt, te::gm::INTERSECTS);

    std::size_t pos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001: objects that intercepts location (-43.6107606714293, -20.3913548070123)" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = static_cast<te::gm::Geometry*>(dataset->getGeometry(pos)->clone());
      //te::gm::Geometry* g = (dataset->getGeometry(pos)).get();
      srid = g->getSRID();

      geomMunic =  static_cast<te::gm::Geometry*>(dataset->getGeometry(pos)->clone());

      std::cout << dataset->getPropertyName(pos) << ": " << geomMunic->asText() << std::endl;

    }
    std::cout << std::endl;
  }

  geomMunic->computeMBR(true);

// now we will retrieve all cities that intercepts a given geometry (OuroPreto Geometry recovered above)
  {
    std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet("public.br_munic_2001", "geom", geomMunic, te::gm::INTERSECTS);
  
    std::size_t pos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);
 
    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001: objects that intercepts the last object found in the previous query" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = static_cast<te::gm::Geometry*>(dataset->getGeometry(pos)->clone());

      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }
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

    std::cout << std::endl << "Created a polygon with: " << pol->getNPoints() << "points. " << std::endl;

    std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet("public.br_munic_2001", "geom", pol, te::gm::INTERSECTS);

    std::size_t pos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001: objects that intercept the polygon" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t"; 

      te::gm::Geometry* g = static_cast<te::gm::Geometry*>(dataset->getGeometry(pos)->clone());

      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }
    std::cout << std::endl;
  }

// now we will retrieve all cities that intersects a given box
  {
    te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);

    std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet("public.br_munic_2001", "geom", &box, te::gm::INTERSECTS);

    std::size_t pos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);

    int row = 0;

    std::cout << std::endl << "=== DataSet: public.br_munic_2001: objects that intercept a box" << std::endl;

    while(dataset->moveNext())
    {
      std::string municName = dataset->getString(2);
      std::cout << std::endl << "City Name: " << municName;

      std::cout << std::endl << "ROW: " << row++ << "\t";

      te::gm::Geometry* g = static_cast<te::gm::Geometry*>(dataset->getGeometry(pos)->clone());
 
      std::cout << dataset->getPropertyName(pos) << ": " << g->asText() << std::endl;

      delete g;
    }
    std::cout << std::endl;
  }
}
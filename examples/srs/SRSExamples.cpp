#include "SRSExamples.h"

#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/srs.h>

#include <iostream>

void CoordinateSystemFactory()
{
  std::cout << "Example how to use the Coordinate System Factory - begin" << std::endl;

  std::vector<int> epsgs;

  // test codes to the main CS used in Brazil:
  // teographic CS: Corrego Alegre, WGS84, SIRGAS 2000 and SAD69
  epsgs.push_back(4225); epsgs.push_back(4326); epsgs.push_back(4674); epsgs.push_back(4618);

  // UTM SAD69 North
  for (unsigned int i=29168; i<=29172; ++i, epsgs.push_back(i));

  // UTM SAD69 South
  for (unsigned int i=29187; i<=29195; ++i, epsgs.push_back(i));

  // UTM Corrego Alegre South
  for (unsigned int i=22521; i<=22525; ++i, epsgs.push_back(i));

  // UTM WGS84 North
  for (unsigned int i=32618; i<=32622; ++i, epsgs.push_back(i));

  // UTM WGS84 South
  for (unsigned int i=32717; i<=32725; ++i, epsgs.push_back(i));

  // UTM SIRGAS North
  for (unsigned int i=31972; i<=31976; ++i, epsgs.push_back(i));

  // Polyconic SAD69
  epsgs.push_back(29101); 
  
  // Antartic Polar Stereographic WGS84
  epsgs.push_back(3031);

  // Deprecated Codes 
  epsgs.push_back(4291); 
  for (unsigned int i=29177; i<=29185; ++i, epsgs.push_back(i));

  te::srs::CoordinateSystem* cs = 0;
  for (size_t i=0; i<epsgs.size();++i)
  {
    // build a CS from an EPSG identifier
    cs = te::srs::CoordinateSystemFactory::make("EPSG",epsgs[i]);
    if (!cs)
    {
      std::cout << std::endl << "Couldn't obtain a valid coordinate system for the identifier: " << epsgs[i] << std:: endl;
      continue;
    }
    std::cout << std::endl << "EPSG " << epsgs[i] << ": " << cs->getName() << std:: endl;
    std::string wkt = cs->getWKT();
    delete cs;

    // try to reconstruct it from the its own generated wkt
    cs = te::srs::CoordinateSystemFactory::make("WKT",wkt);
    if (!cs)
      std::cout << std::endl << "Couldn't obtain a valid coordinate system for the wkt: " << epsgs[i] << std:: endl;
    else
      std::cout << std::endl << "Reconstruction from WKT valid" << std:: endl;
    delete cs;
  }


  std::cout << "Example how to use the Coordinate System Factory - end " << std::endl << std::endl;
    return;
}



void ConvertCoordinates()
{
  std::cout << "Example how Convert coordinates - end " << std::endl << std::endl;

  // 4326 : Lat/Long WGS84
  // 32723 : UTM Zone 23 South WGS

  // 4618 : Lat/Long SAD69
  // 29193 : UTM Zone 23 South SAD69

  std::cout << "Converting a geometry: " << std::endl;
  te::gm::Geometry* geom = new te::gm::Point(-45.0,-23.0,4326); 
  std::cout << geom->asText() << std::endl;

  std::cout << std::endl;

  geom->transform(32723);
  std::cout << geom->asText() << std::endl;

  delete geom;

  std::cout << std::endl;
  std::cout.setf(std::ios::fixed,std::ios::floatfield);
  std::cout.precision(5);

  std::cout << "Converting one Coordinate: " << std::endl;
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
  converter->setSourceSRID(4326);      
	converter->setTargetSRID(32723);     

  double llX = -45.0;
	double llY = 0.0;
	double xyX;
	double xyY;

	converter->convert(llX,llY,xyX,xyY);
  std::cout << llX << " " << llY << " >>>> " << xyX << " " << xyY << std::endl;

  llX = 0;
	llY = 0;
  converter->invert(xyX,xyY,llX,llY);

  std::cout << llX << " " << llY << " <<<< " << xyX << " " << xyY << std::endl;

  llX = -45.0;
	llY = -23.0;

  std::cout << std::endl;
  std::cout << "Converting a list of coordinates: " << std::endl;

  double* xs = new double[3];
	double* ys = new double[3];

 	xs[0] = -45.5;
	xs[1] = -45.6;
	xs[2] = -45.7;

	ys[0] = -23.0;
	ys[1] = -23.1;
	ys[2] = -23.2;

  converter->convert(xs,ys,3,1);

  delete xs;
  delete ys;

  std::cout << std::endl;
  std::cout << "Converting a coordinate after include a new ID and definition to PROJ4 dictionary: " << std::endl;

  // 
  int usercode = te::srs::CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=23 +south +ellps=WGS84 +datum=WGS84 +units=km +no_defs");
  converter->setSourceSRID(usercode);
  converter->setTargetSRID(4326);

  xyX = 500000 * 0.001;   // meter to kilometer
	xyY = 10000000 * 0.001; // meter to kilometer

	converter->convert(xyX,xyY,llX,llY);

  std::cout << xyX << " " << xyY << " >>>> " << llX << " " << llY << std::endl;

  std::cout << "Example how Convert coordinates - end " << std::endl << std::endl;
}

#include "SRSExamples.h"

#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/srs.h>

#include <iostream>

void CoordinateSystemFactory()
{
  std::cout << "Showing how to use the Coordinate System Factory - begin" << std::endl;

  std::vector<int> epsgs;

  // test codes to the main CS used in Brazil:
  
  // Geographic CS: Corrego Alegre, WGS84, SIRGAS 2000 and SAD69
  //  epsgs.push_back(TE_SRS_CORREGO_ALEGRE); epsgs.push_back(TE_SRS_WGS84); epsgs.push_back(TE_SRS_SIRGAS2000); epsgs.push_back(TE_SRS_SAD69);
  
  // UTM SAD69 North
  for (size_t i=TE_SRS_SAD69_UTM_ZONE_18N; i<=TE_SRS_SAD69_UTM_ZONE_22N; epsgs.push_back(i), ++i);

  // UTM SAD69 South
  for (size_t i=TE_SRS_SAD69_UTM_ZONE_17S; i<=TE_SRS_SAD69_UTM_ZONE_25S; epsgs.push_back(i), ++i);

  // UTM Corrego Alegre South
  for (size_t i=TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S; i<=TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S; epsgs.push_back(i), ++i);

  // UTM WGS84 North
  for (size_t i=TE_SRS_WGS84_UTM_ZONE_18N; i<=TE_SRS_WGS84_UTM_ZONE_22N; epsgs.push_back(i), ++i);

  // UTM WGS84 South
  for (size_t i=TE_SRS_WGS84_UTM_ZONE_17S; i<=TE_SRS_WGS84_UTM_ZONE_25S; epsgs.push_back(i), ++i);

  // UTM SIRGAS North
  for (size_t i=TE_SRS_SIRGAS2000_UTM_ZONE_18N; i<=TE_SRS_SIRGAS2000_UTM_ZONE_22N; epsgs.push_back(i), ++i);

  // Polyconic SAD69
  epsgs.push_back(TE_SRS_SAD69_POLYCONIC); 
  
  // Antartic Polar Stereographic WGS84
  epsgs.push_back(TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC);

  std::string wkt;
  te::srs::CoordinateSystem* cs = 0;
  for (size_t i=0; i<epsgs.size();++i)
  {
    // build a CS from an EPSG identifier
    cs = te::srs::CoordinateSystemFactory::make("EPSG",epsgs[i]);
    if (!cs)
    {
      std::cout << std::endl << "EPSG " << epsgs[i] << ": Couldn't obtain a valid coordinate system for the identifier" << std:: endl;
    }
    else 
    {
      std::cout << std::endl << "EPSG " << epsgs[i] << ": " << cs->getName();
      std::string wkt = cs->getWKT();
      delete cs;
      
        // try to reconstruct it from the its own generated wkt
      cs = te::srs::CoordinateSystemFactory::make("WKT",wkt);
      if (!cs)
        std::cout << ": couldn't obtain a valid coordinate system for the wkt" << std:: endl;
      else
        std::cout << ": reconstruction from WKT valid" << std:: endl;
      delete cs;
    }
  }
  std::cout << "Showing how to use the Coordinate System Factory - end " << std::endl << std::endl;
}


void RecognizeSRIDs()
{
  std::cout << "Showing how SRID's are recognized by the converter - begin" << std::endl;
  
  std::vector<int> epsgs;
  
  // test codes to the main CS used in Brazil:
  
  // Geographic CS: Corrego Alegre, WGS84, SIRGAS 2000 and SAD69
  epsgs.push_back(TE_SRS_CORREGO_ALEGRE); epsgs.push_back(TE_SRS_WGS84); epsgs.push_back(TE_SRS_SIRGAS2000); epsgs.push_back(TE_SRS_SAD69);
  
    // UTM SAD69 North
  for (size_t i=TE_SRS_SAD69_UTM_ZONE_18N; i<=TE_SRS_SAD69_UTM_ZONE_22N; epsgs.push_back(i), ++i);
  
    // UTM SAD69 South
  for (size_t i=TE_SRS_SAD69_UTM_ZONE_17S; i<=TE_SRS_SAD69_UTM_ZONE_25S; epsgs.push_back(i), ++i);
  
    // UTM Corrego Alegre South
  for (size_t i=TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S; i<=TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S; epsgs.push_back(i), ++i);
  
    // UTM WGS84 North
  for (size_t i=TE_SRS_WGS84_UTM_ZONE_18N; i<=TE_SRS_WGS84_UTM_ZONE_22N; epsgs.push_back(i), ++i);
  
    // UTM WGS84 South
  for (size_t i=TE_SRS_WGS84_UTM_ZONE_17S; i<=TE_SRS_WGS84_UTM_ZONE_25S; epsgs.push_back(i), ++i);
  
    // UTM SIRGAS North
  for (size_t i=TE_SRS_SIRGAS2000_UTM_ZONE_18N; i<=TE_SRS_SIRGAS2000_UTM_ZONE_22N; epsgs.push_back(i), ++i);
  
    // Polyconic SAD69
  epsgs.push_back(TE_SRS_SAD69_POLYCONIC); 
  
    // Antartic Polar Stereographic WGS84
  epsgs.push_back(TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC);
  
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
  for (size_t i=0; i<epsgs.size();++i)
  {
    try 
    {
      converter->setSourceSRID(epsgs[i]);
      std::cout << "SRS identifier  " << epsgs[i] << " recognized\n"; 
    }
    catch (...) {
      std::cout << "SRS identifier  " << epsgs[i] << " NOT recognized\n";
    }
  }
  std::cout << "Showing how SRID's are recognized by the converter - end " << std::endl << std::endl;
}


void ConvertCoordinates()
{
  std::cout << "Showing how Convert coordinates - end " << std::endl << std::endl;

  std::cout << "Converting a geometry: " << std::endl;
  te::gm::Geometry* geom = new te::gm::Point(-45.0,-23.0,TE_SRS_WGS84); 
  std::cout << geom->asText() << std::endl;

  std::cout << std::endl;

  geom->transform(TE_SRS_WGS84_UTM_ZONE_23S);
  std::cout << geom->asText() << std::endl;

  delete geom;

  std::cout << std::endl;
  std::cout.setf(std::ios::fixed,std::ios::floatfield);
  std::cout.precision(5);

  std::cout << "Converting one Coordinate: " << std::endl;
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
  converter->setSourceSRID(TE_SRS_WGS84);      
	converter->setTargetSRID(TE_SRS_WGS84_UTM_ZONE_23S);     

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
  converter->setTargetSRID(TE_SRS_WGS84);

  xyX = 500000 * 0.001;   // meter to kilometer
	xyY = 10000000 * 0.001; // meter to kilometer

	converter->convert(xyX,xyY,llX,llY);

  std::cout << xyX << " " << xyY << " >>>> " << llX << " " << llY << std::endl;

  std::cout << "Showing how Convert coordinates - end " << std::endl << std::endl;
}

#include "SRSExamples.h"

#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/srs.h>

#include <iostream>

void SpatialReferenceSystemManager()
{  
  std::vector<unsigned int> epsgs;

  // test codes to the main CS used in Brazil:
  
    //Geographic CS: Corrego Alegre, WGS84, SIRGAS 2000 and SAD69
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
  
  std::cout << "Describing coordinate systems by ID - begin" << std::endl;
  std::string name;
  std::string wkt;
  std::string p4txt;
  std::pair<std::string,unsigned int> srid;
  for (size_t i=0; i<epsgs.size();++i)
  {
    std::cout << "\n -- " << epsgs[i] << " -- \n";
    name = te::srs::SpatialReferenceSystemManager::getInstance().getName(epsgs[i]);
    std::cout << "  Name:  " << name;
    srid = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromName(name);
    if (srid.second == epsgs[i])
      std::cout << ". Id from name OK" << std::endl;
    else 
      std::cout << ". Id from name NOT OK" << std::endl;
 
    wkt = te::srs::SpatialReferenceSystemManager::getInstance().getWkt(epsgs[i]);
    std::cout << "  WKT:   " << wkt.substr(0,60) << "... ";
    srid = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromWkt(wkt);
    if (srid.second == epsgs[i])
      std::cout << ". Id from wkt OK" << std::endl;
    else 
      std::cout << " Id from wkt NOT OK" << std::endl; 
    
    p4txt = te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(epsgs[i]);
    std::cout << "  PROJ4: " << p4txt.substr(0,60) << "... ";
    srid = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(p4txt);
    if (srid.second == epsgs[i])
      std::cout << ". Id from PROJ4 txt OK" << std::endl;
    else 
      std::cout << ". Id from PROJ4 txt NOT OK" << std::endl;
  }
  std::cout << "\nDescribing coordinate systems by ID - end " << std::endl << std::endl;
  
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
  std::cout << "Converting a coordinate after include a CS to the Manager: " << std::endl;

  std::string p4txt = "+proj=utm +zone=23 +south +ellps=WGS84 +datum=WGS84 +units=km +no_defs";
  converter->setSourcePJ4txt(p4txt);
  converter->setTargetSRID(TE_SRS_WGS84);

  xyX = 500000 * 0.001;   // meter to kilometer
	xyY = 10000000 * 0.001; // meter to kilometer

	converter->convert(xyX,xyY,llX,llY);

  std::cout << xyX << " " << xyY << " >>>> " << llX << " " << llY << std::endl;

  std::cout << "Showing how Convert coordinates - end " << std::endl << std::endl;
}

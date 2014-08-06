/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file Utils.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Utils.h"
#include "pattern/singleton/Context.h"
#include "../../color/RGBAColor.h"
#include "../../geometry/Polygon.h"
#include "../../geometry/Enums.h"
#include "../../geometry/LinearRing.h"
#include "../../geometry/Point.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../item/PaperConfig.h"
#include "enum/AbstractType.h"
#include "../../srs/SpatialReferenceSystemManager.h"
#include "../../common/Translator.h"

// STL
#include <math.h> 
#include <string>
#include <sstream> 
#include <exception>

te::layout::Utils::Utils() 
{

}

te::layout::Utils::~Utils()
{

}

void te::layout::Utils::drawRectW( te::gm::Envelope box )
{
  te::map::Canvas* canvas = Context::getInstance().getCanvas();

  if(!canvas)
  {
    return;
  }

  te::gm::Polygon* rect = new te::gm::Polygon(1, te::gm::PolygonType, 0, &box);
  
  te::gm::LinearRing* outRingPtr0 = new te::gm::LinearRing(5, te::gm::LineStringType);
  outRingPtr0->setPointN( 0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));
  outRingPtr0->setPointN( 1, te::gm::Point(box.getUpperRightX(), box.getLowerLeftY())); 
  outRingPtr0->setPointN( 2, te::gm::Point(box.getUpperRightX(), box.getUpperRightY()));
  outRingPtr0->setPointN( 3, te::gm::Point(box.getLowerLeftX(), box.getUpperRightY())); 
  outRingPtr0->setPointN( 4, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY())); 

  rect->setRingN(0, outRingPtr0);

  canvas->draw(rect);

}

void te::layout::Utils::drawLineW( te::gm::LinearRing* line )
{
  te::map::Canvas* canvas = Context::getInstance().getCanvas();

  if(!canvas)
  {
    return;
  }
    
  canvas->draw(line);
}

te::gm::LinearRing* te::layout::Utils::createSimpleLine( te::gm::Envelope box )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  line->setNumCoordinates(3);

  line->setPointN(0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));
  line->setPointN(1, te::gm::Point(box.getUpperRightX(), box.getUpperRightY()));
  line->setPointN(2, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));

  return line;
}

te::color::RGBAColor** te::layout::Utils::getImageW( te::gm::Envelope boxmm )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = Context::getInstance().getCanvas();

  if(!canvas)
  {
    return pixmap;
  }

  te::gm::Envelope boxViewport = viewportBox(boxmm);

  if(boxViewport.isValid())
  {
    pixmap = canvas->getImage(0, 0, boxViewport.getWidth(), boxViewport.getHeight());
  }
  return pixmap;
}

int te::layout::Utils::mm2pixel( double mm )
{
  int devDpi = Context::getInstance().getDpiX();
  int px = (mm * devDpi) / 25.4 ;
  return px;
}

void te::layout::Utils::configCanvas( te::gm::Envelope box, bool resize )
{
  te::gm::Envelope boxViewport = viewportBox(box);
  changeCanvas(boxViewport, box, resize); 
}

void te::layout::Utils::configGeoCanvas( te::gm::Envelope boxgeo, te::gm::Envelope boxmm, bool resize )
{
  te::gm::Envelope boxViewport = viewportBox(boxmm);
  changeCanvas(boxViewport, boxgeo, resize);
}

void te::layout::Utils::changeCanvas( te::gm::Envelope viewport, te::gm::Envelope world, bool resize /*= true*/ )
{
  te::map::Canvas* canvas = Context::getInstance().getCanvas();

  if(!canvas)
  {
    return;
  }

  if(resize)
  {
    //Transparent
    te::color::RGBAColor color(255,255,255, 0);
    canvas->setBackgroundColor(color);

    canvas->resize(viewport.getWidth(), viewport.getHeight());
  }
  
  canvas->setWindow(world.getLowerLeftX(), world.getLowerLeftY(), 
    world.getUpperRightX(), world.getUpperRightY()); 
}

te::gm::Envelope te::layout::Utils::viewportBox( te::gm::Envelope box)
{
  te::gm::Envelope boxViewport;
  
  if(!box.isValid())
    return boxViewport;

  boxViewport = viewportBoxFromMM(box);
  return boxViewport;
}

te::gm::Envelope te::layout::Utils::viewportBoxFromMM( te::gm::Envelope box )
{
  te::map::WorldDeviceTransformer transf; // World Device Transformer.

  double zoomFactor = Context::getInstance().getZoomFactor();

  if(zoomFactor < 1.)
    zoomFactor = 1.;
  
  int pxwidth = mm2pixel(box.getWidth() * zoomFactor);
  int pxheight = mm2pixel(box.getHeight() * zoomFactor);
    
  // Adjust internal renderer transformer
  transf.setTransformationParameters(box.getLowerLeftX(), box.getLowerLeftY(), 
    box.getUpperRightX(), box.getUpperRightY(), pxwidth, pxheight);

  te::gm::Envelope boxViewport = transformToViewport(transf, box);
  return boxViewport;
}

te::gm::Envelope te::layout::Utils::transformToViewport( te::map::WorldDeviceTransformer transf, te::gm::Envelope box )
{
  double px1 = 0;
  double py1 = 0;
  double px2 = 0;
  double py2 = 0;
  double pycopy = 0;
  transf.world2Device(box.getLowerLeftX(), box.getLowerLeftY(), px1, py1);
  transf.world2Device(box.getUpperRightX(), box.getUpperRightY(), px2, py2);

  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope boxViewport(px1, py1, px2, py2);
  return boxViewport;
}

te::gm::LinearRing* te::layout::Utils::addCoordsInX( te::gm::Envelope box, double axisCoord, double gap )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(te::gm::LineStringType);
  std::map<int, te::gm::Point> coords;

  int count = 1;
  for(double sub_x = box.getLowerLeftX(); sub_x < box.getUpperRightX(); sub_x +=(gap / 4.))
  {
    coords[count] = te::gm::Point(sub_x, axisCoord);
    count+=1;
  }

  line->setNumCoordinates(count + 1);
  line->setPointN(0, te::gm::Point(box.getLowerLeftX(), axisCoord));

  for(int i = 1 ; i < count ; ++i)
  {
    line->setPointN(i, coords[i]);
  }

  line->setPointN(count, te::gm::Point(box.getUpperRightX(), axisCoord));

  return line;
}

te::gm::LinearRing* te::layout::Utils::addCoordsInY( te::gm::Envelope box, double axisCoord, double gap )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(te::gm::LineStringType);
  std::map<int, te::gm::Point> coords;

  int count = 1;
  for(double sub_y = box.getLowerLeftY(); sub_y < box.getUpperRightY(); sub_y +=(gap / 4.))
  {
    coords[count] = te::gm::Point(axisCoord, sub_y);
    count+=1;
  }

  line->setNumCoordinates(count + 1);
  line->setPointN(0, te::gm::Point(axisCoord, box.getLowerLeftY()));

  for(int i = 1 ; i < count ; ++i)
  {
    line->setPointN(i, coords[i]);
  }

  line->setPointN(count, te::gm::Point(axisCoord, box.getUpperRightY()));

  return line;
}

void te::layout::Utils::textBoundingBox( double &w, double &h, std::string txt )
{
  te::map::Canvas* canvas = Context::getInstance().getCanvas();
  
  if(!canvas)
  {
    return;
  }

  w = 0;
  h = 0;

  te::gm::Polygon* poly = canvas->getTextBoundary(0, 0, txt, 0);
  if(poly)
  {
    //Box = mbr: minimum bounding rectangle
    const te::gm::Envelope* env = poly->getMBR();
    te::gm::Envelope* box = 0;
    box = const_cast<te::gm::Envelope*>(env);
    if(box)
    {
      w = box->getWidth();
      h = box->getHeight();
    }
  }
}

void te::layout::Utils::calculateRulerZoomFactor(double &factor, double &factorView)
{
  int						pageValue=210;
  double        ury = 0.;
  double        lly = 0.;
 
  factor = 1;
  factorView = 0.5;

  te::map::Canvas* canvas = Context::getInstance().getCanvas();
  PaperConfig* pConfig = Context::getInstance().getPaperConfig();

  double paperW = 0.;
  double paperH = 0.;

  pConfig->getPaperSize(paperW, paperH);

  if(pConfig->getPaperOrientantion() != Portrait)
  {
    pageValue=297;	
  }

  if( paperW > paperH)
  {
    factor= (int) (factor * paperW / pageValue);
  }
  else
  {
    factor= (int) (factor * paperH / pageValue);
  }
  if(factor < 1) 
    factor = 1;

  ury = canvas->getHeight();

  // Viewport - device coordinate system
  factorView = 1. / (733./(factorView * std::fabs(ury-lly)));
  factorView = 1. / factorView;
  
  if(factorView < 1) 
    factorView = 1;
}

te::gm::Envelope te::layout::Utils::transformToMM( te::layout::WorldTransformer transf, te::gm::Envelope boxGeo )
{
  double px1 = 0;
  double py1 = 0;
  double px2 = 0;
  double py2 = 0;
  double pycopy = 0;
  transf.system1Tosystem2(boxGeo.getLowerLeftX(), boxGeo.getLowerLeftY(), px1, py1);
  transf.system1Tosystem2(boxGeo.getUpperRightX(), boxGeo.getUpperRightY(), px2, py2);

  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope boxMM(px1, py1, px2, py2);
  return boxMM;
}

te::layout::WorldTransformer te::layout::Utils::getTransformGeo(te::gm::Envelope boxgeo, te::gm::Envelope boxmm)
{
  WorldTransformer transf; // World Transformer.

  if(!boxgeo.isValid())
    return transf;

  if(!boxmm.isValid())
    return transf;
  
  // Adjust internal renderer transformer
  transf.setTransformationParameters(boxgeo, boxmm);

  return transf;
}

std::string te::layout::Utils::convertDecimalToDegree( const double& value, bool bDegrees, bool bMinutes, bool bSeconds )
{
  std::string		degreeValue;
  double			dbValue;
  double			sec;
  double			min;

  dbValue = std::fabs(180.* value/(4.*atan(1.)));
  min = std::fabs((dbValue-(int)dbValue)*60.);
  sec = std::fabs(std::fabs((min-int(min))*60.));
  
  if(roundNumber(sec) >= 60)
  {
    min++;
    sec=0;
  }

  min = std::floor(min);
  if(min >= 60.0)
  {
    min = 0;
    dbValue++;
  }

  char n = (char)-80;

  if(bDegrees)
    degreeValue = convertNumberToString(std::floor(dbValue), 0);
  if(bMinutes)
    degreeValue += n + convertNumberToString(std::floor(min), 0);
  if(bSeconds)
    degreeValue += "' " + convertNumberToString(std::fabs(sec),0) + "''";

  if(bDegrees == false && bMinutes == false && bSeconds == false)
    degreeValue = convertNumberToString(std::floor(dbValue),0) + n + convertNumberToString(std::fabs(min),0) + "' " + convertNumberToString(std::fabs(sec),0) + "''";

  return degreeValue;
}

std::string te::layout::Utils::convertNumberToString( const double& value, int precision )
{
  std::ostringstream convert;
  convert.precision(precision);

  double number = value;  
  convert << number;
  return convert.str();
}

int te::layout::Utils::roundNumber( const double& value )
{
  if (value >= 0)
    return (int)(value+.5);
  else
    return (int)(value-.5);
}

double te::layout::Utils::convertDegreeToDecimal()
{
  return 0;
}

std::string te::layout::Utils::proj4DescToPlanar( int zone )
{
  /* 
  PROJ4
  +proj      Projection name
  +datum  Datum name
  +lat_0    Latitude of origin
  +lon_0   Central meridian 
  +x_0       False easting
  +y_0       False northing   
  +lat_1     Latitude of first standard parallel
  +lat_2     Latitude of second standard parallel
  +units     meters, US survey feet, etc.
  +lat_ts    Latitude of true scale
  +south   Denotes southern hemisphere UTM zone
  +no_defs Don't use the /usr/share/proj/proj_def.dat defaults file 
  */
  
  std::stringstream szone;
  szone << zone;

  std::string proj4 = "+proj=utm";
  proj4+= " +zone="+ szone.str();
  proj4+= " +south"; // pode ser +noth?
  proj4+= " +ellps=intl";
  proj4+= " +towgs84=-206,172,-6,0,0,0,0";
  proj4+= " +units=m"; 
  proj4+= " +no_defs ";
  
  return proj4;
}

std::string te::layout::Utils::proj4DescToGeodesic()
{
  std::string proj4;
  proj4 += "+proj=longlat";
  proj4 += " +ellps=aust_SA";
  proj4 += " +towgs84=-57,1,-41,0,0,0,0";
  proj4 += " +no_defs ";

  return proj4;
}

int te::layout::Utils::calculatePlanarZone( te::gm::Envelope latLongBox )
{
  double longitude = latLongBox.getCenter().x;
  int meridiano = (int)(longitude / 6);
  meridiano = meridiano * 6;

  meridiano = abs(meridiano) + 3;

  double long0 = -meridiano * TeCDR;

  // TeUTM T4
  int zone = ((int)((long0*TeCRD+183.0)/6.0));

  return zone;
}

te::common::UnitOfMeasurePtr te::layout::Utils::unitMeasure( int srid )
{
  te::common::UnitOfMeasurePtr unitPtr;

  // Checks if is Planar Geographic
  std::string authName = "EPSG"; // Now: So far it is the only one supported by TerraLib 5. Future: Review this line!
  te::srs::SpatialReferenceSystemManager::getInstance().isGeographic(srid, authName);
  unitPtr = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(srid, authName);

  return unitPtr;
}

void te::layout::Utils::remapToPlanar( te::gm::Envelope* latLongBox, int zone )
{
  if(!latLongBox->isValid())
    return;
  
  std::string proj4 = proj4DescToPlanar(zone);

  // Get the id of the projection of destination 
  std::pair<std::string, unsigned int> projMeters = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4); 

  std::string proj4geo = proj4DescToGeodesic();

  // Get the id of the projection source 
  std::pair<std::string, unsigned int> currentBoxProj = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4geo); 

  // Remapping 
  int srid = currentBoxProj.second;
  latLongBox->transform(srid, projMeters.second); 
}

void te::layout::Utils::remapToPlanar( te::gm::LinearRing* line, int zone )
{
  if(!line)
    return;

  int npoints = line->getNPoints();

  for(int i = 0 ; i < npoints ; ++i)
  {
    te::gm::Point* p = line->getPointN(i);
    const te::gm::Envelope* env = p->getMBR();
    te::gm::Envelope* en = const_cast<te::gm::Envelope*>(env);
    remapToPlanar(en, zone);
    line->setPoint(i, env->getLowerLeftX(), env->getLowerLeftY());
    p->computeMBR(true);
  }
  line->computeMBR(true);
}

void te::layout::Utils::convertToMillimeter( WorldTransformer transf, te::gm::LinearRing* line )
{
  if(!line)
    return;

  int npoints = line->getNPoints();

  for(int i = 0 ; i < npoints ; ++i)
  {
    te::gm::Point* p = line->getPointN(i);   
    double x = 0;
    double y = 0;
    transf.system1Tosystem2(p->getX(), p->getY(), x, y);    
    line->setPoint(i, x, y);  
    p->computeMBR(true);
  }

  line->computeMBR(true);
}

void te::layout::Utils::drawImage( std::string fileName, te::gm::Envelope box )
{
  te::map::Canvas* canvas = Context::getInstance().getCanvas();

  std::ifstream::pos_type size;
  char* img = imageToChar(fileName, size);
  te::map::ImageType imgType = getFileExtensionType(fileName);
  if(img)
  {
    te::gm::Envelope boxViewport = viewportBox(box);
    canvas->drawImage(0, 0, boxViewport.getWidth(), boxViewport.getHeight(), img, size, imgType);
    delete[] img;
  }
}

char* te::layout::Utils::imageToChar( std::string fileName, std::ifstream::pos_type &size )
{
  char* memblock = 0;

  if(fileName.compare("") == 0)
    return memblock;

  try 
  { 
    std::ifstream file (fileName, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
      size = file.tellg();
      memblock = new char[size]; 
      file.seekg (0, std::ios::beg);
      file.read((char*)memblock, size); // cast to a char* to give to file.read

      file.close();
    }
  }
  catch (std::ifstream::failure &e) 
  {
    std::cerr << e.what() << std::endl;
    std::string errmsg = "Exception opening/reading/closing file: \n ";
    te::common::Exception ex(TE_TR(errmsg));
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return memblock;
}

std::string te::layout::Utils::getFileExtension( std::string fileName )
{
  std::string extension = fileName.substr(fileName.find_last_of("/\\.") + 1);
  return extension;
}

te::map::ImageType te::layout::Utils::getFileExtensionType( std::string fileName )
{
  te::map::ImageType imgType;

  std::string extension = getFileExtension(fileName);
  
  if(extension.compare("png") == 0)
  {
    imgType = te::map::PNG;
  }
  else if(extension.compare("bmp") == 0)
  {
    imgType = te::map::BMP;
  }
  else if(extension.compare("jpeg") == 0 || extension.compare("jpg") == 0)
  {
    imgType = te::map::JPEG;
  }
  else if(extension.compare("gif") == 0)
  {
    imgType = te::map::GIF;
  }
  else if(extension.compare("tiff") == 0)
  {
    imgType = te::map::TIFF;
  }

  return imgType;
}

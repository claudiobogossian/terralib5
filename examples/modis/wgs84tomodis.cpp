#include "modis_utils.h"
#include <converter.h>

void WGS84ToModiSinu(double lon, double lat, double &x, double &y)
{
  double lon1 = lon;
  double lat1 = lat;
  
  // -- change the spheroid
  
  // convert to radian
  lon1 *= 0.0174532925;
  lat1 *= 0.0174532925;
  
	double equad = 2.*0.003352811-pow(0.003352811,2);
	double n = 6378137/sqrt((1-(pow(sin(lat1),2) * equad)));
  
  double x2 = n*cos(lat1)*cos(lon1);
	double y2 = n*cos(lat1)*sin(lon1);
	double z2 = (n*(1-equad))*sin(lat1);
  
	equad = 2.*0-pow(0.,2);
  
  double lat2 = lat1;
  double a;
  double d;
	do
	{
		n = 6371007.181/sqrt((1-(pow(sin(lat2),2) * equad)));
    
		a = (equad*sin(lat2)*n)+z2;
    
		lat2 = atan2(a,(sqrt(pow(x2,2) + pow(y2,2))));
    
		a = pow(sin(lat2),2)*equad;
    
		d = (6371007.181/sqrt(1-a))-n;
	}
	while (fabs(d) > 0.0000001);
  
  // convert back to degree
	lat1 = lat2 * 57.2957795;
	lon1 = atan2(y2,x2) * 57.2957795;

  // -- end change to spheroid
  
  // now convert to sinuoidal
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
  converter->setSourcePJ4txt("+proj=longlat +a=6371007.181 +b=6371007.181 +no_defs");
	converter->setTargetPJ4txt("+proj=sinu +lon_0=0 +x_0=0 +y_0=0 +a=6371007.181 +b=6371007.181 +units=m +no_defs");
  
  converter->convert(lon1,lat1,x,y);
}
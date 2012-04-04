/* !!This file will be replaced by some sort of CSV reader configurated by the application!! */

#ifdef TE_USE_PROJ4

CoordinateSystemFactory::add("PROJ4","+proj=latlong +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +no_defs",4225); // Geog Corrego Alegre
CoordinateSystemFactory::add("PROJ4","+proj=latlong +ellps=WGS84 +datum=WGS84 +no_defs",4326);  			  // Geog WGS84
CoordinateSystemFactory::add("PROJ4","+proj=latlong +ellps=GRS80 +towgs84=0",4674);             			  // Geog SIRGAS 2000
CoordinateSystemFactory::add("PROJ4","+proj=latlong +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +no_defs",4618);    						  // Geog SAD69

// UTM SAD69 North
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29168);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29169);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29170);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29171);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29172);

// UTM SAD69 South
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=17 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29187);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29188);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29189);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29190);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29191);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29192);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=23 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29193);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=24 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29194);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=25 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29195);

// UTM Corrego Alegre South
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",22521);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",22522);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=23 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",22523);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=24 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",22524);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=25 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",22525);

// UTM WGS84 North
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32618);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32619);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32620);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32621);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32622);

// UTM WGS84 South
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=17 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32717);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32718);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32719);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32720);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32721);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32722);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=23 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32723);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=24 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32724);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=25 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",32725);

// UTM SIRGAS North
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +ellps=GRS80 +towgs84=0",31972);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +ellps=GRS80 +towgs84=0",31973);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +ellps=GRS80 +towgs84=0",31974);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +ellps=GRS80 +towgs84=0",31975);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +ellps=GRS80 +towgs84=0",31976);

// Polyconic SAD69
CoordinateSystemFactory::add("PROJ4","+proj=poly +lat_0=0 +lon_0=-54 +x_0=5000000 +y_0=10000000 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",29101);

// Antartic Polar Stereographic WGS84
CoordinateSystemFactory::add("PROJ4","+proj=stere +lat_0=-90 +lat_ts=-71 +lon_0=0 +k_0=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",3031);

// Deprecated Codes
CoordinateSystemFactory::add("PROJ4","+proj=latlong +ellps=GRS67 +no_defs",4291); // SAD69 

CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=17 +south +ellps=GRS67 +units=m +no_defs",29177);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +south +ellps=GRS67 +units=m +no_defs",29178);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +south +ellps=GRS67 +units=m +no_defs",29179);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +south +ellps=GRS67 +units=m +no_defs",29180);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +south +ellps=GRS67 +units=m +no_defs",29181);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +south +ellps=GRS67 +units=m +no_defs",29182);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=23 +south +ellps=GRS67 +units=m +no_defs",29183);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=24 +south +ellps=GRS67 +units=m +no_defs",29184);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=25 +south +ellps=GRS67 +units=m +no_defs",29185);



#endif
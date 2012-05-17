/* !!This file will be replaced by some sort of CSV reader configurated by the application!! */

#ifdef TE_USE_PROJ4

CoordinateSystemFactory::add("PROJ4","+proj=latlong +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +no_defs",TE_SRS_CORREGO_ALEGRE); // Geog Corrego Alegre
CoordinateSystemFactory::add("PROJ4","+proj=latlong +ellps=WGS84 +datum=WGS84 +no_defs",TE_SRS_WGS84);  			  // Geog WGS84
CoordinateSystemFactory::add("PROJ4","+proj=latlong +ellps=GRS80 +towgs84=0",TE_SRS_SIRGAS2000);             			  // Geog SIRGAS 2000
CoordinateSystemFactory::add("PROJ4","+proj=latlong +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +no_defs",TE_SRS_SAD69);    						  // Geog SAD69

// UTM SAD69 North
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_18N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_19N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_20N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_21N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_22N);

// UTM SAD69 South
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=17 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_17S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_18S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_19S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_20S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_21S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_22S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=23 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_23S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=24 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_24S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=25 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_UTM_ZONE_25S);

// UTM Corrego Alegre South
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=23 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=24 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=25 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S);

// UTM WGS84 North
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_18N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_19N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_20N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_21N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_22N);

// UTM WGS84 South
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=17 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_17S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_18S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_19S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_20S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_21S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_22S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=23 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_23S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=24 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_24S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=25 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_UTM_ZONE_25S);

// UTM SIRGAS North
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=17 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_17N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_18N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_19N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_20N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_21N);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_22N);

  // UTM SIRGAS South
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=17 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_17S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=18 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_18S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=19 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_19S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=20 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_20S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=21 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_21S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=22 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_22S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=23 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_23S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=24 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_24S);
CoordinateSystemFactory::add("PROJ4","+proj=utm +zone=25 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",TE_SRS_SIRGAS2000_UTM_ZONE_25S);

// Polyconic SAD69
CoordinateSystemFactory::add("PROJ4","+proj=poly +lat_0=0 +lon_0=-54 +x_0=5000000 +y_0=10000000 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",TE_SRS_SAD69_POLYCONIC);

// Antartic Polar Stereographic WGS84
CoordinateSystemFactory::add("PROJ4","+proj=stere +lat_0=-90 +lat_ts=-71 +lon_0=0 +k_0=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC);

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
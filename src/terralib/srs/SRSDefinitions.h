/* !!This file will be replaced by some sort of CSV reader configurated by the application!! */

/*
 CS is identified within TerraLib by:
 - <authority, id> : external, authority id for interoperability,
 - OGC's WKT description: interoperability
 - useful, human-readable, interface-friendly name
 - PROJ4 text: to support coordinate conversion
 */

SpatialReferenceSystemManager::getInstance().add("Corrego_Alegre",              
                                           "GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]]",
                                           "+proj=latlong +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +no_defs",
                                           TE_SRS_CORREGO_ALEGRE); // Geog Corrego Alegre


SpatialReferenceSystemManager::getInstance().add("WGS_1984",
                                           "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]]",
                                           "+proj=latlong +ellps=WGS84 +datum=WGS84 +no_defs",
                                           TE_SRS_WGS84); // Geog WGS84


SpatialReferenceSystemManager::getInstance().add("Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000",
                                           "GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]]",
                                           "+proj=latlong +ellps=GRS80 +towgs84=0",
                                           TE_SRS_SIRGAS2000);             			  // Geog SIRGAS 2000

SpatialReferenceSystemManager::getInstance().add("South_American_Datum_1969",
                                           "GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]]",
                                           "+proj=latlong +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +no_defs",
                                           TE_SRS_SAD69);    						  // Geog SAD69

 // UTM / Corrego Alegre over Brazil 

SpatialReferenceSystemManager::getInstance().add("Corrego Alegre / UTM zone 21S",
                                           "PROJCS[\"Corrego Alegre / UTM zone 21S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=21 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S);

SpatialReferenceSystemManager::getInstance().add("Corrego Alegre / UTM zone 22S",
                                           "PROJCS[\"Corrego Alegre / UTM zone 22S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=22 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S);

SpatialReferenceSystemManager::getInstance().add("Corrego Alegre / UTM zone 23S",
                                           "PROJCS[\"Corrego Alegre / UTM zone 23S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-45],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=23 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S);

SpatialReferenceSystemManager::getInstance().add("Corrego Alegre / UTM zone 24S",
                                           "PROJCS[\"Corrego Alegre / UTM zone 24S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-39],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=24 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S);

SpatialReferenceSystemManager::getInstance().add("Corrego Alegre / UTM zone 25S",
                                           "PROJCS[\"Corrego Alegre / UTM zone 25S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-33],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=25 +south +ellps=intl +towgs84=-206,172,-6,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S);

  //UTM / SAD 69 over Brazil 


SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 18N",
                                           "PROJCS[\"SAD69 / UTM zone 18N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=18 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_18N);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 19N",
                                           "PROJCS[\"SAD69 / UTM zone 19N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=19 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_19N);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 20N",
                                           "PROJCS[\"SAD69 / UTM zone 20N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=20 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_20N);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 21N",
                                           "PROJCS[\"SAD69 / UTM zone 21N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=21 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_21N);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 22N",
                                           "PROJCS[\"SAD69 / UTM zone 22N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=22 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_22N);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 17S",
                                           "PROJCS[\"SAD69 / UTM zone 17S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-81],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=17 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_17S);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 18S",
                                           "PROJCS[\"SAD69 / UTM zone 18S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=18 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_18S);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 19S",
                                           "PROJCS[\"SAD69 / UTM zone 19S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=19 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_19S);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 20S",
                                           "PROJCS[\"SAD69 / UTM zone 20S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=20 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_20S);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 21S",
                                           "PROJCS[\"SAD69 / UTM zone 21S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=21 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_21S);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 22S",
                                           "PROJCS[\"SAD69 / UTM zone 22S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=22 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_22S);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 23S",
                                           "PROJCS[\"SAD69 / UTM zone 23S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-45],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=23 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_23S);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 24S",
                                           "PROJCS[\"SAD69 / UTM zone 24S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-39],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=24 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_24S);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 25S",
                                           "PROJCS[\"SAD69 / UTM zone 25S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-33],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=25 +south +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_UTM_ZONE_25S);


 // UTM / WGS 1984 over Brazil 


SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 18N",
                                           "PROJCS[\"WGS 84 / UTM zone 18N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=18 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_18N);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 19N",
                                           "PROJCS[\"WGS 84 / UTM zone 19N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=19 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_19N);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 20N",
                                           "PROJCS[\"WGS 84 / UTM zone 20N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=20 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_20N);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 21N",
                                           "PROJCS[\"WGS 84 / UTM zone 21N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=21 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_21N);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 22N",
                                           "PROJCS[\"WGS 84 / UTM zone 22N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=22 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_22N);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 17S",
                                           "PROJCS[\"WGS 84 / UTM zone 17S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-81],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=17 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_17S);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 18S",
                                           "PROJCS[\"WGS 84 / UTM zone 18S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=18 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_18S);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 19S",
                                           "PROJCS[\"WGS 84 / UTM zone 19S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=19 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_19S);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 20S",
                                           "PROJCS[\"WGS 84 / UTM zone 20S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=20 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_20S);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 21S",
                                           "PROJCS[\"WGS 84 / UTM zone 21S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=21 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_21S);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 22S",
                                           "PROJCS[\"WGS 84 / UTM zone 22S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=22 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_22S);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 23S",
                                           "PROJCS[\"WGS 84 / UTM zone 23S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-45],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=23 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_23S);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 24S",
                                           "PROJCS[\"WGS 84 / UTM zone 24S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-39],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=24 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_24S);

SpatialReferenceSystemManager::getInstance().add("WGS 84 / UTM zone 25S",
                                           "PROJCS[\"WGS 84 / UTM zone 25S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-33],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=25 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_UTM_ZONE_25S);


 // UTM / SIRGAS 2000 over Brazil 

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 17N",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 17N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=18 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_17N);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 18N",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 18N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=18 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_18N);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 19N",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 19N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=19 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_19N);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 20N",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 20N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=20 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_20N);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 21N",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 21N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=21 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_21N);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 22N",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 22N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=22 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_22N);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 17S",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 17S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-81],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=17 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_17S);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 18S",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 18S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=18 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_18S);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 19S",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 19S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=19 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_19S);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 20S",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 20S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=20 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_20S);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 21S",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 21S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=21 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_21S);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 22S",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 22S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=22 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_22S);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 23S",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 23S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-45],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=23 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_23S);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 24S",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 24S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-39],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=24 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_24S);

SpatialReferenceSystemManager::getInstance().add("SIRGAS 2000 / UTM zone 25S",
                                           "PROJCS[\"SIRGAS 2000 / UTM zone 25S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-33],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=utm +zone=25 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SIRGAS2000_UTM_ZONE_25S);

// Polyconic SAD69
SpatialReferenceSystemManager::getInstance().add("SAD69 / Brazil Polyconic",
                                           "PROJCS[\"SAD69 / Brazil Polyconic\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Polyconic\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-54],PARAMETER[\"false_easting\",5000000],PARAMETER[\"false_northing\",10000000],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH],UNIT[\"metre\",1]]",
                                           "+proj=poly +lat_0=0 +lon_0=-54 +x_0=5000000 +y_0=10000000 +ellps=aust_SA +towgs84=-67.35,3.88,-38.22,0,0,0,0 +units=m +no_defs",
                                           TE_SRS_SAD69_POLYCONIC);

  // Antartic Polar Stereographic WGS84
SpatialReferenceSystemManager::getInstance().add("WGS 84 / Antarctic Polar Stereographic",
                                           "PROJCS[\"WGS 84 / Antarctic Polar Stereographic\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Polar_Stereographic\"],PARAMETER[\"latitude_of_origin\",-71],PARAMETER[\"central_meridian\",0],PARAMETER[\"scale_factor\",1],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",UNKNOWN],AXIS[\"Northing\",UNKNOWN],UNIT[\"metre\",1]]",
                                           "+proj=stere +lat_0=-90 +lat_ts=-71 +lon_0=0 +k_0=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
                                           TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC);


  // Deprecated Codes
SpatialReferenceSystemManager::getInstance().add("SAD69 (deprecated)",
                                          "GEOGCS[\"SAD69 (deprecated)\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]]",
                                          "+proj=latlong +ellps=GRS67 +no_defs",
                                           4291); // SAD69 

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 17S (deprecated)",
                                          "PROJCS[\"SAD69 / UTM zone 17S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-81],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                          "+proj=utm +zone=17 +south +ellps=GRS67 +units=m +no_defs",
                                           29177);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 18S (deprecated)",
                                          "PROJCS[\"SAD69 / UTM zone 18S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                          "+proj=utm +zone=18 +south +ellps=GRS67 +units=m +no_defs",
                                           29178);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 19S (deprecated)",
                                          "PROJCS[\"SAD69 / UTM zone 19S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]"
                                          ,"+proj=utm +zone=19 +south +ellps=GRS67 +units=m +no_defs",
                                           29179);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 20S (deprecated)",
                                          "PROJCS[\"SAD69 / UTM zone 20S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                          "+proj=utm +zone=20 +south +ellps=GRS67 +units=m +no_defs",
                                           29180);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 21S (deprecated)",
                                          "PROJCS[\"SAD69 / UTM zone 21S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                          "+proj=utm +zone=21 +south +ellps=GRS67 +units=m +no_defs",
                                           29181);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 22S (deprecated)",
                                          "PROJCS[\"SAD69 / UTM zone 22S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                          "+proj=utm +zone=22 +south +ellps=GRS67 +units=m +no_defs",
                                           29182);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 23S (deprecated)",
                                          "PROJCS[\"SAD69 / UTM zone 23S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-45],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                          "+proj=utm +zone=23 +south +ellps=GRS67 +units=m +no_defs",
                                           29183);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 24S (deprecated)",
                                          "PROJCS[\"SAD69 / UTM zone 24S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-39],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                          "+proj=utm +zone=24 +south +ellps=GRS67 +units=m +no_defs",
                                           29184);

SpatialReferenceSystemManager::getInstance().add("SAD69 / UTM zone 25S (deprecated)",
                                          "PROJCS[\"SAD69 / UTM zone 25S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-33],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",
                                          "+proj=utm +zone=25 +south +ellps=GRS67 +units=m +no_defs",
                                           29185);

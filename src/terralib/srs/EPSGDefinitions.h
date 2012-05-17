/* !!This file will be replaced by some sort of CSV reader configurated by the application!! */

CoordinateSystemFactory::add("EPSG","GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]]",TE_SRS_CORREGO_ALEGRE); // Geog Corrego Alegre
CoordinateSystemFactory::add("EPSG","GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]]",TE_SRS_WGS84);  			  // Geog WGS84
CoordinateSystemFactory::add("EPSG","GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]]",TE_SRS_SIRGAS2000);             			  // Geog SIRGAS 2000
CoordinateSystemFactory::add("EPSG","GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]]",TE_SRS_SAD69);    						  // Geog SAD69

// UTM SAD69 North
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 18N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_18N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 19N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]", TE_SRS_SAD69_UTM_ZONE_19N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 20N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_20N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 21N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_21N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 22N\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_22N);

// UTM SAD69 South
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 17S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-81],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_17S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 18S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_18S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 19S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_19S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 20S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_20S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 21S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_21S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 22S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_22S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 23S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-45],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_23S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 24S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-39],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_24S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 25S\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-33],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_UTM_ZONE_25S);

// UTM Corrego Alegre South
CoordinateSystemFactory::add("EPSG","PROJCS[\"Corrego Alegre / UTM zone 21S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"Corrego Alegre / UTM zone 22S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"Corrego Alegre / UTM zone 23S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-45],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"Corrego Alegre / UTM zone 24S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-39],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"Corrego Alegre / UTM zone 25S\",GEOGCS[\"Corrego Alegre\",DATUM[\"Corrego_Alegre\",SPHEROID[\"International 1924\",6378388,297],TOWGS84[-206,172,-6,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-33],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S);

// UTM WGS84 North
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 18N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_18N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 19N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_19N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 20N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_20N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 21N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_21N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 22N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_22N);

// UTM WGS84 South
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 17S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-81],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_17S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 18S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_18S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 19S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_19S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 20S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_20S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 21S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_21S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 22S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_22S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 23S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-45],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_23S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 24S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-39],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_24S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / UTM zone 25S\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-33],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_WGS84_UTM_ZONE_25S);

// UTM SIRGAS North
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 17N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_17N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 18N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_18N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 19N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_19N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 20N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_20N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 21N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_21N);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 22N\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_22N);

  // UTM SIRGAS South
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 17S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_17S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 18S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_18S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 19S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_19S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 20S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_20S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 21S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_21S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 22S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_22S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 23S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_23S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 24S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_24S);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SIRGAS 2000 / UTM zone 25S\",GEOGCS[\"SIRGAS 2000\",DATUM[\"Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000\",SPHEROID[\"GRS 1980\",6378137,298.257222101],TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SIRGAS2000_UTM_ZONE_25S);


// Polyconic SAD69
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / Brazil Polyconic\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967 (SAD69)\",6378160,298.25]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Polyconic\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-54],PARAMETER[\"false_easting\",5000000],PARAMETER[\"false_northing\",10000000],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH],UNIT[\"metre\",1]]",TE_SRS_SAD69_POLYCONIC);

// Antartic Polar Stereographic WGS84
CoordinateSystemFactory::add("EPSG","PROJCS[\"WGS 84 / Antarctic Polar Stereographic\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.01745329251994328]],PROJECTION[\"Polar_Stereographic\"],PARAMETER[\"latitude_of_origin\",-71],PARAMETER[\"central_meridian\",0],PARAMETER[\"scale_factor\",1],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],AXIS[\"Easting\",UNKNOWN],AXIS[\"Northing\",UNKNOWN],UNIT[\"metre\",1]]",TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC);

// Deprecated Codes
CoordinateSystemFactory::add("EPSG","GEOGCS[\"SAD69 (deprecated)\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]]",4291); // SAD69 

CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 17S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-81],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",29177);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 18S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-75],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",29178);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 19S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-69],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",29179);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 20S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-63],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",29180);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 21S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-57],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",29181);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 22S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-51],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",29182);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 23S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-45],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",29183);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 24S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-39],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",29184);
CoordinateSystemFactory::add("EPSG","PROJCS[\"SAD69 / UTM zone 25S (deprecated)\",GEOGCS[\"SAD69\",DATUM[\"South_American_Datum_1969\",SPHEROID[\"GRS 1967\",6378160,298.247167427]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-33],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",10000000],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH],UNIT[\"metre\",1]]",29185);

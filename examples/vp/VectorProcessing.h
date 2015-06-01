#ifndef __VECTORPROCESSINGEXAMPLES_H
#define __VECTORPROCESSINGEXAMPLES_H

// This examples show how to call the Geographic Operation Aggregation algorithm on vector data
// Adapt the source and target datasource information to your environment!

//---------------Aggregation---------------//

//Input data is from a shape file and the output is save in an another shape file.
bool AggregOGRToOGR();

//Input data is from a shape file and the output is save in a postgis database.
bool AggregOGRToPGIS();

//Input data is from a postgis database and the output is save in a postgis too.
bool AggregPGISToPGIS();

//Input data is from a postgis database and the output is save in a shape file.
bool AggregPGISToOGR();


//---------------Intersection---------------//

//Input data is from a shape file and the output is save in an another shape file.
bool IntersectionOGRToOGR();

//Input data is from a shape file and the output is save in a postgis database.
bool IntersectionOGRToPGIS();

//Input data is from a postgis database and the output is save in a postgis too.
bool IntersectionPGISToPGIS();

//Input data is from a postgis database and the output is save in a shape file.
bool IntersectionPGISToOGR();


//---------------Buffer---------------//

//Input data is from a shape file and the output is save in an another shape file.
bool BufferOGRToOGR();

//Input data is from a shape file and the output is save in a postgis database.
bool BufferOGRToPGIS();

//Input data is from a postgis database and the output is save in a postgis too.
bool BufferPGISToPGIS();

//Input data is from a postgis database and the output is save in a shape file.
bool BufferPGISToOGR();


#endif
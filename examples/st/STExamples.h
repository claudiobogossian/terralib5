/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file STExamples.h

  \brief Examples on how to load/manipulate st types.
 */

#ifndef __TERRALIB_EXAMPLES_STEXAMPLES_INTERNAL_STEXAMPLES_H
#define __TERRALIB_EXAMPLES_STEXAMPLES_INTERNAL_STEXAMPLES_H

// TerraLib
#include <terralib/common_fw.h>
#include <terralib/dataaccess_fw.h>
#include <terralib/geometry_fw.h>
#include <terralib/st_fw.h>
#include <terralib/stloader_fw.h>

// STL
#include <vector>
#include <string>

//ST examples
#include "Config.h"

/*! 
  \brief It contains examples about moving objects
*/
void MovingObjectExamples();

/*! 
  \brief It contains examples about coverage
*/
void CoverageExamples();

/*! 
  \brief It loads moving object from KML, by using a metadata file
*/
void MovingObjectsFromKMLAndMetadata(std::vector<te::st::MovingObject*>& output, 
                                     const std::string& xmlFile);
/*! 
  \brief It loads moving object from PostGIS
*/
void MovingObjectsFromPostGIS(std::vector<te::st::MovingObject*>& output);

/*! 
  \brief It prints the moving objects
*/
void PrintMovingObjects(std::vector<te::st::MovingObject*>& output);

/*! 
  \brief It prints a set of trajectories
*/
void PrintTrajectories(std::vector<te::st::Trajectory*>& output);

/*! 
  \brief It prints a time series
*/
void PrintTimeSeries(te::st::TimeSeries* ts);

/*! 
  \brief It prints information about the moving objects, such as temporal and spatial extensions
*/
void PrintMovingObjectInfo(std::vector<te::st::MovingObject*>& output);

/*! 
  \brief It prints the distance between two moving objects. 
*/
void PrintMovingObjectDistance(te::st::MovingObject* mo1, te::st::MovingObject* mo2);

/*! 
  \brief It prints the trajectories generated by the intersection 
  between a moving object and a geometry. 
*/
void PrintMovingObjectIntersection(te::st::MovingObject* mo, te::gm::Geometry* geom);

/*! 
  \brief It prints the trajectories generated by the difference 
  between a moving object and a geometry. 
*/
void PrintMovingObjectDifference(te::st::MovingObject* mo, te::gm::Geometry* geom);

/*!
  \brief This example will print to standard output all information about the datasets stored in the data source.

  \param ds The data source used to retrieve information about its datasets.
 */
void PrintCatalog(te::da::DataSource* ds);

/*!
  \brief It prints datasets in a given data source.

  \param ds The data source you want to print its data.
 */
void PrintDataSets(te::da::DataSource* ds);

/*!
  \brief It prints the data in a given dataset.

  \param dataset Any dataset.
 */
void PrintDataSet(te::da::DataSet* dataset);

/*!
  \brief It prints the type of a given dataset.
*/
void PrintDataSetType(te::da::DataSet* ds);

/*! \brief It loads the data source drivers. */
void LoadModules();


#endif  // __TERRALIB_EXAMPLES_STEXAMPLES_INTERNAL_STEXAMPLES_H


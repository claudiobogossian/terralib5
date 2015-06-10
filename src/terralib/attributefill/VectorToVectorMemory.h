/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/attributefill/VectorToVectorMemory.h

  \brief A class that execute vector to vector operations in the memory.
*/

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTORMEMORY_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTORMEMORY_H

//Terralib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../datatype/Property.h"
#include "../maptools/AbstractLayer.h"
#include "../memory/DataSet.h"
#include "../sam/kdtree.h"
#include "Config.h"
#include "VectorToVectorOp.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

typedef te::sam::kdtree::AdaptativeNode<te::gm::Coord2D, std::vector<te::gm::Point>, te::gm::Point> KD_ADAPTATIVE_NODE;
typedef te::sam::kdtree::AdaptativeIndex<KD_ADAPTATIVE_NODE> KD_ADAPTATIVE_TREE;

namespace te
{
  namespace dt
  {
    class AbstractData;
  }

  namespace gm
  {
    class Geometry;
  }

  namespace stat
  {
    struct NumericStatisticalSummary;
    struct StringStatisticalSummary;
  }

  namespace attributefill
  {
    /*!
      \class VectorToVector

      \brief A class that execute vector to vector operations in the memory.

      \sa DataSourceManager, DataSourceFactory, DataSet, DataSetType
    */
    class TEATTRIBUTEFILLEXPORT VectorToVectorMemory : public VectorToVectorOp
    {
      public:

        /*!
          \brief Constructor.
        */
        VectorToVectorMemory();

        ~VectorToVectorMemory();

        /*!
          \brief It set the input informations

          \param fromLayer Te "From" Layer.
          \param toLayer  The "To" Layer.
        */
        void setInput(te::map::AbstractLayerPtr fromLayer,
                      te::map::AbstractLayerPtr toLayer);

        /*!
          \brief It set the params used to execute.

          \param options Property and string vector map with informations about which property was selected
                         and which operations were selected to this property.
        */
        void setParams(const std::map<te::dt::Property*, std::vector<std::string> >& options);

        /*!
          \brief It set output.

          \param outDsrc The Data Source that will be the result.
          \param dsName  The name of output data set.
        */
        void setOutput(te::da::DataSourcePtr outDsrc, std::string dsName);

        /*!
          \brief It execute the operations.

          \return True if execute.
        */
        bool run();

      protected:

        /*!
          \brief It return a RTree with data set iterator position information.

          \param data The Data Set.
          
          \return The Rtree.
        */
        te::sam::rtree::Index<size_t, 8>* getRtree(te::da::DataSet* data);

        /*!
          \brief It return a KDTree.

          \param data   The Data Set.
          \param toSrid Data Set srid.
          
          \return The KDtree.
        */
        KD_ADAPTATIVE_TREE* getKDtree(te::da::DataSet* data, std::size_t toSrid);

        /*!
          \brief It create a data set type based on selecteds properties and operations.

          \return A Data Set Type.
        */
        te::da::DataSetType* getOutputDataSetType();

        /*!
          \brief It return distincts values of a property from the "From" data set.

          \param fromDs       The "From" data set.
          \param propertyName The property name with the information.
          
          \return A vector of string values.
        */
        std::vector<std::string> getDistinctClasses(te::da::DataSet* fromDs, const std::string& propertyName);

        /*!
          \brief It return a name based on original property name and the selected operation.

          \param prop The original property.
          \param func The selected operation.
          
          \return A name based on the property and operation.
        */
        std::string getPropertyName(te::dt::Property* prop, te::attributefill::OperationType func);

        /*!
          \brief It verify if the operation is a statistical operation.

          \param type The operation type.
          
          \return True if the operation were statistical.
        */
        bool isStatistical(te::attributefill::OperationType type);

        /*!
          \brief It verify all intersection between the "From" and "To" data sets.

          \param toDs   The "To" data set.
          \param fromDs The "From" data set.
          \param rtree  A generated RTree based on the "From" data set.
          
          \return A vector of positions that "To" data set has intersection with "From".
        */
        std::vector<std::size_t> getIntersections(te::da::DataSet* toDs,
                                                  te::da::DataSet* fromDs,
                                                  te::sam::rtree::Index<size_t, 8>* rtree);

        /*!
          \brief It get the value of all positions in "To" data set that has intersection with
                 a position of "From" data set.

          \param fromDs The "From" data set.
          \param dsPos  Vector of intersections positions.
          \param propertyName The property name.
          
          \note The fromDs iterator must be in the correct position.

          \return A vector of values.
        */
        std::vector<te::dt::AbstractData*> getDataValues(te::da::DataSet* fromDs,
                                                         std::vector<std::size_t> dsPos,
                                                         const std::string& propertyName);

        /*!
          \brief It get the numeric values of a vector of abstract data.

          \param data Vector of abstract data.

          \return A vector of double values.
        */
        std::vector<double> getNumValues(std::vector<te::dt::AbstractData*> data);

        /*!
          \brief It get the string values of a vector of abstract data.

          \param data Vector of abstract data.

          \return A vector of string values.
        */
        std::vector<std::string> getStrValues(std::vector<te::dt::AbstractData*> data);

        /*!
          \brief It get the value of required operation type from numeric statistical summary

          \param ss   The numeric statistical summary.
          \param type Required operation type.

          \return The double value.
        */
        double getValue(te::stat::NumericStatisticalSummary ss, te::attributefill::OperationType type);

        /*!
          \brief It get the value of required operation type from string statistical summary

          \param ss   The string statistical summary.
          \param type Required operation type.

          \return The string value.
        */
        std::string getValue(te::stat::StringStatisticalSummary ss, te::attributefill::OperationType type);

        /*!
          \brief It get a string with the mode operation format.

          \param ss   The string statistical summary.

          \return The string value.
        */
        std::string getModeValue(te::stat::NumericStatisticalSummary ss);

        /*!
          \brief It get the class with highest occurrence from "From" data set in "To" data set.

          \param fromDs       The "From" data set.
          \param dsPos        Vector of "To" data set positions that intersect "From".
          \param propertyName Required property name.

          \return The class value.
        */
        te::dt::AbstractData* getClassWithHighestOccurrence(te::da::DataSet* fromDs,
                                                            std::vector<std::size_t> dsPos,
                                                            const std::string& propertyName);

        /*!
          \brief It get the class with highest intersection area from "From" data set in "To" data set.

          \param toDs         The "To" data set.
          \param toSrid       The "To" data set srid.
          \param fromDs       The "From" data set.
          \param fromSrid     The "From" data set srid.
          \param dsPos        Vector of "To" data set positions that intersect "From".
          \param propertyName Required property name.

          \return The class value.
        */
        te::dt::AbstractData* getClassWithHighestIntersectionArea(te::da::DataSet* toDs,
                                                                  std::size_t toSrid,
                                                                  te::da::DataSet* fromDs,
                                                                  std::size_t fromSrid,
                                                                  std::vector<std::size_t> dsPos,
                                                                  const std::string& propertyName);

        /*!
          \brief It get a abstract data with the value based on the type.

          \param strValue The string value.
          \param type     The value type.

          \return The value.
        */
        te::dt::AbstractData* getDataBasedOnType(const std::string& strValue, const int type);

        /*!
          \brief It get the percent per class from "From" data set in "To" data set.

          \param fromDs       The "From" data set.
          \param dsPos        Vector of "To" data set positions that intersect "From".
          \param propertyName Required property name.

          \return Map with class value and percentage.
        */
        std::map<std::string, double> getPercentagePerClass(te::da::DataSet* fromDs,
                                                            std::vector<std::size_t> dsPos,
                                                            const std::string& propertyName);

        /*!
          \brief It get the percentage of intersection area in the total area.

          \param toDs         The "To" data set.
          \param toSrid       The "To" data set srid.
          \param fromDs       The "From" data set.
          \param fromSrid     The "From" data set srid.
          \param dsPos        Vector of "To" data set positions that intersect "From".
          \param propertyName Required property name.

          \return The percentage.
        */
        double getPercentageOfTotalArea(te::da::DataSet* toDs,
                                        std::size_t toSrid,
                                        te::da::DataSet* fromDs,
                                        std::size_t fromSrid,
                                        std::vector<std::size_t> dsPos,
                                        const std::string& propertyName);

        /*!
          \brief It get the percentage of each class intersection area in the total area.

          \param toDs         The "To" data set.
          \param toSrid       The "To" data set srid.
          \param fromDs       The "From" data set.
          \param fromSrid     The "From" data set srid.
          \param dsPos        Vector of "To" data set positions that intersect "From".
          \param propertyName Required property name.

          \return Map of class and percentage.
        */
        std::map<std::string, double> getPercentageOfEachClassByArea(te::da::DataSet* toDs,
                                                                     std::size_t toSrid,
                                                                     te::da::DataSet* fromDs,
                                                                     std::size_t fromSrid,
                                                                     std::vector<std::size_t> dsPos,
                                                                     const std::string& propertyName);

        /*!
          \brief It get the weighted average if intersections values.
          
          \param toDs         The "To" data set.
          \param toSrid       The "To" data set srid.
          \param fromDs       The "From" data set.
          \param fromSrid     The "From" data set srid.
          \param dsPos        Vector of "To" data set positions that intersect "From".
          \param propertyName Required property name.

          \return The weighted average.
        */
        double getWeightedByArea(te::da::DataSet* toDs,
                                 std::size_t toSrid,
                                 te::da::DataSet* fromDs,
                                 std::size_t fromSrid,
                                 std::vector<std::size_t> dsPos,
                                 const std::string& propertyName);

        /*!
          \brief It get the sum of weighted average if intersections values.
          
          \param toDs         The "To" data set.
          \param toSrid       The "To" data set srid.
          \param fromDs       The "From" data set.
          \param fromSrid     The "From" data set srid.
          \param dsPos        Vector of "To" data set positions that intersect "From".
          \param propertyName Required property name.

          \return The sum of weighted average.
        */
        double getWeightedSumByArea(te::da::DataSet* toDs,
                                    std::size_t toSrid,
                                    te::da::DataSet* fromDs,
                                    std::size_t fromSrid,
                                    std::vector<std::size_t> dsPos,
                                    const std::string& propertyName);

        /*!
          \brief It get the minimum distance of objects that not intersect.
          
          \param toDs     The "To" data set.
          \param toSrid   The "To" data set srid.
          \param fromDs   The "From" data set.
          \param fromSrid The "From" data set srid.
          \param kdtree   A generated KDTree based on the "From" data set.

          \return The minimum distance.
        */
        double getMinimumDistance(te::da::DataSet* toDs,
                                  std::size_t toSrid,
                                  te::da::DataSet* fromDs,
                                  std::size_t fromSrid,
                                  KD_ADAPTATIVE_TREE* kdtree);

        /*!
          \brief It verify if the geometry is a polygon.
          
          \param type The geometry type.

          \return True if were polygon.
        */
        bool isPolygon(te::gm::GeomType type);

        /*!
          \brief It verify if the geometry is a line.
          
          \param type The geometry type.

          \return True if were line.
        */
        bool isLine(te::gm::GeomType type);

        /*!
          \brief It verify if the geometry is a point.
          
          \param type The geometry type.

          \return True if were potin.
        */
        bool isPoint(te::gm::GeomType type);

        /*!
          \brief It verify if the geometry is a multi polygon.
          
          \param type The geometry type.

          \return True if were multi polygon.
        */
        bool isMultiPolygon(te::gm::GeomType type);

        /*!
          \brief It verify if the geometry is a multi line.
          
          \param type The geometry type.

          \return True if were multi line.
        */
        bool isMultiLine(te::gm::GeomType type);

        /*!
          \brief It verify if the geometry is a multi point.
          
          \param type The geometry type.

          \return True if were multi point.
        */
        bool isMultiPoint(te::gm::GeomType type);

        /*!
          \brief It get the area of a geometry.
          
          \param geom The geometry.

          \return The area.
        */
        double getArea(te::gm::Geometry* geom);

        /*!
          \brief It get all points of a geometry.
          
          \param geom The geometry.

          \return Vector of points.
        */
        std::vector<te::gm::Point*> getAllPointsOfGeometry(te::gm::Geometry* geom);

        /*!
          \brief Verify if has operations that don't need the intersections.

          \return True if has.
        */
        bool hasNoIntersectionOperations();

        void normalizeClassName(std::string& name);

        bool checkGeometries(te::gm::Geometry* fromGeom, std::size_t fromPos, te::gm::Geometry* toGeom);
    };
  }
}
#endif // __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTORMEMORY_H

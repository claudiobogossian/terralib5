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
 \file VectorToVectorMemory.h
 
 \brief Vector to Vector processing.
 
 \ingroup attributefill
 */

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTORMEMORY_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTORMEMORY_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../maptools/AbstractLayer.h"

#include "../datatype/Property.h"

#include "../memory/DataSet.h"
#include "../sam/kdtree.h"
#include "../statistics/core/Enums.h"

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
    class TEATTRIBUTEFILLEXPORT VectorToVectorMemory : public VectorToVectorOp
    {
      public:

        VectorToVectorMemory();

        ~VectorToVectorMemory();

        void setInput(te::map::AbstractLayerPtr fromLayer,
                      te::map::AbstractLayerPtr toLayer);

        void setParams(const std::map<te::dt::Property*, std::vector<std::string> >& options);

        void setOutput(te::da::DataSourcePtr outDsrc, std::string dsName);

        bool run();

      protected:

        te::sam::rtree::Index<size_t, 8>* getRtree(te::da::DataSet* data);

        KD_ADAPTATIVE_TREE* getKDtree(te::da::DataSet* data, std::size_t toSrid);

        te::da::DataSetType* getOutputDataSetType();

        std::vector<std::string> getDistinctClasses(te::da::DataSet* fromDs, const std::string& propertyName);

        std::string getPropertyName(te::dt::Property* prop, te::attributefill::OperationType func);

        bool isStatistical(te::attributefill::OperationType type);

        std::vector<std::size_t> getIntersections(te::da::DataSet* toDs,
                                                  te::da::DataSet* fromDs,
                                                  te::sam::rtree::Index<size_t, 8>* rtree);

        std::vector<te::dt::AbstractData*> getDataValues(te::da::DataSet* fromDs, std::vector<std::size_t> dsPos, const std::string& propertyName);

        std::vector<double> getNumValues(std::vector<te::dt::AbstractData*> data);

        std::vector<std::string> getStrValues(std::vector<te::dt::AbstractData*> data);

        double getValue(te::stat::NumericStatisticalSummary ss, te::attributefill::OperationType type);

        std::string getValue(te::stat::StringStatisticalSummary ss, te::attributefill::OperationType type);

        std::string getModeValue(te::stat::NumericStatisticalSummary ss);

        std::vector<te::attributefill::OperationType> getSelectedFunctions();

        te::dt::AbstractData* getClassWithHighestOccurrence(te::da::DataSet* fromDs,
                                                            std::vector<std::size_t> dsPos,
                                                            const std::string& propertyName);

        te::dt::AbstractData* getClassWithHighestIntersectionArea(te::da::DataSet* toDs,
                                                                  std::size_t toSrid,
                                                                  te::da::DataSet* fromDs,
                                                                  std::size_t fromSrid,
                                                                  std::vector<std::size_t> dsPos,
                                                                  const std::string& propertyName);

        te::dt::AbstractData* getDataBasedOnType(const std::string& strValue, const int type);

        std::map<std::string, double> getPercentagePerClass(te::da::DataSet* fromDs,
                                                            std::vector<std::size_t> dsPos,
                                                            const std::string& propertyName);

        double getPercentageOfTotalArea(te::da::DataSet* toDs,
                                        std::size_t toSrid,
                                        te::da::DataSet* fromDs,
                                        std::size_t fromSrid,
                                        std::vector<std::size_t> dsPos,
                                        const std::string& propertyName);

        std::map<std::string, double> getPercentageOfEachClassByArea(te::da::DataSet* toDs,
                                                                     std::size_t toSrid,
                                                                     te::da::DataSet* fromDs,
                                                                     std::size_t fromSrid,
                                                                     std::vector<std::size_t> dsPos,
                                                                     const std::string& propertyName);

        double getWeightedByArea(te::da::DataSet* toDs,
                                 std::size_t toSrid,
                                 te::da::DataSet* fromDs,
                                 std::size_t fromSrid,
                                 std::vector<std::size_t> dsPos,
                                 const std::string& propertyName);

        double getWeightedSumByArea(te::da::DataSet* toDs,
                                    std::size_t toSrid,
                                    te::da::DataSet* fromDs,
                                    std::size_t fromSrid,
                                    std::vector<std::size_t> dsPos,
                                    const std::string& propertyName);
        
        double getMinimumDistance(te::da::DataSet* toDs,
                                  std::size_t toSrid,
                                  te::da::DataSet* fromDs,
                                  std::size_t fromSrid,
                                  KD_ADAPTATIVE_TREE* kdtree);

        bool isPolygon(te::gm::GeomType type);
        bool isLine(te::gm::GeomType type);
        bool isPoint(te::gm::GeomType type);
        bool isMultiPolygon(te::gm::GeomType type);
        bool isMultiLine(te::gm::GeomType type);
        bool isMultiPoint(te::gm::GeomType type);

        double getArea(te::gm::Geometry* geom);

        std::vector<te::gm::Point*> getAllPointsOfGeometry(te::gm::Geometry* geom);

        bool hasNoIntersectionOperations();
    };
  }
}
#endif // __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTORMEMORY_H

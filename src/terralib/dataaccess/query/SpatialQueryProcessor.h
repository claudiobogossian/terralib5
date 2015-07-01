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
  \file terralib/dataaccess/query/SpatialQueryProcessor.h

  \brief A basic query processor for spatial restrictions.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SPATIALQUERYPROCESSOR_H
#define __TERRALIB_DATAACCESS_INTERNAL_SPATIALQUERYPROCESSOR_H

// TerraLib
#include "../../common/Enums.h"
#include "../../geometry/Envelope.h"
#include "../dataset/DataSet.h"
#include "../dataset/ObjectIdSet.h"
#include "../datasource/DataSource.h"

// Boost
#include <boost/noncopyable.hpp>

// STL
#include <memory>
#include <vector>

namespace te
{
  namespace da
  {
// Forward declarations
    class DataSetType;
    class DataSourceTransactor;
    class QueryCapabilities;
    class Select;
    struct SpatialRestriction;

    /*!
      \class SpatialQueryProcessor
      
      \brief A basic query processor for spatial restrictions.
    */
    class TEDATAACCESSEXPORT SpatialQueryProcessor : public boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        SpatialQueryProcessor();

        /*! \brief Virtual destructor. */
        virtual ~SpatialQueryProcessor();

        virtual std::auto_ptr<DataSet> getDataSet(const DataSourcePtr& ds, const Select& q,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY);

        virtual std::auto_ptr<DataSet> getDataSet(DataSourceTransactor* t, const QueryCapabilities& capabilities,
                                                  const Select& q,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY,
                                                  bool connected = false);

        virtual std::auto_ptr<ObjectIdSet> getOIDSet(const DataSourcePtr& ds, const Select& q);

        virtual std::auto_ptr<ObjectIdSet> getOIDSet(DataSourceTransactor* t, const QueryCapabilities& capabilities, const Select& q);

      protected:

        virtual std::auto_ptr<ObjectIdSet> getOIDSet(DataSourceTransactor* t, const Select& q);

        virtual ObjectIdSet* getOIDSet(DataSourceTransactor* t, Select& baseSelect, te::da::Expression* attrRestrictions,
                                       SpatialRestriction* restriction, const DataSetType* type,
                                       const std::vector<te::gm::Geometry*>& geomRestrictions);

      private:

        bool supportsSpatialTopologicOperatos(const QueryCapabilities& capabilities,
                                              const std::vector<SpatialRestriction*>& restrictions) const;

        std::string getDataSetName(const Select& q) const;

        Expression* getAttrRestrictions(const Select& q) const;

      private:

        te::gm::Envelope* computeEnvelope(const std::vector<SpatialRestriction*>& restrictions) const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_SPATIALQUERYPROCESSOR_H

/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ogr/DataSetPersistence.h

  \brief A class responsible for persisting a dataset in a OGR managed data source.
 */

#ifndef __TERRALIB_OGR_INTERNAL_DATASETPERSISTENCE_H
#define __TERRALIB_OGR_INTERNAL_DATASETPERSISTENCE_H

// TerraLib
#include "../dataaccess/dataset/DataSetPersistence.h"
#include "../Config.h"

// STL
#include <vector>

namespace te
{
  namespace ogr
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class DataSetPersistence

      \brief This class is responsible for persisting a dataset in a OGR managed data source.
    */
    class TEOGREXPORT DataSetPersistence : public te::da::DataSetPersistence
    {
      public:

        /*! \brief Constructor. */
        DataSetPersistence(DataSourceTransactor* t);

        /*! \brief destructor. */
        ~DataSetPersistence();

        void remove(const std::string& datasetName, const te::da::ObjectIdSet* oids);

        void add(const std::string& datasetName,
                 te::da::DataSet* d,
                 const std::map<std::string, std::string>& options,
                 std::size_t limit = 0);

        void update(const std::string& datasetName,
                    te::da::DataSet* dataset,
                    const std::vector<std::size_t>& properties,
                    const te::da::ObjectIdSet* oids,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit = 0);

      private:

        DataSourceTransactor* m_t;  //!< The DataSource transactor associated to the persistence object.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_OGR_INTERNAL_DATASETPERSISTENCE_H


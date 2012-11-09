/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/Definitions.h

  \brief General type definitions for the Data Access module.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DEFINITIONS_H
#define __TERRALIB_DATAACCESS_INTERNAL_DEFINITIONS_H

// Boost
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace da
  {
    class DataSet;

    typedef boost::shared_ptr<DataSet> DataSetPtr;

    class DataSetItem;

    typedef boost::shared_ptr<DataSetItem> DataSetItemPtr;

    class DataSetType;

    typedef boost::shared_ptr<DataSetType> DataSetTypePtr;

    class DataSource;

    typedef boost::shared_ptr<DataSource> DataSourcePtr;

    class DataSourceCatalogLoader;

    typedef boost::shared_ptr<DataSourceCatalogLoader> DataSourceCatalogLoaderPtr;

    class DataSourceTransactor;

    typedef boost::shared_ptr<DataSourceTransactor> DataSourceTransactorPtr;

    class Sequence;

    typedef boost::shared_ptr<Sequence> SequencePtr;

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DEFINITIONS_H


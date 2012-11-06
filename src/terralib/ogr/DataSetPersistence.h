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
  \file DataSetPersistence.h

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
    namespace da  {class DataSet; class DataSetItem; class DataSetType; class DataSourceTransactor; class Property; }

    class DataSourceTransactor;
    /*!
      \brief This class is responsible for persisting a dataset in a OGR managed data source.
     */
    class TEOGREXPORT DataSetPersistence : public te::da::DataSetPersistence
    {
      public:

        /*! \brief Constructor. */
        DataSetPersistence(DataSourceTransactor* t);

        /*! \brief destructor. */
        ~DataSetPersistence();

          
        /** @name te::da::DataSetPersistence pure pirtual pethods re-Iimplementation
         *  Methods reimplemented from base class.
         */
        //@{

        void create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit = 0);

        void remove(const te::da::DataSetType* dt);

        void remove(const std::string& datasetName);
        
        void remove(const te::da::DataSetType* dt, te::da::DataSet* d, std::size_t limit = 0);

        void remove(const te::da::DataSetType* dt, te::da::DataSetItem* item);

        void add(const te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit = 0);

        void add(const te::da::DataSetType* dt, te::da::DataSetItem* item);

        void update(const te::da::DataSetType* dt,
                          te::da::DataSet* dataset,
                          const std::vector<te::dt::Property*>& properties,
                          const std::map<std::string, std::string>& options,
                          std::size_t limit = 0);

        void update(const te::da::DataSetType* dt,
                          te::da::DataSet* oldD,
                          te::da::DataSet* newD,
                          const std::vector<te::dt::Property*>& properties,
                          std::size_t limit = 0);

        void update(const te::da::DataSetType* dt,
                          te::da::DataSetItem* item,
                          const std::vector<te::dt::Property*>& properties);

        void update(const te::da::DataSetType* dt,
                          te::da::DataSetItem* oldItem,
                          te::da::DataSetItem* newItem,
                          const std::vector<te::dt::Property*>& properties);

        te::da::DataSourceTransactor* getTransactor() const;

        //@}

      private:
        DataSourceTransactor* m_t;  //!< The DataSource transactor associated to the persistence object.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_OGR_INTERNAL_DATASETPERSISTENCE_H


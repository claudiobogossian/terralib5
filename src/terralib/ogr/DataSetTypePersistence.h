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
  \file DataSetTypePersistence.h

  \brief DataSetTypePersistence implementation for OGR data provider.
 */

#ifndef __TERRALIB_OGR_INTERNAL_DATASETTYPEPERSISTENCE_H
#define __TERRALIB_OGR_INTERNAL_DATASETTYPEPERSISTENCE_H

// TerraLib
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "Config.h"

namespace te
{
  namespace ogr
  {
// Forward declarations
    class DataSourceTransactor;

    /*!
      \class DataSetTypePersistence

      \brief DataSetTypePersistence implementation for OGR data provider.

      \sa te::da::DataSetTypePersistence
     */
    class TEOGREXPORT DataSetTypePersistence : public te::da::DataSetTypePersistence
    {
      public:

        /** @name Constructor/Destructor
         *  Initilizer methods.
         */
        //@{

        /*!
          \brief Constructor.

          \param t The OGR DataSourceTransactor from wich this persistence belongs.
         */
        DataSetTypePersistence(DataSourceTransactor* t);

        /*! \brief Destructor. */
        ~DataSetTypePersistence();

        //@}

        /** @name te::da::DataSetTypePersistence Pure Virtual Methods Re-Implementation
         *  Methods reimplemented from base class.
         */
        //@{
        void create(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

        void clone(const std::string& datasetName, const std::string& newDatasetName, const std::map<std::string, std::string>& options);

        void drop(te::da::DataSetType* dt);

        void drop(const std::string& datasetName);

        void rename(te::da::DataSetType* dt, const std::string& newName);

        void add(te::da::DataSetType* dt, te::dt::Property* p);

        void drop(te::dt::Property* p);

        void rename(te::dt::Property* p, const std::string& newName);

        void update(te::dt::Property* oldP, te::dt::Property* newP);

        void add(te::da::DataSetType* dt, te::da::PrimaryKey* pk);
   
        void drop(te::da::PrimaryKey* pk);

        void add(te::da::DataSetType* dt, te::da::UniqueKey* uk);
  
        void drop(te::da::UniqueKey* uk);

        void add(te::da::DataSetType* dt, te::da::Index* index, const std::map<std::string, std::string>& options);

        void drop(te::da::Index* index);

        void add(te::da::DataSetType* dt, te::da::ForeignKey* fk);
   
        void drop(te::da::ForeignKey* fk);
   
        void add(te::da::DataSetType* dt, te::da::CheckConstraint* cc);

        void drop(te::da::CheckConstraint* cc);

        void create(te::da::Sequence* sequence);

        void drop(te::da::Sequence* sequence);

        te::da::DataSourceTransactor* getTransactor() const;

        //@}

      private:

        DataSourceTransactor* m_t;  //!< The OGR DataSource transactor associated to the persistence object.
    };

  } // end namespace ogr
}   // end namespace te

#endif  // __TERRALIB_OGR_INTERNAL_DATASETTYPEPERSISTENCE_H



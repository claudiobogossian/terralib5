/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/DataSetPersistence.h

  \brief This is the PostgreSQL implementation for the DataSetPersistence API.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_DATASETPERSISTENCE_H
#define __TERRALIB_POSTGIS_INTERNAL_DATASETPERSISTENCE_H

// TerraLib
#include "../dataaccess/dataset/DataSetPersistence.h"
#include "Config.h"

namespace te
{
  namespace dt
  {
// Forward declarations
    class AbstractData;
  }
  
  namespace pgis
  {
// Forward declarations
    class Transactor;
      
    /*!
      \brief This is the PostGIS implementation for the DataSetPersistence API.
      
      \sa DataSet, DataSetPersistence, DataSetType, DataSourceTransactor

      \todo Na assinatura do persistence tem que dizer que o dataset ja deve estar na posicao de comecar a trabalhar! Alem disso precisamos de um parametro chamado limit!
    */
    class TEPGISEXPORT DataSetPersistence : public te::da::DataSetPersistence
    {
      using te::da::DataSetPersistence::add;
      
      public:        

        /*!
          \brief Constructor.

          \param t The PostGIS DataSourceTransactor from wich this persistence belongs.
        */
        DataSetPersistence(Transactor* t);

        /*! \brief Destructor. */
        ~DataSetPersistence();   
      
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

      private:

        Transactor* m_t;  //!< The DataSourceTransactor associated to the persistence object.
    };

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_DATASETPERSISTENCE_H



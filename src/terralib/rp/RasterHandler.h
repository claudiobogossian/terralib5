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
  \file terralib/rp/RasterHandler.h
  \brief Raster tuple.
 */

#ifndef __TERRALIB_RP_INTERNAL_RASTERHANDLER_H
#define __TERRALIB_RP_INTERNAL_RASTERHANDLER_H

#include "Config.h"

#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../raster/Raster.h"

#include <boost/shared_ptr.hpp>

namespace te
{
  namespace rp
  {
    /*!
      \class RasterHandler
      \brief RasterHandler
      \ingroup RPModule
     */
    class TERPEXPORT RasterHandler
    {
      public :
        
        RasterHandler();
        
        ~RasterHandler();
        
        /*!
          \brief Build a instance using the given pointers.
          \param dataSourcePtr Data source pointer.
          \param transactorPtr A pointer to a transactor instance related to the data source pointed by dataSourcePtr.
          \param persistencePtr A pointer to a persistence instance related to the instance pointed by transactorPtr.
          \param dataSetPtr A pointer to a data set instance related to the instance pointed by persistencePtr.
          \param rasterPtr A pointer to a raster instance related to the instance pointed by rasterPtr.
         */        
        RasterHandler(
          const boost::shared_ptr< te::da::DataSource >& dataSourcePtr,
          const boost::shared_ptr< te::da::DataSourceTransactor >& transactorPtr,
          const boost::shared_ptr< te::da::DataSetTypePersistence >& persistencePtr,
          const boost::shared_ptr< te::da::DataSet >& dataSetPtr,
          const boost::shared_ptr< te::rst::Raster >& rasterPtr );        
        
        /*!
          \brief Build a instance using the given pointers.
          \param transactorPtr Transactor pointer.
          \param persistencePtr A pointer to a persistence instance related to the instance pointed by transactorPtr.
          \param dataSetPtr A pointer to a data set instance related to the instance pointed by persistencePtr.
          \param rasterPtr A pointer to a raster instance related to the instance pointed by rasterPtr.
         */        
        RasterHandler(
          const boost::shared_ptr< te::da::DataSourceTransactor >& transactorPtr,
          const boost::shared_ptr< te::da::DataSetTypePersistence >& persistencePtr,
          const boost::shared_ptr< te::da::DataSet >& dataSetPtr,
          const boost::shared_ptr< te::rst::Raster >& rasterPtr );
          
        /*!
          \brief Build a instance using the given naked pointers.
          \param dataSourcePtr Data source pointer.
          \param transactorPtr A pointer to a transactor instance related to the data source pointed by dataSourcePtr.
          \param persistencePtr A pointer to a persistence instance related to the instance pointed by transactorPtr.
          \param dataSetPtr A pointer to a data set instance related to the instance pointed by persistencePtr.
          \param rasterPtr A pointer to a raster instance related to the instance pointed by rasterPtr.
          \note All given pionters will be acquired and their respective objects will be deleted at desctruction time.
         */                  
        RasterHandler(
          te::da::DataSource* dataSourcePtr,
          te::da::DataSourceTransactor* transactorPtr,
          te::da::DataSetTypePersistence* persistencePtr,
          te::da::DataSet* dataSetPtr,
          te::rst::Raster* rasterPtr );            
          
        /*!
          \brief Build a instance using the given naked pointers.
          \param transactorPtr Transactor pointer.
          \param persistencePtr A pointer to a persistence instance related to the instance pointed by transactorPtr.
          \param dataSetPtr A pointer to a data set instance related to the instance pointed by persistencePtr.
          \param rasterPtr A pointer to a raster instance related to the instance pointed by rasterPtr.
          \note All given pionters will be acquired and their respective objects will be deleted at desctruction time.
         */                  
        RasterHandler(
          te::da::DataSourceTransactor* transactorPtr,
          te::da::DataSetTypePersistence* persistencePtr,
          te::da::DataSet* dataSetPtr,
          te::rst::Raster* rasterPtr );    
          
        /*!
          \brief Build a instance using the given raster naked pointer.
          \param rasterPtr A pointer to a raster instance.
          \note All given pionters will be acquired and their respective objects will be deleted at desctruction time.
         */                  
        RasterHandler( te::rst::Raster* rasterPtr );           

        //overload
        const RasterHandler& operator=( const RasterHandler& ext );
        
        /*!
          \brief Reset the internal state (all internal pointed objects are deleted).
         */        
        void reset();
        
        /*!
          \brief Reset the internal state (all pointed objects are deleted).
          \param dataSourcePtr Data source pointer.
          \param transactorPtr A pointer to a transactor instance related to the data source pointed by dataSourcePtr.
          \param persistencePtr A pointer to a persistence instance related to the instance pointed by transactorPtr.
          \param dataSetPtr A pointer to a data set instance related to the instance pointed by persistencePtr.
          \param rasterPtr A pointer to a raster instance related to the instance pointed by rasterPtr.
         */        
        void reset( 
          const boost::shared_ptr< te::da::DataSource >& dataSourcePtr,
          const boost::shared_ptr< te::da::DataSourceTransactor >& transactorPtr,
          const boost::shared_ptr< te::da::DataSetTypePersistence >& persistencePtr,
          const boost::shared_ptr< te::da::DataSet >& dataSetPtr,
          const boost::shared_ptr< te::rst::Raster >& rasterPtr );         
        
        /*!
          \brief Reset the internal state (all pointed objects are deleted).
          \param transactorPtr Transactor pointer.
          \param persistencePtr A pointer to a persistence instance related to the instance pointed by transactorPtr.
          \param dataSetPtr A pointer to a data set instance related to the instance pointed by persistencePtr.
          \param rasterPtr A pointer to a raster instance related to the instance pointed by rasterPtr.          
         */        
        void reset( const boost::shared_ptr< te::da::DataSourceTransactor >& transactorPtr,
          const boost::shared_ptr< te::da::DataSetTypePersistence >& persistencePtr,
          const boost::shared_ptr< te::da::DataSet >& dataSetPtr,
          const boost::shared_ptr< te::rst::Raster >& rasterPtr );   
          
        /*!
          \brief Reset the internal state (all pointed objects are deleted).
          \param dataSourcePtr Data source pointer.
          \param transactorPtr A pointer to a transactor instance related to the data source pointed by dataSourcePtr.
          \param persistencePtr A pointer to a persistence instance related to the instance pointed by transactorPtr.
          \param dataSetPtr A pointer to a data set instance related to the instance pointed by persistencePtr.
          \param rasterPtr A pointer to a raster instance related to the instance pointed by rasterPtr.
          \note All given pionters will be acquired and their respective objects will be deleted at desctruction time.
         */        
        void reset( 
          te::da::DataSource* dataSourcePtr,
          te::da::DataSourceTransactor* transactorPtr,
          te::da::DataSetTypePersistence* persistencePtr,
          te::da::DataSet* dataSetPtr,
          te::rst::Raster* rasterPtr ); 
          
        /*!
          \brief Reset the internal state (all pointed objects are deleted).
          \param transactorPtr A pointer to a transactor instance related to the data source pointed by dataSourcePtr.
          \param persistencePtr A pointer to a persistence instance related to the instance pointed by transactorPtr.
          \param dataSetPtr A pointer to a data set instance related to the instance pointed by persistencePtr.
          \param rasterPtr A pointer to a raster instance related to the instance pointed by rasterPtr.
          \note All given pionters will be acquired and their respective objects will be deleted at desctruction time.
         */        
        void reset( 
          te::da::DataSourceTransactor* transactorPtr,
          te::da::DataSetTypePersistence* persistencePtr,
          te::da::DataSet* dataSetPtr,
          te::rst::Raster* rasterPtr );        
          
        /*!
          \brief Reset the internal state (all pointed objects are deleted).
          \param rasterPtr A pointer to a raster instance.
          \note All given pionters will be acquired and their respective objects will be deleted at desctruction time.
         */        
        void reset( te::rst::Raster* rasterPtr );           
          
        /*!
          \brief Returns a pointer the the handled raster instance or NULL if no instance is handled.
          \return Returns a pointer the the handled raster instance or NULL if no instance is handled.
         */                  
        te::rst::Raster* getRasterPtr();
        
      protected :
        
        mutable boost::shared_ptr< te::da::DataSource > m_dataSourcePtr; //!< A pointer to a datasource instance.
        
        mutable boost::shared_ptr< te::da::DataSourceTransactor > m_transactorPtr; //!< A pointer to a transactor instance that may or may not be related to data source instance pointed by m_dataSourcePtr.
        
        mutable boost::shared_ptr< te::da::DataSetTypePersistence > m_persistencePtr; //!< A pointer to a persistence instance related to the transactor pointed by m_transactorPtr.
        
        mutable boost::shared_ptr< te::da::DataSet > m_dataSetPtr; //!< A pointer to a data set instance related to the transactor instance pointed by m_transactorPtr.
        
        mutable boost::shared_ptr< te::rst::Raster > m_rasterPtr; //!< A pointer to a raster instance related to the data set instance pointed by m_dataSetPtr.        
    };
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_RASTERHANDLER_H


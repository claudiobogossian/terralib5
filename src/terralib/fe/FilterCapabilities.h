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
  \file FilterCapabilities.h
  
  \brief This class can be used to create capabilities document of services that use filter encoding.
 */

#ifndef __TERRALIB_FE_INTERNAL_FILTERCAPABILITIES_H
#define __TERRALIB_FE_INTERNAL_FILTERCAPABILITIES_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace fe
  {
    class IdCapabilities;
    class ScalarCapabilities;
    class SpatialCapabilities;

    /*!
      \class FilterCapabilities
      
      \brief This class can be used to create capabilities document of services that use filter encoding.

      The filter capabilities document describes what specific filter
      capabilities are supported by a service. For example, a
      Web Feature Service (WFS) that uses filter encoding would
      include this fragment in its capabilities document to advertise
      what filter capabilities it supports.
      Filter capabilities are divided into three categories:
      <ul>
      <li>spatial capabilities;</li>
      <li>scalar capabilities;</li>
      <li>id capabilities.</li>
      </ul>

      \sa SpatialCapabilities, ScalarCapabilities, IdCapabilities
     */
    class TEFEEXPORT FilterCapabilities
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new FilterCapabilities. */
        FilterCapabilities();

        /*! \brief Destructor. */
        ~FilterCapabilities();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the filter spatial capabilities operations.

          \param spCapabilities The filter spatial capabilities operations.

          \note It will take the ownership of the spatial capabilities.
                So, don't release the spatial capabilities resources.
         */
        void setSpatialCapabilities(SpatialCapabilities* spCapabilities);

        /*!
          \brief It returns the filter spatial capabilities.

          \return The filter spatial capabilities.
         */
        const SpatialCapabilities* getSpatialCapabilities() const;


        /*!
          \brief It sets the filter scalar capabilities operations.

          \param scCapabilities The filter scalar capabilities operations.

          \note It will take the ownership of the scalar capabilities.
                So, don't release the scalar capabilities resources.
         */
        void setScalarCapabilities(ScalarCapabilities* scCapabilities);

        /*!
          \brief It returns the filter scalar capabilities.

          \return The filter scalar capabilities.
         */
        const ScalarCapabilities* getScalarCapabilities() const;

        /*!
          \brief It sets the filter id capabilities.

          \param idCapabilities The filter id capabilities.

          \note It will take the ownership of the id capabilities.
                So, don't release the id capabilities resources.

         */
        void setIdCapabilities(IdCapabilities* idCapabilities);

        /*!
          \brief It returns the filter id capabilities.

          \return The filter id capabilities.
         */
        const IdCapabilities* getIdCapabilities() const;

        //@}

      private:

        SpatialCapabilities* m_spatialCapabilities;  //!< Mandatory.
        ScalarCapabilities* m_scalarCapabilities;    //!< Mandatory.
        IdCapabilities* m_idCapabilities;            //!< Mandatory.          
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_FILTERCAPABILITIES_H

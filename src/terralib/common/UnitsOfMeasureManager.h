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
  \file UnitsOfMeasureManager.h
 
  \brief A singleton class for dealing with units-of-measure.
 */

#ifndef __TERRALIB_COMMON_INTERNAL_UNITSOFMEASUREMANAGER_H
#define __TERRALIB_COMMON_INTERNAL_UNITSOFMEASUREMANAGER_H

// TerraLib
#include "Singleton.h"
#include "Config.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace common
  {
// Forward declaration
    class UnitOfMeasure;

    /*!
      \class UnitsOfMeasureManager

      \brief A singleton class for dealing with units-of-measurement.
      
      Use this class to keep the loaded catalogs of Unit-of-measurements.
 
      \sa UnitOfMeasure, BaseUnitOfMeasure
     */
    class TECOMMONEXPORT UnitsOfMeasureManager : public te::common::Singleton<UnitsOfMeasureManager>
    {
      friend class te::common::Singleton<UnitsOfMeasureManager>;

      public:

        /** @name Accessor methods
         *  Method used to access the data stored in this manager.
         */
        //@{

        /*!
          \brief It inserts a new unit-of-measurement that will be managed by UnitsOfMeasureManager.

          \param uom The unit-of-measurement to be managed by this manager.

          \exception Exception If the unit-of-measurement already exists it will raises an exception.

          \note Don't free the resources used by the unit-of-measurement, the manager will take the ownership of it.

          \note Don't inform a NULL pointer.
         */
        void insert(UnitOfMeasure* uom);

        /*!
          \brief It inserts a new unit-of-measurement that will be managed by UnitsOfMeasureManager.

          It also builds an auxiliary index for the unit-of-measurement based on
          its auxiliary (or alternative) names.

          \param uom              The unit-of-measurement to be managed by this manager.
          \param alternativeNames The list of alternative names (or auxiliary) for identifying an unit-of-measurement.

          \exception Exception If the unit-of-measurement already exists it will raises an exception.

          \note Don't free the resources used by the unit-of-measurement, the manager will take the ownership of it.

          \note Don't inform a NULL pointer.
         */
        void insert(UnitOfMeasure* uom, const std::vector<std::string>& alternativeNames);

        /*!
          \brief It removes the unit-of-measurement from the system.

          \param uom The unit-of-measurement to be removed.

          \exception Exception If the unit-of-measurement doesn't exist it will raises an exception.

          \note The memory pointed by the unit-of-measurement(uom) will be deallocated. Don't reference it anymore.

          \note Don't inform a NULL pointer.
         */
        void remove(UnitOfMeasure* uom);

        /*!
          \brief It returns the unit-of-measurement identified by id.

          \param id The id of the unit-of-measurement we are looking for.

          \return A pointer to an unit-of-measurement (don't free the pointer).

          \note It doesn't check the index range.
         */
        UnitOfMeasure* findById(unsigned int id) const;

        /*!
          \brief It returns the unit-of-measurement identified by a given name, or NULL if none is found.

          \param name The name of the unit-of-measurement we are looking for.

          \return A pointer to an unit-of-measurement (don't free the pointer), or NULL if none is found.
         */
        UnitOfMeasure* findByName(const std::string& name) const;

        /*!
          \brief It returns the unit-of-measurement identified by an alternative name, or NULL if none is found.

          \param name The alternative name of the unit-of-measurement we are looking for.

          \return A pointer to an unit-of-measurement (don't free the pointer) or NULL if none is found.
         */
        UnitOfMeasure* findByAlternativeName(const std::string& name) const;

        /*!
          \brief It returns the unit-of-measurement identified by a given name that can be the official name, or an alternative name, or it returns NULL if none is found.

          \param name The official name, or the alternative name of the unit-of-measurement we are looking for.

          \return A pointer to an unit-of-measurement (don't free the pointer) or NULL if none is found.
         */
        UnitOfMeasure* find(const std::string& name) const;

        /*!
          \brief It returns a pair of iterators over the unit-of-measurement of this manager.

          \return A pair of iterators over the unit-of-measurement of this manager where pair.first will be
                  the beginning and pair.second will be the end iterator.
         */
        std::pair<std::vector<UnitOfMeasure*>::const_iterator,
                  std::vector<UnitOfMeasure*>::const_iterator> getIterator() const;

        /*! \brief It unloads all unit-of-measurement managed by UnitsOfMeasureManager. */
        void clear();

        //@}

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Destructor. */
        ~UnitsOfMeasureManager();

        //@}

      protected:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes the Singleton. */
        UnitsOfMeasureManager();

        //@}

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        UnitsOfMeasureManager(const UnitsOfMeasureManager& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        UnitsOfMeasureManager& operator=(const UnitsOfMeasureManager& rhs);

        //@}

      private:

        std::vector<UnitOfMeasure*> m_uoms;                                 //!< This is the list of all system's unit-of-measurement loaded.
        std::map<std::string, UnitOfMeasure*> m_uomsIdxByName;              //!< An index from unit-of-measurement's name to unit-of-measurement instance (note: we can not have duplicated names).
        std::map<std::string, UnitOfMeasure*> m_uomsIdxByAlternativeName;   //!< An index from unit-of-measurement's alternative name to unit-of-measurement instance (note: we can not have duplicated alternative names).
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_UNITSOFMEASUREMANAGER_H


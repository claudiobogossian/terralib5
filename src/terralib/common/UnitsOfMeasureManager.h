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
  \file terralib/common/UnitsOfMeasureManager.h

  \brief A singleton class for dealing with units-of-measure.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_UNITSOFMEASUREMANAGER_H
#define __TERRALIB_COMMON_INTERNAL_UNITSOFMEASUREMANAGER_H

// TerraLib
#include "Singleton.h"

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

      \brief A singleton class for dealing with units of measure in the TerraLib environment.
      
      This class manages a unique catalogue of known units of measure within the TerraLib environment.
 
      \sa UnitOfMeasure
     */
    class TECOMMONEXPORT UnitsOfMeasureManager : public te::common::Singleton<UnitsOfMeasureManager>
    {
      friend class te::common::Singleton<UnitsOfMeasureManager>;

      public:

        /*!
          \brief Inserts a new unit of measure to be managed.

          \param uom Pointer to unit of measure to be managed. Shouldn't be null. The manager takes the pointer ownership.

          \exception Exception If the unit is already managed.
        */
        void insert(UnitOfMeasure* uom);

        /*!
          \brief Inserts a new unit of measure to be managed and its alternative names.

          \param uom              Pointer to unit of measure to be managed. Shouldn't be null. The manager takes the pointer ownership.
          \param alternativeNames The list of alternative (or auxiliary) names for identifying an unit of measure.

          \exception Exception If the unit is already managed.
        */
        void insert(UnitOfMeasure* uom, const std::vector<std::string>& alternativeNames);

        /*!
          \brief Removes a unit of measure from the manager.

          \param uom Pointer to unit of measure to be managed. Shouldn't be null. The manager will delete the pointer.

          \exception te::common::Exception If the unit is unknown to the manager.
        */
        void remove(UnitOfMeasure* uom);

        /*!
          \brief Returns a unit of measure identified by its identificaton.

          \param id The unit identification.

          \return A pointer unit of measure if found; null pointer otherwise.

          \note The manager maintains the ownership of the returned pointer.
        */
        UnitOfMeasure* findById(unsigned int id) const;

        /*!
          \brief Returns a unit of measure identified by its name.

          \param id The unit identification.

          \return A pointer unit of measure if found; null pointer otherwise.

          \note The manager maintains the ownership of the returned pointer.
        */
        UnitOfMeasure* findByName(const std::string& name) const;

        /*!
          \brief Returns a unit of measure identified by one of its alternative names.

          \param id The unit identification.

          \return A pointer unit of measure if located; null pointer otherwise.\note The manager maintains the ownership of the returned pointer.
        */
        UnitOfMeasure* findByAlternativeName(const std::string& name) const;

        /*!
          \brief Returns a unit of measure identified by its official name or one of its alternative names.

          \param id The unit identification.

          \return A pointer unit of measure if located; null pointer otherwise.

          \note The manager maintains the ownership of the returned pointer.
        */
        UnitOfMeasure* find(const std::string& name) const;

        /*!
          \brief Returns a pair of iterators over the managed units of measure.

          \return A pair of iterators over the unit of measure of this manager where pair.first will be
                  the beginning and pair.second will be the end iterator.
        */
        std::pair<std::vector<UnitOfMeasure*>::const_iterator,
                  std::vector<UnitOfMeasure*>::const_iterator> getIterator() const;

        /*! \brief Removes all units from the catalogue. */
        void clear();

        /*! \brief Destructor. */
        ~UnitsOfMeasureManager();

        /*!
          \brief Retrieves the alternative names for a unit of measure.

          \param uom Pointer to the unit of measure. Shouldn't be null;

          \param altNames Reference to a vector of strings to return the alternative names (output).
        */
        void getAlternativeNames(UnitOfMeasure* uom,std::vector<std::string>& altNames) const;

        /*!
          \brief Calculates a multiplicative factor to convert from a given unit to its base unit and vice-versa.

          \param unitFromName  A unit of measure name.
          \param unitToName    The name of unit to be converted to.

          \return A multiplicative factor to convert from a given unit to another.

          \exception Exception If the conversion is not possible.
        */
        double getConversion(const std::string& unitFromName, const std::string& unitToName) const;

        /*!
          \brief It initializes the list of well kown units of measure.

          The list is read from a JSON file named uom.json in the path
          indicated by TE_JSON_FILES_LOCATION macro.

          \exception Exception It throws an exception if it is not possible to initialize the UOM list.
        */
        void init();

      protected:

        /*! \brief It initializes the Singleton. */
        UnitsOfMeasureManager();

      private:

        std::vector<UnitOfMeasure*> m_uoms;                                 //!< This is the list of all system's unit of measure loaded.
        std::map<std::string, UnitOfMeasure*> m_uomsIdxByName;              //!< An index from unit of measure's name to unit of measure instance (note: we can not have duplicated names).
        std::map<std::string, UnitOfMeasure*> m_uomsIdxByAlternativeName;   //!< An index from unit of measure's alternative name to unit of measure instance (note: we can not have duplicated alternative names).
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_UNITSOFMEASUREMANAGER_H


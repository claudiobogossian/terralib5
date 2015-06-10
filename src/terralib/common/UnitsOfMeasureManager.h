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
 \file terralib/common/UnitsOfMeasureManager.h
 
 \brief A singleton class for dealing with units-of-measure.
 */

#ifndef __TERRALIB_COMMON_INTERNAL_UNITSOFMEASUREMANAGER_H
#define __TERRALIB_COMMON_INTERNAL_UNITSOFMEASUREMANAGER_H

// TerraLib
#include "Singleton.h"
#include "UnitOfMeasure.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace common
  {
    
    /*!
     \class UnitsOfMeasureManager
     
     \brief A singleton class for dealing with units of measure in the TerraLib environment.
     
     This class manages a unique catalogue of known units of measure within the TerraLib environment.

     \ingroup common
     
     \sa UnitOfMeasure
    */
    class TECOMMONEXPORT UnitsOfMeasureManager : public te::common::Singleton<UnitsOfMeasureManager>
    {
      friend class te::common::Singleton<UnitsOfMeasureManager>;
      
    public:
      
      typedef std::map<unsigned int, UnitOfMeasurePtr>::const_iterator const_iterator;
      typedef std::map<unsigned int, UnitOfMeasurePtr>::iterator iterator;
      
      /*!
       \brief Inserts a new unit of measure to be managed.
       
       \param uom Pointer to unit of measure to be managed. Shouldn't be null. The manager takes the pointer ownership.
       
       \exception Exception If the unit is already managed.
       */
      void insert(UnitOfMeasurePtr& uom);
      
      /*!
       \brief Inserts a new unit of measure to be managed and its alternative names.
       
       \param uom              Pointer to unit of measure to be managed. Shouldn't be null. The manager takes the pointer ownership.
       \param alternativeNames The list of alternative (or auxiliary) names for identifying an unit of measure.
       
       \exception Exception If the unit is already managed.
       */
      void insert(UnitOfMeasurePtr& uom, const std::vector<std::string>& alternativeNames);
      
      /*!
       \brief Removes a unit of measure from the manager.
       
       \param uom Pointer to unit of measure to be managed. Shouldn't be null. The manager will delete the pointer.
       
       \exception te::common::Exception If the unit is unknown to the manager.
       */
      void remove(UnitOfMeasurePtr& uom);
      
      /*!
       \brief Returns a unit of measure identified by its identificaton.
       
       \param id The unit identification.
       
       \return A pointer unit of measure if found; null pointer otherwise.
       */
      UnitOfMeasurePtr find(unsigned int id) const;
      
      /*!
       \brief Returns a unit of measure identified by its name.
       
       \param id The unit identification.
       
       \return A unit of measure pointer if found; null pointer otherwise.
       */
      UnitOfMeasurePtr find(const std::string& name) const;
      
      /*!
       \brief Returns a unit of measure identified by its symbol.
       
       \param id The unit symbol.
       
       \return A unit of measure pointer if found; null pointer otherwise.
       */
      UnitOfMeasurePtr findBySymbol(const std::string& symbol) const;
      
      /*! \brief Removes all units from the catalogue. */
      void clear();

      /*!
       \brief Retrieves the alternative names for a unit of measure.
       
       \param uom Pointer to the unit of measure. Shouldn't be null;
       
       \param altNames Reference to a vector of strings to return the alternative names (output).
       */
      void getNames(UnitOfMeasurePtr& uom,std::vector<std::string>& names) const;
      
      /*!
       \brief Check if two units of measure are compatible. 
       
       Check if a unit can be converted to another.
       
       \param unitFromName  A unit of measure name.
       \param unitToName    The name of unit to be converted to.
       
       \return True if the UnitFrom can be converted to UnitTo false otherwise.
       */
      bool areConvertible(const std::string& unitFromName, const std::string& unitToName) const;
      
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
       
       The list is read from the JSON file TERRALIB_DIR/resources/json/uom.json.
       
       \exception Exception It throws an exception if it is not possible to initialize the UOM list.
       */
      void init();
      
      /*!
       \brief It returns an iterator to the beginning of the conteiner.
       
       \return An iterator to the beginning of the conteiner.
       */
      const_iterator begin() const;
      
      /*!
       \brief It returns an iterator to the beginning of the conteiner.
       
       \return An iterator to the beginning of the conteiner.
       */
      iterator begin();
      
      /*!
       \brief It returns an iterator to the end of the conteiner.
       
       \return An iterator to the beginning of the conteiner.
       */
      const_iterator end() const;
      
      /*!
       \brief It returns an iterator to the end of the conteiner.
       
       \return An iterator to the beginning of the conteiner.
       */
      iterator end();
      
      /*!
       \brief It returns the number of units in the manager.
       
       \return The number of units in the manager.
       */
      std::size_t size() const;
      
    protected:
      
      /*! \brief It initializes the Singleton. */
      UnitsOfMeasureManager();
      
      /*! \brief Destructor. */
      ~UnitsOfMeasureManager();
      
    private:
      
      std::map<unsigned int, UnitOfMeasurePtr> m_uoms;                                 
      std::map<std::string, unsigned int> m_uomsIdxByName;
      
    };
    
    inline std::size_t UnitsOfMeasureManager::size() const
    {
      return m_uoms.size();
    }
    
    inline UnitsOfMeasureManager::const_iterator UnitsOfMeasureManager::begin() const
    {
      return m_uoms.begin();
    }
    
    inline UnitsOfMeasureManager::iterator UnitsOfMeasureManager::begin()
    {
      return m_uoms.begin();
    }
    
    inline UnitsOfMeasureManager::const_iterator UnitsOfMeasureManager::end() const
    {
      return m_uoms.end();
    }
    
    inline UnitsOfMeasureManager::iterator UnitsOfMeasureManager::end()
    {
      return m_uoms.end();
    }

    
  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_UNITSOFMEASUREMANAGER_H


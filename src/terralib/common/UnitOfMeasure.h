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
  \file UnitOfMeasure.h
 
  \brief A class for representing a unit of measure.
 */

#ifndef __TERRALIB_COMMON_INTERNAL_UNITOFMEASURE_H
#define __TERRALIB_COMMON_INTERNAL_UNITOFMEASURE_H

#include <vector>
#include <string>

#include "Config.h"

namespace te
{
  namespace common
  {
    /*!
      \class UnitOfMeasure

      \brief A class for representing a unit of measure used for numerical quantities.

      This class is based on the OGC Recommendation Paper — Units of Measure Use
      and Definition Recommendations or http://en.wikipedia.org/wiki/Units_of_measurement.
      This class supports fudamental, or base, units as well as derived units. 
      
      \sa UnitsOfMeasureManager

     */
    class TECOMMONEXPORT UnitOfMeasure
    {
      public:

        /*!
          \enum Type

          \brief Defines the type of measure associated to this unit.
         */
        enum Type
        {
          LengthMeasure,
          AngleMeasure,
          ScaleMeasure
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new UnitOfMeasure.

          \param id       Unique identification number for a Unit of measure.
          \param type     Type of measure associated to this unit.
         */
        UnitOfMeasure(unsigned int id, UnitOfMeasure::Type type= UnitOfMeasure::LengthMeasure);

        /*! \brief Destructor. */
        ~UnitOfMeasure();

        //@}

       /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the identification number of the unit-of-measurement.

          \return The identification number of the unit-of-measurement.
         */
        unsigned int getId() const;

        /*!
          \brief It sets the oficial name associated to the unit-of-measurement that must be a valid name according to the International System of Units (SI).

          \param name The oficial name associated to the unit-of-measurement according to the International System of Units (SI).
         */
        void setName(const std::string& name);

        /*!
          \brief It returns the oficial name associated to the unit-of-measurement according to the International System of Units (SI).

          \return The oficial name associated to the unit-of-measurement according to the International System of Units (SI).
         */
        const std::string& getName() const;

        /*!
          \brief It sets the unit-of-measurement description.
          
          \param description The unit-of-measurement description.
         */
        void setDescription(const std::string& description);

        /*!
          \brief It returns the unit-of-measurement description.
          
          \return The unit-of-measurement description.
         */
        const std::string& getDescription() const;

        /*!
          \brief It sets the unit-of-measurement symbol.
          
          \param s The unit-of-measurement symbol, like: kg, m or km.
         */
        void setSymbol(const std::string& s);

        /*!
          \brief It returns the unit-of-measurement symbol.
          
          \return The unit-of-measurement symbol, like: kg, m or km.
         */
        const std::string& getSymbol() const;

        /*!
          \brief It sets the type of measurement associated to the unit.
          
          \param t The type of measurement associated to the unit.
         */
        void setType(UnitOfMeasure::Type t);

        /*!
          \brief Gets the type of measurement associated to the unit.
          
          \return The type of measurement associated to the unit.
         */
        UnitOfMeasure::Type getType() const;

        //@}

        /** @name Methods for Derived Units of Meaure
         */
        //@{
        
        /*!
        \brief Returns true if this is a base unit and false if it is a derived unit.
        */
        bool isBaseUnit() const;

        /*!
        \brief It sets the id of another Unit of Measure from which this unit derives of. 

        \param baseUnitId The id of another UOM id from which this unit derives of.
        */
        void setBaseUnitId(unsigned int baseUnitId);

        /*!
        \brief It returns the id of another Unit of Measure from which this unit derives of. 

        \return The the id of another Unit of Measure from which this unit derives of. 
        */
        const unsigned int getBaseUnitId() const;

        /*!
          \brief Sets the conversion factors needed to convert this unit to the base unit from which this unit derives of.

          The method used for the conversions based on the formula as defined by 
          POCS Group (see http://www.posc.org/refs/poscUnits22.xml):
          \code
            Y = (AX + B) / (CX + D)
          \endcode
          where Y is the measure in the derivad base unit and X is measure in this unit.

          \param A The A factor in the conversion formula.          
          \param B The B factor in the conversion formula (by default it is equal to zero).
          \param C The C factor in the conversion formula (by default it is equal to zero).
          \param D The D factor in the conversion formula (by default it is equal to one).
         */
        void setConversionFactors(double A, double B=0.0, double C = 0.0, double D = 1.0);

        /*!
          \brief It returns the current conversion factors defined for this unit.

          \param A To return the A factor in the conversion formula.          
          \param B To return the B factor in the conversion formula.          
          \param C To return the C factor in the conversion formula.          
          \param D To return the D factor in the conversion formula.
          \return true if the conversion factors exists (it is not a base unit e.g. Kilometer) or 
                  false otherwise (it is a base unit, e.g. Meter)
         */
        bool getConversionFactors(double& A, double& B, double& C, double& D);

        //@}       
        
        /*!
          \brief It returns the WKT description of a unit of measure.
          
          \return The WKT description of a unit of measure.
         */
        std::string getWKT() const;


      private:

        unsigned int m_id;                //!< Unique identification number for a Unit-of-measurement.
        std::string m_name;               //!< Unit-of-measurement name according to SI.
        std::string m_description;        //!< Unit-of-measurement description.
        std::string m_symbol;             //!< Unit symbol.
        UnitOfMeasure::Type m_type;       //!< Unit type of measure.
        
        unsigned int m_baseUnitId;           //!< Unique identification number of the base unit to which a conversion operation is provided.
        std::vector<double> m_conversionFactors; //!< The conversion factors from this unit to a base unit associated to it.
    };    

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_UNITOFMEASURE_H


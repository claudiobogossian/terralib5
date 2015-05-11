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
  \file terralib/common/UnitOfMeasure.h

  \brief A class for representing a unit of measure.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_UNITOFMEASURE_H
#define __TERRALIB_COMMON_INTERNAL_UNITOFMEASURE_H

// TerraLib
#include "Config.h"
#include "Enums.h"

// STL
#include <vector>
#include <string>

// Boost
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace common
  {
    /*!
      \class UnitOfMeasure

      \brief A class to represent units of measure.

       The use of a unit of measure is meant to encompass the means by which a 
       measured value is tied explicitly to its unit of measure.

       Units of measure can be Base or Derived. Base units are well-defined units 
       which by convention are regarded as dimensionally independent. There is only 
       one Base Unit per type of measure.

       Derived units are those formed by combining base units according to the algebraic 
       relations linking the corresponding quantities.

       Units have an unique numerical identification, an oficial name, a symbol, the
       type of measures that they refer to and an optional description. Derived Units 
       also have the identification of its Base Unit and 4 factors to relate them,
       using the following formula:

       \code
       Y = (AX + B) / (CX + D)
       \endcode

       where Y is the measure in the derived unit and X is measure in base unit.

      \ingroup common

      \sa UnitsOfMeasureManager
    */
    class TECOMMONEXPORT UnitOfMeasure
    {
      public:
      
      /*!
        \brief Creates a new base unit of measure.

        \param id       Unique identification number.
        \param name     Unit official name.
        \param symbol   Unit symbol.
        \param type     Unit type of measure.
        \param description Unit description (by default empty).
      */
      UnitOfMeasure(unsigned int id, const std::string& name, const std::string& symbol,
                    MeasureType type, const std::string& description = "");
      
      /*!
        \brief Creates a new derived unit of measure.

        \param id          Unit unique identification number for a unit of measure.
        \param name        Unit official name.
        \param symbol      Unit symbol.
        \param type        Unit type of measure
        \param baseUnitId  Identification of the base unit from which this is derived of.
        \param A           A factor in the conversion formula.
        \param B           B factor in the conversion formula.
        \param C           C factor in the conversion formula.
        \param D           D factor in the conversion formula.
        \param description Unit description (by default empty).

        \note (C + D) can not be zero.
       */
       UnitOfMeasure(unsigned int id, const std::string& name, const std::string& symbol,
                     MeasureType type, unsigned int baseUnitId, 
                     double A, double B = 0.0, double C = 0.0, double D = 1.0, 
                     const std::string& description = "");
      
      /*! \brief Destructor. */
      ~UnitOfMeasure();

      /*!
       \brief Returns the unit of measure identification.
         
       \return The unit of measure identification.
      */
      unsigned int getId() const;

      /*!
        \brief Returns the unit of measure oficial name.

        \return The unit of measure oficial name.
      */
      const std::string& getName() const;

      /*!
        \brief Sets the unit of measure description.

        \param description The unit of measure description.
      */
      void setDescription(const std::string& description);

      /*!
        \brief Returns the unit of measure description.

        \return The unit of measure description.
      */
      const std::string& getDescription() const;

      /*!
        \brief Returns the unit of measure symbol.

        \return The unit of measure symbol (e.g. kg, m or km).
      */
      const std::string& getSymbol() const;

      /*!
        \brief Returns the unit of measure type.

        \return The unit of measure type.
      */
      MeasureType getType() const;

      /*! \brief Returns true if this is a base unit; otherwise returns false. */
      bool isBaseUnit() const;

      /*!
        \brief Returns the base unit id from which this unit derives of. 

        \return The base unit id from which this unit derives of. 
      */
      const unsigned int getBaseUnitId() const;

      /*!
        \brief Returns the conversion factors to convert the unit to its base unit.

        \param A To return the A factor in the conversion formula (output).
        \param B To return the B factor in the conversion formula (output).
        \param C To return the C factor in the conversion formula (output).
        \param D To return the D factor in the conversion formula (output).

        \note If this is a base unit, the return values are A=1, B=0, C=0 and D=1.
      */
      void getConversionFactors(double& A, double& B, double& C, double& D) const;
      
      /*!
        \brief Returns a multiplicative value to convert the unit to its base unit.

        The retuned value is calculated as (A + B)/(C + D) rounded to a double.

        \return A multiplicative value to convert the unit to its base unit.

        \note If this is a base unit, the returned value is 1.0.
      */
      double getConversionValue() const;
      
      /*!
        \brief Returns the WKT description of a unit of measure.

        \return The WKT description of a unit of measure.
      */
      std::string getWKT() const;

      private:

        unsigned int m_id;                 //!< Unique identification number for a unit of measure.
        std::string  m_name;               //!< unit of measure name according to SI.
        std::string  m_symbol;             //!< Unit symbol.
        MeasureType  m_type;               //!< Unit type of measure.
        std::string  m_description;        //!< unit of measure description.

        unsigned int m_baseUnitId;         //!< Unique identification number of the base unit to which a conversion operation is provided.

        double m_a;
        double m_b;
        double m_c;
        double m_d;

    }; // end class

    typedef boost::shared_ptr<UnitOfMeasure> UnitOfMeasurePtr;
  }    // end namespace common
}      // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_UNITOFMEASURE_H


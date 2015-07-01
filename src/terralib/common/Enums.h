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
  \file terralib/common/Enums.h

  \brief General enumerations.

  \ingroup common
 */

#ifndef __TERRALIB_COMMON_INTERNAL_ENUMS_H
#define __TERRALIB_COMMON_INTERNAL_ENUMS_H

namespace te
{
  namespace common
  {
    /*!
      \enum AccessPolicy

      \brief Supported data access policies (can be used as bitfield).
    */
    enum AccessPolicy
    {          
      NoAccess = 0, /*!< No access allowed             */
      RAccess = 1,  /*!< Read-only access allowed      */
      WAccess = 2,  /*!< Write-only access allowed     */
      RWAccess = 3  /*!< Read and write access allowed */
    };

    /*!
      \enum TraverseType
      
      \brief A dataset can be traversed in two ways:
    */
    enum TraverseType
    {
      FORWARDONLY = 0,    /*!< Used when the dataset is only able to move forward.                */
      BIDIRECTIONAL = 1,  /*!< Used when the dataset is able to move forward and backward.        */
      RANDOM = 2          /*!< Used when the dataset is able to move to any item.                 */
    };

    /*!
      \enum LoggerConfigurationType
          
      \brief Each enumerated type tells TerraLib how the configuration is done for a logger. 
    */
    enum LoggerConfigurationType
    {
      LOGGER_XML_CONFIG,     /*!< It indicates the use of a XML based configuration for a logger.             */
      LOGGER_TXT_CONFIG      /*!< It indicates the use of a plain text file based configuration for a logger. */
    };

    /*!
      \enum MeasureType

      \brief Defines the possible types of unit of measurements.
    */
    enum MeasureType
    {
      Length,  /*!< Refers to lenght measurements.  */
      Area,    /*!< Refers to area measurements.    */
      Volume,  /*!< Refers to volume measurements.  */
      Angle,   /*!< Refers to angular measurements. */
      Scale,   /*!< Refers to scale measurements.   */
      Time,    /*!< Refers to time measurements.    */
      Speed    /*!< Refers to speed measurements.   */
    };
    
    /*!
      \brief Defines some known UOM (Unit of Measure) codes.

      \note Values from 9001 to 9302 coincide with EPSG codes.
    */
    enum
    {
      UOM_Metre        = 9001,
      UOM_Foot         = 9002,
      UOM_NauticalMile = 9030,
      UOM_Kilometre    = 9036,
      UOM_Yard         = 9096,
      UOM_Radian       = 9101,
      UOM_Degree       = 9102,
      UOM_Unity        = 9201
    };


    enum Hemisphere
    {
      NorthHem = 0,  /*!< North hemisphere. */
      SouthHem = 1   /*!< South hemisphere. */
    };

    /*!
      \enum MachineByteOrder

      \brief Endianness.
    */
    enum MachineByteOrder
    {
      XDR = 0,  /*!< Big endian.   */
      NDR = 1   /*!< Little endian. */
    };

    /*! \brief Common TerraLib Exception Codes. */
    enum
    {
      UNKNOWN_EXCEPTION = 0,
      NO_CONNECTION_AVAILABLE = 1
    };

    /*! \brief An anonymous union that marks the last common TerraLib Exception Code. */
    enum
    {
      LAST_COMMON_EXCEPTION_CODE = NO_CONNECTION_AVAILABLE
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_ENUMS_H

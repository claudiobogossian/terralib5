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
  \file terralib/dataaccess/dataset/DataSetTypeConverter.h

  \brief An converter for DataSetType.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPECONVERTER_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPECONVERTER_H

// TerraLib
#include "../../common/CharEncodingConv.h"
#include "../Config.h"
#include "AttributeConverters.h"

// STL
#include <vector>

namespace te
{
// Forward declaration
  namespace dt
  {
    class Property;
  }

  namespace da
  {
// Forward declaration
    class DataSetAdapter;
    class DataSetType;
    class DataSourceCapabilities;

    /*!
      \class DataSetTypeConverter

      \brief An converter for DataSetType.

      \sa DataSetType
     */
    class TEDATAACCESSEXPORT DataSetTypeConverter
    {
      friend class DataSetAdapter;

      public:

        /** @name Constructor/Destructor
         *  Initilizer methods.
         */
        //@{

        /*!
          \brief Constructor.

          \param type A pointer to the DataSetType that will be converted.

          \note Here no automatic property conversion will be made.
          \note The method "add" can be used to do manual conversions.

          \note This class will NOT take the ownership of the given pointer.

          \sa DataSetType
        */
        DataSetTypeConverter(DataSetType* type);

        /*!
          \brief Constructor.

          \param type A pointer to the DataSetType that will be converted.
          \param capabilities A data source capabilities of the data source that the user intend use.
                 e.g. the data source where the given data set type will be saved.
          \param ce The destintion character encoding.

          \note This method will try to create automatic property conversions based on the given capabilities.
                Here we will look up the data type support of DataSourceCapabilities.
                If there is not support for a given data type, the method "getHint" will be used in attempt to retrieve an appropriate data type.

          \note If the method "getHint" does not inform any hint, the specific property will not have a conversion.
          \note The method "add" can be used to do manual conversions.
          \note The method "getNonConvertedProperties" can be used to retrieve that properties that have no defined conversion.
          
          \sa DataSetType, DataSourceCapabilities DataTypeCapabilities
        */
        DataSetTypeConverter(DataSetType* type, const DataSourceCapabilities& capabilities, const te::common::CharEncoding& ce = te::common::UNKNOWN_CHAR_ENCODING);

        /*! \brief Destructor. */
        ~DataSetTypeConverter();

        //@}

        /*!
          \brief This method returns the pointer to the DataSetType that is handled by the converter.

          \return The pointer to the DataSetType that is handled by the converter.
        */
        DataSetType* getConvertee();

        /*!
          \brief This method returns the name of the properties that have not yet been converted.

          \param propertyNames A vector that will be filled with the name of the properties that have not yet been converted.
        */
        void getNonConvertedProperties(std::vector<std::string>& propertyNames) const;

        /*!
          \brief This method returns the position of the properties that have not yet been converted.

          \param propertyPos A vector that will be filled with the position of the properties that have not yet been converted.
        */
        void getNonConvertedProperties(std::vector<std::size_t>& propertyPos) const;

         /*!
          \brief This method tells which properties of the input data set type that have been converted based on the given property name.

          \param propertyName A property name of the DataSetTypeConverter.
          \param convertedPropertyPos A vector that will be filled with the property positions of the input data set type have been converted.
        */
        void getConvertedProperties(const std::string& propertyName, std::vector<std::size_t>& convertedPropertyPos);

        /*!
          \brief This method tells which properties of the input data set type that have been converted based on the given property name.

          \param propertyPos A property position of the DataSetTypeConverter.
          \param convertedPropertyPos A vector that will be filled with the property positions of the input data set type have been converted.
        */
        void getConvertedProperties(std::size_t propertyPos, std::vector<std::size_t>& convertedPropertyPos);

        /*!
          \brief This method tells which Attribute Converter was used in the porperty position.

          \param propertyPos A property position of the DataSetTypeConverter.
          
          \return Attribute Converter name.
        */
        std::string getConverterName(std::size_t propertyPos);

        /*!
          \brief This method removes a property of DataSetTypeConverter.

          \param propertyName The property name of the DataSetTypeConverter that will be removed.
        */
        void remove(const std::string& propertyName);

        /*!
          \brief This method removes a property of DataSetTypeConverter.

          \param propertyPos The property position of the DataSetTypeConverter that will be removed.
        */
        void remove(std::size_t propertyPos);

        /*!
          \brief It adds a conversions to the given property of the input data set type.

          \param propertyName The property name that will be converted.
          \param p The converted property.
          \param attributeConverterName The function name that will be used to do the property values conversion.
        */
        void add(const std::string& propertyName, te::dt::Property* p, const std::string& attributeConverterName = "GenericAttributeConverter");

        /*!
          \brief It adds a conversions to the given property of the input data set type.

          \param propertyPos The property position that will be converted.
          \param p The converted property.
          \param attributeConverterName The function name that will be used to do the property values conversion.
        */
        void add(std::size_t propertyPos, te::dt::Property* p, const std::string& attributeConverterName = "GenericAttributeConverter");

        /*!
          \brief It adds a conversions to the given property of the input data set type.

          \param propertyPos The property position that will be converted.
          \param p The converted property.
          \param conv The function that will be used to do the property values conversion.
        */
        void add(std::size_t propertyPos, te::dt::Property* p, AttributeConverter conv);

        /*!
          \brief It adds a conversion to the given properties of the input data set type.

          \param propertyNames The property names that will be converted.
          \param p The converted property.
          \param attributeConverterName The function name that will be used to do the property values conversion.
        */
        void add(const std::vector<std::string>& propertyNames, te::dt::Property* p, const std::string& attributeConverterName = "GenericAttributeConverter");

        /*!
          \brief It adds a conversion to the given properties of the input data set type.

          \param propertyPos The property positions that will be converted.
          \param p The converted property.
          \param attributeConverterName The function name that will be used to do the property values conversion.
        */
        void add(const std::vector<std::size_t>& propertyPos, te::dt::Property* p, const std::string& attributeConverterName = "GenericAttributeConverter");

        /*!
          \brief It adds a conversion to the given properties of the input data set type.

          \param propertyPos The property positions that will be converted.
          \param p The converted property.
          \param conv The function that will be used to do the property values conversion.
        */
        void add(const std::vector<std::size_t>& propertyPos, te::dt::Property* p, AttributeConverter conv);

        /*!
          \brief Static method that verifies if the given data set type need an converter based on given data source capabilities.

          \param type The data set type that will be verified.
          \param capabilities The data source capabilities that will be used.

          \return Return true if the given data set type need a converter. Otherwise, return false.
        */
        static bool needConverter(DataSetType* type, const DataSourceCapabilities& capabilities);

        DataSetType* getResult() const;

        const std::vector<std::vector<std::size_t> >& getConvertedPropertyIndexes() const;

        const std::vector<AttributeConverter>& getConverters() const;

      private:

        bool isConverted(std::size_t i) const;

      private:

        DataSetType* m_inDataSetType;                              //!< A pointer to DataSetType that will be converted.
        DataSetType* m_outDataSetType;                             //!< The converted DataSetType.
        std::vector<std::vector<std::size_t> > m_propertyIndexes;  //!< A vector that stores the converted property indexes.
        std::vector<AttributeConverter> m_converters;              //!< A vector that stores the attribute converters functions.
        std::vector<std::string> m_functionsNames;                //!< A vector that stores the attribute converters functions names.
        std::vector<std::size_t> m_convertedProperties;            //!< Internal vector to count the references to converted properties.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPECONVERTER_H

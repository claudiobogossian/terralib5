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
  \file terralib/ado/Module.h
   
  \brief A utility class to initialize and terminate TerraLib ADO driver support.  
*/

#ifndef __TERRALIB_ADO_INTERNAL_MODULE_H
#define __TERRALIB_ADO_INTERNAL_MODULE_H

// TerraLib
#include "../plugin/Plugin.h"
#include "Config.h"

namespace te
{
  namespace ado
  {
    /*!
      \class Platform

      \brief A utility class to initialize and terminate TerraLib ADO driver support.

      \sa te::plugin::Plugin
     */
    class Module : public te::plugin::Plugin
    {
      public:

        /** @name Initializer Methods
         *  Constructor/Destructor.
         */
        //@{

        /*! \brief It creates a new C++ module. */
        Module(const te::plugin::PluginInfo& pluginInfo);

        /*! \brief Destructor. */
        ~Module();

        //@}

        /** @name Re-Implmentation from Plugin Interface
         *  Re-Implementation from Plugin Interface.
         */
        //@{

        /*!
          \brief This method will be called by TerraLib to startup some plugin's functionality.
                  
          \exception Exception It throws an exception if the plugin can not be started.
         */
        void startup();

        /*!
          \brief This method will be called by TerraLib to shutdown plugin's functionality.
          
          \exception Exception It throws an exception if the plugin can not be shutdown.
         */
        void shutdown();
        //@}


        static const std::string sm_unknownTypeName;              //!< The string literal representation for the unknown type.
        static const std::string sm_int2TypeName;                 //!< The string literal representation for the int2 type
        static const std::string sm_intTypeName;                  //!< The string literal representation for the int type
        static const std::string sm_int8TypeName;                 //!< The string literal representation for the int8 type
        static const std::string sm_numericTypeName;              //!< The string literal representation for the numeric type
        static const std::string sm_dateTypeName;                 //!< The string literal representation for the date type
        static const std::string sm_timeTypeName;                 //!< The string literal representation for the time type
        static const std::string sm_timeTZTypeName;               //!< The string literal representation for the time type
        static const std::string sm_timeStampTypeName;            //!< The string literal representation for the time stamp type
        static const std::string sm_timeStampTZTypeName;          //!< The string literal representation for the time stamp with time zone type
        static const std::string sm_floatTypeName;                //!< The string literal representation for the float type
        static const std::string sm_doubleTypeName;               //!< The string literal representation for the double type
        static const std::string sm_booleanTypeName;              //!< The string literal representation for the boolean type
        static const std::string sm_charTypeName;                 //!< The string literal representation for the char type
        static const std::string sm_varcharTypeName;              //!< The string literal representation for the varchar type
        static const std::string sm_stringTypeName;               //!< The string literal representation for the string type
        static const std::string sm_byteArrayTypeName;            //!< The string literal representation for the byte array type
        static const std::string sm_pointTypeName;                //!< The string literal representation for ADO point geometry type
        static const std::string sm_pointMTypeName;               //!< The string literal representation for ADO point geometry type 
        static const std::string sm_lineStringTypeName;           //!< The string literal representation for ADO line string geometry type
        static const std::string sm_lineStringMTypeName;          //!< The string literal representation for ADO line string with m geometry type
        static const std::string sm_polygonTypeName;              //!< The string literal representation for ADO polygon geometry type
        static const std::string sm_polygonMTypeName;             //!< The string literal representation for ADO polygon with m geometry type
        static const std::string sm_geometryCollectionTypeName;   //!< The string literal representation for ADO geometry collection geometry type
        static const std::string sm_geometryCollectionMTypeName;  //!< The string literal representation for ADO geometry collection with m geometry type
        static const std::string sm_multiPointTypeName;           //!< The string literal representation for ADO multi-point geometry type
        static const std::string sm_multiPointMTypeName;          //!< The string literal representation for ADO multi-point with m geometry type
        static const std::string sm_multiLineStringTypeName;      //!< The string literal representation for ADO multi-linestring geometry type
        static const std::string sm_multiLineStringMTypeName;     //!< The string literal representation for ADO multi-linestring with m geometry type
        static const std::string sm_multiPolygonTypeName;         //!< The string literal representation for ADO multi-polygon geometry type
        static const std::string sm_multiPolygonMTypeName;        //!< The string literal representation for ADO multi-polygon with m geometry type
        static const std::string sm_geometryTypeName;             //!< The string literal representation for ADO base geometry type
    };
  } // end namespace ado
}   // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEADOEXPORT);

#endif  // __TERRALIB_ADO_INTERNAL_MODULE_H


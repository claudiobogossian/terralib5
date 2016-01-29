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
  \file terralib/rp/Marcos.h

  \brief Internal Raster Processing module macros.
 */

#ifndef __TERRALIB_RP_INTERNAL_MACROS_H
#define __TERRALIB_RP_INTERNAL_MACROS_H

  #include "../common/Logger.h"
  #include "../common/Config.h"
  #include "../common/MathUtils.h"
  #include "Module.h"
  #include "Exception.h"

  #include <boost/lexical_cast.hpp>

  #include <sstream>
  #include <string>
  #include <iostream>
  
  /*!
   \brief Debug mode selection based on NDEBUG define.
   */
  #ifndef TERP_DEBUG_MODE
    #ifndef NDEBUG
      /*! \brief Debug mode selection flag. */
      #define TERP_DEBUG_MODE
      
      /*! \brief STDOUT logging selection flag. */
      #ifndef TERP_ENABLE_STDOUT_LOG
        #define TERP_ENABLE_STDOUT_LOG
      #endif
    #endif
  #endif   

  /*!
   \brief Logs a message to stdout
   \param message Message to be logged.
   */
  #ifdef TERP_ENABLE_STDOUT_LOG
    #define TERP_LOGMSG_STDOUT( message ) \
      std::cout << std::endl << "Message : " \
        << __FILE__ \
        << ":" << __LINE__ \
        << " - " << boost::lexical_cast< std::string >( message ) \
        << std::endl;
  #else
    #define TERP_LOGMSG_STDOUT( message )
  #endif

    /*!
    \brief Logs a error message to stderr
    \param message Message to be logged.
    */
  #ifdef TERP_ENABLE_STDOUT_LOG
    #define TERP_LOGERR_STDOUT( message ) \
      std::cerr << std::endl << "Error : " \
        << __FILE__ \
        << ":" << __LINE__ \
        << " - " << boost::lexical_cast< std::string >( message ) \
        << std::endl;
  #else
    #define TERP_LOGERR_STDOUT( message )
  #endif

    /*!
    \brief Logs a warning message to stdout
    \param message Message to be logged.
    */
  #ifdef TERP_ENABLE_STDOUT_LOG
    #define TERP_LOGWARN_STDOUT( message ) \
      std::cout << std::endl << "Warning : " \
        << __FILE__ \
        << ":" << __LINE__ \
        << " - " << boost::lexical_cast< std::string >( message ) \
        << std::endl;
  #else
    #define TERP_LOGWARN_STDOUT( message )
  #endif
        

  /*!
   \brief Logs a message.
   \param message Message to be logged.
   */
  #define TERP_LOGMSG( message ) \
  { \
    te::rp::Module::setLastLogStr( boost::lexical_cast< std::string >( message ) ); \
    TE_LOG_INFO( boost::lexical_cast< std::string >( message ).c_str() ); \
    TERP_LOGMSG_STDOUT( message ); \
  };

  /*!
   \brief Logs an error message.
   \param message Message to be logged.
   */
  #define TERP_LOGERR( message ) \
  { \
    te::rp::Module::setLastLogStr( boost::lexical_cast< std::string >( message ) ); \
    TE_LOG_ERROR( boost::lexical_cast< std::string >( message ).c_str() ); \
    TERP_LOGERR_STDOUT( message ); \
  };
      
  /*!
   \brief Logs a warning message.
   \param message Message to be logged.
   */
  #define TERP_LOGWARN( message ) \
  { \
    te::rp::Module::setLastLogStr( boost::lexical_cast< std::string >( message ) ); \
    TE_LOG_WARN( boost::lexical_cast< std::string >( message ).c_str() ); \
    TERP_LOGWARN_STDOUT( message ); \
  };
      
  /*!
   \brief Logs a error message and throws.
   \param message Message to be logged.
   */
  #define TERP_LOG_AND_THROW( message ) \
  { \
    TERP_LOGERR( message ); \
    throw te::rp::Exception( boost::lexical_cast< std::string >( message ) ); \
  };
      
  /*!
   \brief Checks if value is true and throws an exception if not.
   \param value Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_TRUE_OR_THROW( value , message ) \
    if( ( value ) == 0 ) { \
      TERP_LOGERR( boost::lexical_cast< std::string >( #value ) ); \
      TERP_LOGERR( boost::lexical_cast< std::string >( message ) ); \
      throw te::rp::Exception( boost::lexical_cast< std::string >( message ) ); \
    };      

  /*!
   \brief Variable watching.
   \param variable Variable to be logged.
   */
  #define TERP_WATCH( variable ) \
    { \
      TERP_LOGMSG( "WATCH - " + boost::lexical_cast< std::string >( #variable ) + \
        "=[" + boost::lexical_cast< std::string >( variable ) +"]" ); \
    };

  /**!   * @brief Checks if value is true and logs an warning message if not.
   \param value Value to be checked.
   \@param message Message to be logged.
   */
  #define TERP_TRUE_OR_LOG( value , message ) \
    if( ( value ) == 0 ) { \
      TERP_LOGWARN( boost::lexical_cast< std::string >( #value ) ); \
      TERP_LOGWARN( boost::lexical_cast< std::string >( message ) ); \
    };

  /*!
   \brief Checks if value is true. For false values a warning message will be logged 
   and a return of context with false value will be done.

   \param value Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_TRUE_OR_RETURN_FALSE( value , message ) \
    if( ( value ) == 0 ) { \
      TERP_LOGWARN( boost::lexical_cast< std::string >( #value ) ); \
      TERP_LOGWARN( boost::lexical_cast< std::string >( message ) ); \
      return false; \
    };
    
  /*!
   \brief Checks if value is true. For false values a warning message will be logged 
   and a return of context will be done.

   \param value Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_TRUE_OR_RETURN( value , message ) \
    if( ( value ) == 0 ) { \
      TERP_LOGWARN( boost::lexical_cast< std::string >( #value ) ); \
      TERP_LOGWARN( boost::lexical_cast< std::string >( message ) ); \
      return; \
    };    
    
  /*!
   \brief Checks if value is false. For true values a warning message 
   will be logged and a return of context with false value will be done.
   *
   \param value Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_FALSE_OR_RETURN_FALSE( value , message ) \
    if( ( value ) != 0 ) { \
      TERP_LOGWARN( boost::lexical_cast< std::string >( #value ) ); \
      TERP_LOGWARN( boost::lexical_cast< std::string >( message ) ); \
      return false; \
    };    
    
  /*!
   \brief Checks if value is false. For true values a warning message 
   will be logged and a return of context will be done.
   *
   \param value Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_FALSE_OR_RETURN( value , message ) \
    if( ( value ) != 0 ) { \
      TERP_LOGWARN( boost::lexical_cast< std::string >( #value ) ); \
      TERP_LOGWARN( boost::lexical_cast< std::string >( message ) ); \
      return; \
    };     

  /*!
   \brief Logs a warning message will and return false.
   \param message Message to be logged.
   */
  #define TERP_LOG_AND_RETURN_FALSE( message ) \
    { \
      TERP_LOGWARN( message ); \
      return false; \
    };
    
  /*!
   \brief Logs a warning message will and return.
   \param message Message to be logged.
   */
  #define TERP_LOG_AND_RETURN( message ) \
    { \
      TERP_LOGWARN( message ); \
      return; \
    };    

  /*!
   \brief Checks if value is false and logs an warning message if not.
   \param value Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_FALSE_OR_LOG( value , message ) \
    if( ( value ) != 0 ) { \
      TERP_LOGWARN( boost::lexical_cast< std::string >( #value ) ); \
      TERP_LOGWARN( boost::lexical_cast< std::string >( message ) ); \
    };

  /*!
   \brief Checks if two values are equal and throws an exception if not.
   \param value1 Value to be checked.
   \param value2 Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_CHECK_EQUAL( value1 , value2 , message ) \
    if( value1 != value2 ) \
    { \
      TERP_LOGERR(std::string( "[" ) + \
        boost::lexical_cast< std::string >( #value1 ) \
        + std::string( "!=" ) + \
        + boost::lexical_cast< std::string >( #value2 ) \
        + "][" \
        + boost::lexical_cast< std::string >( value1 ) \
        + std::string( "!=" ) + \
        + boost::lexical_cast< std::string >( value2 ) ); \
      TERP_LOG_AND_THROW( message ); \
    };

  /*!
   \brief Checks if two values are diferent and throws an exception if not.
   \param value1 Value to be checked.
   \param value2 Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_CHECK_NOT_EQUAL( value1 , value2 , message ) \
    if( value1 == value2 ) \
    { \
      TERP_LOGERR(std::string( "[" ) + \
        boost::lexical_cast< std::string >( #value1 ) \
        + std::string( "==" ) + \
        + boost::lexical_cast< std::string >( #value2 ) \
        + "][" \
        + boost::lexical_cast< std::string >( value1 ) \
        + std::string( "==" ) + \
        + boost::lexical_cast< std::string >( value2 ) ); \
      TERP_LOG_AND_THROW( message ); \
    };

  /*!
   \brief  Checks if two values are equal ( within an EPS ) and throws an exception if not.
   \param value1 Value to be checked.
   \param value2 Value to be checked.
   \param eps EPS ( threshold )
   \param message Message to be logged.
   */
  #define TERP_CHECK_EPS( value1 , value2 , eps , message ) \
    { \
      TERP_TRUE_OR_THROW( ( (eps) >= 0), "Invalid eps" ); \
      double TERP_CHECK_EPS_double_diff = 0; \
      double TERP_CHECK_EPS_double_value1 = (double)(value1); \
      double TERP_CHECK_EPS_double_value2 = (double)(value2); \
      double TERP_CHECK_EPS_double_eps = (double)(eps); \
      if( TERP_CHECK_EPS_double_value1 < TERP_CHECK_EPS_double_value2 ) { \
        TERP_CHECK_EPS_double_diff = ( TERP_CHECK_EPS_double_value2 - \
          TERP_CHECK_EPS_double_value1 ); \
      } else { \
        TERP_CHECK_EPS_double_diff = ( TERP_CHECK_EPS_double_value1 - \
          TERP_CHECK_EPS_double_value2 ); \
      }; \
      if( TERP_CHECK_EPS_double_diff > TERP_CHECK_EPS_double_eps ) \
      { \
        TERP_LOGERR( \
          std::string( "Values are not equal: " ) + \
          boost::lexical_cast< std::string >( #value1 ) + \
          std::string( "=[") + \
          boost::lexical_cast< std::string >( TERP_CHECK_EPS_double_value1 ) + \
          std::string( "] " ) + \
          boost::lexical_cast< std::string >( #value2 ) + \
          std::string( "=[") + \
          boost::lexical_cast< std::string >( TERP_CHECK_EPS_double_value2 ) + \
          std::string( "] eps=[") + \
          boost::lexical_cast< std::string >( TERP_CHECK_EPS_double_eps ) + \
          std::string( "] diff=[") + \
          boost::lexical_cast< std::string >( TERP_CHECK_EPS_double_diff ) \
          ); \
        TERP_LOG_AND_THROW( message ); \
      }; \
    };

  /*!
   \brief Throws an exception for not implemented source.
   */
  #define TERP_NOT_IMPLEMENTED TERP_LOG_AND_THROW( "Not Implemented." );

  /*!
   \brief Checks if value is true and throws an exception if not.
   \note This macro will be disabled for non debug mode.
   \param value Value to be checked.
   \param message Message to be logged.
   */
  #ifdef TERP_DEBUG_MODE
    #define TERP_DEBUG_TRUE_OR_THROW( value , message ) \
      TERP_TRUE_OR_THROW( value , message );
  #else
    #define TERP_DEBUG_TRUE_OR_THROW( value , message );
  #endif
  
  /*!
   \brief Checks if value is true. For false values a warning message will be logged 
   and a return of context with false value will be done.
   
   \note This macro will be disabled for non debug mode.
   \param value Value to be checked.
   \param message Message to be logged.
   */
  #ifdef TERP_DEBUG_MODE
    #define TERP_DEBUG_TRUE_OR_RETURN_FALSE( value , message ) \
      TERP_TRUE_OR_RETURN_FALSE( value , message );
  #else
    #define TERP_DEBUG_TRUE_OR_RETURN_FALSE( value , message );
  #endif
  
  /*!
   \brief Minimum of two values.
   */
  #ifndef MIN
    #define MIN(x,y) std::min( x , y )
  #endif
  
  /*!
   \brief Maximum of two values.
   */
  #ifndef MAX
    #define MAX(x,y) std::max( x , y )
  #endif  
  
  /*!
   \brief Absolute value.
   */
  #ifndef ABS
    #define ABS(x) std::abs( x )
  #endif   

#endif  // __TERRALIB_RP_INTERNAL_MACROS_H


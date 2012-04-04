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
  \file terralib/rp/Marcos.h

  \brief Internal Raster Processing module macros.
 */

#ifndef __TERRALIB_RP_INTERNAL_MACROS_H
#define __TERRALIB_RP_INTERNAL_MACROS_H

  #include "../common/Logger.h"
  #include "../common/Config.h"
  #include "Exception.h"

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
        << " - " << te::rp::macros::toString( message ) \
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
        << " - " << te::rp::macros::toString( message ) \
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
        << " - " << te::rp::macros::toString( message ) \
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
    TE_LOG_INFO( te::rp::macros::toString( message ).c_str() ); \
    TERP_LOGMSG_STDOUT( message ); \
  };

  /*!
   \brief Logs a message.
   \param message Message to be logged.
   */
  #define TERP_LOGERR( message ) \
  { \
    TE_LOG_ERROR( te::rp::macros::toString( message ).c_str() ); \
    TERP_LOGERR_STDOUT( message ); \
  };
      
  /*!
   \brief Logs a warning message.
   \param message Message to be logged.
   */
  #define TERP_LOGWARN( message ) \
  { \
    TE_LOG_WARN( te::rp::macros::toString( message ).c_str() ); \
    TERP_LOGWARN_STDOUT( message ); \
  };
      
  /*!
   \brief Logs a error message and throws.
   \param message Message to be logged.
   */
  #define TERP_LOG_AND_THROW( message ) \
  { \
    TERP_LOGERR( message ); \
    throw te::rp::Exception( te::rp::macros::toString( message ) ); \
  };
      
  /*!
   \brief Checks if value is true and throws an exception if not.
   \param value Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_TRUE_OR_THROW( value , message ) \
    if( ( value ) == 0 ) { \
      TERP_LOGERR( te::rp::macros::toString( message ) + \
        " - " + te::rp::macros::toString( #value ) ); \
      throw te::rp::Exception( te::rp::macros::toString( message ) ); \
    };      

  /*!
   \brief Variable watching.
   \param variable Variable to be logged.
   */
  #define TERP_WATCH( variable ) \
    { \
      TERP_LOGMSG( "WATCH - " + te::rp::macros::toString( #variable ) + \
        "=[" + te::rp::macros::toString( variable ) +"]" ); \
    };

  /**!   * @brief Checks if value is true and logs an warning message if not.
   \param value Value to be checked.
   \@param message Message to be logged.
   */
  #define TERP_TRUE_OR_LOG( value , message ) \
    if( ( value ) == 0 ) { \
      TERP_LOGWARN( te::rp::macros::toString( message ) + \
        " - " + te::rp::macros::toString( #value ) ); \
    };

  /*!
   \brief Checks if value is true. For false values a warning message will be logged 
   and a return of context with false value will be done.

   \param value Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_TRUE_OR_RETURN_FALSE( value , message ) \
    if( ( value ) == 0 ) { \
      TERP_LOGWARN( te::rp::macros::toString( message ) + \
        " - " + te::rp::macros::toString( #value ) ); \
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
      TERP_LOGWARN( te::rp::macros::toString( message ) + \
        " - " + te::rp::macros::toString( #value ) ); \
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
      TERP_LOGWARN( te::rp::macros::toString( message ) + \
        " - " + te::rp::macros::toString( #value ) ); \
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
      TERP_LOGWARN( te::rp::macros::toString( message ) + \
        " - " + te::rp::macros::toString( #value ) ); \
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
      TERP_LOGWARN( te::rp::macros::toString( message ) + \
        " - " + te::rp::macros::toString( #value ) ); \
    };

  /*!
   \brief Checks if two values are equal and throws an exception if not.
   \param value1 Value to be checked.
   \param value2 Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_CHECK_EQUAL( value1 , value2 , message ) \
    TERP_TRUE_OR_THROW( ( ((double)( value1 ) ) == ((double)( value2 ) ) ), \
    std::string( "Values must be equal [" ) + \
    te::rp::macros::toString( value1 ) + "!=" + \
    te::rp::macros::toString( value2 ) + "] - " + \
    te::rp::macros::toString( message ) );

  /*!
   \brief Checks if two values are diferent and throws an exception if not.
   \param value1 Value to be checked.
   \param value2 Value to be checked.
   \param message Message to be logged.
   */
  #define TERP_CHECK_NOT_EQUAL( value1 , value2 , message ) \
    TERP_TRUE_OR_THROW( ( ((double)( value1 )) != ((double)( value2 )) ), \
    std::string( "Values can't be equal [" ) + \
    te::rp::macros::toString( #value1 ) + std::string( "==" ) + \
    te::rp::macros::toString( #value2 ) + std::string( "==" ) + \
    te::rp::macros::toString( value1 ) + std::string( "]" ) );

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
      TERP_TRUE_OR_THROW( \
        TERP_CHECK_EPS_double_diff <= TERP_CHECK_EPS_double_eps, \
        std::string( "Values are not equal: " ) + \
        te::rp::macros::toString( #value1 ) + \
        std::string( "=[") + \
        te::rp::macros::toString( TERP_CHECK_EPS_double_value1 ) + \
        std::string( "] " ) + \
        te::rp::macros::toString( #value2 ) + \
        std::string( "=[") + \
        te::rp::macros::toString( TERP_CHECK_EPS_double_value2 ) + \
        std::string( "] eps=[") + \
        te::rp::macros::toString( TERP_CHECK_EPS_double_eps ) + \
        std::string( "] diff=[") + \
        te::rp::macros::toString( TERP_CHECK_EPS_double_diff ) + \
        std::string( "] - " ) + \
        te::rp::macros::toString( message ) \
        ); \
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
    #define MIN(x,y) ( ( (x) < (y) ) ? (x) : (y) )
  #endif
  
  /*!
   \brief Maximum of two values.
   */
  #ifndef MAX
    #define MAX(x,y) ( ( (x) > (y) ) ? (x) : (y) )
  #endif  
  
  /*!
   \brief Minimum of two values.
   */
  #ifndef ROUND
    #define ROUND(x) ( ( (x) >= 0 ) ? (long int)( (x) + 0.5 ) : (long int)( (x) - 0.5 ) )
  #endif  
  
  /*!
   \brief Absolute value.
   */
  #ifndef ABS
    #define ABS(x) ( ( (x) >= 0 ) ? (x) : -(x) )
  #endif   
  
namespace te
{
  namespace rp
  {
    namespace macros
    {
      /*!
      \brief Data conversion to string.
      \param data Data to be converted.
      \return The converted string.
      */
      template< class T >
      std::string toString( const T& data )
      {
          std::stringstream temp_ss;
          temp_ss.setf(std::ios_base::fixed);
          temp_ss << data;
          return temp_ss.str();
      }      
    };
  };
};

#endif  // __TERRALIB_RP_INTERNAL_MACROS_H


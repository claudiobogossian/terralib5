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
  \file terralib/rp/ArithmeticOperations.h
  \brief Performs arithmetic operation over raster data.
 */

#ifndef __TERRALIB_RP_INTERNAL_ARITHMETICOPERATIONS_H
#define __TERRALIB_RP_INTERNAL_ARITHMETICOPERATIONS_H

#include "Algorithm.h"

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace te
{
  namespace rst
  {
    class Raster;
  }

  namespace rp
  {
    /*!
      \class ArithmeticOperations
      
      \brief Performs arithmetic operation over raster data.
      
      \details The expression (arithmetic stringIt) must be formed by a combination of the following elements, 
               separated by spaces:

        Operators: +, -, *, /

        Real Numbers (negative numbers must follow the form "-1.0")

        Raster bands: R0:1, R0:1, R1:0, .... (R0:1 is a reference to the first raster - with index 0 - from feeder, second band - with index 1).
      
      \note Reference: TerraLib 4 - Image Processing Module
      
      \ingroup rp_gen
     */
    class TERPEXPORT ArithmeticOperations : public Algorithm
    {
      public:

        /*!
          \class InputParameters
          \brief ArithmeticOperations input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:

            std::vector< boost::shared_ptr<te::rst::Raster> > m_rasterVec; //!< Input rasters vector.

            std::string m_arithmeticString;  //!< Arithmetic string.

            bool m_normalize; //!< Output values normalization will be performed to fit the allowed values range (default:false).

            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
            InputParameters();
            
            InputParameters( const InputParameters& );
            
            ~InputParameters();
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const  InputParameters& operator=( const InputParameters& params );
            
            //overload
            AbstractParameters* clone() const;
        };

        /*!
          \class OutputParameters
          \brief ArithmeticOperations output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::string m_rType; //!< Output raster data source type (as described in te::raster::RasterFactory ).
            
            std::map< std::string, std::string > m_rInfo; //!< The necessary information to create the output rasters (as described in te::raster::RasterFactory). 
            
            std::auto_ptr< te::rst::Raster > m_outputRasterPtr; //!< The generated output registered raster.
            
            OutputParameters();
            
            OutputParameters( const OutputParameters& );
            
            ~OutputParameters();
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const  OutputParameters& operator=( const OutputParameters& params );
            
            //overload
            AbstractParameters* clone() const;
        };

        ArithmeticOperations();
        
        ~ArithmeticOperations();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:

        /*!
          \class ExecStackElement
          \brief Execution stack element.
         */ 
        class TERPEXPORT ExecStackElement
        {
          public :
        
            bool m_isRaster; //!< true if this is a raster pointer element.

            bool m_isRealNumber; //!< true if this is a real number element.

            double m_realNumberValue; //!< Real number value.

            unsigned int m_rasterBand; //!< Raster band index.

             boost::shared_ptr<te::rst::Raster> m_raster; //!< Raster pointer.

            ExecStackElement()
            : m_isRaster( false ), m_isRealNumber( false ), m_realNumberValue( 0 ),
              m_rasterBand( 0 )
            {};
          
            ~ExecStackElement() {};
        };

        typedef std::stack< ExecStackElement > ExecStackT; //!< Execution stack type definition.

        ArithmeticOperations::InputParameters m_inputParameters; //!< Input execution parameters.

        bool m_isInitialized; //!< Tells if this instance is initialized.

        /*!
          \brief Execute the automata parsing the given input string.
          \param aStr The input arithmetic expression string.
          \param inRasters Input rasters pointers.
          \param outRaster Output raster pointer (pre-initiated).
          \param generateOutput If true, the output raster data will be generated, if false only the automata execution will be performed..
        */
        bool executeString( const std::string& aStr, 
          std::vector< boost::shared_ptr<te::rst::Raster> > inRasters,
          boost::shared_ptr<te::rst::Raster>& outRaster,
          bool generateOutput ) const;
        
        /*!
          \brief Convert the input tokens vector from the infix notation to postfix notation.
          \param input The input tokens vector.
          \param output The output tokens vector.
        */
        void inFix2PostFix( const std::vector< std::string >& input, 
          std::vector< std::string >& output ) const;
        
        /*!
          \brief Print tokens to stout.
          \param input The input tokens vector.
        */
        void printTokens( const std::vector< std::string >& input ) const;        
        
        /*!
          \brief Returns true if the given token is an operator.
          \param inputToken Input token.
          \return Returns true if the given token is an operator.
        */         
        bool isOperator( const std::string& inputToken ) const;
      
        /*!
          \brief Returns true if the given token is a binary operator.
          \param inputToken Input token.
          \return Returns true if the given token is a binary operator.
        */               
        bool isBinaryOperator( const std::string& inputToken ) const;
      
        /*!
          \brief Returns true if the given token is a unary operator.
          \param inputToken Input token.
          \return Returns true if the given token is a unary operator.
        */            
        bool isUnaryOperator( const std::string& inputToken ) const;
      
        /*!
          \brief Returns true if operator1 has greater of equal precedence over operator2.
          \param operator1 Operator1 input token.
          \param operator2 Operator2 input token.
          \return Returns true if operator1 has greater of equal precedence over operator2.
        */      
        bool op1HasGreaterOrEqualPrecedence( const std::string& operator1, 
          const std::string& operator2 ) const;
        
        /*!
          \brief Returns true if the given token is a raster data token.
          \param token Input token.
          \param rasterIdx The output converted raster index value.
          \param bandIdx The output converted band index value.
          \return Returns true if the given token is a raster data token.
        */           
        bool isRasterBandToken( const std::string& token, unsigned int& rasterIdx,
          unsigned int& bandIdx ) const;
      
        /*!
          \brief Execute the given binary operator using the current given execution stack.
          \param token Operator token.
          \param execStack Execution stack.
          \param generateOutput if true the execution will generate valid output data, if false only dummy stack elements will be generated.
          \return true if OK, false on errors..
        */       
        bool execBinaryOperator( const std::string& token, ExecStackT& 
          execStack, bool generateOutput ) const;
      
        /*!
          \brief Execute the given unary operator using the current given execution stack.
          \param token Operator token.
          \param execStack Execution stack.
          \param generateOutput if true the execution will generate valid output data, if false only dummy stack elements will be generated.
          \return true if OK, false on errors..
        */         
        bool execUnaryOperator( const std::string& token, ExecStackT& 
          execStack, bool generateOutput ) const;
        
        /*!
          \brief Returns true if the given token is a real number.
          \param token Input token.
          \param realValue The output converted value.
          \return Returns true if the given token is a real number.
        */         
        bool isRealNumberToken( const std::string& token, double& realValue ) const;
      
        /*!
          \brief Allocate a new RAM memory raster.
          \param nLines Number of raster lines.
          \param nCols Number of raster columns.
          \param rasterPtr The output raster pointer.
          \return Returns true if OK, false on errors.
        */        
        bool allocResultRaster( unsigned int nLines, unsigned int nCols,
          boost::shared_ptr<te::rst::Raster>& rasterPtr ) const;
        
        /*!
          \brief Split the input string into a vector of token strings
          \param inputStr The input string.
          \param outTokens The generated output tokens vector.
        */         
        void getTokensStrs( const std::string& inputStr,
          std::vector< std::string >& outTokens ) const;
    };
  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_ARITHMETICOPERATIONS_H
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
  \file terralib/rp/GeoMosaic.cpp
  \brief Create a mosaic from a set of geo-referenced rasters.
*/

#include "ArithmeticOperations.h"

#include "Functions.h"
#include "Macros.h"

#include "../common/progress/TaskProgress.h"
#include "../common/StringUtils.h"
#include "../raster/Raster.h"
#include "../raster/Band.h"
#include "../memory/ExpansibleRaster.h"

#include <cfloat>

namespace te
{
  namespace rp
  {

    ArithmeticOperations::InputParameters::InputParameters()
    {
      reset();
    }

    ArithmeticOperations::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    ArithmeticOperations::InputParameters::~InputParameters()
    {
      reset();
    }

    void ArithmeticOperations::InputParameters::reset() throw( te::rp::Exception )
    {
      m_inputRasters.clear();
      m_arithmeticString.clear();
      m_normalize = false;
      m_enableProgress = false;
    }

    const ArithmeticOperations::InputParameters& ArithmeticOperations::InputParameters::operator=(
      const ArithmeticOperations::InputParameters& params )
    {
      reset();

      m_inputRasters = params.m_inputRasters;
      m_arithmeticString = params.m_arithmeticString;
      m_normalize = params.m_normalize;
      m_enableProgress = params.m_enableProgress;

      return *this;
    }

    te::common::AbstractParameters* ArithmeticOperations::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    ArithmeticOperations::OutputParameters::OutputParameters()
    {
      reset();
    }

    ArithmeticOperations::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    ArithmeticOperations::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void ArithmeticOperations::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const ArithmeticOperations::OutputParameters& ArithmeticOperations::OutputParameters::operator=(
      const ArithmeticOperations::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;
      m_outputRasterPtr.reset();

      return *this;
    }

    te::common::AbstractParameters* ArithmeticOperations::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    ArithmeticOperations::ArithmeticOperations()
    {
      reset();
    }

    ArithmeticOperations::~ArithmeticOperations()
    {
    }

    bool ArithmeticOperations::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      TERP_TRUE_OR_RETURN_FALSE( m_isInitialized, "Algoritm not initialized" );
      
      ArithmeticOperations::OutputParameters* outParamsPtr = dynamic_cast<
        ArithmeticOperations::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );

      // Counting the number of operations to be done
      std::string arithmetic_string = m_inputParameters.m_arithmeticString;

      unsigned int operationsNumber = 0;
      {
        for( unsigned sIdx = 0 ; sIdx < arithmetic_string.size() ; ++sIdx )
        { 
          std::string auxStr;
          auxStr.push_back( arithmetic_string[ sIdx ] );
      
          if( isOperator( auxStr ) ) ++operationsNumber;
        }
      }

      // progress

      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( operationsNumber );
        
        progressPtr->setMessage( "Arithmetic Operations" );
      }

      // Executing the arithmetic string

      std::auto_ptr<te::rst::Raster> auxRasterPtr;
      TERP_TRUE_OR_RETURN_FALSE( executeString( arithmetic_string, m_inputParameters.m_inputRasters, auxRasterPtr, true ), 
      "Arithmetic string execution error" );
      
      // Initializing the output raster

      std::vector< te::rst::BandProperty* > bandsProperties;
      bandsProperties.push_back( new te::rst::BandProperty(
        *( m_inputParameters.m_inputRasters[0]->getBand(0)->getProperty() ) ) );  
        
      outParamsPtr->m_outputRasterPtr.reset( 
        te::rst::RasterFactory::make(
          outParamsPtr->m_rType, 
          new te::rst::Grid( *( auxRasterPtr->getGrid() ) ),
          bandsProperties,
          outParamsPtr->m_rInfo,
          0,
          0 ) );
      TERP_TRUE_OR_RETURN_FALSE( outParamsPtr->m_outputRasterPtr.get(),
        "Output raster creation error" );      

      // Copy result data to output raster

      const unsigned int nRows = (unsigned int)auxRasterPtr->getNumberOfRows();
      const unsigned int nCols = (unsigned int)auxRasterPtr->getNumberOfColumns();
      unsigned int row = 0;
      unsigned int col = 0;
      double value = 0;
      te::rst::Raster& auxRasterRef = *auxRasterPtr;
      te::rst::Raster& outRasterRef = *outParamsPtr->m_outputRasterPtr;
  
      double outAllowedMin = 0;
      double outAllowedMax = 0;
      GetDataTypeRange( outParamsPtr->m_outputRasterPtr->getBandDataType( 0 ) , 
        outAllowedMin, outAllowedMax );
  
      if( m_inputParameters.m_normalize && ( outRasterRef.getBand(0)->getProperty()->getType() !=
        te::dt::DOUBLE_TYPE ) )
      {
        // Calculating the output gain and offset
    
        double auxMin = DBL_MAX;
        double auxMax = -1.0 * auxMin;  
    
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            auxRasterRef.getValue( col, row, value, 0 );
        
            if( auxMin > value ) auxMin = value;
            if( auxMax < value ) auxMax = value;
          }
      
          //TERP_FALSE_OR_RETURN( progress_.Increment(), "Canceled by the user" );
        } 
    
        double outputOffset = 0;  
        double outputGain = 1.0;
        if( auxMax != auxMin ) 
        {
          outputOffset = -1.0 * auxMin;
          outputGain = ( ( outAllowedMax - outAllowedMin ) / ( auxMax - auxMin ) );       
        }
      
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            auxRasterRef.getValue( col, row, value, 0 );
        
            value += outputOffset;
            value *= outputGain;
        
            value = MIN( value, outAllowedMax );
            value = MAX( value, outAllowedMin );
        
            outRasterRef.setValue( col, row, value, 0 );
          }
      
          //TERP_FALSE_OR_RETURN( progress_.Increment(), "Canceled by the user" );
        }        
      }
      else
      {
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            auxRasterRef.getValue( col, row, value, 0 );
        
            value = MIN( value, outAllowedMax );
            value = MAX( value, outAllowedMin );        
        
            outRasterRef.setValue( col, row, value, 0 );
          }
      
          //TERP_FALSE_OR_RETURN( progress_.Increment(), "Canceled by the user" );
        }  
      }

      return true;
    }

    void ArithmeticOperations::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool ArithmeticOperations::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      ArithmeticOperations::InputParameters const* inputParamsPtr = dynamic_cast<
        ArithmeticOperations::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the feeder

      TERP_TRUE_OR_RETURN_FALSE( !m_inputParameters.m_inputRasters.empty(),
        "Invalid number of rasters" );

      for( std::size_t t = 0; t <  m_inputParameters.m_inputRasters.size(); ++t )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRasters[t] != 0, 
          "Invalid raster" )

        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRasters[t]->getNumberOfRows() ==
          m_inputParameters.m_inputRasters[0]->getNumberOfRows(),
          "Invalid raster" )

        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRasters[t]->getNumberOfColumns() ==
          m_inputParameters.m_inputRasters[0]->getNumberOfColumns(),
          "Invalid raster" )
      }

      // Checking the expression

      TERP_TRUE_OR_RETURN_FALSE(!m_inputParameters.m_arithmeticString.empty(),
        "Arithmetic string is empty" )

      std::auto_ptr<te::rst::Raster> rasterNull;
      TERP_TRUE_OR_RETURN_FALSE(executeString(m_inputParameters.m_arithmeticString, 
      m_inputParameters.m_inputRasters, rasterNull, false), "Invalid arithmetic string" )

      m_isInitialized = true;

      return true;
    }

    bool ArithmeticOperations::isInitialized() const
    {
      return m_isInitialized;
    }

    bool ArithmeticOperations::executeString( const std::string& aStr, 
      const std::vector< te::rst::Raster* >& inRasters,
      std::auto_ptr<te::rst::Raster>& outRaster,
      bool generateOutput ) const
    {
      std::vector< std::string > infixTokensVec;
      getTokensStrs( aStr, infixTokensVec );

      if( infixTokensVec.size() )
      {
        std::vector< std::string > postfixTokensVec;

        inFix2PostFix( infixTokensVec, postfixTokensVec );

        ExecStackT execStack;
        unsigned int auxRasterIdx = 0;
        unsigned int auxBandIdx = 0;
        double auxRealValue = 0;
    
        for( unsigned int tIdx = 0 ; tIdx < postfixTokensVec.size() ; ++tIdx )
        {
          const std::string& curToken = postfixTokensVec[ tIdx ];
      
          if( isRasterBandToken( curToken, auxRasterIdx, auxBandIdx ) )
          {
            TERP_TRUE_OR_RETURN_FALSE( auxRasterIdx < inRasters.size(), 
            "Invalid raster index found at " + curToken );

            TERP_TRUE_OR_RETURN_FALSE( (std::size_t)auxBandIdx < inRasters[auxRasterIdx]->getNumberOfBands(), 
            "Invalid band index" );
      
            ExecStackElement auxEle;
            auxEle.m_isRaster = true;
            auxEle.m_rasterNPtr = inRasters[auxRasterIdx];
            auxEle.m_rasterBand = auxBandIdx;
          
            execStack.push( auxEle );
          }
          else if( isRealNumberToken( curToken, auxRealValue ) )
          {
            ExecStackElement auxEle;
            auxEle.m_isRealNumber = true;
            auxEle.m_realNumberValue = auxRealValue;
          
            execStack.push( auxEle );
          }      
          else if( isBinaryOperator( curToken ) )
          {
            TERP_TRUE_OR_RETURN_FALSE( execBinaryOperator( curToken, execStack, generateOutput ),
            "Operator " + curToken + " execution error" );
          }
          else if( isUnaryOperator( curToken ) )
          {
            TERP_TRUE_OR_RETURN_FALSE( execUnaryOperator( curToken, execStack, generateOutput ),
            "Operator " + curToken + " execution error" );
          }
          else
          {
            TERP_LOG_AND_RETURN_FALSE( "Invalid operator found: " + curToken );
          }
        }
    
        TERP_TRUE_OR_RETURN_FALSE( execStack.size() == 1, "Invalid stack size" );
        TERP_TRUE_OR_RETURN_FALSE( execStack.top().m_isRaster, "Stack result error" );
      
        outRaster.reset( execStack.top().m_rasterHandler.release() );
      }
  
      return true;
    }

    void ArithmeticOperations::inFix2PostFix( const std::vector< std::string >& input, 
      std::vector< std::string >& output ) const
    {
      output.clear();

      std::stack< std::string > auxStack;
      const unsigned int inputSize = (unsigned int)input.size();

      for( unsigned int inIdx = 0 ; inIdx < inputSize ; ++inIdx )
      {
        const std::string& currInToken = input[ inIdx ];
    
        if( isOperator( currInToken ) )
        {
          while( ( ! auxStack.empty() ) && ( auxStack.top() != "(" ) )
          {
            if ( op1HasGreaterOrEqualPrecedence( auxStack.top(), currInToken ) )
            {
              output.push_back( auxStack.top() );
          
              auxStack.pop();
            }
            else 
            {
              break;
            }
          }
      
          auxStack.push( currInToken );
        }
        else if( currInToken == "(" )
        {
          auxStack.push( currInToken );
        }
        else if( currInToken == ")" )
        {
          while ( ( ! auxStack.empty() ) && ( auxStack.top() != "(" ) )
          {
            output.push_back( auxStack.top() );
        
            auxStack.pop();
          }
      
          if ( ! auxStack.empty() ) 
          {
            auxStack.pop();
          }
        }    
        else
        {
          output.push_back( currInToken );
        }
      }
  
      while ( ! auxStack.empty() )
      {
        output.push_back( auxStack.top() );
    
        auxStack.pop();
      }  
    }

    void ArithmeticOperations::printTokens( const std::vector< std::string >& input ) const
    {
      std::cout << std::endl;
  
      for( unsigned int idx = 0 ; idx < input.size() ; ++idx )
      {
        std::cout << "[" << input[ idx ] << "]";
      }
  
      std::cout << std::endl;
    }

    bool ArithmeticOperations::isOperator( const std::string& inputToken ) const
    {
      return ( isBinaryOperator( inputToken ) || 
        isUnaryOperator( inputToken ) );
    }

    bool ArithmeticOperations::isBinaryOperator( const std::string& inputToken ) const
    {
      return ( ( inputToken == "+" ) || ( inputToken == "-" ) || 
        ( inputToken == "*" ) || ( inputToken == "/" ) ) ? 
        true : false;
    }

    bool ArithmeticOperations::isUnaryOperator( const std::string& inputToken ) const
    {
      return false;
    }

    bool ArithmeticOperations::op1HasGreaterOrEqualPrecedence( const std::string& operator1, 
      const std::string& operator2 ) const
    {
      if( ( operator1 == "*" ) || ( operator1 == "/" ) )
      {
        return true;
      }
      else
      {
        if( ( operator2 == "+" ) || ( operator2 == "-" ) )
        {
          return true;
        }
      }

      return false; 
    }

    bool ArithmeticOperations::isRasterBandToken( const std::string& token, unsigned int& rasterIdx,
      unsigned int& bandIdx ) const
    {
      if( token.size() < 4 ) return false;
      if( token[ 0 ] != 'R' ) return false;
  
      std::string rasterIdxStr;
      unsigned int tIdx = 1;
  
      while( ( tIdx < token.size() ) && isdigit( token[ tIdx ] ) )
      {
        rasterIdxStr.push_back( token[ tIdx ] );
        ++tIdx;
      }
  
      if( token[ tIdx ] != ':' ) return false;
      ++tIdx;
  
      std::string bandIdxStr;
  
      while( ( tIdx < token.size() ) && isdigit( token[ tIdx ] ) )
      {
        bandIdxStr.push_back( token[ tIdx ] );
        ++tIdx;
      }
  
      if( ( rasterIdxStr.size() ) && ( bandIdxStr.size() ) )
      {
        rasterIdx = (unsigned int)atoi( rasterIdxStr.c_str() );
        bandIdx = (unsigned int)atoi( bandIdxStr.c_str() );
        return true;
      }
      else
      {
        return false;
      }
    }

    bool ArithmeticOperations::execBinaryOperator( const std::string& token, ExecStackT& 
      execStack, bool generateOutput ) const
    {
      if( execStack.size() < 2 )
      {
        return false;
      }
      
      ExecStackElement sElem1 = execStack.top();
      execStack.pop();
  
      ExecStackElement sElem2 = execStack.top();
      execStack.pop();   
      
      // Choosing the binary operator function
      
      BinOpFuncPtrT binOptFunctPtr = 0;
      
      if( token == "+" )
      {
        binOptFunctPtr = &ArithmeticOperations::additionBinOp;
      }
      else if( token == "-" )
      { 
        binOptFunctPtr = &ArithmeticOperations::subtractionBinOp;
      }
      else if( token == "*" )
      { 
        binOptFunctPtr = &ArithmeticOperations::multiplicationBinOp;
      }
      else if( token == "/" )
      {
        binOptFunctPtr = &ArithmeticOperations::divisionBinOp;
      }
      else
      {
        TERP_LOG_AND_RETURN_FALSE( "Invalid operator" );
      }
        
      // executing the choosen operator
      
      ExecStackElement outElement;
      outElement.m_isRaster = true;
      outElement.m_rasterBand = 0;        
      
      if( generateOutput )
      {
        if( ( sElem1.m_isRaster ) && ( sElem2.m_isRaster ) )
        {
          TERP_TRUE_OR_RETURN_FALSE( execBinaryOperatorRasterXRaster( 
            *sElem2.m_rasterNPtr, sElem2.m_rasterBand, *sElem1.m_rasterNPtr, 
            sElem1.m_rasterBand, binOptFunctPtr, outElement.m_rasterHandler ),
            "Operator execution error" );
          outElement.m_rasterNPtr = outElement.m_rasterHandler.get();
        }
        else if( ( sElem1.m_isRaster ) && ( sElem2.m_isRealNumber ) )
        {
          TERP_TRUE_OR_RETURN_FALSE( execBinaryOperatorRasterXReal( 
            *sElem1.m_rasterNPtr, sElem1.m_rasterBand, sElem2.m_realNumberValue,
            binOptFunctPtr, outElement.m_rasterHandler, false ),
            "Operator execution error" );            
          outElement.m_rasterNPtr = outElement.m_rasterHandler.get();
        }
        else if( ( sElem1.m_isRealNumber ) && ( sElem2.m_isRaster ) )
        {
          TERP_TRUE_OR_RETURN_FALSE( execBinaryOperatorRasterXReal( 
            *sElem2.m_rasterNPtr, sElem2.m_rasterBand, sElem1.m_realNumberValue,
            binOptFunctPtr, outElement.m_rasterHandler, true ),
            "Operator execution error" );   
          outElement.m_rasterNPtr = outElement.m_rasterHandler.get();
        }
        else
        {
          TERP_LOG_AND_RETURN_FALSE( "Invalid stack elements" );
        }
      }
      
      execStack.push( outElement );
  
      return true;
    }
    
    bool ArithmeticOperations::execBinaryOperatorRasterXRaster( 
      const te::rst::Raster& inRaster1, 
      const unsigned int band1Idx, const te::rst::Raster& inRaster2, 
      const unsigned int band2Idx,
      const BinOpFuncPtrT binOptFunctPtr,
      std::auto_ptr<te::rst::Raster>& outRasterPtr ) const
    {
      if( inRaster1.getGrid()->operator==( *inRaster2.getGrid() ) )
      {
        if( ! allocResultRaster( *inRaster1.getGrid(), outRasterPtr ) )
        {
          return false;
        }
        
        const unsigned int nRows = inRaster1.getNumberOfRows();
        const unsigned int nCols = inRaster1.getNumberOfColumns();
        const te::rst::Band& inBand1 = *inRaster1.getBand( band1Idx );
        const te::rst::Band& inBand2 = *inRaster2.getBand( band2Idx );
        te::rst::Band& outBand = *outRasterPtr->getBand( 0 );
        const double inNoData1 = inBand1.getProperty()->m_noDataValue;
        const double inNoData2 = inBand2.getProperty()->m_noDataValue;
        const double outNoData = outBand.getProperty()->m_noDataValue;
        unsigned int row = 0;
        unsigned int col = 0;
        double value1 = 0;
        double value2 = 0;
        double outValue = 0;
        
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            inBand1.getValue( col, row, value1 );
            inBand2.getValue( col, row, value2 );
            
            if( ( value1 != inNoData1 ) && ( value2 != inNoData2 ) )
            {
              (this->*binOptFunctPtr)( value1, value2, outValue );
              outBand.setValue( col, row, outValue );
            }
            else
            {
              outBand.setValue( col, row, outNoData );
            }
          }
        }
        
        return true;
      }
      else
      {
        return false;
      }
      
      return true;
    }
    
    bool ArithmeticOperations::execBinaryOperatorRasterXReal( 
      const te::rst::Raster& inRaster, 
      const unsigned int bandIdx, const double value, 
      const BinOpFuncPtrT binOptFunctPtr,
      std::auto_ptr<te::rst::Raster>& outRasterPtr,
      const bool realNumberIsRigthtTerm ) const
    {
      if( ! allocResultRaster( *inRaster.getGrid(), outRasterPtr ) )
      {
        return false;
      }
      
      const unsigned int nRows = inRaster.getNumberOfRows();
      const unsigned int nCols = inRaster.getNumberOfColumns();
      const te::rst::Band& inBand = *inRaster.getBand( bandIdx );
      te::rst::Band& outBand = *outRasterPtr->getBand( 0 );
      const double inNoData = inBand.getProperty()->m_noDataValue;
      const double outNoData = outBand.getProperty()->m_noDataValue;
      unsigned int row = 0;
      unsigned int col = 0;
      double value1 = 0;
      double outValue = 0;
      
      if( realNumberIsRigthtTerm )
      {
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            inBand.getValue( col, row, value1 );
            
            if( value1 != inNoData )
            {
              (this->*binOptFunctPtr)( value1, value, outValue );
              outBand.setValue( col, row, outValue );
            }
            else
            {
              outBand.setValue( col, row, outNoData );
            }
          }
        }
      }
      else
      {
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            inBand.getValue( col, row, value1 );
            
            if( value1 != inNoData )
            {
              (this->*binOptFunctPtr)( value, value1, outValue );
              outBand.setValue( col, row, outValue );
            }
            else
            {
              outBand.setValue( col, row, outNoData );
            }
          }
        }
      }
      
      return true;
    }    

    bool ArithmeticOperations::execUnaryOperator( const std::string& token, ExecStackT& 
      execStack, bool generateOutput ) const
    {
      return false;
    }

    bool ArithmeticOperations::isRealNumberToken( const std::string& token, double& realValue ) const
    {
      if( token.size() == 0 ) return false;
      bool hasDot = false;

      unsigned int initIdx = 0;
      if( ( token[ 0 ] == '+' ) || ( token[ 0 ] == '-' ) )
      {
        if( token.size() == 1 ) return false;
        initIdx = 1;
      }
  
      for( unsigned int tIdx = initIdx ; tIdx < token.size() ; ++tIdx )
      {
        if( token[ tIdx ] == '.' )
        {
          if( hasDot ) return false;
          hasDot = true;
        }
        else if( ! isdigit( token[ tIdx ] ) ) return false;
      }
  
      realValue = atof( token.c_str() );
  
      return true;
    }

    bool ArithmeticOperations::allocResultRaster( const te::rst::Grid& grid,
      std::auto_ptr<te::rst::Raster>& rasterPtr ) const
    {
      std::map<std::string, std::string> rinfo;
      
      std::vector< te::rst::BandProperty * > bandsProperties;
      bandsProperties.push_back( new te::rst::BandProperty( 0, te::dt::DOUBLE_TYPE, "" ) );
      bandsProperties[ 0 ]->m_blkw = grid.getNumberOfColumns();
      bandsProperties[ 0 ]->m_blkh = 1;
      bandsProperties[ 0 ]->m_nblocksx = 1;
      bandsProperties[ 0 ]->m_nblocksy = grid.getNumberOfRows();
 
      rasterPtr.reset( new te::mem::ExpansibleRaster( 50, new te::rst::Grid(
        grid ), bandsProperties ) );

      return true;
    }

    void ArithmeticOperations::getTokensStrs( const std::string& inputStr,
      std::vector< std::string >& outTokens ) const
    {
      outTokens.clear();
  
      const unsigned int inputStrSize = (unsigned int)inputStr.size();
      std::string bufferStr;
  
      for( unsigned int inputStrIdx = 0 ; inputStrIdx < inputStrSize ;
        ++inputStrIdx )
      {
        if( inputStr[ inputStrIdx ] == ' ' )
        {
          // flush the last formed token
          if( bufferStr.size() )
          {
            outTokens.push_back( bufferStr );
            bufferStr.clear();
          }
        }
        else
        {
          if( ( inputStr[ inputStrIdx ] == '+' ) ||
            ( inputStr[ inputStrIdx ] == '-' ) )
          {
            bufferStr.append( inputStr.substr( inputStrIdx, 1 ) );
          }      
          else if( ( inputStr[ inputStrIdx ] == ')' ) ||
            ( inputStr[ inputStrIdx ] == '(' ) ||
            isOperator( inputStr.substr( inputStrIdx, 1 ) ) )
          {
            // flush the last formed token
            if( bufferStr.size() )
            {
              outTokens.push_back( bufferStr );
              bufferStr.clear();
            }
        
            outTokens.push_back( inputStr.substr( inputStrIdx, 1 ) );
          }    
          else
          {
            bufferStr.append( inputStr.substr( inputStrIdx, 1 ) );
          }
        }
      }
  
      if( bufferStr.size() )
      {
        outTokens.push_back( bufferStr );
      }
    }
  }
}

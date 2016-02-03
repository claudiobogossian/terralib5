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
  \file terralib/rp/ClassifierSAMStrategy.h
  \brief Spectral Angle Mapper classification strategy.
 */

#ifndef __TERRALIB_RP_INTERNAL_CLASSIFIERSAMSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_CLASSIFIERSAMSTRATEGY_H

#include "ClassifierStrategy.h"
#include "ClassifierStrategyFactory.h"
#include "Config.h"

#include <boost/shared_ptr.hpp>

namespace te
{
  namespace rp
  {
    /*!
      \class ClassifierSAMStrategy

      \brief Spectral Angle Mapper classification strategy.
      
      \details This algorithm maps the spectral similarity of input raster
      to the given reference spectra wich can be either laboratory of field spectra.
      This method assumes that the data have been reduced to apparent reflectance 
      with all dark current and path radiance biases removed.
      Reference: The spectral image processing system (SIPS)- interactive 
      visualization and analysis of imaging spectrometer data. Kruse, F. A.; 
      Lefkoff, A. B.; Boardman, J. W.; Heidebrecht, K. B.; Shapiro, A. T.; 
      Barloon, P. J.; Goetz, A. F. H. The earth and space science information 
      system (ESSIS). AIP Conference Proceedings, Volume 283, pp. 192-201 (1993).
      
      \ingroup rp_class
     */
    class TERPEXPORT ClassifierSAMStrategy : public ClassifierStrategy
    {
      public:
        
        typedef unsigned int ClassIDT; //!< Class ID type definition (zero means invalid ID).
        
        typedef std::vector< double > SampleT; //!< Class sample type definition.
        
        typedef std::vector< SampleT > SamplesT; //!< Class samples container type definition.    
        
        typedef std::map< ClassIDT, SamplesT > ClassesSamplesT; //!< Classes samples container type definition.

        typedef boost::shared_ptr<ClassesSamplesT> ClassesSamplesTPtr; //!< A shared pointer to a multi classes samples container type definition.
      
        /*!
          \class Parameters

          \brief Classifier Parameters
        */
        class TERPEXPORT Parameters : public ClassifierStrategyParameters
        {
          public:
            
            ClassesSamplesTPtr m_trainSamplesPtr; //!< A shared pointer to a always-valid structure where trainning samples are stored.
            
            std::vector< double > m_maxAngularDistances; //!< This is a vector of maximum acceptable angles (radians) between one pixel spectra and the reference spectra for each class (pixels with angular distance higher than this value will not be classifyed as belonging to each class (common used default:0.1 radians).

			Parameters();

            ~Parameters();

            //overload
            const Parameters& operator=(const Parameters& params);

            //overload
            void reset() throw( te::rp::Exception );

            //overload
            AbstractParameters* clone() const;
        };

        ClassifierSAMStrategy();

        ~ClassifierSAMStrategy();

        //overload
        bool initialize(ClassifierStrategyParameters const* const strategyParams) throw(te::rp::Exception);

        //overload
        bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                     const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                     const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception);
        		
        // overload
        std::vector< int > getOutputDataTypes() const; 

      protected :
        
        bool m_isInitialized; //!< Is this instance initialized?
        
        ClassifierSAMStrategy::Parameters m_initParams; //!< Initialization parameters.
        
        SamplesT m_classesMeans; //!< Classes means.
        
        std::vector< ClassIDT > m_classesIndex2ID; //!< An class index ordered vector of classes IDs;
    };

    /*!
      \class ClassifierSAMStrategyFactory

      \brief Spectral Angle Mapper strategy factory.
    */
    class TERPEXPORT ClassifierSAMStrategyFactory : public ClassifierStrategyFactory
    {
      public:

        ClassifierSAMStrategyFactory();

        ~ClassifierSAMStrategyFactory();

        //overload
        te::rp::ClassifierStrategy* build();
    };

  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_CLASSIFIERSAMSTRATEGY_H


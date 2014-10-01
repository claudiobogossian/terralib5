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
  \file terralib/rp/MixtureModel.h

  \brief Raster decomposition using mixture model.
 */

#ifndef __TERRALIB_RP_INTERNAL_MIXTUREMODEL_H
#define __TERRALIB_RP_INTERNAL_MIXTUREMODEL_H

// TerraLib
#include "Algorithm.h"
#include "Config.h"
#include "Matrix.h"
#include "StrategyParameters.h"

namespace te
{
  namespace rst
  {
    class Raster;
    class Band;
  }

  namespace rp
  {
    /*!
      \class MixtureModel

      \brief Raster decomposition using mixture model.

      \details Raster decomposition following the choosed mixture model strategy.

      \note The created output image will be written to the raster instance
      pointed by m_outRasterPtr (in this case the output band must also be
      passed by m_outRasterBand).

      \ingroup rp_class
    */

    class TERPEXPORT MixtureModel : public Algorithm
    {
      public:

        /*!
          \class InputParameters

          \brief MixtureModel input parameters
        */

        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:

            /*! \brief Default constructor. */
            InputParameters();

            /*!
              \brief Copy constructor.

              \param rhs The right-hand side Raster.
            */
            InputParameters(const InputParameters& rhs);

            ~InputParameters();

            /*!
              \brief Set specific mixture model strategy parameters.

              \param p The specific mixture model strategy parameters.
            */
            void setMixtureModelStrategyParams(const StrategyParameters& p);

            /*!
              \brief Returns a pointer to the internal specific mixture model strategy parameters, or null if no parameters are present.
            */
            StrategyParameters const* getMixtureModelStrategyParams() const;

            //overload
            void reset() throw(te::rp::Exception);

            //overload
            const MixtureModel::InputParameters& operator=(const MixtureModel::InputParameters& params);

            //overload
            te::common::AbstractParameters* clone() const;

          public:

            te::rst::Raster const* m_inputRasterPtr;                      //!< Input raster.
            std::vector<unsigned int> m_inputRasterBands;                 //!< Bands to be processed from the input raster.
            std::vector<std::string> m_inputSensorBands;                  //!< The names of the sensor/bands.
            std::map<std::string, std::vector<double> > m_components;     //!< A set of endmembers and its radiances.
            std::string m_strategyName;                                   //!< The mixture model strategy name see each te::rp::MixtureModelStrategyFactory inherited classes documentation for reference.
            StrategyParameters* m_mixtureModelStrategyParamsPtr;          //!< Internal specific mixture model strategy parameters.

        };

        /*!
          \class OutputParameters

          \brief MixtureModel output parameters
        */

        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:

            /*! \brief Default constructor. */
            OutputParameters();

            /*!
              \brief Copy constructor.

              \param rhs The right-hand side Raster.
            */
            OutputParameters(const OutputParameters& rhs);

            ~OutputParameters();

            //overload
            void reset() throw(te::rp::Exception);

            //overload
            const MixtureModel::OutputParameters& operator=(const MixtureModel::OutputParameters& params);

            //overload
            te::common::AbstractParameters* clone() const;

          public:

            std::string m_rType;                                          //!< Output raster data source type (as described in te::raster::RasterFactory ).
            std::map< std::string, std::string > m_rInfo;                 //!< The necessary information to create the raster (as described in te::raster::RasterFactory).
            mutable std::auto_ptr<te::rst::Raster> m_outputRasterPtr;     //!< A pointer to the generated output raster, one band per component plus one error band per component (when parameter m_createErrorRaster is true).
            bool m_normalizeOutput;                                       //!< A flag to indicate that output raster will be normalized, by default [0, 255].
            bool m_createErrorRaster;                                     //!< A flag to indicate that output raster will include the error bands.

        };

        MixtureModel();

        ~MixtureModel();

        //overload
        bool execute(AlgorithmOutputParameters& outputParams) throw(te::rp::Exception);

        //overload
        void reset() throw(te::rp::Exception);

        //overload
        bool initialize(const AlgorithmInputParameters& inputParams) throw(te::rp::Exception);

        bool isInitialized() const;

      protected :

        bool m_instanceInitialized;                        //!< Is this instance already initialized?
        MixtureModel::InputParameters m_inputParameters;   //!< Mixture model execution parameters.
    };

  } // end namespace rp
}   // end namespace te

#endif

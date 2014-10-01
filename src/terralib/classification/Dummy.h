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
  \file terralib/classification/Dummy.h

  \brief Dummy classifier (just for testing purposes).
*/

#ifndef __TERRALIB_CLASSIFICATION_INTERNAL_DUMMY_H
#define __TERRALIB_CLASSIFICATION_INTERNAL_DUMMY_H

// TerraLib
#include "../common/AbstractParameters.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../raster/Grid.h"
#include "Config.h"
#include "Exception.h"

// STL
#include <iostream>
#include <stdlib.h>

namespace te
{
  namespace cl
  {
    /*!
      \class Dummy

      \brief Dummy classifier (just for testing purposes).
    */
    template<class TTRAIN, class TCLASSIFY>
    class Dummy
    {
      public:

        /*!
          \class Parameters

          \brief Classifier Parameters
        */
        class Parameters : public te::common::AbstractParameters
        {
          public:

            unsigned int m_dummyParameter;  //!< A dummy parameter.

            Parameters();

            ~Parameters();

            //overload
            const Parameters& operator=(const Parameters& params);

            //overload
            void reset() throw(te::cl::Exception);

            //overload
            AbstractParameters* clone() const;
        };

      public:

        Dummy();

        ~Dummy();

        bool initialize(const Parameters& params) throw(te::cl::Exception);

        bool train(TTRAIN& itBegin, TTRAIN& itEnd,
                   const std::vector<unsigned int>& attributesIndices,
                   const std::vector<unsigned int>& labels,
                   const bool enableProgressInterface) throw(te::cl::Exception);

        bool classify(TCLASSIFY& itBegin, TCLASSIFY& itEnd,
                      const std::vector<unsigned int>& attributesIndices,
                      std::vector<unsigned int>& classification,
                      const bool enableProgressInterface) throw(te::cl::Exception);
      protected:

        bool m_isInitialized;                              //!< True if this instance is initialized.
        Parameters m_parameters;                           //!< Internal execution parameters.
    };

  }; // end namespace cl
};  // end namespace te

// class of Parameters
template <class TTRAIN, class TCLASSIFY>
te::cl::Dummy<TTRAIN, TCLASSIFY>::Parameters::Parameters()
{
  reset();
}

template <class TTRAIN, class TCLASSIFY>
te::cl::Dummy<TTRAIN, TCLASSIFY>::Parameters::~Parameters()
{
}

template <class TTRAIN, class TCLASSIFY>
typename const te::cl::Dummy<TTRAIN, TCLASSIFY>::Parameters& te::cl::Dummy<TTRAIN, TCLASSIFY>::Parameters::operator=(typename const te::cl::Dummy<TTRAIN, TCLASSIFY>::Parameters& rhs)
{
  reset();

  m_dummyParameter = rhs.m_dummyParameter;

  return *this;
}

template <class TTRAIN, class TCLASSIFY>
void te::cl::Dummy<TTRAIN, TCLASSIFY>::Parameters::reset() throw(te::cl::Exception)
{
  m_dummyParameter = 0;
}

template <class TTRAIN, class TCLASSIFY>
te::common::AbstractParameters* te::cl::Dummy<TTRAIN, TCLASSIFY>::Parameters::clone() const
{
  return new te::cl::Dummy<TTRAIN, TCLASSIFY>::Parameters(*this);
}

// class Dummy Strategy
template <class TTRAIN, class TCLASSIFY>
te::cl::Dummy<TTRAIN, TCLASSIFY>::Dummy()
{
  m_isInitialized = false;
}

template <class TTRAIN, class TCLASSIFY>
te::cl::Dummy<TTRAIN, TCLASSIFY>::~Dummy()
{
}

template <class TTRAIN, class TCLASSIFY>
bool te::cl::Dummy<TTRAIN, TCLASSIFY>::initialize(typename const te::cl::Dummy<TTRAIN, TCLASSIFY>::Parameters& params) throw(te::cl::Exception)
{
  m_isInitialized = false;

  m_parameters = params;

  if (m_parameters.m_dummyParameter < 2)
  {
    TE_TR("The value of dummy must be at least 2.");
    return false;
  }

  m_isInitialized = true;

  return true;
}

template<class TTRAIN, class TCLASSIFY>
bool te::cl::Dummy<TTRAIN, TCLASSIFY>::train(TTRAIN& itBegin, TTRAIN& itEnd,
                                             const std::vector<unsigned int>& attributesIndices,
                                             const std::vector<unsigned int>& labels,
                                             const bool enableProgressInterface) throw(te::cl::Exception)
{
  return true;
}

template<class TTRAIN, class TCLASSIFY>
bool te::cl::Dummy<TTRAIN, TCLASSIFY>::classify(TCLASSIFY& itBegin, TCLASSIFY& itEnd,
                                                const std::vector<unsigned int>& attributesIndices,
                                                std::vector<unsigned int>& classification,
                                                const bool enableProgressInterface) throw(te::cl::Exception)
{
  TCLASSIFY it = itBegin;
  while(it != itEnd)
  {
    classification.push_back(rand() % m_parameters.m_dummyParameter + 1);

    ++it;
  }

  return true;
}

#endif // __TERRALIB_CLASSIFICATION_INTERNAL_DUMMY_H

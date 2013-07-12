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
#include "../../common/AbstractParameters.h"
#include "../../geometry/Coord2D.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Point.h"
#include "../../geometry/Polygon.h"
#include "../../raster/Grid.h"
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
    template<class T>
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

        bool train(T& itBegin, T& itEnd,
                   const std::vector<unsigned int>& attributesIndices,
                   const std::vector<unsigned int>& labels,
                   const bool enableProgressInterface) throw(te::cl::Exception);

        bool classify(T& itBegin, T& itEnd,
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
template <class T> te::cl::Dummy<T>::Parameters::Parameters()
{
  reset();
}

template <class T> te::cl::Dummy<T>::Parameters::~Parameters()
{
}

template <class T> typename const te::cl::Dummy<T>::Parameters& te::cl::Dummy<T>::Parameters::operator=(typename const te::cl::Dummy<T>::Parameters& rhs)
{
  reset();

  m_dummyParameter = rhs.m_dummyParameter;

  return *this;
}

template <class T> void te::cl::Dummy<T>::Parameters::reset() throw(te::cl::Exception)
{
  m_dummyParameter = 0;
}

template <class T> te::common::AbstractParameters* te::cl::Dummy<T>::Parameters::clone() const
{
  return new te::cl::Dummy<T>::Parameters(*this);
}

// class Dummy Strategy
template <class T> te::cl::Dummy<T>::Dummy()
{
  m_isInitialized = false;
}

template <class T> te::cl::Dummy<T>::~Dummy()
{
}

template <class T> bool te::cl::Dummy<T>::initialize(typename const te::cl::Dummy<T>::Parameters& params) throw(te::cl::Exception)
{
  m_isInitialized = false;

  m_parameters = params;

  if (m_parameters.m_dummyParameter < 2)
  {
    TR_CL("The value of dummy must be at least 2.");
    return false;
  }

  m_isInitialized = true;

  return true;
}

template<class T> bool te::cl::Dummy<T>::train(T& itBegin, T& itEnd,
                                               const std::vector<unsigned int>& attributesIndices,
                                               const std::vector<unsigned int>& labels,
                                               const bool enableProgressInterface) throw(te::cl::Exception)
{
  return true;
}

template<class T> bool te::cl::Dummy<T>::classify(T& itBegin, T& itEnd,
                                                  const std::vector<unsigned int>& attributesIndices,
                                                  std::vector<unsigned int>& classification,
                                                  const bool enableProgressInterface) throw(te::cl::Exception)
{
  T it = itBegin;
  while(it != itEnd)
  {
    classification.push_back(rand() % m_parameters.m_dummyParameter + 1);

    ++it;
  }

  return true;
}

#endif // __TERRALIB_CLASSIFICATION_INTERNAL_DUMMY_H

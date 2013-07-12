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
  \file terralib/classification/KMeans.h

  \brief KMeans classifier (just for testing purposes).
*/

#ifndef __TERRALIB_CLASSIFICATION_INTERNAL_KMEANS_H
#define __TERRALIB_CLASSIFICATION_INTERNAL_KMEANS_H

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
      \class KMeans

      \brief KMeans strategy for classification.
             Step-by-step:
             1. Define a random solution, creating K values of mean.
             2. Classify each input value according to the closest mean.
             3. Calculate new means using the classified pixels.
             4. Go back to step 2, or stop depending on the following checks:
             4.1. Check if the new means are different from the previous iteration,
                  using the convergence threshold.
             4.2. Check if maximum number of iterations has achieved.
    */
    template<class T>
    class KMeans
    {
      public:

        /*!
          \class Parameters

          \brief Classifier Parameters
        */
        class Parameters : public te::common::AbstractParameters
        {
          public:

            unsigned int m_K;                    //!< The number of clusters (means) to detect in image.
            unsigned int m_maxIterations;        //!< The maximum of iterations to perform if convergence is not achieved.
            unsigned int m_maxInputPoints;       //!< The maximum number of points used to estimate the clusters (default = 1000).
            double m_epsilon;                    //!< The stop criteria. When the clusters change in a value smaller then epsilon, the convergence is achieved.

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

        KMeans();

        ~KMeans();

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

        unsigned int getClassification(std::vector<double> values);

        bool m_isInitialized;                              //!< True if this instance is initialized.
        std::vector<std::vector<double> > m_KMeans;        //!< The vector of K means.
        Parameters m_parameters;                           //!< Internal execution parameters.
    };

  }; // end namespace cl
};  // end namespace te

// class of Parameters
template <class T> te::cl::KMeans<T>::Parameters::Parameters()
{
  reset();
}

template <class T> te::cl::KMeans<T>::Parameters::~Parameters()
{
}

template <class T> typename const te::cl::KMeans<T>::Parameters& te::cl::KMeans<T>::Parameters::operator=(typename const te::cl::KMeans<T>::Parameters& rhs)
{
  reset();

  m_K = rhs.m_K;
  m_maxIterations = rhs.m_maxIterations;
  m_maxInputPoints = rhs.m_maxInputPoints;
  m_epsilon = rhs.m_epsilon;

  return *this;
}

template <class T> void te::cl::KMeans<T>::Parameters::reset() throw(te::cl::Exception)
{
  m_K = 0;
  m_maxIterations = 0;
  m_maxInputPoints = 0;
  m_epsilon = 0.0;
}

template <class T> te::common::AbstractParameters* te::cl::KMeans<T>::Parameters::clone() const
{
  return new te::cl::KMeans<T>::Parameters(*this);
}

// class KMeans Strategy
template <class T> te::cl::KMeans<T>::KMeans()
{
  m_isInitialized = false;
  m_KMeans.clear();
}

template <class T> te::cl::KMeans<T>::~KMeans()
{
}

template <class T> bool te::cl::KMeans<T>::initialize(typename const te::cl::KMeans<T>::Parameters& params) throw(te::cl::Exception)
{
  m_isInitialized = false;

  m_KMeans.clear();
  m_parameters = params;

  if (m_parameters.m_K < 2)
  {
    TR_CL("The value of K must be at least 2.");
    return false;
  }
  if (m_parameters.m_maxIterations < 1)
    m_parameters.m_maxIterations = 10;
  if (m_parameters.m_maxInputPoints < m_parameters.m_K)
    m_parameters.m_maxInputPoints = 1000;
  if (m_parameters.m_epsilon < 0)
    m_parameters.m_epsilon = 0.0000000001;

  m_isInitialized = true;

  return true;
}

template<class T> bool te::cl::KMeans<T>::train(T& itBegin, T& itEnd,
                                                const std::vector<unsigned int>& attributesIndices,
                                                const std::vector<unsigned int>& labels,
                                                const bool enableProgressInterface) throw(te::cl::Exception)
{
  std::vector<double> maxValues;
  std::vector<double> minValues;

  for (unsigned int i = 0; i < attributesIndices.size(); i++)
  {
    maxValues.push_back(std::numeric_limits<double>::min());
    minValues.push_back(std::numeric_limits<double>::max());
  }

// find maximum and minimum values for each attribute
  std::vector<unsigned int> tmpClassification;
  T it = itBegin;
  while(it != itEnd)
  {
    tmpClassification.push_back(0);
    for (unsigned int i = 0; i < attributesIndices.size(); i++)
    {
      if ((*it)[attributesIndices[i]] > maxValues[i])
        maxValues[i] = (*it)[attributesIndices[i]];
      if ((*it)[attributesIndices[i]] < minValues[i])
        minValues[i] = (*it)[attributesIndices[i]];
    }
    ++it;
  }

  srand((unsigned) time(0));
// starting random K means
  m_KMeans.clear();
  for (unsigned int k = 0; k <= m_parameters.m_K; k++)
  {
    std::vector<double> newMean;
    for (unsigned int i = 0; i < attributesIndices.size(); i++)
      newMean.push_back(rand() % (int) (maxValues[i] - minValues[i]));

    m_KMeans.push_back(newMean);
  }

// estimate K means
  std::vector<double> values;
  std::map<unsigned int, std::vector<double> > tmpValues;
  std::vector<unsigned int> tmpNs;
  unsigned int tmpClass;
  double distanceKMeans;
  double a_minus_b;
  std::vector<std::vector<double> > oldKMeans;
  for (unsigned int k = 0; k <= m_parameters.m_K; k++)
  {
    tmpValues[k] = std::vector<double>(attributesIndices.size());
    tmpNs.push_back(0);
  }

  te::common::TaskProgress task(TR_CL("K-Means algorithm - training step"), te::common::TaskProgress::UNDEFINED, m_parameters.m_maxIterations);
  for (unsigned int t = 0; t < m_parameters.m_maxIterations; t++)
  {
    for (unsigned int k = 0; k <= m_parameters.m_K; k++)
    {
      for (unsigned int i = 0; i < attributesIndices.size(); i++)
        tmpValues[k][i] = 0.0;
      tmpNs[k] = 0;
    }

    // classifying input data according to actual K means
    it = itBegin;
    while(it != itEnd)
    {
      values.clear();
      for (unsigned int i = 0; i < attributesIndices.size(); i++)
        values.push_back((*it)[attributesIndices[i]]);
      tmpClass = getClassification(values);
      for (unsigned int i = 0; i < attributesIndices.size(); i++)
        tmpValues[tmpClass][i] += values[i];
      tmpNs[tmpClass]++;
      ++it;
    }

    oldKMeans = m_KMeans;
// recomputing K means
    for (unsigned int k = 0; k <= m_parameters.m_K; k++)
      for (unsigned int i = 0; i < attributesIndices.size(); i++)
        if (tmpNs[k] > 0)
          m_KMeans[k][i] = (double) (tmpValues[k][i] / (double) tmpNs[k]);

// checking convergence
    distanceKMeans = 0.0;
    for (unsigned int k = 0; k <= m_parameters.m_K; k++)
      for (unsigned int i = 0; i < attributesIndices.size(); i++)
      {
        a_minus_b = m_KMeans[k][i] - oldKMeans[k][i];
        distanceKMeans += a_minus_b * a_minus_b;
      }
    distanceKMeans = sqrt(distanceKMeans);
    if (distanceKMeans < m_parameters.m_epsilon)
      break;

    task.pulse();
  }

  return true;
}

template<class T> bool te::cl::KMeans<T>::classify(T& itBegin, T& itEnd,
                                                   const std::vector<unsigned int>& attributesIndices,
                                                   std::vector<unsigned int>& classification,
                                                   const bool enableProgressInterface) throw(te::cl::Exception)
{
  T it = itBegin;
  std::vector<double> values;
  while(it != itEnd)
  {
    values.clear();
    for (unsigned int i = 0; i < attributesIndices.size(); i++)
      values.push_back((*it)[attributesIndices[i]]);
    classification.push_back(getClassification(values));

    ++it;
  }

  return true;
}

template<class T> unsigned int te::cl::KMeans<T>::getClassification(std::vector<double> values)
{
  double minDistance = std::numeric_limits<double>::max();
  double distance;
  unsigned int clusterNumber = 0;

  for (unsigned int k = 1; k <= m_parameters.m_K; k++)
  {
    distance = te::rp::GetEuclideanDistance(m_KMeans[k], values);
    if (distance < minDistance)
    {
      minDistance = distance;
      clusterNumber = k;
    }
  }

  return clusterNumber;
}

#endif // __TERRALIB_CLASSIFICATION_INTERNAL_KMEANS_H

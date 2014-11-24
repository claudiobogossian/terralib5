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

  \brief KMeans strategy for classification.
*/

#ifndef __TERRALIB_CLASSIFICATION_INTERNAL_KMEANS_H
#define __TERRALIB_CLASSIFICATION_INTERNAL_KMEANS_H

// TerraLib
#include "../common/AbstractParameters.h"
#include "../common/progress/TaskProgress.h"
#include "../raster/Grid.h"
#include "Config.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <map>
#include <vector>

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
    template<class TTRAIN, class TCLASSIFY>
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

        bool train(TTRAIN& itBegin, TTRAIN& itEnd,
                   const std::vector<unsigned int>& attributesIndices,
                   const std::vector<unsigned int>& labels,
                   const bool enableProgressInterface) throw(te::cl::Exception);

        bool classify(TCLASSIFY& itBegin, TCLASSIFY& itEnd,
                      const std::vector<unsigned int>& attributesIndices,
                      std::vector<unsigned int>& classification,
                      const bool enableProgressInterface) throw(te::cl::Exception);

      protected:

        unsigned int getClassification(std::vector<double> values);

        bool m_isInitialized;                              //!< True if this instance is initialized.
        std::vector<std::vector<double> > m_KMeans;        //!< The vector of K means.
        Parameters m_parameters;                           //!< Internal execution parameters.
    };

  } // end namespace cl
}  // end namespace te

// class of Parameters
template <class TTRAIN, class TCLASSIFY>
te::cl::KMeans<TTRAIN, TCLASSIFY>::Parameters::Parameters()
{
  reset();
}

template <class TTRAIN, class TCLASSIFY>
te::cl::KMeans<TTRAIN, TCLASSIFY>::Parameters::~Parameters()
{
}

template <class TTRAIN, class TCLASSIFY>
const typename te::cl::KMeans<TTRAIN, TCLASSIFY>::Parameters& te::cl::KMeans<TTRAIN, TCLASSIFY>::Parameters::operator=(const typename te::cl::KMeans<TTRAIN, TCLASSIFY>::Parameters& rhs)
{
  reset();

  m_K = rhs.m_K;
  m_maxIterations = rhs.m_maxIterations;
  m_epsilon = rhs.m_epsilon;

  return *this;
}

template <class TTRAIN, class TCLASSIFY>
void te::cl::KMeans<TTRAIN, TCLASSIFY>::Parameters::reset() throw(te::cl::Exception)
{
  m_K = 0;
  m_maxIterations = 0;
  m_epsilon = 0.0;
}

template <class TTRAIN, class TCLASSIFY>
te::common::AbstractParameters* te::cl::KMeans<TTRAIN, TCLASSIFY>::Parameters::clone() const
{
  return new te::cl::KMeans<TTRAIN, TCLASSIFY>::Parameters(*this);
}

// class KMeans Strategy
template <class TTRAIN, class TCLASSIFY>
te::cl::KMeans<TTRAIN, TCLASSIFY>::KMeans()
{
  m_isInitialized = false;
  m_KMeans.clear();
}

template <class TTRAIN, class TCLASSIFY>
te::cl::KMeans<TTRAIN, TCLASSIFY>::~KMeans()
{
}

template <class TTRAIN, class TCLASSIFY>
bool te::cl::KMeans<TTRAIN, TCLASSIFY>::initialize(const typename te::cl::KMeans<TTRAIN, TCLASSIFY>::Parameters& params) throw(te::cl::Exception)
{
  m_isInitialized = false;

  m_KMeans.clear();
  m_parameters = params;

  if (m_parameters.m_K < 2)
  {
    throw te::cl::Exception(TE_TR("The value of K must be at least 2."));
    return false;
  }
  if (m_parameters.m_maxIterations < 1)
    m_parameters.m_maxIterations = 10;
  if (m_parameters.m_epsilon < 0)
    m_parameters.m_epsilon = 0.0000000001;

  m_isInitialized = true;

  return true;
}

template<class TTRAIN, class TCLASSIFY>
bool te::cl::KMeans<TTRAIN, TCLASSIFY>::train(TTRAIN& itBegin, TTRAIN& itEnd,
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
  TTRAIN it = itBegin;
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

  te::common::TaskProgress task(TE_TR("K-Means algorithm - training step"), te::common::TaskProgress::UNDEFINED, m_parameters.m_maxIterations);
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

template<class TTRAIN, class TCLASSIFY>
bool te::cl::KMeans<TTRAIN, TCLASSIFY>::classify(TCLASSIFY& itBegin, TCLASSIFY& itEnd,
                                                 const std::vector<unsigned int>& attributesIndices,
                                                 std::vector<unsigned int>& classification,
                                                 const bool enableProgressInterface) throw(te::cl::Exception)
{
  TCLASSIFY it = itBegin;
  std::vector<double> values(attributesIndices.size());
  classification.clear();

// count number of elements to be classified  
  unsigned int N = 0;
  while(it != itEnd)
  {
    N++;
    ++it;
  }
  classification.resize(N);

// classify elements
  te::common::TaskProgress task(TE_TR("K-Means algorithm - classifying data"), te::common::TaskProgress::UNDEFINED, N);
  unsigned j = 0;
  it = itBegin;
  while(it != itEnd)
  {
    for (unsigned int i = 0; i < attributesIndices.size(); i++)
      values[i] = (*it)[attributesIndices[i]];
    classification[j++] = getClassification(values);

    ++it;
    task.pulse();
  }

  return true;
}

template<class TTRAIN, class TCLASSIFY>
unsigned int te::cl::KMeans<TTRAIN, TCLASSIFY>::getClassification(std::vector<double> values)
{
  double minDistance = std::numeric_limits<double>::max();
  double distance;
  unsigned int clusterNumber = 0;

  for (unsigned int k = 1; k <= m_parameters.m_K; k++)
  {
    distance = GetEuclideanDistance(m_KMeans[k], values);
    if (distance < minDistance)
    {
      minDistance = distance;
      clusterNumber = k;
    }
  }

  return clusterNumber;
}

#endif // __TERRALIB_CLASSIFICATION_INTERNAL_KMEANS_H

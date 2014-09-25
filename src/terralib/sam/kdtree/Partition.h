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
  \file terralib/sam/kdtree/Partition.h

  \brief Partition algorithms.
*/

#ifndef __TERRALIB_SAM_KDTREE_INTERNAL_PARTITION_H
#define __TERRALIB_SAM_KDTREE_INTERNAL_PARTITION_H

namespace te
{
  namespace sam
  {
    namespace kdtree
    {
      /*!
        \brief This function partition the container in two parts:
               k-1 elements to the left (elements less than or equals to k-th element) and
               the right part with all other elements (elements greater than or equal to k-th element).

        \param A            Container of elements to partition.
        \param kthElement   Position of the k-th element, around the container will be pertitionated.
        \param firstElement Position of the first element.
        \param lastElement  Position of the las element.
        \param compFunctor  Functor to compare elements: implements the function "less than".
      */
      template<class CONTAINER, class COMPFUNCTOR>
      void HoareFind(CONTAINER& A, const std::size_t& kthElement, const std::size_t& firstElement, const std::size_t& lastElement, const COMPFUNCTOR& compFunctor)
      {
        std::size_t m = firstElement;
        std::size_t n = lastElement;

        // Test if the median is in the bounds
        if(kthElement < firstElement || kthElement > lastElement)
          return;

        // Do Container partition
        while(m < n)
        {
          std::size_t i = m;
          std::size_t j = n;

          typename CONTAINER::value_type r = A[kthElement];

          while(i <= j)
          {
            while(compFunctor(A[i], r))
              ++i;

            while(compFunctor(r, A[j]))
              --j;

            if(i <= j)
            {
              typename CONTAINER::value_type w = A[i];

              A[i] = A[j];
              A[j] = w;

                ++i;
              --j;
            }
          }

          if(kthElement <= j) // if the meet point was to the right, so all points above j are greater than the k-th element
            n = j;
          else if(i <= kthElement) // otherwise, if the meeting point was to the left of k-th, so all elements before i are already in the correct location
            m = i;
          else
            break;
        }
      }

      /*!
        \brief This partition the container like a bidimensional K-d Tree using Hoare algorithms.

        \param dataSet Container elements to be sorted like a K-d Tree.
        \param first Position of the first element in container, where the sort will begin.
        \param last Position of the last element, where the sort ends.
        \param level Indicates the axis to begin the sort ('x' or 'y') and is used during the recursion process.
        \param lessThanCompFunctorByX Functor to compare elements along the 'x' axis.
        \param lessThanCompFunctorByY Functor to compare elements along the 'y' axis.

        \note The expected complexity is O(N log N), where N is the number of elements in container.
      */
      template<class CONTAINER, class LESSTHANX, class LESSTHANY>
      void kdsort(CONTAINER& dataSet, const std::size_t& first, const std::size_t& last, const char& level, const LESSTHANX& lessThanCompFunctorByX, const LESSTHANY& lessThanCompFunctorByY)
      {
        const std::size_t kth = (last - first + 1) / 2;

        if(level == 'x')
        {
          // Move data around X axis
          HoareFind(dataSet, first + kth, first, last, lessThanCompFunctorByX);

          // Recursive sort the left half and right half
          if(first + kth > first)
            kdsort(dataSet, first, first + kth - 1, 'y', lessThanCompFunctorByX, lessThanCompFunctorByY);

          if((first + kth) < last)
            kdsort(dataSet, first + kth + 1, last, 'y', lessThanCompFunctorByX, lessThanCompFunctorByY);
        }
        else
        {
          // Move data around Y axis
          HoareFind(dataSet, first + kth, first, last, lessThanCompFunctorByY);

          // Recursive sort the left half and right half
          if(first + kth > first)
            kdsort(dataSet, first, first + kth - 1, 'x', lessThanCompFunctorByX, lessThanCompFunctorByY);

          if(first + kth < last)
            kdsort(dataSet, first + kth + 1, last, 'x', lessThanCompFunctorByX, lessThanCompFunctorByY);
        }
      }

    } // end namespace kdtree
  }   // end namespace sam
}     // end namespace te

#endif  // __TERRALIB_SAM_KDTREE_INTERNAL_PARTITION_H

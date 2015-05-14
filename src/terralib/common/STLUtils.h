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
  \file terralib/common/STLUtils.h

  \brief This file contains several utility functions for dealing with STL containers.

  \ingroup common
*/

#ifndef __TERRALIB_COMMON_INTERNAL_STLUTILS_H
#define __TERRALIB_COMMON_INTERNAL_STLUTILS_H

// STL 
#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace te
{
  namespace common
  {
    /*!
      \brief This function can be applied to a vector of pointers. It will delete each pointer in the vector.

      \param v The vector of pointers to be cleaned.

      \note It will not clear the vector at the end of the cleanup.
    */
    template<class T> inline void FreeContents(const std::vector<T*>& v)
    {
      std::size_t size = v.size();

      for(std::size_t i = 0; i < size; ++i)
        delete (v[i]);
    }

    /*!
      \brief This function can be applied to a list of pointers. It will delete each pointer in the vector.

      \param l The list of pointers to be cleaned.

      \note It will not clear the list at the end of the cleanup.
    */
    template<class T> inline void FreeContents(std::list<T*>& l)
    {
      for(typename std::list<T*>::const_iterator it = l.begin(); it != l.end(); ++it)
        delete (*it);
    }

    /*!
      \brief This function can be applied to a map of pointers. It will delete each pointer in the map.

      \param m The map of pointers to be cleaned.

      \note It will not clear the map at the end of the cleanup.
    */
    template<class K, class V> inline void FreeContents(std::map<K, V*>& m)
    {
      typename std::map<K, V*>::const_iterator it = m.begin();
      typename std::map<K, V*>::const_iterator itend = m.end();

      while(it != itend)
      {
        delete it->second;
        ++it;
      }
    }

    /*!
      \brief This function can be applied to a map of pointers. It will delete each pointer in the map.

      \param m The map of pointers to be cleaned.

      \note It will not clear the map at the end of the cleanup.
    */
    template<class V, class C> inline void FreeContents(std::set<V*, C>& m)
    {
      typename std::set<V*, C>::const_iterator it = m.begin();
      typename std::set<V*, C>::const_iterator itend = m.end();

      while(it != itend)
      {
        delete *it;
        ++it;
      }
    }

    /*!
      \brief This function can be applied to a vector of pointers. It will delete each pointer in the vector.

      \param v The vector of pointers to be cleaned.

      \note It will not clear the vector at the end of the cleanup.
    */
    template<class IT> inline void FreeContents(IT it, IT itend)
    {
      while(it != itend)
      {
        delete *it;
        ++it;
      }
    }

    /*!
      \brief This function can be applied to a pointer to a vector of pointers.
      
      It will delete each pointer in the vector and then delete the pointer to the vector.

      \param v A pointer to a vector of pointers to be cleaned.
    */
    template<class T> inline void Free(std::vector<T*>* v)
    {
      if(v == 0)
        return;

      std::size_t size = v->size();

      for(std::size_t i = 0; i < size; ++i)
        delete ((*v)[i]);

      delete v;
    }

    /*!
      \brief It releases the char array.

      \param carray A NULL terminated array of characters pointers.
    */
    inline void Free(char** carray)
    {
      for(char** aux = carray; *aux; ++aux)
        delete [] (*aux);

      delete [] carray;
    }

    /*!
      \brief It releases the given array.

      \param a The array that will be released.
      \param s The array size.
    */
    template<class T> inline void Free(T** a, std::size_t s)
    {
      for(std::size_t i = 0; i < s; ++i)
        delete [] (a[i]);

      delete [] a;
    }

    /*!
      \brief It finds for a given key in the map and returns a pointer if something is found or NULL otherwise.

      \param m The map of pointers where the key will be searched.
      \param k The key.

      \return A pointer if something is found or NULL otherwise.
    */
    template<class K, class V> inline V* GetPValue(const std::map<K, V*>& m, const K& k)
    {
      typename std::map<K, V*>::const_iterator it = m.find(k);

      if(it != m.end())
        return it->second;

      return 0;
    }

    /*!
      \brief It finds for a given key in the map and returns a pointer if something is found or NULL otherwise.

      \param m The map of pointers where the key will be searched.
      \param k The key.

      \return A pointer if something is found or NULL otherwise.
    */
    template<class K, class C> inline K* GetPValue(const std::set<K*, C>& m, K* k)
    {
      typename std::set<K*, C>::const_iterator it = m.find(k);

      if(it != m.end())
        return *it;

      return 0;
    }

    /*!
      \brief It finds for a given key in the map and returns a pointer if something is found or NULL otherwise.

      \param m The map of pointers where the key will be searched.
      \param k The key.

      \return A pointer if something is found or NULL otherwise.

      \todo Make m parameter be a constant type!
    */
    template<class K, class V> inline V** GetPPValue(std::map<K, V*>& m, const K& k)
    {
      typename std::map<K, V*>::iterator it = m.find(k);

      if(it != m.end())
        return &(it->second);

      return 0;
    }

    /*!
      \brief This function can be applied to a vector of pointers.
      
      It will clone element by element supposing a vector of pointers.

      \param src The source vector with pointers to be cloned.
      \param dst The target vector with cloned pointers.

      \note The type T must implement a method called clone().
    */
    template<class T> inline void Clone(const std::vector<T*>& src, std::vector<T*>& dst)
    {
      std::size_t size = src.size();

      dst.reserve(size);

      for(std::size_t i = 0; i < size; ++i)
        dst.push_back(static_cast<T*>(src[i]->clone()));
    }

    /*!
      \brief This function can be applied to a vector of pointers. It will copy element by element through its copy constructor supposing a vector of pointers.

      \param src The source vector with pointers to be cloned.
      \param dst The target vector with cloned pointers.

      \note The type T must implement the copy constructor method.
    */
    template<class T> inline void Copy(const std::vector<T*>& src, std::vector<T*>& dst)
    {
      std::size_t size = src.size();

      dst.reserve(size);

      for(std::size_t i = 0; i < size; ++i)
        dst.push_back(new T(*src[i]));
    }

    template<class T> inline bool Contains(const std::vector<T>& src, const T& value)
    {
      return std::find(src.begin(), src.end(), value) != src.end();
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_STLUTILS_H


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
  \file terralib/common/ThreadingPolicies.h

  \brief Threading policies that can be applied to TerraLib classes that need some synchronization schema.

  \ingroup common
*/

#ifndef __TERRALIB_COMMON_INTERNAL_THREADINGPOLICIES_H
#define __TERRALIB_COMMON_INTERNAL_THREADINGPOLICIES_H

// BOOST
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#define TE_SINGLETHREADING 1
#define TE_MULTITHREADING  2

//#define TE_THREADING_POLICIES TE_SINGLETHREADING

#if !TE_THREADING_POLICIES
  #define TE_THREADING_POLICIES TE_MULTITHREADING
#endif

#if !defined TE_DEFAULT_MUTEX_POLICY
  #define TE_DEFAULT_MUTEX_POLICY  ::boost::mutex
#endif

#if !defined TE_DEFAULT_LOCK_READING_POLICY
  #define TE_DEFAULT_LOCK_READING_POLICY  ::boost::lock_guard<TE_DEFAULT_MUTEX_POLICY>
#endif

#if !defined TE_DEFAULT_LOCK_WRITING_POLICY
  #define TE_DEFAULT_LOCK_WRITING_POLICY  ::boost::lock_guard<TE_DEFAULT_MUTEX_POLICY>
#endif

namespace te
{
  namespace common
  {
    /*!
      \class NonLockable

      \brief This policy doesn't actually perform a lock.

      Requirements on types:
      <ul>
      <li>NONE. </li>
      </ul>
    */
    template<class Host,
             class MutexPolicy = void,
             class LockReadingPolicy = void,
             class LockWritingPolicy = void> class NonLockable
    {
      public:

        typedef MutexPolicy MutexPolicyType;
        typedef LockReadingPolicy LockReadingPolicyType;
        typedef LockWritingPolicy LockWritingPolicyType;

        /*!
          \class NonLockingPolicy

          \brief This class doesn't truelly acquire a lock over the host object/class.

          Requirements on LockingPolicy type:
          <ul>
          <li>NONE</li>
          </ul>
        */
        template<class LockingPolicy> class NonLockingPolicy
        {
          public:

            /*! \brief Constructor to keep the semantics of class-level lock. */
            explicit NonLockingPolicy()
            {
            }

            /*!
              \brief Constructor.
              
              \param host The host object that uses the lock semantics.
            */
            explicit NonLockingPolicy(const Host&)
            {              
            }

            /*!
              \brief Constructor.
              
              \param host A pointer to the host object that uses the lock semantics.
            */
            explicit NonLockingPolicy(const Host*)
            {
            }

            /*! \brief Destructor. */
            ~NonLockingPolicy()
            {
            }

          private:

            NonLockingPolicy(const NonLockingPolicy&);
            NonLockingPolicy& operator=(const NonLockingPolicy&);
        };

        typedef volatile Host VolatileType;
        typedef NonLockingPolicy<LockReadingPolicy> LockRead;
        typedef NonLockingPolicy<LockWritingPolicy> LockWrite;
    };

#if TE_THREADING_POLICIES == TE_MULTITHREADING

    /*!
      \class ObjectLevelLockable

      \brief This policy assures an object-level locking scheme for a derived class.

      Requirements on types:
      <ul>
      <li>Host: is the type of the derived class from this policy. </li>
      <li>MutexPolicy: any type that have a lock and unlock method. When using boost it can be one of the follwoing: boost::mutex, bost::timed_mutex, bost::recursive_mutex, boost::recursive_timed_mutex or boost::shared_mutex.</li>
      <li>The LockReadingPolicy and LockWritingPolicy may be: boost::lock_guard, boost::unique_lock, boost::shared_lock, boost::upgrade_lock, boost::upgrade_to_unique_lock</li>
      </ul>
    */
    template<class Host,
             class MutexPolicy = TE_DEFAULT_MUTEX_POLICY,
             class LockReadingPolicy = TE_DEFAULT_LOCK_READING_POLICY,
             class LockWritingPolicy = TE_DEFAULT_LOCK_WRITING_POLICY> class ObjectLevelLockable
    {
      public:

        /*!
          \class ObjectLevelLockingPolicy

          \brief This class acquires a lock over the host object.

          Requirements on type LockingPolicy:
          <ul>
          <li>it must be one of: boost::lock_guard, boost::unique_lock, boost::shared_lock, boost::upgrade_lock, boost::upgrade_to_unique_lock</li>
          </ul>
        */
        template<class LockingPolicy> class ObjectLevelLockingPolicy
        {
          public:

            /*!
              \brief Through this constructor the host object will be locked.
              
              \param host The host object to be locked.
            */
            explicit ObjectLevelLockingPolicy(const Host& host)
              : m_lock(host.m_mtx)
            {              
            }

            /*!
              \brief Through this constructor the pointed host object will be locked.
              
              \param host A pointer to the host object to be locked.
            */
            explicit ObjectLevelLockingPolicy(const Host* host)
              :  m_lock(host->m_mtx)
            {
            }

            /*! \brief Destructor. */
            ~ObjectLevelLockingPolicy()
            {
            }

          private:

            ObjectLevelLockingPolicy(const ObjectLevelLockingPolicy&);
            ObjectLevelLockingPolicy& operator=(const ObjectLevelLockingPolicy&);

          private:

            LockingPolicy m_lock; //!< lock policy.
        };

        typedef volatile Host VolatileType;
        typedef ObjectLevelLockingPolicy<LockReadingPolicy> LockRead;
        typedef ObjectLevelLockingPolicy<LockWritingPolicy> LockWrite;

        friend class ObjectLevelLockingPolicy<LockReadingPolicy>;
        friend class ObjectLevelLockingPolicy<LockWritingPolicy>;

      protected:

        /*! \brief Default constructor. */
        ObjectLevelLockable()
          : m_mtx()
        {
        }

        /*! \brief Copy constructor. */
        ObjectLevelLockable(const ObjectLevelLockable&)
          : m_mtx()
        {
        }

        /*! \brief Destructor. */
        ~ObjectLevelLockable()
        {
        }

      private:

        mutable MutexPolicy m_mtx;  //!< The mutex used to control the object lock.
    };

    /*!
      \class ClassLevelLockable

      \brief This policy assures a class-level locking scheme for a derived class.

      Requirements on types:
      <ul>
      <li>Host: is the type of the derived class from this policy. </li>
      <li>MutexPolicy: any type that have a lock and unlock method. When using boost it can be one of the follwoing: boost::mutex, bost::timed_mutex, bost::recursive_mutex, boost::recursive_timed_mutex or boost::shared_mutex.</li>
      <li>The LockReadingPolicy and LockWritingPolicy may be: boost::lock_guard, boost::unique_lock, boost::shared_lock, boost::upgrade_lock, boost::upgrade_to_unique_lock</li>
      </ul>
    */
    template<class Host,
             class MutexPolicy = TE_DEFAULT_MUTEX_POLICY,
             class LockReadingPolicy = TE_DEFAULT_LOCK_READING_POLICY,
             class LockWritingPolicy = TE_DEFAULT_LOCK_WRITING_POLICY> class ClassLevelLockable
    {
      public:

        /*!
          \class ClassLevelLockingPolicy

          \brief This class acquires a lock over the object class.

          Requirements on type LockingPolicy:
          <ul>
          <li>it must be one of: boost::lock_guard, boost::unique_lock, boost::shared_lock, boost::upgrade_lock, boost::upgrade_to_unique_lock</li>
          </ul>
        */
        template<class LockingPolicy> class ClassLevelLockingPolicy : public boost::noncopyable
        {
          public:

            /*! \brief Default constructor. */
            explicit ClassLevelLockingPolicy()
              : m_lock(Host::sm_mtx)
            {
            }

            /*! \brief Constructor to keep the semantics of object-level lock. */
            explicit ClassLevelLockingPolicy(const Host& host)
              : m_lock(Host::sm_mtx)
            {              
            }

            /*! \brief Constructor to keep the semantics of object-level lock. */
            explicit ClassLevelLockingPolicy(const Host* host)
              : m_lock(Host::sm_mtx)
            {
            }

            /*! \brief Destructor. */
            ~ClassLevelLockingPolicy()
            {
            }

          private:

            ClassLevelLockingPolicy(const ClassLevelLockingPolicy&);
            ClassLevelLockingPolicy& operator=(const ClassLevelLockingPolicy&);

          private:

            LockingPolicy m_lock; //!< lock policy.
        };

        typedef volatile Host VolatileType;
        typedef ClassLevelLockingPolicy<LockReadingPolicy> LockRead;
        typedef ClassLevelLockingPolicy<LockWritingPolicy> LockWrite;

        friend class ClassLevelLockingPolicy<LockReadingPolicy>;
        friend class ClassLevelLockingPolicy<LockWritingPolicy>;

      private:

        static MutexPolicy sm_mtx;  //!< The mutex used to control the class lock.        
    };

    template<class Host, class MutexPolicy, class LockReadingPolicy, class LockWritingPolicy> MutexPolicy ClassLevelLockable<Host, MutexPolicy, LockReadingPolicy, LockWritingPolicy>::sm_mtx;

#else // TE_THREADING_POLICIES == TE_SINGLETHREADING
  template<class Host,
           class MutexPolicy = void,
           class LockReadingPolicy = void,
           class LockWritingPolicy = void> class ObjectLevelLockable : public NonLockable<ObjectLevelLockable<Host, MutexPolicy, LockReadingPolicy, LockWritingPolicy>, MutexPolicy, LockReadingPolicy, LockWritingPolicy>
  {
  };

  template<class Host,
           class MutexPolicy = TE_DEFAULT_MUTEX_POLICY,
           class LockReadingPolicy = TE_DEFAULT_LOCK_READING_POLICY,
           class LockWritingPolicy = TE_DEFAULT_LOCK_WRITING_POLICY> class ClassLevelLockable : public NonLockable<ClassLevelLockable<Host, MutexPolicy, LockReadingPolicy, LockWritingPolicy>, MutexPolicy, LockReadingPolicy, LockWritingPolicy>
  {
  };

#endif // TE_THREADING_POLICIES == TE_MULTITHREADING

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_THREADINGPOLICIES_H

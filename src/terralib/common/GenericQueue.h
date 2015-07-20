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
  \file terralib/common/GenericQueue.h

  \brief Implements a generic queue.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_GENERICQUEUE_H
#define __TERRALIB_COMMON_INTERNAL_GENERICQUEUE_H

// TerraLib
#include "Exception.h"

namespace te
{
  namespace common
  {
    /*!
    \struct NodeT

    \brief Struct that represents a node in the Queue.

    \tparam T Data type of the data stored in a node.
    */
    template <typename T>
    struct NodeT
    {
      T m_value;              //!< Stored value.
      NodeT<T>* m_next;       //!< Pointer to the next node.

      /*!
      \brief Constructor.
      \param v The value to be stored.
      */

      NodeT(T v)
      {
        m_value = v;          // Stores the value.
        m_next = 0;           // Sets the next null;
      }

      /*!
      \brief Destructor.
      */
      ~NodeT()
      {
        delete m_next;
      }
    };

    /*!
    \struct QueueT

    \brief Struct that implements the generic queue.

    Queues have well known behavior: first in, first out.

    \tparam T Type of the data to be stored.

    \note The queue DOES NOT takes the ownership of the pointers stored in it.
    */
    template <typename T>
    struct QueueT
    {
      NodeT<T>* m_first;       //!< Pointer to the first node.
      NodeT<T>* m_last;        //!< Pointer to the last node.
      unsigned int m_size;     //!< Number of elements contained in the queue.

      /*!
      \brief Constructor.
      */
      QueueT()
      {
        m_first = 0;        // Sets first pointer to null.
        m_last = m_first;   // Sets last pointer to null.
        m_size = 0;         // Sets the size to 0.
      }

      /*!
      \brief Destructor.
      */
      ~QueueT()
      {
        while(m_size > 0)   // Do while queue not empty.
          remove(0);         // Removes the first element.
      }

      /*!
      \brief Adds an element to the end of the queue.

      \param v Element to be stored.
      */
      void add(const T& v)
      {
        NodeT<T>* novo = new NodeT<T>(v);

        if(m_first == 0)   // Fila vazia.
        {
          m_first = novo;      // Aponta o primeiro para novo.
          m_last = m_first;  // Aponta o último para o primeiro.
        }
        else                     // Fila não vazia.
        {
          m_last->m_next = novo;  // Aponta o próximo do último para o novo.
          m_last = novo;             // Aponta o último para o novo.
        }

        m_size++;
      }

      /*!
      * \brief Removes the element at position \a pos.
      *
      * \param pos The position of the element in the queue.
      *
      * \return The elemen stored at position \a pos.
      *
      * \exception If \a pos is greater or equals than the queue size, a te::common::Exception will be raised.
      */
      T remove(const unsigned int& pos)
      {
        if(pos >= m_size)
          throw Exception("Queue remove error: index out of boundaries.");

        T res;
        NodeT<T>* aux = m_first;

        if(pos == 0) // Remove first
        {
          m_first = m_first->m_next;
          res = aux->m_value;
          aux->m_next = 0;
          delete aux;
        }
        else
        {
          for(unsigned int i = 0; i < pos - 1; i++)
            aux = aux->m_next;

          NodeT<T>* aux2 = aux->m_next;

          if(aux2->m_next == 0) // Remove last
          {
            aux->m_next = 0;
            m_last = aux;
            res = aux2->m_value;
            delete aux2;
          }
          else
          {
            // Remove in the midle
            aux->m_next = aux2->m_next;
            aux2->m_next = 0;
            res = aux2->m_value;
            delete aux2;
          }
        }

        m_size--;

        return res;
      }

      /*!
      * \brief Inserts a data at a specific position.
      *
      * \param v Value to be stored.
      *
      * \param pos Position where to put it.
      *
      * \exception If \a pos were greater than the queue size a te::common::Exception will be raised.
      */
      void insert(const T& v, const unsigned int& pos)
      {
        if(pos > m_size)
          throw Exception("Queue insert error: index out of boundaries.");

        NodeT<T>* novo = new NodeT<T>(v);

        if(pos == 0) // insert at the beggining
        {
          novo->m_next = m_first;
          m_first = novo;

          if(m_size == 0)
            m_last = m_first;
        }
        else if(pos == m_size) // insert at the end
        {
          m_last->m_next = novo;
          m_last = novo;
        }
        else
        {
          NodeT<T>* aux = m_first; // Insert between two nodes

          for(unsigned int i = 0; i < pos - 1; i++)
            aux = aux->m_next;

          NodeT<T>* aux2 = aux->m_next;

          aux->m_next = novo;
          novo->m_next = aux2;
        }

        m_size++;
      }

      /*!
      * \brief Returns the value stored at \pos position of the queue.
      *
      * \param pos The required position.
      *
      * \return The value stored at required position.
      *
      * \exception If \a pos were greater or equal than queue size a te::common::Exception will be raised.
      */
      T getValue(const unsigned int& pos) const
      {
        if(pos >= m_size)
          throw Exception("Queue getValue error: index out of boundaries.");

        NodeT<T>* aux = m_first;

        for(unsigned int i = 0; i < pos; i++)
          aux = aux->m_next;

        return aux->m_value;
      }

      /*!
      * \brief Return the position of a value in the queue.
      *
      * \param v The value we are searching for.
      *
      * \return The position of the value in the queue.
      *
      * \exception If queue were empty or does not contains the \a v, a te::common::Exception will be raised.
      */
      unsigned int position(const T& v) const
      {
        if(m_size > 0)
        {
          NodeT<T>* aux = m_first;
          unsigned int count = 0;

          while(aux != 0)
          {
            if(aux->m_value == v)
              return count;

            count++;
            aux = aux->m_next;
          }
        }

        throw Exception("Queue does not contains data.");
      }

      /*!
      * \brief Returns the size of the queue.
      *
      * \return The number of items stored at the queue.
      */
      unsigned int getSize() const
      {
        return m_size;
      }
    };
  }
}

#endif  // __TERRALIB_COMMON_INTERNAL_GENERICQUEUE_H

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
  \file terralib/common/URL.h

  \brief A helper class for URL encode/decode.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_URL_H
#define __TERRALIB_COMMON_INTERNAL_URL_H

// TerraLib
#include "Config.h"

// STL 
#include <cassert>
#include <string>

// uripp - MIT
#include "URI/uri.h"

namespace te
{
  namespace common
  {
    /*!
      \class URL

      \brief A class URL represents a Uniform Resource Locator, 
      a pointer to a "resource" on the World Wide Web. A resource 
      can be something as simple as a file or a directory, or it 
      can be a reference to a more complicated object, such as a 
      query to a database or to a search engine. This class is 
      based on the MIT code uripp http://uripp.sourceforge.net/ .

      \ingroup common
    */
    class TECOMMONEXPORT URL
    {
      public:
        typedef uri::query::iterator  queryIterator; ///< query iterator type
        typedef uri::query::const_iterator  const_queryIterator; ///< query const iterator type
        typedef uri::path::const_iterator   const_pathIterator;  ///< path const iterator type
    
        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Default constructor of a new URL. */
        URL();

        /*!
          \brief It initializes a new URL with the given valid string url.

          \param s The valid encoded string url.
         */
        URL(const std::string& s);

        /*! \brief Destructor */
        ~URL();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns URL string.

          \return The protocol.
         */
        std::string getString();

        /*!
          \brief It returns encoded URL string.

          \return The protocol.
         */
        std::string getEncodedString();

        /*!
          \brief It sets the encoded URL string.

          \return The protocol.
         */
         void setEncodedString(const std::string&);

        /*!
          \brief It returns the protocol.

          \return The protocol.
         */
        const std::string& getProtocol() const;

        /*!
          \brief It sets the protocol.

          \param s The protocol
         */
        void setProtocol(const std::string& s);

        /*!
          \brief It returns the host.

          \return The host.
         */
        const std::string& getHost() const;

        /*!
          \brief It sets the host.

          \param s The host.
         */
        void setHost(const std::string& s);

        /*!
          \brief It returns the host type.

          \return The host type.
          \note it can be: null, DOMAIN_NAME, IP_ADDRESS or IPV6_ADDRESS
         */
        uri::authority::host_type_e getHostType() const;

        /*!
          \brief It returns the host port.

          \return The host port.
         */
        unsigned short getHostPort() const;

        /*!
          \brief It sets the host port.

          \param p The host port.
         */
        void setHostPort(unsigned short p);

        /*!
          \brief It returns the login.

          \return The login.
         */
        std::string getLogin() const;

        /*!
          \brief It sets the login.

          \param s The login.
         */
        void setLogin(const std::string& s);

        /*!
          \brief It returns the password.

          \return The login.
         */
        std::string getPassword() const;

        /*!
          \brief It sets the password.

          \param s The login.
         */
        void setPassword(const std::string& s);

        /*!
          \brief It returns the path.

          \return The path string.
         */
        std::string getPathString() const;

        /*!
          \brief It returns the encoded path.

          \return The encoded path string.
         */
        std::string getEncodedPathString() const;

        /*!
          \brief It sets the encoded path.

          \param path The encoded path string.
         */
        void setEncodedPathString(const std::string& path);

        /*!
          \brief It cleans the path.

         */
        void clearPath();

        /*!
          \brief It adds the path.

          \param p The path string to be added.
          \note The input path string can not be encoded.
         */
        void addPath(const std::string& p);

        /*!
          \brief It Checks if the path is absolute.

          \return True if the path is absolute, false otherwise.
         */
        bool isAbsolutePath() const;

        /*!
          \brief It sets the absolute path propertie.

          \param v True for absolute path, false otherwise.
         */
        void setIsAbsolutePath(bool v);

        /*!
          \brief It Checks if the path is directory.

          \return True if the path is directory, false otherwise.
         */
        bool isDirectoryPath() const;

        /*!
          \brief It sets the directory path propertie.

          \param v True for directory path, false otherwise.
         */
        void setIsDirectoryPath(bool v);

        /*!
          \brief It Checks if the path is empty.

          \return True if the path is empty, false otherwise.
         */
        bool isEmptyPath() const;

        /*!
          \brief It returns the begin const path iterator.

          \return The begin const path iterator.
         */
        const_pathIterator beginPath() const;

        /*!
          \brief It returns the end const path iterator.

          \return The end const path iterator.
         */
        const_pathIterator endPath() const;

        /*!
          \brief It returns the query string.

          \return The query string.
         */
        std::string getQueryString();

        /*!
          \brief It returns the encoded query.

          \return The encoded query string.
         */
        std::string getEncodedQueryString() const;

        /*!
          \brief It sets the encoded query.

          \param query The encoded query string.
         */
        void setEncodedQueryString(const std::string& query);

        /*!
          \brief It returns the query size.

          \return The query size.
         */
        int getQuerySize() const;

        /*!
          \brief It cleans the vector of queries.

         */
        void clearQuery();

        /*!
          \brief It cleans the vector of queries.
          
          \param query The query string.
          \note The input query string can not be encoded.
         */
        void addQuery(const std::string& key, const std::string& value);

        /*!
          \brief It returns the begin query iterator.

          \return The begin query iterator.
         */
        queryIterator beginQuery();

        /*!
          \brief It returns the end query iterator.

          \return The end query iterator.
         */
        queryIterator endQuery();

        /*!
          \brief It returns the begin const query iterator.

          \return The begin const query iterator.
         */
        const_queryIterator beginQuery() const;

        /*!
          \brief It returns the end const query iterator.

          \return The end const query iterator.
         */
        const_queryIterator endQuery()const;

        /*!
          \brief it finds the key and return query iterator.
          \param key The key to be found.

          \return The query iterator.
         */
        te::common::URL::queryIterator findQueryKey(const std::string& key);

        /*!
          \brief it finds the key and return const query iterator.
          \param key The key to be found.

          \return The const query iterator.
         */
        te::common::URL::const_queryIterator findQueryKey(const std::string& key) const;

        /*!
          \brief It returns the fragment.

          \return The fragment string.
         */
        std::string getFragmentString() const;

        /*!
          \brief It returns encoded the fragment.

          \return The encoded fragment string.
         */
        std::string getEncodedFragmentString() const;

        /*!
          \brief It sets the fragment.

          \param f The fragment string.
         */
        void setFragmentString(const std::string& f);

        /*!
          \brief It sets the encoded fragment.

          \param f The encoded fragment string.
         */
        void setEncodedFragmentString(const std::string& f);

        //@}

      private:

        uri::uri m_uri;
    };
  }   // end namespace common
}     // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_URL_H


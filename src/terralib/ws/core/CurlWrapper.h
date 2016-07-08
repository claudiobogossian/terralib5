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
  \file terralib/ws/core/CurlWrapper.h

  \brief A Wrapper for Lib Curl.

  \author Emerson Moraes
 */

#ifndef __TERRALIB_WS_CORE_CURLWRAPPER_H
#define __TERRALIB_WS_CORE_CURLWRAPPER_H

// LibCurl
#include <curl/curl.h>

//STL
#include <iostream>
#include <memory>

namespace te
{
  namespace ws
  {
    namespace core
    {
    /*!
      \class CurlWrapper

      \brief An abstraction to Lib Curl functions.
    */
    class CurlWrapper
    {
    public:
      CurlWrapper();

      virtual ~CurlWrapper();


//      CurlWrapper(CurlWrapper&& other) = delete;

//      CurlWrapper& operator=(CurlWrapper&& other) = delete;

//      CurlWrapper(CurlWrapper& curl) = delete;
//      CurlWrapper& operator=(const CurlWrapper& other) = delete;

      /*!
        \brief Method to download a file retrieved from given URL and save into the specified file path.

        \param url URL that the file will be downloaded.
        \param filePath file path where downloaded file will be saved.
      */
      virtual void downloadFile(const std::string& url, const std::string& filePath) const;

      /*!
        \brief Method to download a file retrieved from given URL and save into the specified File.

        \param url URL that the file will be downloaded.
        \param file File where downloaded file will be write.
      */
      virtual void downloadFile(const std::string& url, std::FILE* file) const;

    private:
      std::shared_ptr<CURL>  m_curl;
    };

    }
  }
}

#endif

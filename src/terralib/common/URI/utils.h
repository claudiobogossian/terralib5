// Copyright zooml.com 2008 All rights reserved.
#ifndef __TERRALIB_COMMON_INTERNAL_URI_UTILS_H
#define __TERRALIB_COMMON_INTERNAL_URI_UTILS_H
#include "../Config.h"
#include <string>
#include <stack>
#include <cstddef>
namespace te
{
  namespace common
  {
    namespace uri
    {
      std::string TECOMMONEXPORT convert(bool v); ///< bool to string, true="1", false="0"
      std::string TECOMMONEXPORT convert(int v); ///< int to string
      std::string TECOMMONEXPORT convert(unsigned int v); ///< uint to string
      std::string TECOMMONEXPORT convert(long int v); ///< long int to string
      std::string TECOMMONEXPORT convert(unsigned long int v); ///< unsigned long int to string
      std::string TECOMMONEXPORT convert(long long int v); ///< long long int to string
      std::string TECOMMONEXPORT convert(unsigned long long int v); ///< unsigned long long int to string
  #	ifndef _WIN32
  //    std::string TECOMMONEXPORT convert(std::ptrdiff_t v); ///< ptrdiff_t to string
  #	endif
      std::string TECOMMONEXPORT convert(double v); ///< double to string
      std::string TECOMMONEXPORT convert(const std::string& v); ///< string to trimmed string
      inline std::string convert(const char* v) {return v;} ///< cstring to string
      /// String to bool, returning true if set, which is when
      /// the string is not empty or all isspace.
      /// isspace before and/or after value OK.
      /// The following, and their uppercase, qualify as true:
      /// 1, 't', "true", 'y', "yes", "on". Their complements are
      /// considered false, and all else error.
      /// @exception std::invalid_argument on conversion error
      bool TECOMMONEXPORT convert(const std::string& s, bool& v);
      /// String to int, returning true if set, which is when
      /// the string is not empty or all isspace.
      /// isspace before and/or after value OK.
      /// @exception std::invalid_argument on conversion error
      bool TECOMMONEXPORT convert(const std::string& s, int& v);
      /// String to unsigned int, returning true if set, which
      /// is when the string is not empty or all isspace.
      /// isspace before and/or after value OK.
      /// @exception std::invalid_argument on conversion error
      bool TECOMMONEXPORT convert(const std::string& s, unsigned int& v);
      /// String to size_t, returning true if set, which
      /// is when the string is not empty or all isspace.
      /// isspace before and/or after value OK.
      /// @exception std::invalid_argument on conversion error
      bool TECOMMONEXPORT convert(const std::string& s, unsigned long int& v);
      /// String to long long int, returning true if set, which
      /// is when the string is not empty or all isspace.
      /// isspace before and/or after value OK.
      /// @exception std::invalid_argument on conversion error
      bool TECOMMONEXPORT convert(const std::string& s, long long int& v);
      /// String to unsigned long long int, returning true if set,
      /// which is when the string is not empty or all isspace.
      /// isspace before and/or after value OK.
      /// @exception std::invalid_argument on conversion error
      bool TECOMMONEXPORT convert(const std::string& s, unsigned long long int& v);
      /// String to double, returning true if set, which is
      /// when the string is not empty or all isspace.
      /// isspace before and/or after value OK.
      /// @exception std::invalid_argument on conversion error
      bool TECOMMONEXPORT convert(const std::string& s, double& v);
      /// String to trimmed string, returning true if set, which
      /// is when the string is not empty or all isspace.
      bool TECOMMONEXPORT convert(const std::string& s, std::string& v);
      bool TECOMMONEXPORT isspaces(const char* s); ///< Test if string is empty or all isspace.
      /// Parse hex chars at pos, returning success, and set the char and advance
      /// first on success.
      bool TECOMMONEXPORT parse_hex(const std::string& s, size_t pos, char& chr);
      /// Convert the char v to hex and add the 2 chars to the end of s.
      void TECOMMONEXPORT append_hex(char v, std::string& s);
    }
  }
}
#endif

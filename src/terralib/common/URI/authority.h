/*
Copyright (c) 2009 zooml.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef __TERRALIB_COMMON_INTERNAL_AUTHORITY_H
#define __TERRALIB_COMMON_INTERNAL_AUTHORITY_H
#include "../Config.h"
#include <string>
#include <map> // Added by Lauro
#include <iostream>
namespace te
{
  namespace common
  {
    namespace uri
    {
      /** \brief URI authority component (without userinfo).
       *
       * The authority string and the type of the authority is stored in this
       * object. The syntax is checked with the corresponding
       * type. For example if this indicates \c DOMAIN_NAME then
       * the string can be parsed as a \c domain_name object.
       *
       * Syntax: This does not support userinfo nor IPvFuture.
       * Requires domain-name instead of reg-name.
       * <pre>
       * authority  = host [ ":" port ]
       * host       = IP-literal | IPv4address | domain-name
       * IP-literal = "[" IPv6address "]"
       * port       = *DIGIT
       * </pre> */
      class TECOMMONEXPORT authority {
      public:
          /// Enumeration.
          enum host_type_e {
              null,
              DOMAIN_NAME, ///< domain name
              IP_ADDRESS, ///< IP address
              IPV6_ADDRESS ///< IP v6 address
          };
          authority(); ///< Construct.
          /// Construct from string. This test to be sure host
          /// is of a valid form (see class description).
          /// @exception std::invalid_argument if invalid or empty
          authority(const std::string& v);
          bool empty() const {return is_null();} ///< Test if null/empty.
          bool is_null() const {return host_.empty();} ///< Test if null/empty.
          const std::string& host() const {return host_;} ///< Get host.
          host_type_e host_type() const {return host_type_;} ///< Get host type.
          unsigned short port() const {return port_;} ///< Get port (0 if none).
          void port(unsigned short p) {port_ = p;} ///< Set port. Added by Lauro
          void setDefaultPort(const std::string& scheme);///< Set Default port for the protocol. Added by Lauro
          int getDefaultPort(const std::string& scheme);///< Get Default port for the protocol. Added by Lauro
          void setLogin(const std::string& login);///< Set login. Added by Lauro
          std::string getLogin() const;///< Get login. Added by Lauro
          std::string getEncodedLogin() const;///< Get endoded login. Added by Lauro
          void setPassword(const std::string& password);///< Set password. Added by Lauro
          std::string getPassword() const;///< Get password. Added by Lauro
          std::string getEncodedPassword() const;///< Get encoded password. Added by Lauro
          std::string string() const; ///< Calculate string.
          std::ostream& operator <<(std::ostream& os) const; ///< Stream out.
          static const char IP_LITERAL_BEGIN_CHAR; ///< IP literal begin ('[')
          static const char IP_LITERAL_END_CHAR; ///< IP literal end (']')
          static const char PORT_SEPARATOR_CHAR; ///< port separator (':')
      private:
          friend bool TECOMMONEXPORT parse(std::string::const_iterator& first, std::string::const_iterator last, authority& v);
          void loadDefaultPorts(); // Added by Lauro
          host_type_e host_type_;
          std::string host_;
          unsigned short port_;
          std::string login_;
          std::string password_;
          
          /// Protocol to default port. Added by Lauro
          static std::map<std::string, unsigned short> protocol2DefaultPort_;
      };
      /// Stream out URI authority.
      inline std::ostream& operator <<(std::ostream& os, const authority& v) {return v.operator <<(os);}
      /// Parse URI authority, returning whether found or not and advancing
      /// first and setting authority if found. Does not skip leading space.
      bool TECOMMONEXPORT parse(std::string::const_iterator& first, std::string::const_iterator last, authority& v);
      bool parseLoginPassword(std::string::const_iterator& first, std::string::const_iterator last, authority& v); // Added by Lauro
      bool decodeAndConcate(std::string::const_iterator& it, std::string& s); // Added by Lauro
      bool encodeAndConcate(std::string::const_iterator& it, std::string& s); // Added by Lauro
    }
  }
}
#endif

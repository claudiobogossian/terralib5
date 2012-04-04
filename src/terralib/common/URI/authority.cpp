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
#include "authority.h"
#include "ip_address.h"
#include "ipv6_address.h"
#include "domain_name.h"
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <cstdio>

namespace te
{
  namespace common
  {
    namespace uri
    {
      std::map<std::string, unsigned short> authority::protocol2DefaultPort_; // Added by Lauro

      const char authority::IP_LITERAL_BEGIN_CHAR = '[';
      const char authority::IP_LITERAL_END_CHAR = ']';
      const char authority::PORT_SEPARATOR_CHAR = ':';
      authority::authority() : port_(0) {loadDefaultPorts();}
      authority::authority(const std::string& v) : port_(0) {
          loadDefaultPorts();
          std::string::const_iterator first = v.begin();
          if (!parse(first, v.end(), *this) || first != v.end())
              throw std::invalid_argument("invalid URI authority: \"" + v + "\"");
      }
      void authority::loadDefaultPorts() {
          if(protocol2DefaultPort_.empty()) {
              char buf[1000];
              unsigned short v, i, ii;
              std::string s, protocol;
              FILE* fp = fopen(TE_PROTOCOL_DEFAULT_PORTS_FILE, "r"); // path to be used in release mode
              if(fp == (FILE*)null) // path to be used in debugger mode
                fp = fopen("../../../../src/terralib/common/URI/protocolPorts.txt", "r");

              while(fgets(buf, 1000, fp) != (char*)null) {
                  s = buf;
                  if(s.find("Port Assignments:") != std::string::npos)
                      break;
              }

              while(fgets(buf, 1000, fp) != (char*)null)
              {
                  if(buf[0] == '#' || buf[0] == '\n' || buf[0] == ' ' || buf[0] == '\t')
                      continue;
                  s = buf;
                  if(s.find("/") == std::string::npos)
                      continue;

                  i = 0;
                  while(!(s[i] == ' ' || s[i] == '\t'))
                      i++;
                  protocol = s.substr(0, i);
                  while(s[i] == ' ' || s[i] == '\t')
                      i++;
                  ii = i;
                  while(s[i] != '/')
                      i++;
                  v = (unsigned short)atoi(s.substr(ii, i-ii).c_str());
                  protocol2DefaultPort_[protocol] = v;
              }
              fclose(fp);
          }
      }
      void authority::setDefaultPort(const std::string& scheme) {
          if(protocol2DefaultPort_.find(scheme) == protocol2DefaultPort_.end())
              port_ = 0;
          else
              port_ = protocol2DefaultPort_[scheme];
      }
      int authority::getDefaultPort(const std::string& scheme) {
          if(protocol2DefaultPort_.find(scheme) == protocol2DefaultPort_.end())
              return 0;
          else
              return protocol2DefaultPort_[scheme];
      }
      void authority::setLogin(const std::string& login) {
          login_ = login;
      }
      std::string authority::getLogin() const{ // Lauro
          return login_;
      }
      std::string authority::getEncodedLogin() const{ // Lauro
          std::string s;
          std::string::const_iterator it = login_.begin();
          while(it != login_.end())
          {
              char c = *it;
              if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                  s += c;
              else
                  encodeAndConcate(it, s);
              it++;
          }
          return s;
      }
      void authority::setPassword(const std::string& password) { // Lauro
          password_ = password;
      }
      std::string authority::getPassword() const{ // Lauro
          return password_;
      }
      std::string authority::getEncodedPassword() const{ // Lauro
          std::string s;
          std::string::const_iterator it = password_.begin();
          while(it != password_.end())
          {
              char c = *it;
              if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                  s += c;
              else
                  encodeAndConcate(it, s);
              it++;
          }
          return s;
      }
      std::string authority::string() const { // Lauro
          std::ostringstream oss;
          operator <<(oss);
          return oss.str();
      }
      std::ostream& authority::operator <<(std::ostream& os) const { // Lauro
          if(login_.empty() == false)
              os << login_;
          if(password_.empty() == false)
              os << ":" << password_;
          if(login_.empty() == false)
              os << "@";
          os << host_;
          if (port_)
              os << PORT_SEPARATOR_CHAR << port_;
          return os;
      }
      bool parse(std::string::const_iterator& first, std::string::const_iterator last, authority& v) {
          parseLoginPassword(first, last, v);
          std::string::const_iterator f = first;
          authority::host_type_e e;
          // Just parse to get the end.
          // Note string is not put in canonical form.
          ip_address ipaddr;
          if (parse(f, last, ipaddr)) // IP address.
              e = authority::IP_ADDRESS;
          else {
              domain_name dn;
              if (parse(f, last, dn)) // Domain name.
                  e = authority::DOMAIN_NAME;
              else {
                  if (f == last || *f != authority::IP_LITERAL_BEGIN_CHAR)
                      return false;
                  ++f;
                  ipv6_address ipv6addr;
                  if (!parse(f, last, ipv6addr)) // Not IP v6 address.
                      return false;
                  if (f == last || *f != authority::IP_LITERAL_END_CHAR)
                      return false;
                  e = authority::IPV6_ADDRESS;
                  ++f;
              }
          }
          std::string host(first, f);
          unsigned int port = 0;
          if (f != last && *f == authority::PORT_SEPARATOR_CHAR) {
              ++f;
              std::string::const_iterator anchor = f;
              for (; f != last; ++f) {
                  char c = *f;
                  if (c < '0' || '9' < c)
                      break;
                  else {
                      port = port * 10 + (c - '0');
                      if (65535 < port)
                          return false; // Port out of range.
                  }
              }
              if (f == anchor)
                  return false; // Separator but no digits.
          }
          v.host_type_ = e;
          v.host_ = host;
          v.port_ = (unsigned short)port;
          first = f;
          return true;
      }

      bool parseLoginPassword(std::string::const_iterator& first, std::string::const_iterator last, authority& v) { // Lauro
          std::string::const_iterator f = first;
          std::string login, password;

          while(f != last && *f != '/')
          {
              if(*f == '@')
                break;
              f++;
          }
          if(!(f == last || *f == '/'))
          {
              f = first;
              while(*f != '@')
              {
                  if(*f != ':')
                  {
                      if(*f != '%')
                          login += *f;
                      else
                      {
                          if(decodeAndConcate(f, login) == false)
                            return false;
                      }
                  }
                  else
                  {
                      f++;
                      while(*f != '@')
                      {
                          if(*f != '%')
                              password += *f;
                          else
                          {
                              if(decodeAndConcate(f, password) == false)
                                  return false;
                          }
                          f++;
                      }
                      f++;
                      first = f;
                      break;
                  }
                  f++;
              }

              if(login.empty() == false)
                v.setLogin(login);
              if(password.empty() == false)
                v.setPassword(password);
          }
          return true;
      }

      bool decodeAndConcate(std::string::const_iterator& f, std::string& s) // Lauro
      {
          char c, c1, c2;
          f++;
          if(*f >= '0' && *f <= '9')
              c1 = *f - 0x30;
          else if(*f >= 'a' && *f <= 'f')
              c1 = *f - 0x57;
          else if(*f >= 'A' && *f <= 'F')
              c1 = *f - 0x37;
          else
              return false;
          f++;
          if(*f >= '0' && *f <= '9')
              c2 = *f - 0x30;
          else if(*f >= 'a' && *f <= 'f')
              c2 = *f - 0x57;
          else if(*f >= 'A' && *f <= 'F')
              c2 = *f - 0x37;
          else
              return false;
          c = (c1 << 4) | c2;
          s += c;
          return true;
      }

      bool encodeAndConcate(std::string::const_iterator& f, std::string& s) // Lauro
      {
          char buf[3];
          unsigned char c = *f;
          s += '%';
          sprintf(buf, "%x", c);
          buf[2] = 0;
          s += buf;
          return true;
      }
    }
  }
}

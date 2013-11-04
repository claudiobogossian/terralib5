#ifndef __MODISDATABASE_H__
#define __MODISDATABASE_H__
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <boost/regex.hpp>

void MSearch(const boost::filesystem::path& mpath, std::vector<std::string>& fnames);

struct MODISRecord
{
  MODISRecord():
  filen(""),
  product(""),
  resolution(0),
  year(0),
  day(0),
  h(0),
  v(0)
  {}
  
  void printToCout()
  {
    std::cout << "File: " << filen << ", product: " << product << ", year: " << year << " day: " << day  << ", H: " << h << ", V: " << v << std::endl;
  }
  
  std::string filen;
  std::string product;
  unsigned int resolution;
  unsigned int year;
  unsigned int day;
  unsigned int h;
  unsigned int v;
};

class MODISDatabase
{
  /*! A mult-index container with the following indexes:
   1) a non-unique index by product;
   2) a non-unique index by year;
   3) a non unuque index by day;
   */
  typedef boost::multi_index_container
    <
      MODISRecord,
      boost::multi_index::indexed_by
      <
        boost::multi_index::ordered_unique<BOOST_MULTI_INDEX_MEMBER(MODISRecord,std::string,filen)>,
        boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(MODISRecord,std::string,product)>,
        boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(MODISRecord,unsigned int,year)>,
        boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(MODISRecord,unsigned int,day)>
      >
    > MODISDb;
  
  MODISDb m_db;
  
public:
  
  MODISDatabase(std::string rootpath);
  
  size_t size() const;
  
  void clear();
  
  // Dump contents to cout
  void printAll();
  
  MODISRecord findByFile(const std::string& filename) const;
  
  std::vector<MODISRecord> findByProduct(const std::string& product) const;
  
  std::vector<MODISRecord> findByYear(unsigned int year) const;
  
  std::vector<MODISRecord> findByDay(unsigned int day) const;
};

#endif
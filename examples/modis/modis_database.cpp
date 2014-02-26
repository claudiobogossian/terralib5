#include "modis_database.h"

void MSearch(const boost::filesystem::path& mpath, std::vector<std::string>& fnames)
{
  if (boost::filesystem::is_regular_file(mpath))
  {
    std::string aux = mpath.leaf().string();
    unsigned pos = aux.find("MOD");
    if (pos==0)
    {
      aux = mpath.string();
      fnames.push_back(aux);
    }
    return;
  }
  
  for (boost::filesystem::directory_iterator it(mpath), itEnd; it !=itEnd; ++it)
    MSearch(*it,fnames);
}

MODISDatabase::MODISDatabase(std::string rootpath)
{
  std::vector<std::string> modis_files;
  boost::filesystem::path mpath(rootpath);
  
  MSearch(mpath,modis_files);
  
  boost::regex efile("([[:alnum:]]+)\.A([[:digit:]]{4})([[:digit:]]{3})\.h([[:digit:]]{1,3})v([[:digit:]]{1,3})\.([[:alnum:]]+)\.([[:alnum:]]+)\.hdf");
  boost::match_results<std::string::const_iterator> presult;
  
  BOOST_FOREACH(std::string& mfile, modis_files)
  {
    boost::filesystem::path fpath(mfile);
    std::string filename = fpath.leaf().string();
    
    if (boost::regex_search(filename, presult, efile))
    {
      MODISRecord rec;
      rec.filen = filename;
      rec.product = presult[1];
      rec.year = boost::lexical_cast<unsigned int>(presult[2]);
      rec.day = boost::lexical_cast<unsigned int>(presult[3]);
      rec.h = boost::lexical_cast<unsigned int>(presult[4]);
      rec.v = boost::lexical_cast<unsigned int>(presult[5]);
      m_db.insert(rec);
    }
  }
}

size_t MODISDatabase::size() const
{
  return m_db.size();
}

void MODISDatabase::clear()
{
  m_db.clear();
}

void MODISDatabase::printAll()
{
  MODISDb::const_iterator it = m_db.begin();
  while (it != m_db.end())
  {
    MODISRecord rec(*it);
    rec.printToCout();
    ++it;
  }
}

MODISRecord MODISDatabase::findByFile(const std::string& filename) const
{
  boost::multi_index::nth_index<MODISDb,0>::type::iterator it = boost::multi_index::get<0>(m_db).find(filename);
  if (it == boost::multi_index::get<0>(m_db).end())
    return MODISRecord();
  
  MODISRecord result(*it);
  return result;
}

std::vector<MODISRecord> MODISDatabase::findByProduct(const std::string& product) const
{
  std::vector<MODISRecord> result;
  
  boost::multi_index::nth_index<MODISDb,1>::type::iterator it_l = boost::multi_index::get<1>(m_db).lower_bound(product);
  boost::multi_index::nth_index<MODISDb,1>::type::iterator it_u = boost::multi_index::get<1>(m_db).upper_bound(product);
  while (it_l != it_u)
  {
    result.push_back(*it_l);
    ++it_l;
  }
  
  return result;
}

std::vector<MODISRecord> MODISDatabase::findByYear(unsigned int year) const
{
  std::vector<MODISRecord> result;
  
  boost::multi_index::nth_index<MODISDb,2>::type::iterator it_l = boost::multi_index::get<2>(m_db).lower_bound(year);
  boost::multi_index::nth_index<MODISDb,2>::type::iterator it_u = boost::multi_index::get<2>(m_db).upper_bound(year);
  while (it_l != it_u)
  {
    result.push_back(*it_l);
    ++it_l;
  }
 
  return result;
}

std::vector<MODISRecord> MODISDatabase::findByDay(unsigned int day) const
{
  std::vector<MODISRecord> result;
  
  boost::multi_index::nth_index<MODISDb,3>::type::iterator it_l = boost::multi_index::get<3>(m_db).lower_bound(day);
  boost::multi_index::nth_index<MODISDb,3>::type::iterator it_u = boost::multi_index::get<3>(m_db).upper_bound(day);
  while (it_l != it_u)
  {
    result.push_back(*it_l);
    ++it_l;
  }
  
  return result;
}

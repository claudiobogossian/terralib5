// MODIS
#include "modis_utils.h"
//#include "data_set.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

void msearch(const boost::filesystem::path& mpath, std::string product, std::string hv, ProdHVDate& fnames)
{
  if (boost::filesystem::is_regular_file(mpath))
  {
      
// Take the filename from the full path and check if it starts with MOD
      std::string filename = mpath.leaf().string();
      std::string::size_type pos = filename.find("MOD");
      if (pos != 0) return;

// Extract the fields from filename - MOD09A1.A2013201.h11v11.005.2013213171641.hdf
      boost::char_separator<char> sep(".");
      boost::tokenizer<boost::char_separator<char> > tokens(filename, sep);
      boost::tokenizer<boost::char_separator<char> >::iterator beg;
      beg = tokens.begin();
      std::string mproduct(*beg);
      ++beg; if (beg == tokens.end()) return;
      std::string mdate(*beg);
      ++beg; if (beg == tokens.end()) return;
      std::string mhv(*beg);

      if (!product.empty() && product != mproduct) return;
      if (!hv.empty() && hv != mhv) return;
//    std::cout  << "File " << mpath.string() << std::endl;
// Compose the date in YYYMMDD format
      int year;
      int jday;
      char a;
      sscanf(mdate.c_str(),"%1c%4d%3d",&a,&year,&jday);
      if (a!='A') return;
      boost::gregorian::date d(year,boost::gregorian::Jan,1);
      boost::gregorian::date_duration dd(jday-1);
      boost::gregorian::date bdate = d + dd;
      mdate = to_iso_extended_string(bdate);

      fnames[mproduct][mhv][mdate] = mpath.string();
      return;
  }

  for (boost::filesystem::directory_iterator it(mpath), itEnd; it !=itEnd; ++it)
    msearch(*it,product,hv,fnames);
}

void modis_print(const unsigned char* values,
                 const std::vector<boost::gregorian::date>* times)
{
  std::size_t nvals = times->size();

  for(std::size_t i = 0; i != nvals; ++i)
  {
    std::cout << "date: " << boost::gregorian::to_simple_string((*times)[i])
              << ", value: " << *((boost::int16_t*)(values)) << std::endl;

    values += sizeof(boost::int16_t);
  }
}


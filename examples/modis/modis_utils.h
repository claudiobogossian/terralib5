#ifndef __modis_utils__
#define __modis_utils__

// STL
#include <memory>
#include <string>

// Boost
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>


typedef std::map <std::string, std::map <std::string, std::map <std::string, std::string > > > ProdHVDate; //[product][hv][date]
typedef std::map <std::string, std::map <std::string, std::map <std::string, std::string > > >::iterator ProdHVDateIterator; // Loop on Product
typedef std::map <std::string, std::map <std::string, std::string > >::iterator HVDateIterator;

void msearch(const boost::filesystem::path& mpath, std::string product, std::string hv, ProdHVDate& fnames);


// pressupoe que values e um vetor de int16
void modis_print(const unsigned char* values,
                 const std::vector<boost::gregorian::date>* times);

#endif // __modis_utils__


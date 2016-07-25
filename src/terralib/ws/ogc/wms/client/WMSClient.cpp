#include "WMSClient.h"

// TerraLib
#include "../../../../core/translator/Translator.h"
#include "../../../core/CurlWrapper.h"
#include "WMSClient.h"

// Boost
#include <boost/filesystem.hpp>

#include <string>

te::ws::ogc::WMSClient::WMSClient(const std::string usrDataDir, const std::string uri, const std::string version) :
  m_version(version),
  m_uri(uri)
{
  m_dataDir = usrDataDir + "/wms/";

  std::string baseUrl = m_uri.scheme() + "://" + m_uri.host();

  if (!m_uri.port().empty())
  {
    baseUrl = baseUrl + ":" + m_uri.port();
  }

  baseUrl = baseUrl + m_uri.path() + "?";

  if (!m_uri.query().empty())
  {
    std::string query = m_uri.query();

    size_t endsWith = query.rfind("&");

    if (endsWith != (query.size() - 1))
    {
      query.append("&");
    }

    baseUrl = baseUrl + query;
  }

  m_uri = te::core::URI(baseUrl);

  m_curl = std::shared_ptr<te::ws::core::CurlWrapper>(new te::ws::core::CurlWrapper());

  if (boost::filesystem::is_directory(usrDataDir) && !boost::filesystem::exists(m_dataDir))
    boost::filesystem::create_directories(m_dataDir);
}

te::ws::ogc::WMSClient::~WMSClient()
{
}

void te::ws::ogc::WMSClient::updateCapabilities()
{
  std::string url = m_uri.uri();

  if (m_version == "1.3.0")
  {
    url = url + "SERVICE=WMS" + "&VERSION=" + m_version + "&REQUEST=GetCapabilities";
  }
  else
  {
    throw te::common::Exception(TE_TR("WMS version not supported!"));
  }

  m_curl->setTaskMessage(TE_TR("Getting Capabilities"));

  // Request the WCS Capabilities XML file
  std::string xmlPath = this->makeFileRequest(url, "capabilities.xml");

  // Parse the XML file into a struct
  m_capabilities = m_parser.parseCapabilities(xmlPath);
}

const te::ws::ogc::wms::WMSCapabilities& te::ws::ogc::WMSClient::getCapabilities() const
{
  return m_capabilities;
}

std::string te::ws::ogc::WMSClient::makeFileRequest(const std::string url, const std::string fileName) const
{
  std::string path = m_dataDir + fileName;

  m_curl->downloadFile(url, path);

  return path;
}

void te::ws::ogc::WMSClient::setCurlWrapper(te::ws::core::CurlWrapper* curlWrapper)
{
  m_curl.reset(curlWrapper);
}

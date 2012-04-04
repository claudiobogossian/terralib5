//******************************************************************************************************************************************
//****************codigo da uripp - MIT*****************************************************************************************************
//******************************************************************************************************************************************
#include <terralib/common/URL.h>
#include <terralib/common/Exception.h>

#include <cstdlib>

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    std::string s, protocol, login, password, host, path, query, fragment, eS, ePath, eQuery, eFragment, key, value;
    int port, hostType;
    bool isAbs, isDir, isEmpty;

    //neste exemplo o recurso a ser localizado eh um diretorio: 
    //o diretorio estah na raiz da maquina mail.google.com
    //2 queries: hl=pt-BR  e  sh va=1
    //fragment id: in+box/12ebb3a026a6d103
    te::common::URL url("https://mail.google.com/mail/?hl=pt-BR&sh+va=1#in+box/12ebb3a026a6d103");
    s = url.getString();
    eS = url.getEncodedString();
    protocol = url.getProtocol();
    host = url.getHost();
    port = url.getHostPort();
    hostType = url.getHostType();
    url.setLogin("lauro");
    url.setPassword("mypass");
    host = url.getHost();
    port = url.getHostPort();
    hostType = url.getHostType();
    login = url.getLogin();
    password = url.getPassword();

    //O teste abaixo troca o separador '&' por ';'
    url = te::common::URL("https://mail.google.com/mail/?hl=pt-BR;sh+va=1#in+box/12ebb3a026a6d103");
    s = url.getString();
    eS = url.getEncodedString();
    protocol = url.getProtocol();
    host = url.getHost();
    port = url.getHostPort();
    hostType = url.getHostType();

    //colocando login e password
    url.setLogin("lauro");
    url.setPassword("mypass");
    host = url.getHost();
    port = url.getHostPort();
    hostType = url.getHostType();
    login = url.getLogin();
    password = url.getPassword();
    path = url.getPathString();
    isAbs = url.isAbsolutePath();
    isDir = url.isDirectoryPath();
    isEmpty = url.isEmptyPath();
    path = url.getPathString();
    ePath = url.getEncodedPathString();
    query = url.getQueryString();
    eQuery = url.getEncodedQueryString();
    eFragment = url.getEncodedFragmentString();

    url = te::common::URL("https://%fateis:my%23Senha@mail.google.com:1234/mail/?hl=pt-BR&sh+va=1#in+box/12ebb3a026a6d103");
    s = url.getString();
    eS = url.getEncodedString();
    protocol = url.getProtocol();
    host = url.getHost();
    port = url.getHostPort();
    hostType = url.getHostType();
    login = url.getLogin();
    password = url.getPassword();
    path = url.getPathString();
    isAbs = url.isAbsolutePath();
    isDir = url.isDirectoryPath();
    isEmpty = url.isEmptyPath();
    path = url.getPathString();
    ePath = url.getEncodedPathString();
    query = url.getQueryString();
    eQuery = url.getEncodedQueryString();
    eFragment = url.getEncodedFragmentString();

    // teste para encontrar a chave "hl" e trocar seu valor para 'pt-PORTUGAL'
    te::common::URL::queryIterator it = url.findQueryKey("hl");
    it->second = "pt-PORTUGAL";
    query = url.getQueryString();
    s = url.getString();
    eS = url.getEncodedString();

    // O exemplo abaixo mostra uma URL que leva a uma pagina html
    // query: NodeId=193
    url.setEncodedString("http://br.franceguide.com/Viajante/Turismo-e-Deficiencia/home.html?NodeID=193");
    s = url.getString();
    eS = url.getEncodedString();

    // usando iterator para ver todos os queries.
    te::common::URL::const_queryIterator qit;
    for(qit = url.beginQuery(); qit != url.endQuery(); qit++)
    {
      key = qit->first;
      value =qit->second;
    }

    url = te::common::URL();
    url.setProtocol("ftp");
    // o protocolo ftp usa a porta 21 por default
    url.setHost("www.dpi.inpe.br"); //default port for ftp: 21

    // alterando a porta default para 220
    url.setHostPort(220);

    // mudando o dominio para www.ibm.com e a porta para 8020
    url.setHost("www.ibm.com:8020");

    // configurando o dominio usando ip_adress e porta 8020
    url.setHost("172.16.254.1:8020"); //ip_adress

    // configurando o dominio usando ipv6_adress e porta 8020
    url.setHost("[2001:db8:0:1234:0:567:8:1]:8020"); //ipv6_adress

    // adicionando path
    url.addPath("lauro hara");
    url.addPath("informações");

//    url.setIsAbsolutePath(true); // ISSO AINDA ME DEIXA DUVIDAS...
    url.setIsAbsolutePath(false); // ISSO AINDA ME DEIXA DUVIDAS... 
    // Acho que nao importa se o path eh ou nao absoluto, sempre tem 
    // que ter '/' depois do dominio. CERTO OU ERRADO??????

    // configurando queries com chaves e valores que usam '&'
    // a entrada deve ser necessariamente sem codificacao
    url.addQuery("key1", "value&1");
    url.addQuery("key&2", "value&2");

    // configurando fragment id - entrada sem codificacao
    url.setEncodedFragmentString("in+box/12ebb3a026a6d103");

    // configurando fragment id - entrada com codificacao
    url.setFragmentString("in box/12ebb3a026a6d103");

    protocol = url.getProtocol();
    host = url.getHost();
    port = url.getHostPort();
    hostType = url.getHostType();
    path = url.getPathString();
    isAbs = url.isAbsolutePath();
    isDir = url.isDirectoryPath();
    isEmpty = url.isEmptyPath();
    ePath = url.getEncodedPathString();
    eQuery = url.getEncodedQueryString();
    query = url.getQueryString();
    eFragment = url.getEncodedFragmentString();
    fragment = url.getFragmentString();

    qit = url.beginQuery();
    key = qit->first;
    value =qit->second;
    qit++;
    key = qit->first;
    value =qit->second;

    s = url.getString();
    eS = url.getEncodedString();

    // ISSO AINDA ME DEIXA DUVIDAS...
    // isso nao funciona!
    url = te::common::URL("file:///lixo/lauro/RomeoAndJuliet.pdf");
    host = url.getHost();
    path = url.getPathString();
    isAbs = url.isAbsolutePath();
    s = url.getString();

    // isso funciona!
    url = te::common::URL("file://C:/lixo/LauroPortoseguro.pdf");
    host = url.getHost();
    path = url.getPathString();
    isAbs = url.isAbsolutePath();
    s = url.getString();

    // parece que isso nao funciona!??????????????? pelo menos no visual c++
    url = te::common::URL("file:../../../../lixo/lauro/RomeoAndJuliet.pdf");
    host = url.getHost();
    path = url.getPathString();
    isAbs = url.isAbsolutePath();
    s = url.getString();

    // segundo o Dihel isto nao faz sentido porque o protocolo file so serve para maquina local
    url = te::common::URL("file:lauro:mySenha@C:/lixo/RomeoAndJuliet.pdf");
    host = url.getHost();
    path = url.getPathString();
    isAbs = url.isAbsolutePath();
    s = url.getString();

    // quando o arquivo esta em outra maquina deve-se usar outro protocolo
    url = te::common::URL("http://lauro:mySenha@sputnik/lixo/lauro/Desktop/RomeoAndJuliet.pdf");
    host = url.getHost();
    path = url.getPathString();
    isAbs = url.isAbsolutePath();
    s = url.getString();

    url = te::common::URL("ftp://lauro:mySenha@sputnik:40/lixo/lauro/Desktop/RomeoAndJuliet.pdf");
     host = url.getHost();
    path = url.getPathString();
    isAbs = url.isAbsolutePath();
    s = url.getString();
    eS = url.getEncodedString();

    // criando uma URL vazia
    url = te::common::URL();
    //preenchendo por partes
    url.setProtocol("telnet");
    url.setHost("lauro:mysenha@sputnik:45");

    s = url.getString();
    eS = url.getEncodedString();
////////////////////////////////////////////////////////////////////////////////////////////////
// usando diretamente a uripp //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

  //  bool e, n, r;
  //  std::string encodeStr;
  //
  //  std::string senc = te::common::uri::urisyn::encode(te::common::uri::urisyn::PATH_TRAITS, "este faz ?teste/com dir/arq.txt");
  //  //te::common::uri::path path22("faz?teste/comdir/arq.txt");
  //
  //  te::common::uri::path path1;
  //  path1 += "teste";
  //  path1 += "t%3";
  //  std::string ssa = path1.encoding();
  //  std::string ssa2 = path1.string();
  //
  //  te::common::uri::uri URI("http://example.org/doc.pdf#view=fitb&nameddest=Chapter3");
  //  e = URI.empty();
  //  n = URI.is_null();
  //  r = URI.relative();
  //  te::common::uri::scheme& scheme = URI.scheme();  
  //  scheme = te::common::uri::scheme("ftp"); //altera protocolo para ftp
  //  te::common::uri::scheme& scheme2 = URI.scheme();  
  //  te::common::uri::authority& authority = URI.authority();
  //  authority = te::common::uri::authority("dpi.inpe.br:8020"); //altera domain name para dpi.inpe.br e porta 8020
  //  authority = te::common::uri::authority("172.16.254.1:8020"); //altera ip_adress para 172.16.254.1 e porta 8020
  //  authority = te::common::uri::authority("[2001:db8:0:1234:0:567:8:1]:8020"); //altera ipv6_adress para 2001:db8:0:1234:0:567:8:1 e porta 8020
  //  std::string host = authority.host();
  //  std::string s = authority.string();
  //  // há uma especificacao para gerar ip_adress e ipv6_adress. Caso o endereco esteja fora da especificacao ocorrera um ERRO.
  //  authority = te::common::uri::authority("localhost"); //altera para local
  //  te::common::uri::authority autority2 = URI.authority();
  //  te::common::uri::path& path = URI.path();
  //  path.clear();
  //  path += "dir+1";
  //  path += "dir 2";
  //  path += "dir 3";
  //  path += "dir 4";
  //  path += "teste.txt";
  //  std::string pathstr = path.encoding();
  ////  path.is_directory(true);
  //  path.absolute(true); //coloca "/" no inicio do path.
  //  path.is_directory(true); //coloca "/" no fim do path.
  //  pathstr = path.encoding();
  //  path.clear();
  //  path += te::common::uri::path("/exemplo/teste/dir+teste/arq.txt");
  //  pathstr = path.string();
  //  std::string pathstr2 = path.encoding();
  //  te::common::uri::path& path2 = URI.path();
  //  te::common::uri::query query = URI.query();
  //  te::common::uri::fragment fragment = URI.fragment();
  //  encodeStr = URI.encoding();
  //  query.push_back(std::make_pair("q 1", "aaa"));
  //  query.push_back(std::make_pair("q 2", "bbb"));
  //  std::string sq = query.encoding();
  //  te::common::uri::query::iterator qit = query.begin();
  //  sq = qit->first;
  //  sq = qit->second;
  //  qit++;
  //  sq = qit->first;
  //  sq = qit->second;
  //  query = te::common::uri::query("hl=pt-BR&sh+va=1");
  //
  //  URI = te::common::uri::uri("https://mail.google.com/mail/?hl=pt-BR&sh+va=1#in+box/12ebb3a026a6d103");
  //  e = URI.empty();
  //  n = URI.is_null();
  //  r = URI.relative();
  //  scheme = URI.scheme();
  //  authority = URI.authority();
  //  path = URI.path();
  //  pathstr = te::common::uri::convert(path);
  //  bool pb = path.is_directory();
  //  query = URI.query();
  //  fragment = URI.fragment();
  //  std::string sf = fragment.string();
  //  std::string sfd = fragment.encoding();
  //
  //  fragment = te::common::uri::fragment("in+box/12ebb3a026a6d103");
  //
  //  URI = te::common::uri::uri("http://msdn.microsoft.com/en-us/library/35bhkfb6(v=vs.71).aspx");
  //  e = URI.empty();
  //  n = URI.is_null();
  //  r = URI.relative();
  //  scheme = URI.scheme();
  //  authority = URI.authority();
  //  path = URI.path();
  //  query = URI.query();
  //  fragment = URI.fragment();
  //
  //  URI = te::common::uri::uri("http://br.franceguide.com/Viajante/Turismo-e-Deficiencia/home.html?NodeID=193");
  //  e = URI.empty();
  //  n = URI.is_null();
  //  r = URI.relative();
  //  scheme = URI.scheme();
  //  authority = URI.authority();
  //  path = URI.path();
  //  query = URI.query();
  //  fragment = URI.fragment();
  //
  //  URI = te::common::uri::uri("http://translate.google.com.br/#en|pt|%0AWhen%20a%20system%20uses%20a%20local%20addressing%20scheme%2C%20it%20is%20useful%20to%20provide%20a%20mapping%20from%20local%20addresses%20into%20URIs%20so%20that%20references%20to%20objects%20within%20the%20addressing%20scheme%20may%20be%20referred%20to%20globally%2C%20and%20possibly%20accessed%20through%20gateway%20servers.%0A%0AFor%20a%20new%20naming%20scheme%2C%20any%20mapping%20scheme%20may%20be%20defined%20provided%20it%20is%20unambiguous%2C%20reversible%2C%20and%20provides%20valid%20URIs.%20It%20is%20recommended%20that%20where%20hierarchical%20aspects%20to%20the%20local%20naming%20scheme%20exist%2C%20they%20be%20mapped%20onto%20the%20hierarchical%20URL%20path%20syntax%20in%20order%20to%20allow%20the%20partial%20form%20to%20be%20used.%0A%0AIt%20is%20also%20recommended%20that%20the%20conventional%20scheme%20below%20be%20used%20in%20all%20cases%20except%20for%20any%20scheme%20which%20encodes%20binary%20data%20as%20opposed%20to%20text%2C%20in%20which%20case%20a%20more%20compact%20encoding%20such%20as%20pure%20hexadecimal%20or%20base%2064%20might%20be%20more%20appropriate.%20For%20example%2C%20the%20conventional%20URI%20encoding%20method%20is%20used%20for%20mapping%20WAIS%2C%20FTP%2C%20Prospero%20and%20Gopher%20addresses%20in%20the%20URI%20specification.%0A");
  //  e = URI.empty();
  //  n = URI.is_null();
  //  r = URI.relative();
  //  scheme = URI.scheme();
  //  authority = URI.authority();
  //  path = URI.path();
  //  query = URI.query();
  //  fragment = URI.fragment();
  //
  //  te::common::uri::authority A("www.dpi.inpe.br");
  //  bool b = A.empty();
  //  bool b1 = A.is_null();
  //  host = A.host();
  //  int t = A.host_type();
  //  unsigned short port = A.port();
  //  s = A.string();
  //
  }
  catch(te::common::Exception& e)
  {
    std::cout << std::endl << "An exception has occuried: " << e.what() << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();
  
  return EXIT_SUCCESS;
}

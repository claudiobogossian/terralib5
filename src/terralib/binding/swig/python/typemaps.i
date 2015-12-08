/*
 * Defining a typemap for std::auto_ptr
 */

%include std_auto_ptr.i

namespace std
{
  %template (StrVector) vector <string>;
  %template (StrStrMap) map <string, string>;
}

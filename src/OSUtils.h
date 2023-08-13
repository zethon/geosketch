#pragma once

#include <string>

// There's some weirdness going on in Ubuntu where using the / operator
// on Ubuntu was throwing an error in some instances. Instead I set out
// to use boost::filesystem::path::seperator but that turned out to be
// a pain since it is multibyte on Windows! So I did this manually.
#ifdef _WINDOWS
#   define PATH_SEPERATOR   '\\'
#else
#   define PATH_SEPERATOR   '/'
#endif

namespace lz
{

void openBrowser(const std::string& url_str);
std::string getOsString();
std::string getUserFolder();
std::string getDataFolder();

} // namespace lz

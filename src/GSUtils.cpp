#ifdef _WINDOWS
#   include <windows.h>
#   include <shellapi.h>
#   include <Shlobj.h>
#   include <codecvt>
#else
#   include <unistd.h>
#   include <sys/types.h>
#   include <pwd.h>
#   include <boost/process.hpp>
#endif

#ifdef __APPLE__
#   include <CoreFoundation/CFBundle.h>
#   include <ApplicationServices/ApplicationServices.h>
#   include <boost/dll.hpp>
#endif

#include "GSUtils.h"

#include <boost/filesystem.hpp>

#include <fmt/core.h>

namespace fs = boost::filesystem;

namespace gs
{

bool validateResourceFolder(const fs::path& folder)
{
    namespace fs = boost::filesystem;
    
    if (!fs::exists(folder)) return false;
    const auto f = folder / "images" / "splash.jpg";
    return fs::exists(folder);
}

std::optional<std::string> defaultResourceFolder()
{
    const auto exepath = boost::dll::program_location().parent_path();
    auto resfolder = exepath / "resources";
    if (validateResourceFolder(resfolder.string())) return resfolder.string();

    resfolder = exepath.parent_path() / "Resources";
    if (validateResourceFolder(resfolder.string())) return resfolder.string();

    auto parent = exepath.parent_path();
    resfolder = parent / "Resources";
    if (validateResourceFolder(resfolder.string())) return resfolder.string();

    resfolder = parent / "resources";
    if (validateResourceFolder(resfolder.string())) return resfolder.string();
    
    return {};
}

void openBrowser(const std::string& url_str)
{
    if (url_str.empty()) return;

#ifdef _WINDOWS
    ShellExecute(0, 0, url_str.c_str(), 0, 0, SW_SHOWNORMAL);
#elif defined(__APPLE__)
    // only works with `http://` prepended
    CFURLRef url = CFURLCreateWithBytes(
        // allocator
        nullptr,

        // URLBytes
        (UInt8*)url_str.c_str(),     // URLBytes

        // length
        static_cast<std::int32_t>(url_str.length()),

        // encoding
        kCFStringEncodingASCII,

        // baseURL
        NULL
    );

    LSOpenCFURLRef(url, nullptr);
    CFRelease(url);
#elif defined(__linux__)
    boost::process::system("/usr/bin/xdg-open", url_str,
        boost::process::std_err > boost::process::null,
        boost::process::std_out > boost::process::null);
#else
    throw NotImplementedException("openBrowser");
#endif
}

std::string getOsString()
{
#ifdef _WINDOWS
    return "windows";
#elif defined(__APPLE__)    
    return "macos";
#elif defined(__linux__)
    return "linux";
#else
    return "unknown"
#endif
}

#ifdef _WINDOWS
std::string getWindowsFolder(int csidl)
{
    std::string retval;

    WCHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, csidl, NULL, 0, path)))
    {
        std::wstring temp(path);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
        retval = convert.to_bytes(temp);
    }
    else
    {
        throw std::runtime_error("could not retrieve user folder");
    }

    return retval;
}
#endif

std::string getUserFolder()
{
    std::string retval;

#ifdef _WINDOWS
    TCHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, path)))
    {
        std::stringstream ss;
        ss << path;
        retval = ss.str();
    }
    else
    {
        throw std::runtime_error("could not retrieve user folder");
    }
#else
struct passwd *pw = getpwuid(getuid());
retval = pw->pw_dir;
#endif

    return retval;
}

std::string getDataFolder()
{
    std::string retval;

#ifdef _WINDOWS
    retval = getWindowsFolder(CSIDL_COMMON_APPDATA);
#else
    struct passwd *pw = getpwuid(getuid());
    retval = pw->pw_dir;
#endif

    return retval;
}

} // namespace tt

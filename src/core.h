#pragma once

#ifdef _WINDOWS
#pragma warning(disable:4530)
#pragma warning(disable:4068)
#endif

#define __STRINGIFY__(x)        #x
#define __EXPAND__(x)           __STRINGIFY__(x)

#define VERSION_MAJOR			1
#define	VERSION_MINOR			0
#define VERSION_PATCH			0
#define VERSION					__EXPAND__(VERSION_MAJOR) "." __EXPAND__(VERSION_MINOR) "." __EXPAND__(VERSION_PATCH)
#define BUILDTIMESTAMP	        __DATE__ " " __TIME__

#define APP_NAME_LONG           "GeoSketch"
#define APP_DOMAIN              "geosketch"
#define APP_TITLE               APP_NAME_LONG " " VERSION


# GeoSketch

GeoSketch is a country drawing game that tests a players knowledge of maps and geography, as well as a player's drawing skills.

## Building

### Windows

Conan profile used to build (i.e. `~/.conan/profiles/default`)
```
[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64
build_type=RelWithDebInfo
compiler=msvc
compiler.runtime=dynamic
compiler.version=193
[options]
[build_requires]
[env]
```

```
conan install .. --profile=default
cmake --build . -DCMAKE_BUILD_TYPE=RelWithDebInfo
```
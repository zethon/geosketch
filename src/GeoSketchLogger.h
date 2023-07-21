#pragma once
#include <memory>

#include <spdlog/spdlog.h>

#include <lua.hpp>

namespace gs
{

namespace log
{

constexpr auto GLOBAL_LOGGER = "gs";

using SpdLogPtr = std::shared_ptr<spdlog::logger>;

[[maybe_unused]] SpdLogPtr rootLogger();
SpdLogPtr initializeLogger(const std::string& name);

} // namespace log

} // namespace gs
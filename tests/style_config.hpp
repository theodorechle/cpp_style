#ifndef STYLE_CONFIG_HPP
#define STYLE_CONFIG_HPP

#include "../../cpp_style/src/style/abstract_configuration.hpp"

constexpr std::string_view PIXEL_UNIT = "px";
constexpr std::string_view PERCENTAGE_UNIT = "%";

// const style::Config guiStyleConfig = style::Config{{{"margin-left", {false, true, true, true, false, {}}},
//                                                     {"margin-right", {false, true, true, true, false, {}}},
//                                                     {"margin-top", {false, true, true, true, false, {}}},
//                                                     {"margin-bottom", {false, true, true, true, false, {}}},
//                                                     {"margin", {false, true, true, true, false, {}}},
//                                                     {"padding-left", {false, true, true, true, false, {}}},
//                                                     {"padding-right", {false, true, true, true, false, {}}},
//                                                     {"padding-top", {false, true, true, true, false, {}}},
//                                                     {"padding-bottom", {false, true, true, true, false, {}}},
//                                                     {"padding", {false, true, true, true, false, {}}},
//                                                     {"border-left", {false, true, true, true, false, {}}},
//                                                     {"border-right", {false, true, true, true, false, {}}},
//                                                     {"border-top", {false, true, true, true, false, {}}},
//                                                     {"border-bottom", {false, true, true, true, false, {}}},
//                                                     {"border", {false, true, true, true, false, {}}},
//                                                     {"width", {false, true, true, true, false, {}}},
//                                                     {"height", {false, true, true, true, false, {}}},
//                                                     {"max-width", {false, true, true, true, false, {}}},
//                                                     {"min-width", {false, true, true, true, false, {}}},
//                                                     {"max-height", {false, true, true, true, false, {}}},
//                                                     {"min-height", {false, true, true, true, false, {}}},
//                                                     {"border-left-color", {true, false, true, true, true, {}}},
//                                                     {"border-right-color", {false, true, true, true, false, {}}},
//                                                     {"border-top-color", {false, true, true, true, false, {}}},
//                                                     {"border-bottom-color", {false, true, true, true, false, {}}},
//                                                     {"border-color", {false, true, true, true, false, {}}},
//                                                     {"background-color", {false, true, true, true, false, {}}},
//                                                     {"padding", {false, true, true, true, false, {}}},
//                                                     {"padding", {false, true, true, true, false, {}}},
//                                                     {"padding", {false, true, true, true, false, {}}}},
//                                                    {std::string(PIXEL_UNIT), std::string(PERCENTAGE_UNIT)}};

const style::config::Config guiStyleConfig = style::config::Config{{}, {std::string(PIXEL_UNIT), std::string(PERCENTAGE_UNIT)}};

#endif // STYLE_CONFIG_HPP
#include "test_config.hpp"

style::config::Config *testConfig() {
    style::config::ConfigRuleNode *paddingConfig =
        new style::config::ConfigRuleNode(style::Token::Unit, new style::config::ConfigRuleNode(style::Token::Int));
    style::config::ConfigRuleNode *textColorConfig = new style::config::ConfigRuleNode(style::Token::Hex);

    std::string PIXEL_UNIT = "px";
    std::string PERCENTAGE_UNIT = "%";

    style::config::Config *guiStyleConfig =
        new style::config::Config{{{"padding", {paddingConfig}}, {"text-color", {textColorConfig}}}, {PIXEL_UNIT, PERCENTAGE_UNIT}};
    return guiStyleConfig;
}

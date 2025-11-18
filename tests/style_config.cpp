#include "style_config.hpp"

style::config::Config *testConfig() {
    style::config::ConfigRuleNode *paddingConfig = new style::config::ConfigRuleNode(style::Token::Unit);
    paddingConfig->addChild(new style::config::ConfigRuleNode(style::Token::Int));
    style::config::ConfigRuleNode *textColorConfig = new style::config::ConfigRuleNode(style::Token::Hex);
    style::config::Config *guiStyleConfig = new style::config::Config{{{"padding", {paddingConfig}}, {"text-color", {textColorConfig}}}, {}};
    return guiStyleConfig;
}

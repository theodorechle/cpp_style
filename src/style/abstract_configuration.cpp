#include "abstract_configuration.hpp"

#include "lexer.hpp"

namespace style::config {
    bool configChecker(Config config) {
        Lexer lexer = Lexer();
        for (std::pair<std::string, ConfigRuleNode> rule : config.rules) {

        }

        // TODO: units
    }
} // namespace style::config

- register where errors occurs to be able to signal them (without raising errors and killing the app)
- check for multiple same identifiers (in style_manager, not when parsing)
- nested tuples?

## Tests
Add tests for config units

## Rework modifiers
- enable modifiers who aren't at the end of the required components list

# IN PROGRESS
- style validity (enums, existing units, ...) should be done here directly, not in external (gui here)
- valid rules shoud be defined in a config file, so the style can itself filter them, and enums will be checked just after the parsing, not in an external program

- register where errors occurs to be able to signal them
- enable modifiers who aren't at the end of the required components list
- check for multiple same identifiers
- nested tuples?
- rename: styleComponent
- remove empty blocks

- style validity (enums, existing units, ...) should be done here directly, not in external (gui here)
- valid rules shoud be defined in a config file, so the style can itself filter them, and enums will be checked just after the parsing, not in an external program
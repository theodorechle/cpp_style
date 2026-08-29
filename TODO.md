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

# Refacto
## Parser
button        .selected >         #myLabel     { }
  ^    ^      ^   ^     ^         ^   ^         ^
name  rel.  symb. name relation symb. name    name
|           selectors list                  |       |
|                   selectors block         | block |


button                 ,            list           {}
| selectors list | separator  | selectors list |       |
|               selectors block                | block |


button             {         font-size        :            14      px          ;                 }
  ^                ^             ^            ^             ^       ^          ^                 ^
name         block opening     name  assignment operator  integer  unit  expression end    block closing
|           |               |                      rule assignment                       |               |
| sel. list |                                       rules block                                          |


button         {color: blue;   font-size: 14px;}
| sel. list  | | rule assign. |  rule assign. | |
| sel. block |           rules block            |


button         {color: blue;   font-size: 14px;      label       {}       }
| sel. list  | | rule assign. |  rule assign.  | sel.block |  rules block  |
| sel. block |                         rules block                         |

           @import            'a-style-file'              ;
           ^   ^              ^ ^          ^              ^
special sel. import  file start file name file end  expression end


## Still TODO:
- import
- nested blocks
- multiple independant style blocks

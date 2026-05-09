![build status](https://github.com/theodorechle/cpp_style/actions/workflows/c-cpp.yml/badge.svg)

# style
A lexer and parser for scss-like files.

## This library supports:
### Data types
- int: positive and negative integers (e.g.: `53`)
- float: floats with a dot and at list a digit before or after it (e.g.: `7.5`)
- string: any sequence of characters surrounded by simple or double quotes (the only characters which can't be used in the string are quotes who are the same as the one used to surround the string) (e.g.: `"Hello world!"`)
- enum value: a sequence of letters. Available enum values are determined in the (![config](#Config)) (e.g.: `horizontal`)
- tuple: A list of data types surrounded by parenthesises with data types comma-separated (e.g.: `(5, blue, "a string")`)

Operators and functions are not yet supported.

### Paths
- Element name: identify elements by name (.e.g.: `label`)
- Identifiers: identify a unique element (e.g.: `#my-element`)
- Classes: identify a group of elements (e.g.: `.my-elements`)
- Modifiers: identify elements who are currently altered (e.g.: `:hovered`)

The following relations between the above element identifiers are available:
- same element: simply put two identifiers one after an other (element names must be put first, since there is no special character to distinguish them) (.e.g: `label#my-label.my-elements:hovered`)
- direct parent: add a `>` between the identifiers to required the element after to have the left one as parent (e.g.: `label>#my-label` means `label` is the parent of `#my-label`)
- any parent: add a space between the identifiers so the right element has the left one as any kind of ancestor (parent, grandparent, ...) (e.g.: `label #my-label` means `label` is an ancestor of `#my-label`)

When using nested style, when we start a path in the nested part, default is any parent.
```scss
label {
    #my-label {}
}
```
is the same as `label #my-label`.

In order to have them referencing the same element, we need to add a `&` before:
```scss
label {
    &#my-label {}
}
```

For direct-parent, we still only need to add a `>`:
```scss
label {
    >#my-label {}
}
```

### Comments
Single-line and multiline comments are both supported.
Single-line comments are written with `//` at the start:
```scss
button {
    // this is a comment
}
```

Multi-line comments are surrounded by `/*` and `*/`:
```scss
button {
    /*
    this is a multiline comment
    */
}
```

### Examples
Example files used for testing are available <a href="../tests/tests-files">here</a>.

### Tests
Compile the tests with `make tests`.

Debug logs can be added with `make tests DEBUG=1`.

And run them with `bin/tests`.

## Config
The config is an important part to be able to use this library.

For now, it must be written in C++ and an app using it may have to be recompiled each time the config is changed.

I plan to allow the use of configuration files in the future.

Config can be done by instanciating a style::config::Config object.

It expects multiple parameters:
- the rules
- the units
- and the inheritable rules

### Rules
We must specify a map of available rules:

For each rule name, we must specify the accepted values.

These values are specified with style::config::ConfigRuleNode objects, each one accepting a token and an optional child and next node.

The token is the type of value accepted, such as `Int`, `Unit` or `Hex`.

The child and next parameters allows creating a tree, where child is obviously a child of the node, and next is more like the brother of the node, so an other child of the parent. All tree structures in the library are based on child and next nodes.

If we want to specify a rule for padding for example, where the value would be an integer with a unit, we would write
```cpp
{"padding",
    {new ConfigRuleNode(style::Token::Unit,
        new ConfigRuleNode(style::Token::Int))
    }
}
```
With this, we have specified that for the rule name `padding`, the want an `Int`eger with a `Unit`.

Note that `Unit` is specified as the parent of `Int`. It's because the most important type is considered to be a `Unit`, and so the parser orders them like this.

We can use some more complex configuration, like this:
```cpp
{"text-color",
    {
        new ConfigRuleNode(style::Token::Hex),
        new ConfigRuleNode(style::Token::Tuple,
            new ConfigRuleNode(style::Token::Int, nullptr,
            new ConfigRuleNode(style::Token::Int, nullptr,
            new ConfigRuleNode(style::Token::Int)
            ))
        )
    }
}
```
Here, we defined two possible values for a rule named `text-color`:
- An hexadecimal value, with `Hex`
- And a tuple of three integers

For the latter one, we described it as a `Tuple` with an `Int` child who has an `Int` brother who itself has an `Int` brother.
At the end, it only means we have a `Tuple` with three `Int` childs.

For defining rule values, the following tokens can be used:
- `Enum`
- `Tuple`
- `String`
- `Int`
- `Float`
- `Bool`
- `Unit`
- `Hex`


The `Function` token is also available, but since the implementation is not currently functional, it should not be used.

You can find other tokens in the code, but they are not available for defining rules values.
You should refer to the array `style::VALUE_TYPE_TOKENS` for the exhaustive list of tokens which can be used.

The config validity can be check using `style::config::configChecker`.

The `style::NodesToStyleComponents` class (which `style::StyleDeserializer` relies on) automatically runs this check when deserializing new style.

### Units
The Config class also expects us to provide the available units.

If we want to have two units, one for pixels and one for percentages, you could add `{"px", "%"}`.

The strings provided are only used for parsing. What is done with them is up to the user of the library.

### Inheritable rules
The last part of the config is the name of the rules which can be inherited from a parent element.

For example, if `font-name` and `text-color` should be inheritable, we simply need to add `{"font-name", "text-color"}`.

### Limitations
For now, we cannot specify only some units should be usable for a rule. Any unit can be picked, even if some of them are not meaningful for the use cases.

No range can be specified, for example, only allowing positive integers for a size rule or integers between 0 and 255 for rgb colors.

## Actually using the library
🚧
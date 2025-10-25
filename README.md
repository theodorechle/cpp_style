# style
A lexer and parser for scss-like files.

## FEATURES:
### Style types (expressed in pseudo regex)
- int: [0-9]+
- float: [0-9]+\.[0-9]\*
- string: [^,;\s{}()[]]+
- tuple[type,...]: \\( *\<style-type\>( \*, \*\<style-type\>)\* \*\\)
- bool

### Operators
- \+ addition
- \- substraction
- \* multiplication
- \/ division

### Functions
regex: [\w\d][\w\d\\-]+[\w\d]
- round(int|float)

#### Units
An int can be followed by:
 - % to apply a percentage of the parent size (automatically choose width or height)
 - pw to apply a percentage of the parent width
 - ph to apply a percentage of the parent height
 - ww to apply a percentage of the window width
 - wh to apply a percentage of the window height
 - px to be in pixels (default)

#### ! support custom styles

### Modifiers
Shown in precedence priority order:

focused
</br>
clicked
</br>
hovered

Custom modifiers are not yet available

### Identifiers (single element)
An identifier (id) can be used to apply to a specific element

### Classes (groups of elements)
A class can be used to apply style to multiple elements at once

### Relation between elements
- direct parent
- any parent
- same element

## SYNTAX:
Ampersand (&) at first element in nested style means to apply following style in same element instead of a child.

### Syntax

styleComponents = <element-name|.class|#id|:modifier>
componentsSpaceBeforeName = < element-name|.class|#id|:modifier
dataTypes = int|float|bool|string
```
<stylesComponents><['>'|' ']<componentsSpaceBeforeName>[...][, ...] { 
    <style-name>: <dataTypes|tuple[dataTypes,...]>[,...];
    <style-name>: <int|float> +|-|/|*|% <int|float>; // not done
    <style-name>: function(<dataTypes|tuple[dataTypes,...]>, ...); // not done

    <&|styleComponents><componentsSpaceBeforeName>[...] {
        // recursive (styles, inner elements|classes|ids|modifiers)
        [...]
    }
    // comment
    /*
    multiline comment
    */
}
```

### Examples
<a href="../../tests/style_tests_lexer_and_parser/tests-files">See example files</a>

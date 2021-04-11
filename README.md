# grep

This is an implementation of the UNIX grep utility. The usage is as follows:
```
grep REGEXP
```
where `REGEXP` is a regular expression as specified later.  The utility will
read lines from standard input and write to standard output those lines that
match `REGEXP`.

A subset of POSIX extended regular expressions is supported. Specifically,
the following:
* Any character that does not otherwise have a special meaning matches the
  character itself.
* `.` matches any character.
* `^` matches the empty string at the beginning of a line.
* `$` matches the empty string at the end of a line.
* `\` followed by a special character matches that special character.
* `[` and `]` delimit a bracket expression.
  + If `^` immediately follows `[`, the bracket expression matches any
    character not contained in the bracket expression.
  + Otherwise, the bracket expression matches any character contained in the
    bracket expression.
  + `]` immediately following `[` or the `^` immediately following `[` does not
    close the bracket expression, but instead adds the `]` itself to the
    bracket expression.
  + Two characters joined by `-` adds the inclusive range of characters
    between the two characters to the bracket expression.
  + `-` immediately following `[` or the `^` immediately following `[`, or
    immediately preceding the `]` does not denote a range, but instead adds the
    character `-` itself to the bracket expression.
* A regular expression followed by `*` matches the concatenation of 0 or more
  strings that each matches the regular expression preceding `*`.
* The concatenation of two regular expressions matches the concatenation of
  a string that matches the first regular expression and a string that matches
  the second.
* Two regular expressions joined by `|` matches a string that matches either
  of the regular expressions.

The operators in order of precedence, from high to low, are `*`, concatenation,
and `|`. Additionally, `(` and `)` can be used to give the highest precedence
to the enclosed subexpression.

This implementation simply treats each byte as a character, with special
characters in regular expressions interpreted according to their values in
ASCII.

# Lazy, Top-Down Suffix Tree Algorithm

This repository contains a program that implements lazy suffix tree
construction, inspired by the following:

```
Giegerich, R. and Kurtz, S. and Stoye, J.
Efficient Implementation of Lazy Suffix Trees,
Software-Practice and Experience
33(11):1035-1049, 2003
```

## Configuration

It is important to consider the configuration file `include/config.h`
before running the program. An incorrect situation may cause undefined
behavior.

## Usage

Using `make testrun` builds and queries patterns from two standard files.

In general, program is compiled via `make`. This generates the binary
`bin/wotd` which is invoked in the following way.

```
    # If one argument is given, this is assumed to be the absolute path to a
    # file, whose content is used to build a suffix tree. The program first
    # randomly generates patterns which is printed to a a pattern file
    # depending on `include/config.h`. These patterns are then queried on the
    # suffix tree. The random patterns are logged to a file specified in
    # `config.h`.

    ./bin/wotd TEXTFILE

    # If two arguments are given, the first is again assumed to be the absolute
    # path to a file, whose content is used to build a suffix tree. The second
    # argument is assumed to be a patternfile, that is, a file whose lines
    # specify the patterns that will be used to query the suffix tree.

    ./bin/wotd TEXTFILE PATTERNFILE
```
## Documentation

The purpose of every public function is explained in its corresponding
headerfile in `include`.

## Acknowledgements

The initial commit `fd3d0c8769696459a0957dc62604ab974f097866 "Initial Commit"`
is a clone of the `wotd` program, originally developed by [Stefan Kurtz et.
al](https://www.biologie.uni-hamburg.de/service/wissenschaftsservice/mitarbeiter/professoren-promotionsverfahren/skurtz.html).
All changes and modifications were under permission of the original
authors.


Realize of chemical task:
# Chemical formula task

Implement a program that gets input, parses it an output a result. The general task is to calculate amount of elements in a string according to some rules. Output format is completely free, use anything you want, just print all the found elements. Make your program extendable as the task will be extended after each part.

## Part 1

The input contains a string with chemistry-like elements with its values. Each element starts with a UPPER-case letter and may other lower-case letters after it. After the element it might be a number of such elements. Do not forget to check for errors. Examples of input strings are (# notes a comment and is not a part of an input string):

```
Fe   # Fe => 1
Fe2  # Fe => 2
Ag12 # Ag => 12
N0   # Error
N-1  # Error
cl   # Error
2F   # Error
```

## Part 2

Input may contain more than one element. Examples:

```
Fe2O3 # => Fe => 2, O => 3
H2SO4 # => H => 2, S => 1, O => 4
HOH   # => H => 2, O => 1
```

## Part 3

Input may contain subgroups in brackets. Each bracket group may have multiplier after it. Brackets may be both `()` and `[]`. An input string may contain more than one group of brackets. Examples:

```
Fe2(SO4)3 # Fe => 2, S => 3, O => 12
K[Fe(NO3)2]4 # K => 1, Fe => 4, N => 8, O => 24
```

## Part 4

Support ligand groups in an input string. Ligand group is defined in a form `N * M`. Both ligand group parts may contain everything from previous parts of the task. Both parts of ligand group may have multiplier as a prefix of the group. Example:

```
CuSO4 * 5H2O # Cu => 1, S => 1, O => 4, H => 10, O => 5
```

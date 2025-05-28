
# Expression Notation Converter

This is a CS110(Discrete Structures 2) Project. A command-line C program that builds and traverses expression trees based on user-specified notations: infix, prefix, or postfix. It converts the input expression to the other two notations using tree traversal techniques.

## 🧠 Features

- Detects and supports three expression notations: infix, prefix, and postfix
- Builds binary expression trees from the given input
- Converts the given expression to the other two notations using tree traversal:
  - Infix → Prefix & Postfix
  - Prefix → Infix & Postfix
  - Postfix → Prefix & Infix
- Proper handling of operator precedence and associativity
- Expression validation and error messages for unsupported formats
- Help guide available via `--help`

## 🧪 Example Usage

```bash
./main "1 + 2 ^ (3 * 4) / 5" --infix
```

### Output:
```
===================================================================

 EXPRESSION NOTATION CONVERTER
 Discrete Structures 2
 BSCS 2A

 [ TEAM MEMBERS ]
    Michael Xavier E. Canonizado
    Simon Narvaez
    Bryan Surname

===================================================================

 CONVERSION RESULTS

 [ ORIGINAL EXPRESSION ]
    Infix   : 1 + 2 ^ (3 * 4) / 5

 [ CONVERTED EXPRESSIONS ]
    Prefix  : + 1 / ^ 2 * 3 4 5
    Postfix : 1 2 3 4 * ^ 5 / +

===================================================================
```

## 📌 Help Guide

```bash
./main --help
```

Output:

```
Expression Notation Guidelines

   Usage: ./main "<expression>" --<notation>
   E.g:    ./main "1 + 2 ^ (3 * 4) / 5" --infix

Valid operations: + | - | * | / | ^

Expression rules:
    - Must be enclosed in ""
    - Supports variables (A,B,C,x,y,z) and positive integers
        - A + B + C
        - 1 + 2 + 3
        - A + 1 + B
    - Integers may be multiple characters
        - A + BC + DEF
        - 1 + 23 + 456
    - Operands and operators must be separated by spaces
        - 1+2+3 (wrong)
        - 1 + 2 + 3 (correct)
    - Precedence is specified with '()'
        - (1 + 2) + 3 ^ (4 + 5)
```

## 🔧 Build & Run

```bash
gcc -o main main.c
./main "A + B * C" --infix
```

## 👨‍💻 Authors

- Michael Xavier E. Canonizado
- Simon Narvaez
- Bryan Surname

## 📚 Subject

Discrete Structures 2 — BSCS 2A

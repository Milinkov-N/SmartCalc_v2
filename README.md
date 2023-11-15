# SmartCalc v2.0

Standard calculator written in C++ with the Object-Oriented Programming paradigm and Model-View-Controller pattern. In addition to basic arithmetic operations such as add/subtract and multiply/divide, calculator have the ability to calculate arithmetic expressions by following the order, as well as some mathematical functions (sine, cosine, logarithm, etc.). Besides calculating expressions, it's also support the use of the x variable and the graphing of the corresponding function.

Program also implements credit calculator.

## Features

- program is developed in C++ language using C++17 standard.
- program follows the Google style.
- full coverage of expression calculation modules with unit-tests using the GTest library.
- GUI via Qt framework.
- program is implemented using the MVC pattern.
- both integers and real numbers with a dot can be input into the program.
- calculating arbitrary bracketed arithmetic expressions in infix notation.
- calculating arbitrary bracketed arithmetic expressions in infix notation with substitution of the value of the variable x as a number.
- plotting a graph of a function given by an expression in infix notation with the variable x (with coordinate axes, mark of the used scale and an adaptive grid).
- verifiable accuracy of the fractional part is at least to 7 decimal places.
- bracketed arithmetic expressions in infix notation must support the following arithmetic operations and mathematical functions:

  - **Operators**:
    | Operator name | Infix notation |
    | --------- | ------ |
    | Brackets | (a + b) |
    | Addition | a + b |
    | Subtraction | a - b |
    | Multiplication | a \_ b |
    | Division | a / b |
    | Power | a ^ b |
    | Modulus | a mod b |
    | Unary plus | +a |
    | Unary minus | -a |

  - **Functions**:
    | Function description | Function |
    | ------ | ------ |
    | Computes cosine | cos(x) |
    | Computes sine | sin(x) |
    | Computes tangent | tan(x) |
    | Computes arc cosine | acos(x) |
    | Computes arc sine | asin(x) |
    | Computes arc tangent | atan(x) |
    | Computes square root | sqrt(x) |
    | Computes natural logarithm | ln(x) |
    | Computes common logarithm | log(x) |

## Bonus. Credit calculator

Program provides a special mode "credit calculator":

```
Input: total credit amount, term, interest rate, type (annuity, differentiated)
Output: monthly payment, overpayment on credit, total payment
```

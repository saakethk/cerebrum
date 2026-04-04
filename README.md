# Cerebrum
A very heavily optimized efficient library for quantitative calculations and storage written in C++ and exposed via python.

# Current Implementation
- The database class is implemented via DuckDB.
- The source class is currently implemented via openbb.

# Key Dependencies
- https://docs.openbb.co/odp/python
- https://docs.pola.rs/user-guide/getting-started/#installing-polars
- https://duckdb.org

# To-Do
1. Write basic chunk functionality for internal and leaf nodes
2. Test chunk functionality and make abstract easy interface to work with it
3. Integrate chunk with table and make basic splittign functionality

# To Run C++ Core
1. Ensure your in root directory
2. g++ -std=c++20 -Wall -Werror -Wextra -g -o out core/*.cpp
3. For debugging, lldb ./out

# Cerebrum Core
It is a B+ tree which utilizes a hybrid storage method between columnar and row-wise. Every leaf node contains the key and there is then a 2d vector with all the values. It's goal is to be very efficient for statistical estimation and forecasting.

## Leaf Node Structure
$$
\mathbf{K = key},\\
[
  \mathbf{attribute_1} = \left[ 
    \begin{array}{c|c|c}
    x_1 & ... & x_n \\
    \end{array} 
  \right],\\
  ...,\\
  \mathbf{attribute_n} = \left[ 
    \begin{array}{c|c|c}
    y_1 & ... & y_n \\
    \end{array} 
  \right]
]

$$


# Future Plans
1. Make it so that you can calculate markov chain probailities
2. Make it so that you can store data in very compressed format
3. Make some example use cases for it
4. Publish it to PyPi





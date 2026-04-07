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
1. splitting for leaf chunks
2. check for duplicate keys
3. test the leaf chunks fully

# To Run C++ Core
1. cd core
2. make main
3. make test

# Cerebrum Core
It is a B+ tree which utilizes a hybrid storage method between columnar and row-wise. Every leaf node contains the key and there is then a 2d vector with all the values. It's goal is to be very efficient for statistical estimation and forecasting.

## Limitations
- Keys: 0 <= k <= 4,294,967,295 (maximum value of unsigned int in c++)
  - keys must also be <b>unique</b>
- Values: minimum value of double in c++ <= v <= maximum value of double in c++

## Leaf Node Structure
$$
\mathbf{keys} = \left[ 
  \begin{array}{c|c|c}
  k_1 & ... & k_n \\
  \end{array} 
\right],\\
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
5. Adapative radix tree for caching





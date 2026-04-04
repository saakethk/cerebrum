# cerebrum
A very heavily optimized efficient library for quantitative calculations and storage written in c++ and exposed via python.

# Current Implementation
- The database class is implemented via DuckDB.
- The source class is currently implemented via openbb.

# Key Dependencies
- https://docs.openbb.co/odp/python
- https://docs.pola.rs/user-guide/getting-started/#installing-polars
- https://duckdb.org

# To-D0
1. Write basic chunk functionality for internal and leaf nodes
2. Test chunk functionality and make abstract easy interface to work with it
3. Integrate chunk with table and make basic splittign functionality

# To Run C++ Core
1. Ensure your in root directory
2. g++ -std=c++20 -Wall -Werror -Wextra -g -o out core/*.cpp
3. For debugging, lldb ./out






# Data
The goal of this function is to give back the data associated with a stock and abstract away working with database.

## Expected Inputs
The class will take in database connector object.
  - Database connector object should abstract away basic functionality.
It will also have functions that can be called for any sort of data retrieval.
  - Will use [Polars](https://pola.rs) - a more modern, efficient version of pandas

## Expected Outputs
- Cached index: Should allow for easy parsing of similarity
  - Should be in database table
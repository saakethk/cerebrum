- Computing indicators and dealing with computational data is computationally expensive
  - Purely for quant signals. (efficient because we limit scope to numeric vars)
  - Abstract away while keeping performance in native library
    - Remove middle libraries like numpy and such
    - Coded in C++ and compiled for native performance
  - Regex to compute indicators
    - Provide ability to compute indicators via string expressions like "(prev_price - curr_price) / prev_price"
      - Provide support for elementary operations for columns
      - Every time attribute is added we ahve to add ability to compute this fast

- Primary operations we need to support are;
  - save(): save the quantitative data in a space efficient manner (implement some sort of lossless compression)
    - Expected: O(n)
  - load(): load data from a saved file
    - Expected: O(n)

  - search(id): be able to search for a specific timestamp (timestamp will be the primary id as this is for quant)
    - Expected: O(log(n))
  - insert(ticker): be able to insert a tick for a stock with whatever attributes were initialized
    - Expected: O(log(n))
  - delete(ticker): be able to delete a tick for a stock with whatever attributes were initialized
    - Expected: O(log(n))
  
  - add_attribute(): be able to add a new custom attribute to the table like a indicator or derived attribute
    - Expected: O(n)
  - remove_attribute(): be able to remove a new custom attribute to the table like a indicator or derived attribute
    - Expected: O(1)

  - apply(attribute, operand, val): apply a operation with a value to a column
    - Expected: O(n)
  - upscale(): go from a smaller frequency like 1d to a larger freqeuncy like 1w
    - Expected: O(n)
  - downscale(): go from a larger frequency like 1w to a smaller frequency like 1d
    - Expected: O(n)

  - min(attribute): get minimum value of specified attribute
    - Expected: O(1)
  - max(attribute): get maximum value of specified attribute
    - Expected: O(1)
  - bounds(attribute): get bounds of a attribute (min, max)
    - Expected: O(1)
  - missing_ranges(): get the missing ranges in the continous data like (2020-01-01, 2021-01-01). Makes filling in missing data easy.
    - Expected: O(log(n))

  - addCompute(name, equation): add a computed column (This is going to have to update if a value in the middle, start or end gets updated)
    - Expected: O(n)
    - Calls apply func
  - removeCompute(name, equation): remove a computed column
    - Expected: O(1)

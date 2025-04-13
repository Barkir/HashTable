# Hash Table Research

## What we need to do?
1. Write your own implementation of hash table
2. Analyze it's speed using ```perf, valgrind, hotspot```
3. Find the __hottest__ functions
4. Optimize them using SIMD instructions and assembler
5. Count the boost
6. EAT, SLEEP, REPEAT

## What is a hash table?
- Hash table is a structure for storing your data
- It is easy and fast to write
- It works faster than any other storing structure (tree, list etc.)

### Main idea
```
you have these bins where you put your data
to put in a bin you count hash of your data

# this is how you get the number of bin
---------------------
index = hash % bins |
---------------------

0       1       2       3
|       |       |       |
[]      []      []      []
[]      []      []      []
[]      []      []      []
```

### Better hash == Random hash.
Your data should be kept smoothly to keep an optimal [load factor](#load-factor)

#### Load-factor

```python
load_factor = sum([len(list) for list in hash_table]) / bins
```


## Parsing text
For parsing text we will use python script ```bookparser.py``` that generates us 16-bit aligned text.

This allows us to load our strings as __m128 variables and use intrinsic functions for string comparsion.
It boosts our performance a lot, because we can compare 16 bits in one tact.






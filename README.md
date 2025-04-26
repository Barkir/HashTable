# Hash Table Research

  | OS | Hardware |
  |------|------|
  | Ubuntu 24.10 | AMD Ryzen 7 6800H with Radeon Graphics            3.20 GHz |
  | DESKTOP-JTSUO0I       | 16 GB RAM                                                  |
  | Ubuntu clang version 19.1.1 (1ubuntu1) |
  | gcc (Ubuntu 14.2.0-4ubuntu2) 14.2.0    |

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
Your data should be kept smoothly to keep an optimal [load factor](#load-factor) of 1.5 but we are going to keep it **15** for better optimization.


Keeping load factor as low as possible reduces **linear search** in bins, that's why it makes hash tables work faster

#### Load-factor

```python
load_factor = sum([len(list) for list in hash_table]) / bins
```
#### Hash-function
- Using djb2 hash function (fast & easy to write)
- In the next iterations we will optimize this spot and use SIMD hash functions
```c
int64_t HashFunction(const void * elem, size_t size)
{
    int64_t hash = 5381;
    const char * str = (const char *) elem;
    for (int i = 0; i < size; i++)
    {
        hash = ((hash << 5) + hash) + str[i];
    }

    return hash;
}
```

## Parsing text
For parsing text we will use python script ```bookparser.py``` that generates us a word-list file, where you can choose its alignment.

This allows us to load our strings as __m128 or _m256 variables and use intrinsic functions for string comparsion.
It will boost our perfomance.



The book we are going to parse is ```HARRY POTTER!!!```
---

Hope my optimizations will work faster than ```-O3``` with the help of Magic!

We are going to parse the text with several methods.
The first method is default
```
...
Harry
Potter
and
the
Sorcerers
Stone
CHAPTER
ONE
THE
BOY
WHO
LIVED
Mr
...
```

Loading Parameters into our table
| Bins | Load Factor | Words Loaded | Words Searched |
|------|-------------|--------------|----------------|
 128| 15.6| 2000 | 20000

```
 <<<LOG FILE OF HASH TABLE>>>
Load factor = 15.625000
[BIN 0]
----------------------------------
	 did
	 hed
	 hed
	 Potters
	 hed
	 upset
	 upset
	 hed
	 upset
	 Potters
	 hed
	 did
	 Potters
	 Potters
	 Potters
	 Potters
	 did
----------------------------------
...
```
We will use **perf**, **hotspot**  and **hyperfine** for profiling our hash table.

Use these commands for profiling
```
hyperfine --warmup 100 './run'
hyperfine --runs 1000 './run'

sudo perf record ./run
sudo hotspot
```

It is needed to load frequently used blocks of memory to cache, this way we can make the experiment more repeatable,
because it won't be dependent of cache-misses.

| Compiler | Flags | Time|
|----------|-------|------------|
| g++ | -O0 |163.2 ms ±  1.3 ms 	|
| g++ | -O1 |158.5 ms ±  0.9 ms  	|
| g++ | -O2 |162.3 ms ±   2.9 ms  |
| g++ | -O3 |161.5 ms ±  1.6 ms	|
| g++ | PGO (profile guided optimization) | 170.4 ms ±  4.9 ms


![alt text](readme/image-1.png)



As we see, the hottest function here is ```_strcmp_avx2```

### First enemy - strcmp

Let's write our own strcmp function using intrinsic functions.

A little bit about inline assembler syntax

```
__asm__ (
    "asm code"
    : output_operands // return value
    : input_operands  // parameters
    : clobbers		  // trash registers
);
```
This is a C implementation of our strcmp function

```c
int strcmp_asm(const char * s1, const char * s2)
{
    __m256i ymm_string1 = _mm256_loadu_si256((__m256i*) s1);
    __m256i ymm_string2 = _mm256_loadu_si256((__m256i*) s2);

    int mask = _mm256_testc_si256(ymm_string1, ymm_string2);

    return mask;
}
```

This is how it looks like in asm

```
strcmp_asm(char const*, char const*):
        vmovdqu ymm0, YMMWORD PTR [rdi]
        xor     eax, eax
        vptest  ymm0, YMMWORD PTR [rsi]
        setc    al
        vzeroupper
        ret
```

This is how we write it using inline assembler

1. We move a string by address %1 (which is in rdi register) to ymm0
2. Then we use ```vptest``` instructuion which counts (~a & b) of ymm register and address and sets the carry flag to 1 if it equals zero (which is basically means registers are equal)
3. ```setc``` is for setting the lowest byte of rax to the value of carry flag
4. ```vzeroupper``` zeros hugher 128 bits of ymm

```"+&r" (mask)``` - _'+'_ is for read&write mode, _'&'_ is a mark that this value is modified (prevents compiler from assigning the same register to input and output). _'r'_ - request for placing it in **general-purpose register**



```c
static inline int strcmp_asm(const char * el1, const char * el2)
{

    int mask = 0;

    __asm__ __volatile__ (
    ".intel_syntax noprefix             \n\t"
    "vmovdqu ymm0, [%1]                \n\t"
    "xor eax, eax                       \n\t"
    "vptest ymm0,  [%2]                \n\t"
    "setc %b0                            \n\t"
    "vzeroupper                         \n\t"
    ".att_syntax prefix                 \n\t"

    :"+&r" (mask)
    :"r" (el1), "r" (el2)
    : "ymm0"

    );

    return mask;

}
```

![alt text](readme/image.png)

| Previous time | Current time |Boost|
|------|-----|------|
 163.0 ms ±  1.3 ms | 145.7 ms ± 1.8 ms | 12.4% |

### Second enemy - hash function

The second approach we will use is intrinsic functions.
You won't believe it, but **crc32** has its intrinsic-implementation.


#### This is called
![image](readme/magic.gif)

---

This is a function for counting crc32 hash using intrinsic functions.


```
uint32_t icrc32(const char* string)
{
    uint32_t crc = 0;
    crc = _mm_crc32_u64(crc, *((uint64_t*)string +  0));
    crc = _mm_crc32_u64(crc, *((uint64_t*)string +  1));
    crc = _mm_crc32_u64(crc, *((uint64_t*)string +  2));
    crc = _mm_crc32_u64(crc, *((uint64_t*)string +  3));

    return crc;
}
```

| Previous time | Current time |Boost|
|------|-----|------|
 145.7 ms ±  1.3 ms | 132.3 ms ± 1.8 ms | 10.1% |

![alt text](readme/second.png)

Now we see that strcmp is the hottest function again

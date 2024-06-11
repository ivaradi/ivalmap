# Interval map for C++

A simple C++ interval map implementation.

```IntervalMap<Key, Value>``` maps ranges of the key to the same value.

Example:

```C++
IntervalMap<int, char> ivalmap('X');  // all key values are mapped to 'X'

ivalmap.set(1, 10, 'Z');    // key values <1 are mapped to 'X', [1, 9]are mapped to 'Z'
                            // >=10 are mappped to 'X'
ivalmap.set(3, 7, 'Y');     // key values <1 are mapped to 'X', 1 and 2 are mapped to 'Z',
                            // [3, 6] are mapped to 'Y', [7, 9] are mapped to 'Z', and
                            // >=10 are mapped to 'X'
```

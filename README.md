# Things to Remember

1. The expression:

```cpp
x2.find(s) != string::npos
```

means:  
**"Check if the string `s` is a substring of the string `x2`."**

- `x2.find(s)` tries to find the first occurrence of `s` inside `x2`.
- If `s` is found, `find()` returns the starting index (a number).
- If `s` is **not** found, `find()` returns `string::npos` (which means "not found").
- So, `x2.find(s) != string::npos` is `true` if `s` exists in `x2`, and `false` otherwise.

**In your code:**  
- `x2` is `x` concatenated with itself (`x + x`).
- This line checks if `s` appears anywhere in that doubled string.

---

2. Sum of a Vector

```cpp
(accumulate(a.begin(), a.end(), 0))
```
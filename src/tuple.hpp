#pragma once

#include <stddef.h>

// Forward declaration of the tuple template
template <typename... Ts>
struct tuple;

// Tuple specialization for at least one element
template <typename T, typename... Ts>
struct tuple<T, Ts...> {
    T head;
    tuple<Ts...> tail;

    tuple(const T& head, const Ts&... tail) : head(head), tail(tail...) {}

    // Size of the tuple
    constexpr size_t size() const { return 1 + tail.size(); }
};

// Tuple specialization for the last element
template <typename T>
struct tuple<T> {
    T head;

    tuple(const T& head) : head(head) {}

    constexpr size_t size() const { return 1; }
};


// Implementation for `get` requires a way to "count down" to the element we're interested in.
// We'll use a size_t template parameter for the index, and specialize for the base case of 0.

// General template for `get` - peels off the first type until the index is 0
template <size_t idx, typename T, typename... Ts>
struct tuple_getter {
    static auto& get(tuple<T, Ts...>& t) {
        // Recursively call `get` on the tail of the tuple
        return tuple_getter<idx - 1, Ts...>::get(t.tail);
    }
};

// Specialization for `get` when idx is 0 - returns the head of the tuple
template <typename T, typename... Ts>
struct tuple_getter<0, T, Ts...> {
    static T& get(tuple<T, Ts...>& t) {
        return t.head;
    }
};

// User-facing `get` function template
template <size_t idx, typename... Ts>
auto& get(tuple<Ts...>& t) {
    return tuple_getter<idx, Ts...>::get(t);
}

// Helper to apply a function to each element of the tuple
template <typename F, typename T, typename... Ts>
void apply_to_each(F f, tuple<T, Ts...>& t) {
    f(t.head); // Apply the function to the head
    apply_to_each(f, t.tail); // Recurse into the tail
}

// Base case for the recursion
template <typename F, typename T>
void apply_to_each(F f, tuple<T>& t) {
    f(t.head); // Apply the function to the last element
}
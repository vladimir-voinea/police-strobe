#pragma once

#include "tuple.hpp"
#include "Arduino.h"
/*
    variadic template class called "all" that takes any number of types and variables of those types. it has a
    function called apply that takes a lambda and calls the lambda with all the member variables of the class, in sequence.
    does not use std::tuple or any other STL function or class
*/


template <typename... Ts>
class multiple
{
public:
    multiple(Ts... args) : m_args(args...) {}

 template<typename F>
    void apply(F f) {
        apply_impl(f, m_args, make_index_sequence<sizeof...(Ts)>{});
    }

private:
    // Assuming you have a custom tuple implementation
    tuple<Ts...> m_args;

    // Helper to generate a sequence of indices
    template<size_t... Is>
    struct index_sequence {};

    template<size_t N, size_t... Is>
    struct make_index_sequence : make_index_sequence<N-1, N-1, Is...> {};

    template<size_t... Is>
    struct make_index_sequence<0, Is...> : index_sequence<Is...> {};

    // Apply a function to each element of the tuple, using indices
    template<typename F, size_t... Is>
    void apply_impl(F f, const tuple<Ts...>& t, index_sequence<Is...>) {
        (..., (apply_element(f, get<Is>(t), Is)));
    }

    // Function to apply 'f' to a single element, with index for printing
    template<typename F, typename T>
    void apply_element(F f, T& t, size_t index) {
        f(t);
        Serial.print("index: ");
        Serial.println(index);
    }
};
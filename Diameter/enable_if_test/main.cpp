/*
 * main.cpp
 *
 *  Created on: 24 Jul 2016
 *      Author: alanmc
 */

#include <type_traits>

#include <iostream>
#include <string>
#include <memory>

template <typename T>
struct is_shared_ptr : public std::false_type {};

template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : public std::true_type {};

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
void serialise(T& t) {
    std::cout << "Serialising integral types" << std::endl;
}

template <typename T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
void serialise(T& t) {
    std::cout << "Serialising enum types" << std::endl;
}


//template <typename T>
//void serialise(T val) {
//    std::cout << "Serialising Enumeration" << std::endl;
//}


/*
template <>
void serialise<const std::string&>(const std::string& str) {
    std::cout << "Serialising string" << std::endl;
}
*/

int main() {

    int i = 10;

    enum class Test { Hello, World };

    Test t = Test::Hello;

    serialise(i);

    serialise(t);

    std::cout << is_shared_ptr<int>::value << std::endl;

    std::cout << is_shared_ptr<std::shared_ptr<int>>:: value << std::endl;

    return 0;
}



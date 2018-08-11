#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <tuple>
#include <algorithm>
#include <type_traits>


////Vector
template<typename T> struct is_vector : public std::false_type {};
template<typename ...Args>
struct is_vector<std::vector<Args...>> : public std::true_type {};

////List
template<typename T> struct is_list : public std::false_type {};
template<typename ...Args>
struct is_vector<std::list<Args...>> : public std::true_type {};

////String
template<typename T> struct is_string : public std::false_type {};
template<>
struct is_string<std::string> : public std::true_type {};


template<typename T>
void print_ip(const T& ip){
    if constexpr (is_vector<T>::value || is_list<T>::value){
        for (auto v : ip){
            print_ip(v);
        }
    }
    if constexpr (is_string<T>::value){
        std::cout << ip << std::endl;
    }
    if constexpr (std::is_integral<T>::value){
        //Cut byte-by-byte from high to low
        const std::size_t num_bytes=sizeof(T);
        for (size_t i=0; i<num_bytes; i++) {
            std::size_t j=num_bytes-i-1;
            std::cout << (unsigned)(( ip >> (j*8) ) & 0xFF);
            if (i < num_bytes-1) {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

////Tuple
template<std::size_t I = 0, typename ...Args>
typename std::enable_if< (I == sizeof...(Args)), void >::type
print_ip(const std::tuple<Args...>& ip) { }

template<std::size_t I = 0, typename ...Args>
typename std::enable_if< (I < sizeof...(Args)), void >::type
print_ip(const std::tuple<Args...>& ip) {
        print_ip(std::get<I>(ip));
        print_ip<I+1,Args...>(ip);
}



int main() {
    print_ip(char(-1));
    print_ip(short(0));
    print_ip(int(2130706433));
    print_ip(long(8875824491850138409));
    print_ip(std::string("hello"));
    print_ip(std::vector<uint32_t>{1,2,2130706433,4,4130706433});
    print_ip(std::list<long>{1,2,8875824491850138409,4,5});
    print_ip(std::make_tuple<int,short,uint64_t,char>(2130706433,0,8875824491850138409,-1));
    return 0;
}
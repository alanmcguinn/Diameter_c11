#ifndef _DIAMETER_TRAITS_H
#define _DIAMETER_TRAITS_H

namespace Diameter {
namespace Events {

template <typename T>
struct is_shared_ptr : public std::false_type {};

template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : public std::true_type {};

template <typename T>
struct is_shared_ptr<const std::shared_ptr<T>> : public std::true_type {};

template <typename T>
struct is_diameter_base_shared_ptr : public std::false_type {};

template <>
struct is_diameter_base_shared_ptr<std::shared_ptr<Diameter_Base>> : public std::true_type {};

}
}

#endif /* _DIAMETER_TRAITS_H */


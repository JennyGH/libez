#pragma once
#include <map>
#include <list>
#include <vector>
#include <string>
#define __DECLARE_HAS_MEMBER_TRAITS(member)                                                                                                                    \
    template <typename T, typename... Args>                                                                                                                    \
    struct has_member_##member                                                                                                                                 \
    {                                                                                                                                                          \
        template <typename U>                                                                                                                                  \
        static auto check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());                                               \
        template <typename U>                                                                                                                                  \
        static std::false_type check(...);                                                                                                                     \
                                                                                                                                                               \
    public:                                                                                                                                                    \
        static constexpr bool value = std::is_same<decltype(check<T>(0)), std::true_type>::value;                                                              \
    }

namespace ez
{
    namespace base
    {
        __DECLARE_HAS_MEMBER_TRAITS(emplace_back);

        template <class container_type, class serializor_type>
        std::string join(const container_type& container, serializor_type serializor, const std::string& join_element = ",")
        {
            std::string joined;
            auto        iter = container.begin();
            auto        end  = container.end();
            for (; iter != end;)
            {
                joined.append(serializor(*iter));
                if (++iter != end)
                {
                    joined.append(join_element);
                }
            }
            return joined;
        }
        template <class container_type>
        std::string join(const container_type& container, const std::string& join_element = ",")
        {
            std::string joined;
            auto        iter = container.begin();
            auto        end  = container.end();
            for (; iter != end;)
            {
                joined.append(*iter);
                if (++iter != end)
                {
                    joined.append(join_element);
                }
            }
            return joined;
        }

        template <class container_type, class element_type>
        typename std::enable_if<ez::base::has_member_emplace_back<container_type>::value, void>::type _emplace_back(
            container_type& container,
            element_type&&  element)
        {
            container.emplace_back(element);
        }
        template <class container_type, class element_type>
        typename std::enable_if<!ez::base::has_member_emplace_back<container_type>::value, void>::type _emplace_back(
            container_type& container,
            element_type&&  element)
        {
            container.emplace(element);
        }

        template <
            class output_container_type,
            class input_container_type = output_container_type,
            class callback_type        = typename output_container_type::value_type(const typename input_container_type::value_type&)>
        output_container_type map(const input_container_type& container, callback_type callback)
        {
            output_container_type mapped;
            for (const auto& e : container)
            {
                _emplace_back(mapped, callback(e));
            }
            return mapped;
        }

        template <class container_type, class callback_type = bool(const typename container_type::value_type&)>
        container_type filter(const container_type& container, callback_type callback)
        {
            container_type filtered;
            for (const auto& e : container)
            {
                if (callback(e))
                {
                    _emplace_back(filtered, e);
                }
            }
            return filtered;
        }

    } // namespace base
} // namespace ez
#undef __DECLARE_HAS_MEMBER_TRAITS
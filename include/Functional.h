#pragma once
#include "Queue.h"

/**
 * @brief Class for Functional Programming
 * @tparam Container : Datatype with `.begin()`, `.end()`, `.push()`, `.pop()`, `Copyconstrcter`
 * @tparam T : Datatype that goes into Container.
 * @remark : template-template parameter 문법에 대해 더 알고싶다면 [https://wikidocs.net/422](https://wikidocs.net/422) 를 참고해주세요
 */
template <template <typename> class Container, typename T>
class Functional : public Container<T> {
public:
    Functional() {}
    Functional(const Container<T>& base) : Container<T>(base) {}

    template <class F>
    auto map(const F& action) const {
        using ResultType = decltype(action(*this->begin()));
        Functional<Container, ResultType> temp;

        for (const auto& elem : *this) {
            temp.push_(action(elem));
        }

        return temp;
    }

    template <typename Elem>
    Functional<Container, T> push(const Elem& data) const {
        Functional<Container, T> temp(*this);
        temp.push_(data);
        return temp;
    }
};

#ifndef EDYN_PARALLEL_COMPONENT_INDEX_SOURCE_HPP
#define EDYN_PARALLEL_COMPONENT_INDEX_SOURCE_HPP

#include "edyn/comp/shared_comp.hpp"
#include "edyn/util/tuple_util.hpp"
#include <entt/entity/fwd.hpp>
#include <entt/core/type_info.hpp>
#include <type_traits>

namespace edyn {

/**
 * Provides a means to obtain a stable index for each component type, both
 * standard and external. The indices correspond to the location of the
 * component in the tuple of components.
 */
struct component_index_source {
    virtual ~component_index_source() = default;

    template<typename Component, typename IndexType = size_t>
    IndexType index_of() const {
        static_assert(std::is_integral_v<IndexType>);
        if constexpr(tuple_has_type<Component, shared_components_t>::value) {
            return tuple_type_index_of<IndexType, Component, shared_components_t>::value;
        } else {
            // Get external component index by `entt::type_index`.
            return static_cast<IndexType>(index_of_id(entt::type_index<Component>::value()));
        }
    }

    template<typename IndexType, typename... Component>
    auto indices_of() const {
        return std::array<IndexType, sizeof...(Component)>{index_of<Component, IndexType>()...};
    }

    virtual size_t index_of_id(entt::id_type id) const {
        auto idx = SIZE_MAX;
        std::apply([&](auto ... c) {
            ((entt::type_index<decltype(c)>::value() == id ?
                (idx = edyn::tuple_type_index_of<size_t, decltype(c), shared_components_t>::value) : (size_t)0), ...);
        }, shared_components_t{});
        return idx;
    }

    virtual entt::id_type type_id_of(size_t index) const {
        auto id = std::numeric_limits<entt::id_type>::max();

        if (index < std::tuple_size_v<shared_components_t>) {
            visit_tuple(shared_components_t{}, index, [&](auto &&c) {
                id = entt::type_index<std::decay_t<decltype(c)>>::value();
            });
        }

        return id;
    }
};

template<typename... Components>
struct component_index_source_ext : public component_index_source {
    size_t index_of_id(entt::id_type id) const override {
        auto idx = component_index_source::index_of_id(id);

        if (idx == SIZE_MAX) {
            ((entt::type_index<Components>::value() == id ?
                (idx = edyn::index_of_v<size_t, Components, Components...>) : (size_t)0), ...);

            if (idx != SIZE_MAX) {
                constexpr auto num_shared_comp = std::tuple_size_v<shared_components_t>;
                idx += num_shared_comp;
            }
        }

        return idx;
    }

    entt::id_type type_id_of(size_t index) const override {
        auto id = component_index_source::type_id_of(index);
        constexpr auto num_shared_comp = std::tuple_size_v<shared_components_t>;

        if (id == std::numeric_limits<entt::id_type>::max() &&
            index < num_shared_comp  + sizeof...(Components)) {
            visit_tuple(std::tuple<Components...>{}, index - num_shared_comp, [&](auto &&c) {
                id = entt::type_index<std::decay_t<decltype(c)>>::value();
            });
        }

        return id;
    }
};

}

#endif // EDYN_PARALLEL_COMPONENT_INDEX_SOURCE_HPP

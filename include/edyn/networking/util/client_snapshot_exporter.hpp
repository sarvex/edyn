#ifndef EDYN_NETWORKING_UTIL_CLIENT_SNAPSHOT_EXPORTER_HPP
#define EDYN_NETWORKING_UTIL_CLIENT_SNAPSHOT_EXPORTER_HPP

#include <tuple>
#include <entt/entity/fwd.hpp>
#include "edyn/comp/action_list.hpp"
#include "edyn/networking/comp/action_history.hpp"
#include "edyn/networking/comp/network_dirty.hpp"
#include "edyn/networking/comp/networked_comp.hpp"
#include "edyn/networking/packet/registry_snapshot.hpp"
#include "edyn/serialization/memory_archive.hpp"
#include "edyn/util/tuple_util.hpp"

namespace edyn {

class client_snapshot_exporter {
protected:
    template<typename... Components, unsigned... ComponentIndex>
    void export_by_type_id(const entt::registry &registry,
                           entt::entity entity, entt::id_type id,
                           packet::registry_snapshot &snap,
                           [[maybe_unused]] std::tuple<Components...>,
                           std::integer_sequence<unsigned, ComponentIndex...>) const {
        ((entt::type_index<Components>::value() == id ?
            internal::snapshot_insert_entity<Components>(registry, entity, snap, ComponentIndex) : void(0)), ...);
    }

    template<typename... Components>
    void export_all_tuple(const entt::registry &registry, packet::registry_snapshot &snap,
                          const std::tuple<Components...> &components) const {
        internal::snapshot_insert_entity_components_all(registry, snap, components,
                                                        std::make_index_sequence<sizeof...(Components)>{});
    }

    template<typename... Components>
    void export_dirty_tuple(const entt::registry &registry, packet::registry_snapshot &snap,
                            const std::tuple<Components...> &components) const {
        constexpr auto indices = std::make_integer_sequence<unsigned, sizeof...(Components)>{};
        auto network_dirty_view = registry.view<network_dirty>();

        network_dirty_view.each([&](entt::entity entity, const network_dirty &n_dirty) {
            n_dirty.each([&](entt::id_type id) {
                export_by_type_id(registry, entity, id, snap, components, indices);
            });
        });
    }

public:
    virtual ~client_snapshot_exporter() = default;

    // Write all networked entities and components into a snapshot.
    virtual void export_all(const entt::registry &registry, packet::registry_snapshot &snap) const {
        export_all_tuple(registry, snap, networked_components);
    }

    // Write all dirty networked entities and components into a snapshot.
    virtual void export_dirty(const entt::registry &registry, packet::registry_snapshot &snap) const {
        export_dirty_tuple(registry, snap, networked_components);
    }

    void export_actions(const entt::registry &registry, packet::registry_snapshot &snap) const {
        internal::snapshot_insert_all<action_history>(registry, snap,
            tuple_index_of<unsigned, action_history>(networked_components));
    }

    void append_current_actions(entt::registry &registry, double time) const {
        if (m_append_current_actions_func != nullptr) {
            (*m_append_current_actions_func)(registry, time);
        }
    }

protected:
    using append_current_actions_func_t = void(entt::registry &registry, double time);
    append_current_actions_func_t *m_append_current_actions_func {nullptr};
};

template<typename... Components>
class client_snapshot_exporter_ext : public client_snapshot_exporter {

    template<typename Action>
    static void append_actions(entt::registry &registry, double time) {
        auto view = registry.view<action_list<Action>, action_history>();
        for (auto [entity, list, history] : view.each()) {
            if (list.actions.empty()) {
                continue;
            }

            auto data = std::vector<uint8_t>{};
            auto archive = memory_output_archive(data);
            archive(list);
            history.entries.emplace_back(time, std::move(data));
        }
    }

    template<typename... Actions>
    static void append_current_actions(entt::registry &registry, double time) {
        (append_actions<Actions>(registry, time), ...);
    }

public:
    template<typename... Actions>
    client_snapshot_exporter_ext([[maybe_unused]] std::tuple<Actions...>) {
        if constexpr(sizeof...(Actions) > 0) {
            m_append_current_actions_func = &append_current_actions<Actions...>;
        }
    }

    static constexpr auto components_tuple() {
        return std::tuple_cat(networked_components, std::tuple<Components...>{});
    }

    void export_all(const entt::registry &registry, packet::registry_snapshot &snap) const override {
        export_all_tuple(registry, snap, components_tuple());
    }

    void export_dirty(const entt::registry &registry, packet::registry_snapshot &snap) const override {
        export_dirty_tuple(registry, snap, components_tuple());
    }
};

}

#endif // EDYN_NETWORKING_UTIL_CLIENT_SNAPSHOT_EXPORTER_HPP

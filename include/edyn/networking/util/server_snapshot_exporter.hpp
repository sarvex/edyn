#ifndef EDYN_NETWORKING_UTIL_SERVER_SNAPSHOT_EXPORTER_HPP
#define EDYN_NETWORKING_UTIL_SERVER_SNAPSHOT_EXPORTER_HPP

#include <entt/core/type_info.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>
#include "edyn/networking/comp/entity_owner.hpp"
#include "edyn/networking/comp/network_dirty.hpp"
#include "edyn/networking/comp/networked_comp.hpp"
#include "edyn/networking/packet/registry_snapshot.hpp"
#include "edyn/comp/dirty.hpp"

namespace edyn {

extern bool(*g_is_networked_input_component)(entt::id_type);
extern bool(*g_is_action_list_component)(entt::id_type);

class server_snapshot_exporter {
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
                            entt::entity dest_client_entity, const std::tuple<Components...> &components) const {
        auto network_dirty_view = registry.view<network_dirty>();
        auto owner_view = registry.view<entity_owner>();
        constexpr auto indices = std::make_integer_sequence<unsigned, sizeof...(Components)>{};

        for (auto entity : snap.entities) {
            auto owned_by_client = !owner_view.contains(entity) ? false :
                std::get<0>(owner_view.get(entity)).client_entity == dest_client_entity;
            auto [n_dirty] = network_dirty_view.get(entity);

            n_dirty.each([&](entt::id_type id) {
                // Do not include input components of entities owned by destination
                // client as to not override client input on the client-side.
                // Clients own their input.
                if (owned_by_client && ((*g_is_networked_input_component)(id) || (*g_is_action_list_component)(id))) {
                    return;
                }

                export_by_type_id(registry, entity, id, snap, components, indices);
            });
        }
    }

public:
    virtual ~server_snapshot_exporter() = default;

    // Write all networked entities and components into a snapshot.
    virtual void export_all(const entt::registry &registry, packet::registry_snapshot &snap) const {
        export_all_tuple(registry, snap, networked_components);
    }

    // Write all dirty entities and components into a snapshot.
    virtual void export_dirty(const entt::registry &registry, packet::registry_snapshot &snap,
                              entt::entity dest_client_entity) const {
        export_dirty_tuple(registry, snap, dest_client_entity, networked_components);
    }
};

template<typename... Components>
class server_snapshot_exporter_ext : public server_snapshot_exporter {
public:
    static constexpr auto components_tuple() {
        return std::tuple_cat(networked_components, std::tuple<Components...>{});
    }

    void export_all(const entt::registry &registry, packet::registry_snapshot &snap) const override {
        export_all_tuple(registry, snap, components_tuple());
    }

    void export_dirty(const entt::registry &registry, packet::registry_snapshot &snap, entt::entity dest_client_entity) const override {
        export_dirty_tuple(registry, snap, dest_client_entity, components_tuple());
    }
};

}

#endif // EDYN_NETWORKING_UTIL_SERVER_SNAPSHOT_EXPORTER_HPP

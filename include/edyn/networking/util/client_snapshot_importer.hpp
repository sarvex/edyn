#ifndef EDYN_NETWORKING_UTIL_CLIENT_SNAPSHOT_IMPORTER_HPP
#define EDYN_NETWORKING_UTIL_CLIENT_SNAPSHOT_IMPORTER_HPP

#include <entt/entity/registry.hpp>
#include "edyn/comp/merge_component.hpp"
#include "edyn/networking/comp/networked_comp.hpp"
#include "edyn/networking/sys/client_side.hpp"
#include "edyn/networking/packet/registry_snapshot.hpp"

namespace edyn {

class client_snapshot_importer {
protected:
    template<typename Component>
    void import_components(entt::registry &registry, const entity_map &emap,
                           const std::vector<entt::entity> &entities,
                           const pool_snapshot_data_impl<Component> &pool) const {
        for (size_t i = 0; i < pool.entity_indices.size(); ++i) {
            auto entity_index = pool.entity_indices[i];
            auto remote_entity = entities[entity_index];

            if (!emap.contains(remote_entity)) {
                continue;
            }

            auto local_entity = emap.at(remote_entity);

            if (!registry.valid(local_entity)) {
                continue;
            }

            if constexpr(std::is_empty_v<Component>) {
                if (!registry.any_of<Component>(local_entity)) {
                    registry.emplace<Component>(local_entity);
                }
            } else {
                auto comp = pool.components[i];
                internal::map_child_entity(registry, emap, comp);

                if (registry.any_of<Component>(local_entity)) {
                    registry.patch<Component>(local_entity, [&](auto &&current) {
                        merge_component(current, comp);
                    });
                } else {
                    registry.emplace<Component>(local_entity, comp);
                }
            }
        }
    }

    template<typename Component>
    void import_components_local(entt::registry &registry,
                                 const std::vector<entt::entity> &entities,
                                 const pool_snapshot_data_impl<Component> &pool) const {
        for (size_t i = 0; i < pool.entity_indices.size(); ++i) {
            auto entity_index = pool.entity_indices[i];
            auto local_entity = entities[entity_index];

            if (!registry.valid(local_entity)) {
                continue;
            }

            if constexpr(std::is_empty_v<Component>) {
                if (!registry.any_of<Component>(local_entity)) {
                    registry.emplace<Component>(local_entity);
                }
            } else {
                auto &comp = pool.components[i];

                if (registry.any_of<Component>(local_entity)) {
                    registry.patch<Component>(local_entity, [&](auto &&current) {
                        merge_component(current, comp);
                    });
                } else {
                    registry.emplace<Component>(local_entity, comp);
                }
            }
        }
    }

    template<typename... Components>
    void import_tuple(entt::registry &registry, const entity_map &emap, const packet::registry_snapshot &snap,
                      const std::tuple<Components...> &components) const {
        for (auto &pool : snap.pools) {
            visit_tuple(components, pool.component_index, [&](auto &&c) {
                using Component = std::decay_t<decltype(c)>;
                auto *typed_pool = static_cast<pool_snapshot_data_impl<Component> *>(pool.ptr.get());
                import_components(registry, emap, snap.entities, *typed_pool);
            });
        }
    }

    template<typename... Components>
    void import_local_tuple(entt::registry &registry, const packet::registry_snapshot &snap,
                      const std::tuple<Components...> &components) const {
        for (auto &pool : snap.pools) {
            visit_tuple(components, pool.component_index, [&](auto &&c) {
                using Component = std::decay_t<decltype(c)>;
                auto *typed_pool = static_cast<pool_snapshot_data_impl<Component> *>(pool.ptr.get());
                import_components_local<Component>(registry, snap.entities, *typed_pool);
            });
        }
    }

public:
    virtual ~client_snapshot_importer() = default;

    // Load remote snapshot into a registry.
    virtual void import(entt::registry &registry, const entity_map &emap,
                        const packet::registry_snapshot &snap) {
        import_tuple(registry, emap, snap, networked_components);
    }

    // Load a snapshot that's been mapped to local (using
    // `registry_snapshot::convert_remloc`) into a registry.
    virtual void import_local(entt::registry &registry,
                              const packet::registry_snapshot &snap) {
        import_local_tuple(registry, snap, networked_components);
    }
};

template<typename... Components>
class client_snapshot_importer_ext : public client_snapshot_importer {
public:
    static constexpr auto all_components_tuple() {
        return std::tuple_cat(networked_components, std::tuple<Components...>{});
    }

    void import(entt::registry &registry, const entity_map &emap,
                const packet::registry_snapshot &snap) override {
        import_tuple(registry, emap, snap, all_components_tuple());
    }

    void import_local(entt::registry &registry,
                      const packet::registry_snapshot &snap) override {
        import_local_tuple(registry, snap, all_components_tuple());
    }
};

}

#endif // EDYN_NETWORKING_UTIL_CLIENT_SNAPSHOT_IMPORTER_HPP

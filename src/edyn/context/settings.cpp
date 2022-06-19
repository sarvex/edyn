#include "edyn/context/settings.hpp"
#include "edyn/comp/shared_comp.hpp"
#include "edyn/util/registry_operation_builder.hpp"
#include "edyn/parallel/component_index_source.hpp"

namespace edyn {

std::unique_ptr<registry_operation_builder> make_reg_op_builder_default() {
    return std::make_unique<registry_operation_builder>();
}

settings::settings()
    : index_source(new component_index_source)
{}

}

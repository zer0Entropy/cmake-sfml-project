#include <memory>
#include <vector>

#include "component.hpp"
#include "../ui/widget.hpp"

struct WidgetComponent: public Component {
    WidgetNode          node;
};
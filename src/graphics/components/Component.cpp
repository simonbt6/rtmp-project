#include "Component.hpp"

namespace Graphics
{
    Entity* Component::GetEntity() const
    {
        return m_Entity;
    }

    ComponentType* Component::GetType() const
    {
        return nullptr;
    }
}
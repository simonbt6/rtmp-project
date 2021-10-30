#include "Entity.hpp"

namespace Graphics
{
    template<typename T>
    const T* Entity::GetComponent() const
    {
        return GetComponentInternal<T>();
    }

    template<typename T>
    T* Entity::GetComponent()
    {
        return (T*) GetComponentInternal<T>();
    }

    template<typename T>
    const T* Entity::GetComponentInternal() const
    {
        ComponentType* type = T::GetStaticType();
        auto it = m_Components.find(type);
        if (it == m_Components.end())
            return nullptr;
        return (T*) it->second;
    }
}
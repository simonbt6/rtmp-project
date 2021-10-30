#pragma once

#include <string>

namespace Graphics
{
    class Entity;

    struct ComponentType
    {
        std::string name;
    };

    class Component
    {
        protected:
            Entity* m_Entity;

        public:
            virtual Entity* GetEntity() const;
            virtual ComponentType* GetType() const;

    };
};
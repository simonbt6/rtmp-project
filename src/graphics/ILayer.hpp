#pragma once
/**
 * @author Simon Brisebois-Therrien 
 * @date 2021-10-24
 * 
 */



#include <inttypes.h>

namespace Graphics
{
    class Layer
    {
        protected:
            bool m_Visible;

        public:
            Layer(){};
            virtual ~Layer(){};

            inline bool IsVisible() const { return m_Visible; }
            inline void SetVisible(bool visible) { m_Visible = visible; }


            virtual void Init(){};
            virtual void OnEvent(){};
            virtual void OnTick(){};
            virtual void OnUpdate(){};
            virtual void OnUpdateInternal(){};
            virtual void OnRender(){};
        
        protected:
            // virtual bool OnResize(uint32_t width, uint32_t height){return};



    };
};
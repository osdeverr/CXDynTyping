//
//  Interfaceable.h
//  BetterReflection
//
//  Created by Nikita Ivanov on 4/30/20.
//  Copyright © 2020 CXFramework. All rights reserved.
//

#ifndef Interfaceable_h
#define Interfaceable_h
#include <unordered_map>
#include <stdexcept>
#include "Interface.h"

namespace CX
{
    class Interfaceable
    {
    public:
        template<class I>
        bool HasInterface()
        {
            return mImplementedInterfaces[I::CXGetIHandle()] != nullptr;
        }
        
        template<class I>
        I& GetInterface()
        {
            I* pIface = (I*) mImplementedInterfaces[I::CXGetIHandle()];
            if(pIface)
                return *pIface;
            else
                throw std::invalid_argument("Can't GetInterface() an unimplemented interface: did you forget to check HasInterface<I>()?");
        }
    private:
        template<class I>
        void Implement(I* iface)
        {
            auto h = I::CXGetIHandle();
            mImplementedInterfaces[h] = iface;
        }
        
        std::unordered_map<IfaceHandle, void*> mImplementedInterfaces;
    };
}

#endif /* Interfaceable_h */

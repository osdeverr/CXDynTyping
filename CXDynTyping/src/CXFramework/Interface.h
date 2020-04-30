//
//  Interface.h
//  BetterReflection
//
//  Created by Nikita Ivanov on 4/30/20.
//  Copyright © 2020 CXFramework. All rights reserved.
//

#ifndef Interface_h
#define Interface_h

namespace CX
{
    using IfaceHandle = const void*;
    
    template<class I>
    class Interface
    {
    private:
        static char _HandleDummy;
#ifdef CXCFG_HIDE_IHANDLES
        friend class Interfaceable;
#else
    public:
#endif
        static constexpr IfaceHandle CXGetIHandle() {
            return &_HandleDummy;
        }
    };
    template<class I>
    char Interface<I>::_HandleDummy = 0;
}

#endif /* Interface_h */

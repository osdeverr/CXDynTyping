//
//  main.cpp
//  BetterReflection
//
//  Created by Nikita Ivanov on 4/29/20.
//  Copyright © 2020 CXFramework. All rights reserved.
//

#include <iostream>
#include "TypeSystem.h"
#include "Value.h"

void DoStuff(BRefl::Value&& v)
{
    if(v.HasInterface<BRefl::Number>())
    {
        auto& num = v.GetInterface<BRefl::Number>();
        int val = num;
        std::cout << v.GetInterface<BRefl::Typeable>().type().name() << " " << val << std::endl;
        
        if(!num.isConst())
        {
            num = 1920.f;
        }
    }
}

int main(int argc, const char * argv[]) {
    int a = 0;
    DoStuff(a);
    std::cout << a << std::endl;
    
    return 0;
}

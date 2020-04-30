//
//  TypeSystem.h
//  BetterReflection
//
//  Created by Nikita Ivanov on 4/29/20.
//  Copyright © 2020 CXFramework. All rights reserved.
//

#ifndef TypeSystem_h
#define TypeSystem_h
#include <string>
#include <typeinfo>

namespace BRefl
{
    struct Type
    {
        virtual ~Type() {}
        virtual void* allocate() const = 0;
        
        virtual std::string name() const = 0;
        virtual size_t size() const = 0;
        
        virtual bool is(const Type& other) const = 0;
    };
    
    template<typename T>
    class TypeImpl : public Type
    {
    public:
        void* allocate() const {
            if constexpr(std::is_const<T>::value)
                throw std::runtime_error("No RW access to type");
            else {
                return new T;
            }
        }
        std::string name() const
        {
            return typeid(T).name();
        }
        size_t size() const
        {
            return sizeof(T);
        }
        bool is(const Type& other) const
        {
            return name() == other.name();
        }
    };
}

#endif /* TypeSystem_h */

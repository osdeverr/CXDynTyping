//
//  Value.h
//  BetterReflection
//
//  Created by Nikita Ivanov on 4/30/20.
//  Copyright © 2020 CXFramework. All rights reserved.
//

#ifndef Value_h
#define Value_h
#include "CXFramework/Interfaceable.h"
#include "TypeSystem.h"
#include <type_traits>
#include <stdexcept>
#include <functional>

namespace BRefl
{
    struct Value : public CX::Interfaceable
    {
        virtual ~Value() {}
        
        Value() {}
        template<typename T>
        Value(T& val);
        template<typename T>
        Value(const T& val);
    };
    
    struct Typeable : public CX::Interface<Typeable>
    {
        virtual ~Typeable() {}
        virtual Type&& type() const = 0;
    };
    
    template<typename T>
    class TypeableImpl : public Typeable
    {
    public:
        Type&& type() const {
            return TypeImpl<T>();
        }
    };
    
    struct Number : public CX::Interface<Number>
    {
        virtual ~Number() {}
        
        using longest_type = long double;
        
        virtual Number& operator=(const longest_type val) = 0;
        virtual bool isConst() = 0;
        
        template<typename T, typename U = std::enable_if_t<std::is_arithmetic<T>::value>>
        longest_type operator+(const T val) {
            return longest_type(*this) + longest_type(val);
        }
        template<typename T, typename U = std::enable_if_t<std::is_arithmetic<T>::value>>
        longest_type operator-(const T val) {
            return longest_type(*this) - longest_type(val);
        }
        template<typename T, typename U = std::enable_if_t<std::is_arithmetic<T>::value>>
        longest_type operator*(const T val) {
            return longest_type(*this) * longest_type(val);
        }
        template<typename T, typename U = std::enable_if_t<std::is_arithmetic<T>::value>>
        longest_type operator/(const T val) {
            return longest_type(*this) / longest_type(val);
        }
        
        template<typename T, typename U = std::enable_if_t<std::is_arithmetic<T>::value>>
        operator const T() {
            return (const T) operator const longest_type();
        }
        
        template<typename T, typename U = std::enable_if_t<std::is_arithmetic<T>::value>>
        Number& operator=(const T val) {
            *this = (const longest_type) val;
            return *this;
        }
        
        Number() {}
        template<typename T, typename U = std::enable_if_t<std::is_arithmetic<T>::value>>
        Number(T& val);
    protected:
        virtual operator longest_type() = 0;
        virtual operator const longest_type() const = 0;
    };
    
    template<typename T, typename U = std::enable_if_t<std::is_arithmetic<T>::value>>
    class NumberImpl : public Number
    {
    public:
        NumberImpl(T& ref) : mRef(ref) {
            //
        }
        bool isConst() {
            return std::is_const<T>::value;
        }
        operator longest_type() {
            if constexpr(std::is_const<T>::value)
                throw std::runtime_error("No RW access to value");
                
            return longest_type(mRef);
        }
        operator const longest_type() const {
            return longest_type(mRef);
        }
        Number& operator=(const longest_type val) {
            if constexpr(std::is_const<T>::value)
                throw std::runtime_error("No RW access to value");
            else {
                mRef.get() = val;
            }
            return *this;
        }
    private:
        std::reference_wrapper<T> mRef;
    };
    
    template<typename T>
    class ValueImpl : public Value
    {
    public:
        ValueImpl(T& val) : mRef(val) {
            Implement(new TypeableImpl<T>());
            
            if constexpr(std::is_arithmetic<T>::value)
                Implement(new NumberImpl<T>(mRef.get()));
        }
        Type&& type() const {
            return TypeImpl<T>();
        }
    private:
        std::reference_wrapper<T> mRef;
    };
    
    template<typename T>
    Value::Value(T& val) {
        *this = ValueImpl<T>(val);
    }
    template<typename T>
    Value::Value(const T& val) {
        *this = ValueImpl<const T>(val);
    }
    
    template<typename T, typename U>
    Number::Number(T& val) {
        *this = NumberImpl<T>(val);
    }
}

#endif /* Value_h */

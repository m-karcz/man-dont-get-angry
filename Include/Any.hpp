#pragma once

#include <memory>

template<typename Type>
const char* typeNameGenFunc()
{
    return __PRETTY_FUNCTION__;
}

using TypeNameGenFuncType = const char*(*)(void);


struct TypeInfo
{
    template<typename Type>
    static TypeInfo get()
    {
        return TypeInfo{&typeNameGenFunc<Type>};
    }
    template<typename Type>
    static TypeInfo of(const Type&)
    {
        return get<Type>();
    }
    inline bool operator==(const TypeInfo& rhs) const
    {
        return func == rhs.func;
    }
    inline bool operator!=(const TypeInfo& rhs) const
    {
        return not(*this == rhs);
    }
    inline bool operator<(const TypeInfo& rhs) const
    {
        return func < rhs.func;
    }
    std::string name()
    {
        return func();
    }
private:
    TypeInfo(TypeNameGenFuncType func) : func{func}
    {}
    TypeNameGenFuncType func;
};

struct WrongTypeAccessException : std::logic_error
{
    using std::logic_error::logic_error;
};

struct Any
{
    struct Holder
    {
        TypeInfo info;
    };
    template<typename Type>
    struct TypedHolder : Holder
    {
        TypedHolder(const Type& value) : 
            Holder{TypeInfo::get<Type>()},
            value(value)
        {}
        TypedHolder(Type&& value) :
            Holder{TypeInfo::get<Type>()},
            value(value)
        {}
        Type value;
    };
    template<typename Type>
    auto makeTypedHolder(Type&& value)
    {
        return std::make_shared<TypedHolder<std::decay_t<Type>>>(std::forward<Type>(value));
    }
    template<typename Type>
    inline bool has() const
    {
        return holder->info == TypeInfo::get<Type>();
    }
    template<typename Type>
    const Type& get() const
    {
        if(holder and has<Type>())
        {
            return static_cast<TypedHolder<Type>*>(holder.get())->value;
        }
        throw WrongTypeAccessException{holder->info.name()};
    }
    template<typename Type>
    Any(Type&& value)
    {
        holder = makeTypedHolder(std::forward<Type>(value));
    }
private:
    std::shared_ptr<Holder> holder;
};

/* This file is part of the Pangolin Project.
 * http://github.com/stevenlovegrove/Pangolin
 *
 * Copyright (c) 2014 Steven Lovegrove
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <stdexcept>
#include <string.h>
#include <cmath>

#include <pangolin/var/varvalue.h>
#include <pangolin/var/varwrapper.h>
#include <pangolin/var/varstate.h>

namespace pangolin
{

template<typename T>
inline void InitialiseNewVarMetaGeneric(
    VarValue<T>& v, const std::string& name
) {
    // Initialise meta parameters
    const std::vector<std::string> parts = pangolin::Split(name,'.');
    v.Meta().full_name = name;
    v.Meta().friendly = parts.size() > 0 ? parts[parts.size()-1] : "";
    v.Meta().range[0] = 0.0;
    v.Meta().range[1] = 0.0;
    v.Meta().increment = 0.0;
    v.Meta().flags = META_FLAG_NONE;
    v.Meta().logscale = false;
    v.Meta().generic = true;

    VarState::I().NotifyNewVar<T>(name, v);
}

template<typename T>
inline void InitialiseNewVarMeta(
    VarValue<T>& v, const std::string& name,
    double min = 0, double max = 0, int flags = META_FLAG_TOGGLE,
    bool logscale = false
) {
    // Initialise meta parameters
    const std::vector<std::string> parts = pangolin::Split(name,'.');
    v.Meta().full_name = name;
    v.Meta().friendly = parts.size() > 0 ? parts[parts.size()-1] : "";
    v.Meta().range[0] = min;
    v.Meta().range[1] = max;
    if (std::is_integral<T>::value) {
        v.Meta().increment = 1.0;
    } else {
        v.Meta().increment = (max - min) / 100.0;
    }
    v.Meta().flags = flags;
    v.Meta().logscale = logscale;
    v.Meta().generic = false;

    VarState::I().NotifyNewVar<T>(name, v);
}

template<typename T>
class Var
{
public:
    static T& Attach(
        const std::string& name, T& variable,
        double min, double max, bool logscale = false
    ) {
        // Find name in VarStore
        VarValueGeneric*& v = VarState::I()[name];
        if(v) {
            throw std::runtime_error("Var with that name already exists.");
        }else{
            // new VarRef<T> (owned by VarStore)
            VarValue<T&>* nv = new VarValue<T&>(variable);
            v = nv;
            if(logscale) {
                if (min <= 0 || max <= 0) {
                    throw std::runtime_error("LogScale: range of numbers must be positive!");
                }
                InitialiseNewVarMeta<T&>(*nv, name, std::log(min), std::log(max), META_FLAG_TOGGLE, logscale);
            }else{
                InitialiseNewVarMeta<T&>(*nv, name, min, max, META_FLAG_TOGGLE, logscale);
            }
        }
        return variable;
    }

    static T& Attach(
        const std::string& name, T& variable, int flags = META_FLAG_NONE
        ) {
        // Find name in VarStore
        VarValueGeneric*& v = VarState::I()[name];
        if (v) {
            throw std::runtime_error("Var with that name already exists.");
        }
        else{
            // new VarRef<T> (owned by VarStore)
            VarValue<T&>* nv = new VarValue<T&>(variable);
            v = nv;
            InitialiseNewVarMeta<T&>(*nv, name, 0.0, 0.0, flags);
        }
        return variable;
    }

    static T& Attach(
        const std::string& name, T& variable, bool toggle
        ) {
        return Attach(name, variable, toggle ? META_FLAG_TOGGLE : META_FLAG_NONE);
    }

    ~Var()
    {
        delete ptr;
    }

    Var( VarValueGeneric& v )
        : ptr(0)
    {
        InitialiseFromGeneric(&v);
    }


    Var( const std::string& name )
        : ptr(0)
    {
        // Find name in VarStore
        VarValueGeneric*& v = VarState::I()[name];
        if(v && !v->Meta().generic) {
            InitialiseFromGeneric(v);
        }else{
            // new VarValue<T> (owned by VarStore)
            VarValue<T>* nv;
            if(v) {
                // Specialise generic variable
                nv = new VarValue<T>( Convert<T,std::string>::Do( v->str->Get() ) );
                delete v;
            }else{
                nv = new VarValue<T>( T() );
            }
            v = nv;
            var = nv;
            InitialiseNewVarMeta(*nv, name);
        }
    }

    Var(const std::string& name, const T& value, int flags = META_FLAG_NONE)
        : ptr(0)
    {
        // Find name in VarStore
        VarValueGeneric*& v = VarState::I()[name];
        if(v && !v->Meta().generic) {
            InitialiseFromGeneric(v);
        }else{
            // new VarValue<T> (owned by VarStore)
            VarValue<T>* nv;
            if(v) {
                // Specialise generic variable
                nv = new VarValue<T>( Convert<T,std::string>::Do( v->str->Get() ) );
                delete v;
            }else{
                nv = new VarValue<T>(value);
            }
            v = nv;
            var = nv;
            InitialiseNewVarMeta(*nv, name, 0, 1, flags);
        }
    }

    Var(const std::string& name, const T& value, bool toggle)
        : Var(name, value, toggle ? META_FLAG_TOGGLE : META_FLAG_NONE)
    {
    }

    Var(
        const std::string& name, const T& value,
        double min, double max, bool logscale = false
    ) : ptr(0)
    {
        // Find name in VarStore
        VarValueGeneric*& v = VarState::I()[name];
        if(v && !v->Meta().generic) {
            InitialiseFromGeneric(v);
        }else{
            // new VarValue<T> (owned by VarStore)
            VarValue<T>* nv;
            if(v) {
                // Specialise generic variable
                nv = new VarValue<T>( Convert<T,std::string>::Do( v->str->Get() ) );
                delete v;
            }else{
                nv = new VarValue<T>(value);
            }
            var = nv;
            v = nv;
            if(logscale) {
                if (min <= 0 || max <= 0) {
                    throw std::runtime_error("LogScale: range of numbers must be positive!");
                }
                InitialiseNewVarMeta(*nv, name, std::log(min), std::log(max), META_FLAG_TOGGLE, true);
            }else{
                InitialiseNewVarMeta(*nv, name, min, max);
            }
        }
    }

    void Reset()
    {
        var->Reset();
    }

    const T& Get() const
    {
        try{
            return var->Get();
        }catch(const BadInputException &e)
        {
            const_cast<Var<T> *>(this)->Reset();
            return var->Get();
        }
    }

    operator const T& () const
    {
        return Get();
    }

    const T* operator->()
    {
        try{
            return &(var->Get());
        }catch(BadInputException)
        {
            Reset();
            return &(var->Get());
        }
    }

    void operator=(const T& val)
    {
        var->Set(val);
    }

    void operator=(const Var<T>& v)
    {
        var->Set(v.var->Get());
    }

    VarMeta& Meta()
    {
        return var->Meta();
    }

    bool GuiChanged()
    {
        if(var->Meta().gui_changed) {
            var->Meta().gui_changed = false;
            return true;
        }
        return false;
    }

    VarValueT<T>& Ref()
    {
        return *var;
    }

protected:
    // Initialise from existing variable, obtain data / accessor
    void InitialiseFromGeneric(VarValueGeneric* v)
    {
        // Macro hack to prevent code duplication
#   define PANGO_VAR_TYPES(x)                                \
        x(bool) x(int8_t) x(uint8_t) x(int16_t) x(uint16_t)  \
        x(int32_t) x(uint32_t) x(int64_t) x(uint64_t)        \
        x(float) x(double)

        if( !strcmp(v->TypeId(), typeid(T).name()) ) {
            // Same type
            var = (VarValueT<T>*)(v);
        }else if( std::is_same<T,std::string>::value ) {
            // Use types string accessor
            var = (VarValueT<T>*)(v->str);
        }else
#       define PANGO_CHECK_WRAP(x)                           \
        if( !strcmp(v->TypeId(), typeid(x).name() ) ) {      \
            ptr = new VarWrapper<T,x>( *(VarValueT<x>*)v );  \
            var = ptr;                                       \
        }else
        PANGO_VAR_TYPES(PANGO_CHECK_WRAP)
        {
            // other types: have to go via string
            // Wrapper, owned by this object
            ptr = new VarWrapper<T,std::string>( *(v->str) );
            var = ptr;
        }
    }
#undef PANGO_VAR_TYPES
#undef PANGO_CHECK_WRAP

    // Holds reference to stored variable object
    // N.B. mutable because it is a cached value and Get() is advertised as const.
    mutable VarValueT<T>* var;

    // ptr is non-zero if this object owns the object variable (a wrapper)
    VarValueT<T>* ptr;
};

}
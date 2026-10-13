#pragma once
#include <dlfcn.h>
#include <cstddef>
#include <cstdint>

using Il2CppDomain   = void*;
using Il2CppAssembly = void*;
using Il2CppImage    = void*;
using Il2CppClass    = void*;
using Il2CppObject   = void*;
using Il2CppMethod   = void*;
using Il2CppField    = void*;
using Il2CppString   = void*;
using Il2CppArray    = void*;

struct Il2CppApi {
    Il2CppDomain*   (*domain_get)() = nullptr;
    Il2CppAssembly**(*domain_get_assemblies)(Il2CppDomain*, size_t*) = nullptr;
    Il2CppImage*    (*assembly_get_image)(Il2CppAssembly*) = nullptr;
    const char*     (*image_get_name)(Il2CppImage*) = nullptr;
    Il2CppClass*    (*class_from_name)(Il2CppImage*, const char*, const char*) = nullptr;
    Il2CppMethod*   (*class_get_method_from_name)(Il2CppClass*, const char*, int) = nullptr;
    Il2CppField*    (*class_get_field_from_name)(Il2CppClass*, const char*) = nullptr;
    void*           (*runtime_invoke)(Il2CppMethod*, void*, void**, void**) = nullptr;
    void*           (*object_unbox)(Il2CppObject*) = nullptr;
    size_t          (*field_get_offset)(Il2CppField*) = nullptr;
    Il2CppObject*   (*object_new)(Il2CppClass*) = nullptr;
    Il2CppString*   (*string_new)(const char*) = nullptr;
    uint32_t        (*array_length)(Il2CppArray*) = nullptr;
    void*           (*array_addr_with_size)(Il2CppArray*, int, uintptr_t) = nullptr;
    Il2CppClass*    (*object_get_class)(Il2CppObject*) = nullptr;
    const char*     (*class_get_name)(Il2CppClass*) = nullptr;
};

inline Il2CppApi g_api;

inline void Il2CppInit() {
#define L(n) g_api.n = (decltype(g_api.n))dlsym(RTLD_DEFAULT, "il2cpp_" #n)
    L(domain_get); L(domain_get_assemblies); L(assembly_get_image);
    L(image_get_name); L(class_from_name); L(class_get_method_from_name);
    L(class_get_field_from_name); L(runtime_invoke); L(object_unbox);
    L(field_get_offset); L(object_new); L(string_new);
    L(array_length); L(array_addr_with_size); L(object_get_class);
    L(class_get_name);
#undef L
}

inline Il2CppClass* FindClass(const char* ns, const char* name) {
    size_t n = 0;
    auto asms = g_api.domain_get_assemblies(g_api.domain_get(), &n);
    for (size_t i = 0; i < n; ++i) {
        auto img = g_api.assembly_get_image(asms[i]);
        if (!img) continue;
        auto c = g_api.class_from_name(img, ns, name);
        if (c) return c;
    }
    return nullptr;
}

template<typename T>
inline T Field(void* obj, size_t off) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(obj) + off);
}

template<typename T>
inline void SetField(void* obj, size_t off, T v) {
    *reinterpret_cast<T*>(reinterpret_cast<char*>(obj) + off) = v;
}

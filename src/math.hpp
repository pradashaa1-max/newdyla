#pragma once
#include <cmath>

struct Vec3 { float x,y,z; };
struct Vec2 { float x,y; };
struct Quat { float x,y,z,w; };

inline float Len(const Vec3& v){ return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); }
inline Vec3  Sub(const Vec3& a, const Vec3& b){ return {a.x-b.x, a.y-b.y, a.z-b.z}; }
inline Vec3  Add(const Vec3& a, const Vec3& b){ return {a.x+b.x, a.y+b.y, a.z+b.z}; }
inline Vec3  Mul(const Vec3& v, float s){ return {v.x*s, v.y*s, v.z*s}; }
inline Vec3  Norm(Vec3 v){
    float l = Len(v);
    return l < 1e-6f ? Vec3{} : Vec3{v.x/l, v.y/l, v.z/l};
}
inline float Dot(const Vec3& a, const Vec3& b){ return a.x*b.x + a.y*b.y + a.z*b.z; }
inline float AngDeg(const Vec3& from, const Vec3& to, const Vec3& fwd){
    Vec3 d = Norm(Sub(to, from));
    float c = fmaxf(-1.f, fminf(1.f, Dot(d, fwd)));
    return acosf(c) * 57.2957795f;
}
inline Vec3 BoneOffset(int bone){
    // 0 head, 1 neck, 2 chest
    float h = bone==0 ? 1.65f : bone==1 ? 1.45f : 1.15f;
    return {0, h, 0};
}

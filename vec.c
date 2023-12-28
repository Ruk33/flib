typedef union vec {
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
} vec;

vec vadd(vec a, vec b)
{
    vec r = {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w,
    };
    return r;
}

vec vsub(vec a, vec b)
{
    vec r = {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w,
    };
    return r;
}

vec vscale(vec a, float x)
{
    vec r = {
        a.x * x,
        a.y * x,
        a.z * x,
        a.w * x,
    };
    return r;
}

float vlen2(vec src)
{
    float r = 
        src.x * src.x + 
        src.y * src.y +
        src.z * src.z;
    return r;
}

float vlen(vec src)
{
    float r = sqrtf(vlen2(src));
    return r;
}

vec vnorm(vec src)
{
    float len = vlen(src);
    vec r = {0};
    if (len > 0.0f) {
        float ilen = 1 / len;
        r.x = src.x * ilen;
        r.y = src.y * ilen;
        r.z = src.z * ilen;
        r.w = src.w * ilen;
    }
    return r;
}

float vdist2(vec src, vec target)
{
    float dx = target.x - src.x;
    float dy = target.y - src.y;
    float dz = target.z - src.z;
    float r = 
        dx * dx + 
        dy * dy + 
        dz * dz;
    return r;
}

float vdist(vec src, vec target)
{
    float r = sqrtf(vdist2(src, target));
    return r;
}

float vdot(vec a, vec b)
{
    float r = a.x * b.x + a.y * b.y + a.z * b.z;
    return r;
}

vec vcross(vec a, vec b)
{
    vec r = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
    return r;
}

vec vdir2(vec src, vec target)
{
    vec r = vnorm(vsub(target, src));
    return r;
}

float vangle2(vec src, vec target)
{
    float r = acosf(vdot(vnorm(src), vnorm(target)));
    return r;
}

vec vlerp(vec src, vec to, float t)
{
    vec r = {
        (1.0 - t) * src.x + t * to.x,
        (1.0 - t) * src.y + t * to.y,
        (1.0 - t) * src.z + t * to.z,
        (1.0 - t) * src.w + t * to.w,
    };
    return r;
}


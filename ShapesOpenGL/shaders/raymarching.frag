#version 460 core

#define LIGHT_CHECKER  vec3(1., 0.8, 0.)
#define DARK_CHECKER   vec3(0., 0.5, 0.)
#define ZERO_CHECKER vec3(1., 0., 0.)
#define SKY vec3(1., 1., 1.)
//#define SKY vec3(0., 0., 0.)
#define AA  2
#define AAR 0.5

#define PLANE_POS -1.

#define MAX_STEPS 100
#define MAX_DIST 100.
#define CLOSENESS 0.01

#define FOV float(1.2)

#define MAX_NUM_TOTAL_CUBOIDS 1

struct Cuboid
{
    vec3 pos;
    vec3 size;
};

layout (std140) uniform CuboidBlock
{
    Cuboid cuboids[MAX_NUM_TOTAL_CUBOIDS];
};

out vec4 FragColor;

// * camera info
uniform vec3 uCameraPosition;
uniform vec3 uCameraFront;
//uniform vec4 uCameraQuaternion;
//uniform vec2 uCameraNearSize;
//uniform float uCameraNear;
//uniform float uCameraFar;
//uniform float uCameraAspectRatio;
uniform mat4 uView;
in vec2 texCoords;

uniform sampler2D screenTexture;

uniform vec2 uResolution;
uniform float uTime;


vec3 checker(in vec2 uv, in float size) {
    uv /= size;
    if (max(abs(uv.x), abs(uv.y)) < .5) {
        return ZERO_CHECKER;
    }
    vec2 f = floor(uv);
    float isLight = mod(f.x + f.y, 2.);
    //return mix(DARK_CHECKER, LIGHT_CHECKER, isLight);
    return LIGHT_CHECKER;
}

vec2 planeSdf(in vec3 uv) {
    return vec2(uv.y - PLANE_POS, 1.);
}

vec2 ballSdf(in vec3 uv) {
    uv.y /= 1;
    vec3 ball = vec3(0., 0., 0.0);
    float r = 1.;
    return vec2(length(uv - ball) - r, 1.);
}

// rot & translate

mat2 rotate(float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c, -s, s, c);
}

mat3 rotateX(float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    return mat3(
        1.0, 0.0, 0.0,
        0.0, c, -s,
        0.0, s, c
    );
}

mat3 rotateY(float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    return mat3(
        c, -s, 0.0,
        s, c, 0.0,
        0.0, 0.0, 1.0
    );
}

mat3 rotateZ(float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    return mat3(
        c, -s, 0.0,
        s, c, 0.0,
        0.0, 0.0, 1.0
    );
}

vec3 translate(vec3 p, vec3 offset)
{
    return p - offset;
}

// =====

float sdBox( vec3 p, vec3 pos, vec3 b )
{
  // vec3 pos = vec3(0.0, 0.0, 0.0);
  vec3 q = abs(p - pos) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdRotatedBox(vec3 p, vec3 pos, vec3 size, vec3 angles)
{
    p = translate(p, pos);

    // p.xy = rotate(angle) * p.xy;
    p = rotateX(angles.x) * p;
    p = rotateY(angles.y) * p;
    p = rotateZ(angles.z) * p;

    p = translate(p, -pos);

    return sdBox(p, pos, size);
}

float sdSphere( vec3 p, float s )
{
    vec3 pos = vec3(0.0, 0.0, 0.0);
    return length(p - pos)-s;
}

float sdbEllipsoidV2( in vec3 p, in vec3 r )
{
    vec3 pos = vec3(1.0, 0.0, 0.0);
    p -= pos;
    float k1 = length(p/r);
    float k2 = length(p/(r*r));
    return k1*(k1-1.0)/k2;
}

float opIntersection( float d1, float d2 )
{
    return max(d1,d2);
}


// @return distance (x) and id(y)
vec2 sceneSdf(in vec3 uv) {
  vec2 p = planeSdf(uv);
  //vec2 b = ballSdf(uv);
  //if (b.x < p.x) {
  //    return b;
  //}
  return p;
  // return b;
}

float sdScene(in vec3 p)
{
    //float s = sdSphere(p, 1);
    //float e = sdbEllipsoidV2(p, vec3(2.0, 1.0, 1.0));
    // float b = sdBox(p, vec3(3.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
    float b = sdBox(p, cuboids[0].pos, cuboids[0].size);
    // float b = sdRotatedBox(p, vec3(3.0, 0.0, 0.0)
    // , vec3(1.0, 1.0, 1.0)
    // , vec3(radians(0.0), radians(0.0), radians(0.0)));
    // float intersection = opIntersection(e, b);
    //return intersection;
    //if (e < b)
   //     return e;
    return b;
}

float rayMarch(vec3 origin, vec3 direction) {
    float dist = 0.;
    for(int i=0; i < MAX_STEPS; ++i) {
        vec3 ray = origin + direction * dist;
        // float step = sceneSdf(ray).x;
        float step = sdScene(ray);
        dist += step;
        if (step < CLOSENESS || dist > MAX_DIST) break;
    }
    return min(dist, MAX_DIST);
}

mat3 calcLookAtMatrix(vec3 origin, vec3 target, float roll) {
  vec3 rr = vec3(sin(roll), cos(roll), 0.0);
  vec3 ww = normalize(target - origin);
  vec3 uu = normalize(cross(ww, rr));
  vec3 vv = normalize(cross(uu, ww));

  return mat3(uu, vv, ww);
}

void main()
{
    vec4 diffuseColor = texture(screenTexture, texCoords);
    //vec2 uv = (2.0 * gl_FragCoord.xy - uResolution.xy) / uResolution.y;
    vec2 uv = (gl_FragCoord.xy - 0.5 * uResolution.xy) / uResolution.y;
    vec3 eye = uCameraPosition;
    float d = 1.0 / tan(radians(80.0f) / 2.0);
    vec3 projection = normalize(vec3(uv.x, uv.y, d));
    vec3 target = uCameraPosition + uCameraFront;
    float roll = 0.0;
    // mat3 L = uView.xyz;
    mat3 L = calcLookAtMatrix(eye, target, roll);

    vec3 ro = eye;
    vec3 rd = normalize(L * projection);
    float dist = rayMarch(ro, rd);
    vec3 ray = ro + rd * dist;
    float skyMix = smoothstep(MAX_DIST * 0.6, MAX_DIST * 0.8, dist);
    vec3 color = checker(ray.xz, 1.0); //* smoothstep(16., 0., ray.y);
    vec3 final = mix(color, SKY, skyMix);
    // vec3 col;
    // render(col, uv);

    // vec4 rayMarchColor = rayMarch(texCoords, diffuseColor.rgb);
    // vec2 uv = (gl_FragCoord.xy * 2.0f - uResolution.xy) / uResolution.y;
    // gl_FragColor = mix(diffuseColor, col, 1);
    // gl_FragColor = col;
    // gl_FragColor = vec4(mix(diffuseColor.rgb, col, 1), 1.0);
    //FragColor = vec4(color, 1.0);
    FragColor = vec4(mix(diffuseColor.rgb, final, 0.5), 1.0);
}


// https://www.shadertoy.com/view/3tVcDz

// create struct Cuboid
// create struct Ellipsoid
// sdScene function reimplement with loop
// return volume value from the shader
// get the center of intersection volume && color it
// * draw outlines of shapes && implement distance effect
// * draw meshes on shapes

//CPU:
// reimplement support functions for ellipsoid.
// set bool to draw intersection on shader and get volume values
#version 460 core

#define LIGHT_CHECKER  vec3(1., 0.8, 0.)
#define INTERSECTION_COLOR  vec3(0.0, 1.0, 1.0)
#define DARK_CHECKER   vec3(0., 0.5, 0.)
#define ZERO_CHECKER vec3(1., 0., 0.)
#define SKY vec3(1., 1., 1.)
#define COLOR_ELLIPSOID vec3(1.0, 0.0, 0.0)
#define COLOR_CUBOID vec3(0.0, 1.0, 0.0)
//#define SKY vec3(0., 0., 0.)
#define AA  2
#define AAR 0.5

#define PLANE_POS -1.

#define MAX_STEPS 100
#define MIN_STEP 0.01
#define STEP_SIZE 0.1
#define MAX_DIST 100.
#define MIN_DIST 0.01
#define CLOSENESS 0.01

#define FOV float(1.2)

#define NUM_ELLIPSOIDS 3
#define NUM_CUBOIDS 1

#define MAX_NUM_ELLIPSOIDS 100
#define MAX_NUM_CUBOIDS 100

vec3 shapeColors[NUM_ELLIPSOIDS + NUM_CUBOIDS];

struct Ellipsoid
{
    vec3 pos;
    float padding03;
    vec4 size;
};

layout (std140, binding = 0) uniform EllipsoidBlock
{
    Ellipsoid ellipsoids[MAX_NUM_ELLIPSOIDS];
} ellipsoid_block;


struct Cuboid
{
    vec3 pos;
    float padding03;
    vec4 size;
};

layout (std140, binding = 1) uniform CuboidBlock
{
    Cuboid cuboids[MAX_NUM_CUBOIDS];
} cuboid_block;

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

float sdbEllipsoidV2( in vec3 p, in vec3 pos, in vec3 r )
{
    p -= pos;
    float k1 = length(p/r);
    float k2 = length(p/(r*r));
    return k1*(k1-1.0)/k2;
}

float opIntersection( float d1, float d2 )
{
    return max(d1,d2);
}

float opUnion( float d1, float d2 )
{
    return min(d1,d2);
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

float rnd(float i) {
	return mod(4000.*sin(23464.345*i+45.345),1.);
}

float sdScene(in vec3 p, out vec3 resColor)
{
    // Initialize color
    // Define colors for individual shapes
    for (int i = 0; i < NUM_ELLIPSOIDS + NUM_CUBOIDS; i++) {
        if (i < NUM_ELLIPSOIDS)
            shapeColors[i] = COLOR_ELLIPSOID;
        else
            shapeColors[i] = COLOR_CUBOID;
    }
    float shapeDistances[NUM_ELLIPSOIDS + NUM_CUBOIDS];
    for (int i = 0; i < NUM_ELLIPSOIDS + NUM_CUBOIDS; i++) {
        shapeDistances[i] = MAX_DIST;
    }
    // Initialize distance and intersection variables
    float minDist = MAX_DIST;
    float intersection = MAX_DIST;
    float nextIntersection = MAX_DIST;
    int intersectionShapeIndex = -1;
    int closestShapeIndex = -1; // Index of the closest shape
    // Loop through ellipsoids
   for (int i = 0; i < NUM_ELLIPSOIDS; i++) {
        float dist = sdbEllipsoidV2(p, ellipsoid_block.ellipsoids[i].pos, ellipsoid_block.ellipsoids[i].size.xyz / 2.0);
        if (dist < minDist)
        {
            minDist = dist;
            closestShapeIndex = i;
            
        }
        shapeDistances[i] = dist;
    }
    for (int j = 0; j < NUM_CUBOIDS; j++) {
        float dist = sdBox(p, cuboid_block.cuboids[j].pos, cuboid_block.cuboids[j].size.xyz / 2.0);
        if (dist < minDist)
        {
            minDist = dist; 
            closestShapeIndex = NUM_ELLIPSOIDS + j;
            
        }
        shapeDistances[NUM_ELLIPSOIDS + j] = dist;
    }
    // intersection = opIntersection(shapeDistances[0], shapeDistances[1]);
    if (NUM_ELLIPSOIDS + NUM_CUBOIDS > 1)
    {
        //intersection = shapeDistances[0];
        //intersectionShapeIndex = 0;
        for (int i = 0; i < NUM_ELLIPSOIDS + NUM_CUBOIDS; i++)
        {
            if (shapeDistances[i] > intersection && shapeDistances[i] != MAX_DIST || intersection == MAX_DIST) 
            {
                intersection = shapeDistances[i];
                intersectionShapeIndex = i;
            }
        }
    }
    if (intersection != MAX_DIST)
    {
        resColor = shapeColors[intersectionShapeIndex];
        return intersection;
    }
    else
    {
        resColor = vec3(0.0);
        return min(shapeDistances[0], shapeDistances[1]);
    }

    resColor = vec3(0.0);
    return minDist;
    
}

float rayMarch(vec3 origin, vec3 direction, out vec3 resColor) {
    vec3 color = vec3(0.0); // Accumulated color
    float alpha = 0.0; // Accumulated alpha
    float dist = 0;
    for (int i = 0; i < MAX_STEPS; ++i)
    {
        vec3 ray = origin + direction * dist;// MIN_STEP * float(i);

        // Calculate signed distance
        float step = sdScene(ray, color);
        dist += step;

        // Early termination if we're close enough to a surface
        if (abs(dist) < MIN_DIST)
        {
            alpha = 1.0;
            break;
        }
        else if (abs(dist) > MAX_DIST)
        {
            //resColor = LIGHT_CHECKER;
            break;
        }

        // Accumulate color and alpha
        color += (1.0 - alpha) * alpha * resColor;
        alpha += (1.0 - alpha) * STEP_SIZE * dist;

        // Terminate if accumulated alpha is close to 1
    }
    resColor = color;
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
    vec2 uv = (gl_FragCoord.xy - 0.5 * uResolution.xy) / uResolution.y;
    vec3 eye = uCameraPosition;
    float d = 1.0 / tan(radians(80.0f) / 2.0);
    vec3 projection = normalize(vec3(uv.x, uv.y, d));
    vec3 target = uCameraPosition + uCameraFront;
    float roll = 0.0;
    mat3 L = calcLookAtMatrix(eye, target, roll);

    vec3 color = LIGHT_CHECKER;
    vec3 ro = eye;
    vec3 rd = normalize(L * projection);
    float dist = rayMarch(ro, rd, color);
    vec3 ray = ro + rd * dist;
    float skyMix = smoothstep(MAX_DIST * 0.6, MAX_DIST * 0.8, dist);
    vec3 final = mix(color, SKY, skyMix);
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
#version 460 core

/* skew constants for 3d simplex functions */
const float F3 =  0.3333333;
const float G3 =  0.1666667;

const float SPHERE_RADIUS = 6;
const int MAX_SPHERE_COUNT = 24;

struct Sphere {
    vec3 position;
    vec4 quaternion;
    vec3 color;
    float radius;
};

// * camera info
uniform vec3 uCameraPosition;
uniform vec3 uCameraFront;
uniform vec4 uCameraQuaternion;
// uniform vec3 uCameraDirection;
uniform vec2 uCameraNearSize;
uniform float uCameraNear;
uniform float uCameraFar;
uniform float uCameraAspectRatio;

uniform Sphere uSpheres[MAX_SPHERE_COUNT];
uniform int uSpheresCount;

// out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

uniform vec2 uResolution;
uniform float uTime;

const float FOV = 1.3;
const int MAX_STEPS = 256;
const float MIN_DIST = 0.0;
const float MAX_DIST = 500.0;
const float EPSILON = 0.0001;

vec2 map(vec3 p)
{
    float sphereDist = length(p) - 1.0;
    float sphereID = 1.0;
    vec2 sphere = vec2(sphereDist, sphereID);
    vec2 res = sphere;
    return res;
}

vec2 rayMarch(vec3 ro, vec3 rd) 
{
    vec2 hit, object;
    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = ro + object.x * rd;
        hit = map(p);
        object.x += hit.x;
        object.y += hit.y;
        if (abs(hit.x) < EPSILON || object.x > MAX_DIST) break;
    }
    return object;
}

mat3 getCam(vec3 ro, vec3 lookAt) 
{
    vec3 camF = normalize(vec3(lookAt - ro));
    vec3 camR = normalize(cross(vec3(0.0, 1.0, 0.0), camF));
    vec3 camU = cross(camF, camR);
    return mat3(camR, camU, camF);
}

vec3 applyQuaternion(vec3 v, vec4 q) {

		// calculate quat * vector
    vec4 qv = vec4(
    q.w * v.x + q.y * v.z - q.z * v.y,
    q.w * v.y + q.z * v.x - q.x * v.z,
    q.w * v.z + q.x * v.y - q.y * v.x,
    -q.x * v.x - q.y * v.y - q.z * v.z
    );

		// calculate result * inverse quat
    return vec3(
    qv.x  * q.w + qv.w * -q.x + qv.y * -q.z - qv.z * -q.y,
    qv.y * q.w + qv.w * -q.y + qv.z * -q.x - qv.x  * -q.z,
    qv.z * q.w + qv.w * -q.z + qv.x  * -q.y - qv.y * -q.x
    );

}

vec3 getCameraToPixelVector(vec2 coords) {

    vec2 uv = (coords - 0.5) * 2.0;
    // uv.x *= uResolution.x / uResolution.y;

    vec3 rayDirection = normalize(vec3(uv, -FOV));
    // vec3 rayDirection = normalize(vec3(uv + uCameraFront.xy, -FOV + uCameraFront.z));

    // * vector direction correction based on camera rotation
    // rayDirection = applyQuaternion(rayDirection, uCameraQuaternion);

    // * direction of the vector
    // vec3 pixelViewDirection = normalize(cameraToPixelRotated);

    return rayDirection;

}

void render(inout vec3 col, in vec2 uv)
{
    //vec3 ro = vec3(0.0, 0.0, 3.0);
    // vec3 ro = uCameraPosition;
    vec3 ro = uCameraPosition;
    vec3 rd = getCameraToPixelVector(uv);

    vec2 object = rayMarch(ro, rd);

    if (object.x < MAX_DIST) {
        col += 12.0 / object.x;
    }
}

void main()
{
    vec4 diffuseColor = texture(screenTexture, texCoords);
    vec2 uv = (2.0 * gl_FragCoord.xy - uResolution.xy) / uResolution.y;
    vec3 col;
    render(col, uv);

    // vec4 rayMarchColor = rayMarch(texCoords, diffuseColor.rgb);
    // vec2 uv = (gl_FragCoord.xy * 2.0f - u_resolution.xy) / u_resolution.y;
    // gl_FragColor = mix(diffuseColor, col, 1);
    // gl_FragColor = col;
    // gl_FragColor = vec4(mix(diffuseColor.rgb, col, 1), 1.0);
    gl_FragColor = vec4(mix(diffuseColor.rgb, col, 0.5), 1.0);
}

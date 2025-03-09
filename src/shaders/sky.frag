#version 330 core
out vec4 FragColor;
in vec3 TexCoords;

uniform vec3 skyColorBottom;
uniform vec3 skyColorTop;

uniform vec3 lightDirection; // The sun direction in world space
uniform vec2 resolution;

uniform mat4 inv_proj;
uniform mat4 inv_view;

uniform bool enableLensFlare;

#define SUN_DIR normalize(lightDirection)

void raySphereintersectionSkyMap(vec3 rd, float radius, out vec3 startPos) {
    float t;
    vec3 sphereCenter_ = vec3(0.0);
    float radius2 = radius * radius;
    vec3 L = -sphereCenter_;
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, L);
    float c = dot(L, L) - radius2;
    float discr = b * b - 4.0 * a * c;
    t = max(0.0, (-b + sqrt(discr)) / 2);
    startPos = rd * t;
}

vec3 computeClipSpaceCoord(ivec2 fragCoord) {
    vec2 ray_nds = 2.0 * vec2(fragCoord.xy) / resolution.xy - 1.0;
    return vec3(ray_nds, 1.0);
}

vec2 computeScreenPos(vec3 worldDir) {
    vec4 clipPos = inv_proj * vec4(worldDir, 1.0);
    clipPos /= clipPos.w;
    vec2 screenPos = clipPos.xy * 0.5 + 0.5;
    return screenPos;
}

vec3 getSunGlow(const vec3 d, float powExp) {
    float sun = clamp(dot(SUN_DIR, d), 0.0, 1.0);
    return 1.2 * vec3(1.0, 0.8, 0.3) * pow(sun, powExp);
}

// Updated lens flare function with better visibility
float lensFlare(vec2 uv, vec2 sunPos) {
    vec2 d = uv - sunPos;
    float dist = length(d);
    
    // Stronger radial glow
    float glow = exp(-dist * 10.0);

    // Streaks (horizontal and vertical)
    float streaks = exp(-abs(d.x) * 30.0) + exp(-abs(d.y) * 30.0);

    // Ghosting rings (enhanced visibility)
    float ghost = exp(-dist * 8.0) * sin(dist * 25.0);

    // Brighter and more pronounced flares
    return glow + 0.8 * streaks + 0.5 * ghost;
}

vec4 colorCubeMap(vec3 endPos, vec3 d, vec2 fragCoord) {
    vec3 col = mix(skyColorBottom, skyColorTop, clamp(1 - exp(8.5 - 17.0 * clamp(normalize(d).y * 0.5 + 0.5, 0.0, 1.0)), 0.0, 1.0));
    col += getSunGlow(d, 350.0);

    if (enableLensFlare) {
        vec2 sunScreenPos = computeScreenPos(SUN_DIR);
        vec2 uv = fragCoord / resolution;

        float flare = lensFlare(uv, sunScreenPos);
        col += vec3(1.2, 0.9, 0.4) * flare; // Increased brightness for visibility
    }

    return vec4(col, 1.0);
}

void main() {
    ivec2 fragCoord = ivec2(gl_FragCoord.xy);
    vec4 ray_clip = vec4(computeClipSpaceCoord(fragCoord), 1.0);
    vec4 ray_view = inv_proj * ray_clip;
    ray_view = vec4(ray_view.xy, -1.0, 0.0);
    vec3 worldDir = (inv_view * ray_view).xyz;
    worldDir = normalize(worldDir);

    vec3 startPos, endPos;
    raySphereintersectionSkyMap(worldDir, 0.5, endPos);

    vec4 bg = colorCubeMap(endPos, worldDir, fragCoord.xy);
    FragColor = bg;
}

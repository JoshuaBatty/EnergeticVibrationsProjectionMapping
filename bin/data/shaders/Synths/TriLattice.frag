//------------CONTROLABLE PARAMETERS -----------//
//# NUM_TRIANGLES = (1.0) #  <--- SLIDER_1
//# SIZE = (0.0) #       <--- SLIDER 2
//# COLOUR_MIX = (1.0) #     <--- SLIDER_3

vec2 uv2tri(vec2 uv)
{
    float sx = uv.x - uv.y / 2.0; // skewed x
    float offs = step(fract(1.0 - uv.y), fract(sx));
    return vec2(floor(sx) * 2.0 + offs, floor(uv.y));
}

vec3 TriLattice()
{    
    float res = resolution.y / (remap(param1,0.0,1.0,2.0,8.0));
    
    vec2 uv = (gl_FragCoord.xy - resolution.xy / 2.0) / res;
    
    vec3 p = vec3(dot(uv, vec2(1.0, 0.5)), dot(uv, vec2(-1.0, 0.5)), uv.y);
    vec3 p1 = fract(+p);
    vec3 p2 = fract(-p);
    
    // distance from borders
    float d1 = min(min(p1.x, p1.y), p1.z);
    float d2 = min(min(p2.x, p2.y), p2.z);
    float d = min(d1, d2);
    
    // border line
    float thickness = remap(param2,0.0,1.0,0.014,0.2);
    float c = clamp((d - thickness) * res, 0.0, 1.0);
    
    // gradient inside triangles
    float r = rand(uv2tri(uv));
    c *= abs(0.53 - fract(d + r + time * 0.2)) * 2.0;
    
    // color variation
    float cb = sin(time * 4.8 + r * 32.984) * 0.5 + 0.5;
    vec3 rgb = mix(vec3(.760, 0., 0), vec3(1, .5, .5), cb);
    vec3 rgb2 = mix(vec3(.90, 1.5, 1.5), vec3(1, 0.95, 1.5), cb);
    vec3 final_rgb = mix(rgb, rgb2, param3);
    return hue(final_rgb * c,hue_offset);
}


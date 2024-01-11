/*
A large portion of this shader is taken from this:

Shader from Godot Shaders - the free shader library.
godotshaders.com/shader/brick-tiles

Feel free to improve and change this shader according to your needs
and consider sharing the modified result on godotshaders.com.

Tiling effect from https://thebookofshaders.com/09/
*/

shader_type spatial;

// Uniforms
uniform vec3 brick_color : source_color = vec3(0.8, 0.4, 0.2);
uniform vec3 mortar_color : source_color = vec3(0.6, 0.6, 0.6);
uniform float brick_width : hint_range(0.01, 2.0) = 0.2;
uniform float brick_height : hint_range(0.01, 2.0) = 0.1;
uniform float brick_depth : hint_range(0.01, 2.0) = 0.1;

varying vec3 world_position;

void vertex()
{
    world_position = (MODEL_MATRIX * vec4(VERTEX, 1.0)).xyz;
}

vec3 brick_tile(vec3 _st, float _zoom)
{
	_st *= _zoom;

	// Here is where the offset is happening
	_st.x += (step(1.0, mod(_st.y, 2.0)) * 0.5);
	_st.z += (step(1.0, mod(_st.y, 2.0)) * 0.5);
	return fract(_st);
}
float box(vec3 _st, vec3 _size, vec3 _ratio)
{
	_size = (vec3(0.5, 0.5, 0.5) * _size * _ratio);
	vec3 uv = smoothstep(_size, _size + vec3(1e-4), _st);
	uv *= smoothstep(_size, _size + vec3(1e-4), (vec3(1.0) - _st));
	return uv.x * uv.y * uv.z;
}
void fragment() {
    // Calculate brick pattern using world coordinates
	vec3 st = world_position;

	st = brick_tile(st, 2);
    // Create a brick pattern by checking if the fragment is in a brick or mortar region
	vec3 finalColor = mix(mortar_color, brick_color, box(st, vec3(brick_width,brick_height,brick_depth), vec3(0.4,0.4,0.4)));
    // Apply the color to the fragment
    ALBEDO = finalColor;
}

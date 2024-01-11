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

vec2 brick_tile(vec2 _st, float _zoom)
{
	_st *= _zoom;

	// Here is where the offset is happening
	_st.x += (step(1.0, mod(_st.y, 2.0)) * 0.5);

	return fract(_st);
}

void fragment() {
    // Calculate brick pattern using world coordinates
	vec3 st = world_position;
    vec2 brick_pattern = vec2(
        mod(world_position.x * 3.0, 1.0),
        mod(world_position.y * 1.0, 1.0)
    );
	st.x += (step(1.0, mod(world_position.y, 2.0)) * 0.5);
    // Create a brick pattern by checking if the fragment is in a brick or mortar region
    vec3 finalColor = mix(mortar_color, brick_color, step(brick_depth, mod(st,1)));
    // Apply the color to the fragment
    ALBEDO = finalColor;
}

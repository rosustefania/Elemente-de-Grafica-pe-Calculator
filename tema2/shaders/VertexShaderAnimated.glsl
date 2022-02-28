#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_normal;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;


float rand(vec2 co){
  return fract(sin(dot(co ,vec2(12.9898,78.233))) * 43758.5453);
}


void main()
{
	frag_color =  object_color;
	frag_normal = v_normal;
	frag_texture = v_texture;

	float def = rand(0.05 * v_position.xy) * 2;
	vec3 position = v_position * def;
	
	gl_Position = Projection * View * Model * vec4(position , 1.0);
}

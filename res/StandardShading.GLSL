#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace[10];

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
//uniform vec3 LightPosition_worldspace;
uniform vec3  LightPosits[10];
uniform vec3  LightColors[10];
uniform float LightPowers[10];
uniform int   LightCount;

void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	
	// Material properties
	vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	//float distance = length( LightPosition_worldspace - Position_worldspace );

	// Normal of the computed fragment, in camera space
	// Direction of the light (from the fragment to the light)
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	
	// Eye vector (towards the camera)
	vec3 n = normalize( Normal_cameraspace );
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	vec3 l;
	vec3 R;
	float cosTheta;
	float cosAlpha;
	
	color =MaterialAmbientColor; 
	float distance;
		// Ambient : simulates indirect lighting
			
		for (int i=0; i<LightCount;++i)
		{
			l = normalize( LightDirection_cameraspace[i] );
			R = reflect(-l,n);
			cosTheta = clamp( dot( n,l ), 0,1 );
			cosAlpha = clamp( dot( E,R ), 0,1 );
			distance = length(LightPosits[i]-Position_worldspace);
		color +=
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColors[i] *LightPowers[i] * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColors[i] *  LightPowers[i] * pow(cosAlpha,5) / (distance*distance);
		}
}
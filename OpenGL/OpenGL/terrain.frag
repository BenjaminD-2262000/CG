#version 330 core

in vec3 v_Color;
in float Height;
in vec2 texCoord;
in vec3 Position;
in vec3 Normal;
in vec4 fragPosLight;
in vec3 playerPos;


// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D diffuse1;
uniform sampler2D specular1;
uniform sampler2D shadowMap;

// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;
//is the flashlight on
uniform bool flashlightOn;


out vec4 color;


vec4 pointLight()
{	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - Position;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 0.1;
	float b = 0.05;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.05f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - Position);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;


	// Define a threshold height for texture switching
    float thresholdHeight = 13.001f;

    // Define a random factor (range from 0.0 to 1.5f)
    float randomFactor = fract(sin(dot(Position, vec3(12.9898, 78.233, 45.543))) * 43758.5453) * 1.5f;

    // Adjust the threshold height based on the random factor
    float adjustedThreshold = thresholdHeight + randomFactor;

    // Select the texture based on the adjusted threshold height
    if (Height < adjustedThreshold)
		return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
	else
		return (texture(diffuse1, texCoord) * (diffuse * inten + ambient) + texture(specular1, texCoord).r * specular * inten) * lightColor;

}


vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.10f;

	// diffuse lighting
	vec3 normal = normalize(Normal);

	vec3 lightDirection = normalize(vec3(-12.0f, 32.0f, -2.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - Position);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	float shadow = 0.0f;
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);

	}
    // Define a threshold height for texture switching
    float thresholdHeight = 13.001f;

    // Define a random factor (range from 0.0 to 1.5f)
    float randomFactor = fract(sin(dot(Position, vec3(12.9898, 78.233, 45.543))) * 43758.5453) * 1.5f;

    // Adjust the threshold height based on the random factor
    float adjustedThreshold = thresholdHeight + randomFactor;

    // Select the texture based on the adjusted threshold height
    if (Height < adjustedThreshold)
		return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) + ambient) + texture(specular0, texCoord).r * specular  * (1.0f - shadow)) * lightColor;
	else
		return (texture(diffuse1, texCoord) * (diffuse * (1.0f - shadow) + ambient) + texture(specular1, texCoord).r * specular  * (1.0f - shadow)) * lightColor;

}

vec4 spotLight()
{
	vec4 flashLightColor = vec4(0.9f, 0.9f, 0.0f, 1.0f);
	// controls how big the area that is lit up is
	float outerCone = 0.60f;
	float innerCone = 0.65f;

	// ambient lighting
	float ambient = 0.05f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - playerPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - playerPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	    // Define a threshold height for texture switching
    float thresholdHeight = 13.001f;

    // Define a random factor (range from 0.0 to 1.5f)
    float randomFactor = fract(sin(dot(Position, vec3(12.9898, 78.233, 45.543))) * 43758.5453) * 1.5f;

    // Adjust the threshold height based on the random factor
    float adjustedThreshold = thresholdHeight + randomFactor;

    // Select the texture based on the adjusted threshold height
    if (Height < adjustedThreshold)
		return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * flashLightColor;
	else
		return (texture(diffuse1, texCoord) * (diffuse * inten + ambient) + texture(specular1, texCoord).r * specular * inten) * flashLightColor;
}
void main()
{
    float h = (Height + 16)/32.0f;

	if (flashlightOn)
		color = spotLight() + pointLight();
	else
		color = pointLight();
}
#version 330 core

out vec4 FragColor;

in vec3 currentPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;
in vec4 fragPosLight;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
uniform sampler2D shadowMap;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

uniform bool directionalLightEnabled;
uniform vec3 dirLightAngle;
uniform bool depthVisual;
uniform float fogLevel;

vec4 pointLight()
{
    vec3 lightVec = lightPos - currentPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(texture(normal0, texCoord).xyz * 2.0 - 1.0);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

  // Specular lighting
  float specularLight = 0.50f;
  vec3 viewDirection = normalize(camPos - currentPos);
  vec3 reflectionDirection = reflect(-lightDirection, normal);

  vec3 halfwayVec = normalize(viewDirection + lightDirection);

  float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
  float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 directionalLight()
{
    // ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

  // Specular lighting
  float specularLight = 0.50f;
  vec3 viewDirection = normalize(camPos - currentPos);
  vec3 reflectionDirection = reflect(-lightDirection, normal);

  vec3 halfwayVec = normalize(viewDirection + lightDirection);

  float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
  float specular = specAmount * specularLight;

  float shadow = 0.0;
  vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
  if (lightCoords.z <= 1.0) {
    lightCoords = (lightCoords + 1.0) / 2.0;
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

	return (texture(diffuse0, texCoord) * (diffuse * (1.0 - shadow) + ambient) + texture(specular0, texCoord).r * specular * (1.0 - shadow)) * lightColor;
}

vec4 spotLight()
{
    // controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currentPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

float near = 0.1;
float far = 100.0;

float linearDepth(float depth) {
  return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.5, float offset = 30.0) {
  float zVal = linearDepth(depth);
  return (1 / (1 + exp(-steepness * (zVal - offset))));
}

void main() {
    if (directionalLightEnabled) {
        if (depthVisual) {
          float depth = logisticDepth(gl_FragCoord.z, 0.5, fogLevel);
          FragColor = (directionalLight() + pointLight()) * (1.0 - depth) + vec4(depth * vec3(0.85, 0.85, 0.90), 1.0);
        }
        else {
          FragColor = directionalLight() + pointLight();
        }
    } else {
        if (depthVisual) {
          float depth = logisticDepth(gl_FragCoord.z, 0.5, fogLevel);
          FragColor = pointLight() * (1.0 - depth) + vec4(depth * vec3(0.85, 0.85, 0.90), 1.0);
        }
        else {
          FragColor = pointLight();
        }
    }

    // FragColor = spotLight();
}

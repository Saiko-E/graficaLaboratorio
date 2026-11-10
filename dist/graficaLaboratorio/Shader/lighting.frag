#version 330 core

#define NUMBER_OF_POINT_LIGHTS 8
#define NUMBER_OF_SPOT_LIGHTS 4

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLights[NUMBER_OF_SPOT_LIGHTS];
uniform Material material;
uniform int transparency;
uniform int useImportedModelTexture;
uniform int cameraNeutralMaterial;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

vec4 FetchDiffuse() {
    return (useImportedModelTexture == 1) ? texture(texture_diffuse1, TexCoords) : texture(material.diffuse, TexCoords);
}
vec3 FetchSpecular() {
    return (useImportedModelTexture == 1) ? texture(texture_specular1, TexCoords).rgb : texture(material.specular, TexCoords).rgb;
}

// Function prototypes
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main( )
{
    if (cameraNeutralMaterial == 1) {
        vec4 albedo = FetchDiffuse();
        if (albedo.a < 0.1) discard;

        vec3 n = normalize(Normal);
        vec3 lightDir = normalize(vec3(-0.35, 0.85, 0.50));
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 halfDir = normalize(lightDir + viewDir);

        float diffuse = max(dot(n, lightDir), 0.0);
        float highlight = pow(max(dot(n, halfDir), 0.0), 64.0);
        // Keep the imported camera colors intact, including the Liga MX logo.
        vec3 finalColor = albedo.rgb * (0.76 + 0.28 * diffuse)
                        + vec3(0.94, 0.96, 1.0) * highlight * 0.06;

        color = vec4(clamp(finalColor, 0.0, 1.0), albedo.a);
        return;
    }
    // Properties
    vec3 norm = normalize( Normal );
    vec3 viewDir = normalize( viewPos - FragPos );
    
    // Directional lighting
    vec3 result = CalcDirLight( dirLight, norm, viewDir );
    
    // Point lights
    for ( int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++ )
    {
        result += CalcPointLight( pointLights[i], norm, FragPos, viewDir );
    }
    
    // Floodlights
    for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
 	
    color = vec4(result, FetchDiffuse().a);
	  if(color.a < 0.1 && (transparency==1 || useImportedModelTexture==1))
        discard;

}

// Calculates the color when using a directional light.
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize( -light.direction );
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Combine results
    vec3 ambient = light.ambient * FetchDiffuse().rgb;
    vec3 diffuse = light.diffuse * diff * FetchDiffuse().rgb;
    vec3 specular = light.specular * spec * FetchSpecular();
    
    return ( ambient + diffuse + specular );
}

// Calculates the color when using a point light.
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );
    
    // Combine results
    vec3 ambient = light.ambient * FetchDiffuse().rgb;
    vec3 diffuse = light.diffuse * diff * FetchDiffuse().rgb;
    vec3 specular = light.specular * spec * FetchSpecular();
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ( ambient + diffuse + specular );
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );
    
    // Spotlight intensity
    float theta = dot( lightDir, normalize( -light.direction ) );
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );
    
    // Combine results
    vec3 ambient = light.ambient * FetchDiffuse().rgb;
    vec3 diffuse = light.diffuse * diff * FetchDiffuse().rgb;
    vec3 specular = light.specular * spec * FetchSpecular();
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return ( ambient + diffuse + specular );
}

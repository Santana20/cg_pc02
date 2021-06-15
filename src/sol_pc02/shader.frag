#version 330 core

in vec3 color;
in vec2 textCoord;

in vec3 Normal;
in vec3 crntPos;

// OUTS
out vec4 FragColor;


//UNIFORMS
uniform sampler2D tex0;

uniform vec4 lightColor;

uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
  float ambient = 0.35f;

  vec3 normal = normalize(Normal);
  vec3 lightDirection = normalize(lightPos - crntPos);

  float diffuse = max(dot(normal, lightDirection), 0.0f);

  
  float specularLight = 0.5f;
  vec3 viewDirection = normalize(camPos - crntPos);
  vec3 reflectionDirection = reflect(-lightDirection, normal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 32);
  float specular = specAmount * specularLight;


  FragColor = texture(tex0, textCoord) * lightColor * (diffuse + ambient + specular);
}
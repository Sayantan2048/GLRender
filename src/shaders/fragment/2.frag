#version 450 core

#define PI 3.14159265359
#define alpha 0.15
#define specularCoef 0.7

uniform vec3 camPos = vec3(0, 0, 0);
uniform vec3 lightPos = vec3(0, 0, 5);
uniform float power = 5;

in GS_OUT { 
  vec3 pNormal;
  vec4 color;
  vec4 modelPos;
} fs_in;

out vec4 out_Color;

float ggxDist(const vec3 h, const vec3 n) {
  float cosine_sq = dot(h, n);
  cosine_sq *= cosine_sq;
  float tan_sq = 1.0 / cosine_sq - 1.0;
  float alpha_sq = alpha * alpha;

  tan_sq += alpha_sq;
  tan_sq *= tan_sq;
  float D = alpha_sq / (PI * cosine_sq * cosine_sq * tan_sq);

  return D;
}

float ggxG1(const vec3 v, const vec3 h, const vec3 n) {
  float cosv_sq = dot(v, n);
  cosv_sq *= cosv_sq;
  float tanv_sq = 1.0 / cosv_sq - 1.0;

  if (tanv_sq <= 1e-15)
    return 1.0;
  else if (dot(h, v) <= 1e-15)
    return 0.0;

  float alpha_sq = alpha * alpha;

  tanv_sq *= alpha_sq;
  tanv_sq += 1.0;
  tanv_sq = 1.0 + sqrt(tanv_sq);

  return 2.0/tanv_sq;
}

float ggxBrdf(const vec3 wo, const vec3 wi, const vec3 n) {
    vec3 h = normalize(wo + wi);
    
    float D = ggxDist(h, n);
    float G = ggxG1(wo, h, n) * ggxG1(wi, h, n);

    return (specularCoef * D * G) / (4.0 * dot(wi, n) * dot(wo, n)) + (1 - specularCoef) / PI;
}

void main(void)
{
  vec3 camDir = camPos - fs_in.modelPos.xyz;
  vec3 lightDir = normalize(lightPos - fs_in.modelPos.xyz);
  
  vec3 surfNorm = normalize(fs_in.pNormal * dot(fs_in.pNormal, camDir));
  float cosine = dot(lightDir, surfNorm);
  
  out_Color = fs_in.color * power * (cosine < 0 ? 0 : cosine * ggxBrdf(normalize(camDir), lightDir, surfNorm));
}
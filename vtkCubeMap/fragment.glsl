//VTK::System::Dec
//VTK::Output::Dec

in vec3 normalVCVSOutput;

uniform samplerCube cubemap;

void main()
{
  gl_FragColor = texture(cubemap, normalVCVSOutput);
}
//VTK::System::Dec
//VTK::Output::Dec
varying vec3 tcoordVC;
uniform samplerCube cubemap;

void main()
{
  gl_FragColor = texture(cubemap, tcoordVC);
}

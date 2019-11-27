//VTK::System::Dec

attribute vec4 vertexMC;

attribute vec3 tcoordMC;
varying vec3 tcoordVC;
void main()
{
  tcoordVC = tcoordMC;
  gl_Position = vertexMC;
}

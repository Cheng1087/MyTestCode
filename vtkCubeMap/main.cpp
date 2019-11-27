#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkImageData.h"
#include "vtkImageFlip.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkJPEGReader.h"
#include "vtkNew.h"
#include "vtkOpenGLPolyDataMapper.h"
#include "vtkOpenGLRenderWindow.h"
#include "vtkPLYReader.h"
#include "vtkECPolyDataNormals.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSkybox.h"
#include "vtkSmartPointer.h"
#include "vtkOpenGLTexture.h"
#include "vtkSTLReader.h"
#include "vtkTextureMapToSphere.h"
#include "vtkOpenGLRenderer.h"
#include "vtkECPolyDataNormals.h"
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
int cubemap();
int skybox();
int useShader(vtkOpenGLPolyDataMapper* mapper, int flag);
std::string ReadGlslFile(const char* path);
int main()
{
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.0, 0.0, 0.0);
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->SetSize(400, 400);
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(renderWindow);
    vtkNew<vtkOpenGLTexture> texture;
    texture->CubeMapOn();
	texture->SetMipmap(1);
	
	//texture->SetTextureType()
    vtkNew<vtkPLYReader> reader;
    reader->SetFileName("D:\\meshes\\obj\\bunny.ply");
	reader->Update();
    vtkNew<vtkSTLReader> reader2;
    reader2->SetFileName("D:\\QT\\ECV\\ECVTmp\\testECVisualization\\Topo.stl");
	reader2->Update();
	vtkSmartPointer<vtkPolyData> poly = vtkPolyData::New();
	poly = reader->GetOutput();
	
	vtkNew<vtkPolyDataNormals> norms;
    vtkNew<vtkECPolyDataNormals> norms;
    norms->SetInputConnection(reader->GetOutputPort());
	

	//norms->SetInputConnection(textureMap->GetOutputPort());
    for (int i = 0; i < 6; i++)
    {
        vtkNew<vtkJPEGReader> imgReader;
        //imgReader->SetFileName(vtkTestUtilities::ExpandDataFileName(argc, argv, fpath[i]));
        //imgReader->SetFileName("D:\\build\\vtk-v8.1.2\\bin\\ExternalData\\Testing\\Data\\skybox-nz.jpg");
        imgReader->SetFileName("D:\\QT\\ECV\\ECVTmp\\testECVisualization\\face.jpg");
        //imgReader->SetFileName("D:\\build\\vtk-v8.1.2\\bin\\ExternalData\\Testing\\Data\\skybox\\negx.jpg");

        vtkNew<vtkImageFlip> flip;
        flip->SetInputConnection(imgReader->GetOutputPort());
        flip->SetFilteredAxis(1); // flip y axis
        texture->SetInputConnection(i, flip->GetOutputPort(0));
    }

    vtkNew<vtkOpenGLPolyDataMapper> mapper;
    mapper->SetInputConnection(norms->GetOutputPort());
	//mapper->SetInputConnection(textureMap->GetOutputPort());

    std::string vs = ReadGlslFile("D:\\QT\\ECV\\ECVTmp\\shader\\vertex.glsl");
	//std::string vs = ReadGlslFile("D:\\WEBSource\\TexturedSphere\\vertex.glsl");
	mapper->SetVertexShaderCode(vs.c_str());

    std::string fs = ReadGlslFile("D:\\QT\\ECV\\ECVTmp\\shader\\fragment.glsl");
	//std::string fs = ReadGlslFile("D:\\WEBSource\\TexturedSphere\\fragment.glsl");
	mapper->SetFragmentShaderCode(fs.c_str());

    vtkNew<vtkActor> actor;
    renderer->AddActor(actor);
    actor->SetTexture(texture);
    actor->SetMapper(mapper);

    //useShader(mapper, 1);

    //renderer->ResetCamera();
    //renderer->GetActiveCamera()->Zoom(1.4);
    renderWindow->Render();

    iren->Start();
    return 1;
}

int skybox()
{
	vtkNew<vtkRenderer> renderer;
	renderer->SetBackground(0.0, 0.0, 0.0);
	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->SetSize(400, 400);
	renderWindow->AddRenderer(renderer);
	vtkNew<vtkRenderWindowInteractor> iren;
	iren->SetRenderWindow(renderWindow);

//	vtkNew<vtkLight> light;
//	light->SetLightTypeToSceneLight();
//	light->SetPosition(1.0, 7.0, 1.0);
//	renderer->AddLight(light);

	vtkNew<vtkPLYReader> reader;
	reader->SetFileName("D:\\meshes\\obj\\bunny.ply");

	vtkNew<vtkECPolyDataNormals> norms;
	norms->SetInputConnection(reader->GetOutputPort());

	const char* fpath[] =
	{
	"Data/skybox/posx.jpg",
	"Data/skybox/negx.jpg",
	"Data/skybox/posy.jpg",
	"Data/skybox/negy.jpg",
	"Data/skybox/posz.jpg",
	"Data/skybox/negz.jpg"
	};

	vtkNew<vtkTexture> texture;
	texture->CubeMapOn();
	texture->InterpolateOn();
	texture->RepeatOff();
	texture->EdgeClampOn();

	// mipmapping works on many systems but is not
	// core 3.2 for cube maps. VTK will silently
	// ignore it if it is not supported. We commented it
	// out here to make valid images easier
	// texture->MipmapOn();

	for (int i = 0; i < 6; i++)
	{
		vtkNew<vtkJPEGReader> imgReader;
		imgReader->SetFileName("D:\\QT\\ECV\\ECVTmp\\testECVisualization\\face.jpg");
		vtkNew<vtkImageFlip> flip;
		flip->SetInputConnection(imgReader->GetOutputPort());
		flip->SetFilteredAxis(1); // flip y axis
		texture->SetInputConnection(i, flip->GetOutputPort(0));
	}

	vtkNew<vtkOpenGLPolyDataMapper> mapper;
	mapper->SetInputConnection(norms->GetOutputPort());

	vtkNew<vtkActor> actor;
	actor->SetPosition(0, 0, 0);
	actor->SetScale(6.0, 6.0, 6.0);
	actor->GetProperty()->SetSpecular(0.8);
	actor->GetProperty()->SetSpecularPower(20);
	actor->GetProperty()->SetDiffuse(0.1);
	actor->GetProperty()->SetAmbient(0.1);
	actor->GetProperty()->SetDiffuseColor(1.0, 0.0, 0.4);
	actor->GetProperty()->SetAmbientColor(0.4, 0.0, 1.0);
	renderer->AddActor(actor);
	actor->SetTexture(texture);
	actor->SetMapper(mapper);

	useShader(mapper, 0);

	vtkNew<vtkSkybox> world;
	world->SetTexture(texture);
	renderer->AddActor(world);
	
	//world->SetVisibility(0);

	renderer->GetActiveCamera()->SetPosition(0.0, 0.55, 2.0);
	renderer->GetActiveCamera()->SetFocalPoint(0.0, 0.55, 0.0);
	renderer->GetActiveCamera()->SetViewAngle(60.0);
	renderer->GetActiveCamera()->Zoom(1.1);
	renderer->GetActiveCamera()->Azimuth(0);
	renderer->GetActiveCamera()->Elevation(5);
	renderer->GetActiveCamera()->Roll(-10);
	renderer->ResetCameraClippingRange();

	renderWindow->Render();

	vtkNew<vtkInteractorStyleTrackballCamera> style;
	renderWindow->GetInteractor()->SetInteractorStyle(style);

	iren->Start();


	return 1;
}

int cubemap()
{
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.0, 0.0, 0.0);
	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->SetSize(400, 400);
	renderWindow->AddRenderer(renderer);
	vtkNew<vtkRenderWindowInteractor> iren;
	iren->SetRenderWindow(renderWindow);

    vtkNew<vtkTexture> texture;
//	vtkNew<vtkOpenGLTexture> texture;
	texture->CubeMapOn();

	vtkNew<vtkPLYReader> reader;
	reader->SetFileName("D:\\meshes\\obj\\bunny.ply");
	vtkNew<vtkSTLReader> reader2;
	reader2->SetFileName("D:\\QT\\ECV\\ECVTmp\\testECVisualization\\Topo.stl");
	vtkNew<vtkECPolyDataNormals> norms;
	//norms->SetInputConnection(reader->GetOutputPort());
	norms->SetInputConnection(reader->GetOutputPort());

	const char* fpath[] =
	{
	"Data/skybox-px.jpg",
	"Data/skybox-nx.jpg",
	"Data/skybox-py.jpg",
	"Data/skybox-ny.jpg",
	"Data/skybox-pz.jpg",
	"Data/skybox-nz.jpg"
	};

	for (int i = 0; i < 6; i++)
	{
		vtkNew<vtkJPEGReader> imgReader;
		//imgReader->SetFileName(vtkTestUtilities::ExpandDataFileName(argc, argv, fpath[i]));
		//imgReader->SetFileName("D:\\build\\vtk-v8.1.2\\bin\\ExternalData\\Testing\\Data\\skybox-nz.jpg");
		imgReader->SetFileName("D:\\QT\\ECV\\ECVTmp\\testECVisualization\\face.jpg");
		//imgReader->SetFileName("D:\\build\\vtk-v8.1.2\\bin\\ExternalData\\Testing\\Data\\skybox\\negx.jpg");

		vtkNew<vtkImageFlip> flip;
		flip->SetInputConnection(imgReader->GetOutputPort());
		flip->SetFilteredAxis(1); // flip y axis
		texture->SetInputConnection(i, flip->GetOutputPort(0));
	}

	vtkNew<vtkOpenGLPolyDataMapper> mapper;
	mapper->SetInputConnection(norms->GetOutputPort());

	vtkNew<vtkActor> actor;
	renderer->AddActor(actor);
	actor->SetTexture(texture);
	actor->SetMapper(mapper);



	//renderer->ResetCamera();
	//renderer->GetActiveCamera()->Zoom(1.4);
	renderWindow->Render();

	vtkNew<vtkInteractorStyleTrackballCamera> style;
	renderWindow->GetInteractor()->SetInteractorStyle(style);


	iren->Start();
	return 1;
}
int useShader(vtkOpenGLPolyDataMapper* mapper, int flag)
{
	if (flag == 0)
	{
		mapper->AddShaderReplacement(
			vtkShader::Vertex,
			"//VTK::PositionVC::Dec", // replace
			true, // before the standard replacements
			"//VTK::PositionVC::Dec\n" // we still want the default
			"varying vec3 TexCoords;\n",
			false // only do it once
		);
		mapper->AddShaderReplacement(
			vtkShader::Vertex,
			"//VTK::PositionVC::Impl", // replace
			true, // before the standard replacements
			"//VTK::PositionVC::Impl\n" // we still want the default
			"vec3 camPos = -MCVCMatrix[3].xyz * mat3(MCVCMatrix);\n"
			"TexCoords.xyz = reflect(vertexMC.xyz - camPos, normalize(normalMC));\n",
			false // only do it once
		);
		mapper->AddShaderReplacement(
			vtkShader::Fragment,
			"//VTK::Light::Dec", // replace
			true, // before the standard replacements
			"//VTK::Light::Dec\n" // we still want the default
			"varying vec3 TexCoords;\n",
			false // only do it once
		);
		mapper->AddShaderReplacement(
			vtkShader::Fragment,
			"//VTK::Light::Impl", // replace
			true, // before the standard replacements
			"  vec3 cubeColor = texture(texture_0, normalize(TexCoords)).xyz;\n"
			"//VTK::Light::Impl\n"
			"  gl_FragData[0] = vec4(ambientColor + diffuse + specular + specularColor*cubeColor, opacity);\n"
			, // we still want the default
			false // only do it once
		);
	}
	if (flag == 1)
	{
		mapper->AddShaderReplacement(
			vtkShader::Vertex,
			"//VTK::Normal::Dec",  // replace the normal block
			true,                  // before the standard replacements
			"//VTK::Normal::Dec\n" // we still want the default
			"  out vec4 myVertexMC;\n",
			false // only do it once
		);
		mapper->AddShaderReplacement(
			vtkShader::Vertex,
			"//VTK::Normal::Impl",  // replace the normal block
			true,                   // before the standard replacements
			"//VTK::Normal::Impl\n" // we still want the default
			"  myVertexMC = vertexMC;\n",
			false // only do it once
		);
	}
	return 1;
}

std::string ReadGlslFile(const char* path)
{
	std::string str;
	std::ifstream VertexShaderStream(path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			str += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		throw std::exception("ReadGlslFile error!!!");
	}
	return str;
}
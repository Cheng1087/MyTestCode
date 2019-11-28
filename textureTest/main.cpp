#include <QApplication>
#include <vtkGenericOpenGLRenderWindow.h>
#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "vtkJPEGReader.h"
#include "vtkTexture.h"
#include "src/PrepareBlock3D.h"
#include "src/BlockMaterialVisualObject.h"
#include "vtkQImageToImageSource.h"
#include "vtkOBJReader.h"
#include "vtkSTLReader.h"
#include "vtkImageFlip.h"
#include "vtkImageData.h"
#include "gtest/gtest.h"
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
using namespace ECVG;
using namespace ECV_TEST;
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    QApplication app(argc,argv);
    Q_INIT_RESOURCE(textures);
    QTVtkBlockWindow win;
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground2(1.0, 1.0, 1.0);
    renderer->SetBackground(0, 0, 0);
    renderer->SetGradientBackground(1);
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow
            = vtkGenericOpenGLRenderWindow::New();

    renderer->SetRenderWindow(renderWindow);
    renderer->ResetCamera();
    win.setWin(renderer->GetRenderWindow(),renderer);
    bool flag;
    flag = 0;
    if(flag)
    {
        win.setTextures();
        std::vector<Block3D_ptr> blocks = getAllBlocks();
        win.setBlocks(blocks);
        win.showBlocks();
    }
    if(!flag)
    {
        win.setCubeMapTextures();
        vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
        reader->SetFileName("D:\\MyTestCode\\textureTest\\meshes\\bunny.obj");
        reader->Update();
        vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
        polydata = reader->GetOutput();
        win.testBunny(polydata);
    }
    win.render();
    win.show();
    win.resize(1240,760);
    return QApplication::exec();
}


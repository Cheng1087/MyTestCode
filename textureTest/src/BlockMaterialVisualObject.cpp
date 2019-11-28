#include "BlockMaterialVisualObject.h"

#include <QVTKOpenGLWidget.h>
#include <QApplication>
#include <QScreen>
#include <QtGui/QKeyEvent>
#include <QtGui/QWindow>
#include <QtCore/QTimer>
#include <QCheckBox>
#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkPolygon.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkActor.h>
#include <QVTKWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkProperty.h>
#include <vtkAssembly.h>
#include <vtkTexture.h>
#include <vtkTextureMapToPlane.h>
#include <vtkQImageToImageSource.h>
#include <vtkTextureMapToSphere.h>
#include <vtkTextureMapToCylinder.h>
#include <vtkImplicitTextureCoords.h>
#include <vtkThresholdTextureCoords.h>
#include <vtkTransformTextureCoords.h>
#include <vtkTriangularTCoords.h>
#include <vtkAppendPolyData.h>
#include <vtkVolumeTexture.h>
#include "vtkTexturingHelper.h"
#include <vtkOpenGLPolyDataMapper.h>
#include "ECMath/Point.h"
#include "Dataset/ComputingElements/Block.h"
#include "Dataset/TopologyCells/Cell3D.h"
#include "vtkTextureMapToECObject.h"
#include "vtkECPolyDataNormals.h"
#include "vtkTextureMapToBox.h"
using namespace ECVG;
using namespace ECB;
using namespace ECMath;
using namespace std;

/**
 * @brief QTVtkWindow::QTVtkWindow
 * @param parent
 * create ui, add signals and slots
 */
QTVtkBlockWindow::QTVtkBlockWindow(QWidget *parent)
    :QWidget (parent)
{
    m_VTKWindow = new QVTKOpenGLWidget(this);
    m_VTKWindow->setObjectName(QString::fromUtf8("m_VTKWindow"));
    m_VTKWindow->setGeometry(QRect(0, 0, 1240, 760));
    m_VTKWindow->setEnableHiDPI(true);
    _assembly = vtkAssembly::New();
    _vtkPts = vtkPoints::New();
}
QTVtkBlockWindow::~QTVtkBlockWindow()
{

}
/**
 * @brief QTVtkWindow::render
 * render the vtkwindow
 */
void QTVtkBlockWindow::render()
{
    m_VTKWindow->GetRenderWindow()->Render();
}
/**
 * @brief QTVtkWindow::setWin
 * @param renWin
 * @param renderer
 * set render window to the vtkwindow and set the renderer
 */
void QTVtkBlockWindow::setWin(vtkRenderWindow *renWin, vtkRenderer *renderer)
{
    renderer->AddActor(_assembly);
    m_VTKWindow->SetRenderWindow(renWin);
    m_VTKWindow->GetRenderWindow()->AddRenderer(renderer);
    m_VTKWindow->setEnableHiDPI(true);
}

void QTVtkBlockWindow::setBlocks(std::vector<ECB::SmartPtr<ECD::Block3D> >& blocks)
{
    _blocks.clear();
    _blocks = blocks;
}
/**
 * @brief QTVtkBlockWindow::appendPoints
 * @param pts1
 * @param pts2
 * append ECMath::Points pts2 to vtkPoints pts1
 */
void QTVtkBlockWindow::appendPoints(vtkPoints* pts1,ECMath::Points* pts2)
{
    for(ECB::ID i=0;i<pts2->size();++i)
    {
        const Point& tp = pts2->at(i);
        pts1->InsertNextPoint(tp[0],tp[1], tp[2]);
    }
}
/**
 * @brief QTVtkBlockWindow::appendIDList
 * @param ids
 * @param size
 * make each ID in IDList add a number of size because the Points had appended to a another.
 * @return
 */
ECB::IDList* QTVtkBlockWindow::appendIDList(ECB::IDList* ids,ECB::ID size)
{
    ECB::IDList_ptr copy = IDList::New();
    copy->resize(ids->size());
    for(ECB::ID i=0;i<ids->size();++i)
    {
        copy->at(i) = ids->at(i) + size;
    }
    return copy.release();
}
/**
 * @brief QTVtkBlockWindow::copyPoints
 * @param pts
 * @param vtkPts
 * copy ecPts to vtkPts
 */
void QTVtkBlockWindow::copyPoints(const ECMath::Points* pts, vtkPoints* vtkPts)
{
    EC_ASSERT(pts && vtkPts);
    vtkPts->Reset();
    for (ID i = 0 ; i < pts->size() ; i++ )
    {
        const Point& tp = (*pts)[i];
        vtkPts->InsertNextPoint(tp[0],tp[1], tp[2]);
    }
}

bool QTVtkBlockWindow::addTexture(vtkTexture* texture,std::string name)
{
    _textures.push_back(texture);
    _map.insert(std::pair<std::string,vtkTexture*>(name,texture));
    return true;
}
vtkTexture* QTVtkBlockWindow::getTexture(std::string name)
{
    auto  it = _map.find(name);
    if(it == _map.end())
    {
        cout<<"there is no texture map to this material!";
        return nullptr;
    }
    return it->second;
}
vtkSmartPointer<vtkPoints> QTVtkBlockWindow::getCurVTKPts()
{
    return _vtkPts.Get();
}
vtkSmartPointer<vtkActor> QTVtkBlockWindow::getActorForTexture(vtkSmartPointer<vtkTexture> texture)
{
    auto it = _mapTextureAndActor.find(texture);
    if(it == _mapTextureAndActor.end())
    {
        cout<<"no vtkActor map to this vtkTexture!"<<endl;
        return nullptr;
    }
    return it->second;
}
vtkSmartPointer<vtkAppendPolyData> QTVtkBlockWindow::getAppendForTexture(vtkSmartPointer<vtkTexture> texture)
{
    auto it = _mapTextureAndAppend.find(texture);
    if(it == _mapTextureAndAppend.end())
    {
        cout<<"no appendpolydata map to this vtkTexture!"<<endl;
        return nullptr;
    }
    return it->second;
}
void QTVtkBlockWindow::initActors()
{
    for(ECB::ID i=0;i<_map.size();++i)
    {
        vtkSmartPointer<vtkAppendPolyData> append = vtkSmartPointer<vtkAppendPolyData>::New();
        vtkSmartPointer<vtkOpenGLPolyDataMapper> mapper = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
        //use cubemap
        if(_textures.at(i)->GetCubeMap())
        {
            vtkNew<vtkECPolyDataNormals> norms;
            norms->SetInputConnection(append->GetOutputPort());
            mapper->SetInputConnection(norms->GetOutputPort());
            useCubeMapGlsl(mapper);
        }
        // not use cubemap
        if(!_textures.at(i)->GetCubeMap())
        {
            vtkSmartPointer<vtkTextureMapToECObject> textureMap = vtkSmartPointer<vtkTextureMapToECObject>::New();
            textureMap->SetInputConnection(append->GetOutputPort());
            mapper->SetInputConnection(textureMap->GetOutputPort());
        }
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

        actor->SetTexture(_textures.at(i));
        actor->SetMapper(mapper);

        _mapTextureAndActor.insert(std::pair<vtkSmartPointer<vtkTexture>,vtkSmartPointer<vtkActor> >(_textures.at(i),actor));
        _mapTextureAndAppend.insert(std::pair<vtkSmartPointer<vtkTexture>,vtkSmartPointer<vtkAppendPolyData> >(_textures.at(i),append));
        _assembly->AddPart(actor);
    }
}
bool QTVtkBlockWindow::showBlocks()
{
    if(!_blocks.size() && !_textures.size())
    {
        cout<<"blocks or textures not set yet!"<<endl;
        return false;
    }
    initActors();
    ECB::ID count = 0;
    for(ECB::ID i=0;i<_blocks.size();++i)
    {
        count += _blocks.at(i)->getCell3D()->getNumberOfSelfPoints();
        if(count < ECB::MAXID)
        {
            showBlock(_blocks.at(i));
        }else {
            count = _blocks.at(i)->getCell3D()->getNumberOfSelfPoints();
            initActors();
            getCurVTKPts()->Reset();
            showBlock(_blocks.at(i));
        }
    }
    return true;
}
void QTVtkBlockWindow::showBlock(ECB::SmartPtr<ECD::Block3D> block)
{
    ECD::Cell3D_ptr cell = block->getCell3D();
    vtkIdType cursize = getCurVTKPts()->GetNumberOfPoints();
    appendPoints(getCurVTKPts(),cell->getAllPoints());
    ECD::Cell2D_ptr face;
    std::string jointMatName;
    for(ECB::ID i=0;i<cell->getNumberOfFaces();++i)
    {
        face = cell->getFace(i);
        jointMatName = block->getJointMaterial(block->getCell3D()->getFace(i))->getMaterialName();
        vtkSmartPointer<vtkPolygon> vtkpoly = vtkSmartPointer<vtkPolygon>::New();
        vtkSmartPointer<vtkCellArray> vtkpolys = vtkSmartPointer<vtkCellArray>::New();
        vtkpoly->GetPointIds()->SetNumberOfIds(face->getNumberOfSelfPoints());
        for(ECB::ID j=0;j<face->getNumberOfSelfPoints();++j)
        {
            vtkpoly->GetPointIds()->SetId(j,face->getID4Index(j)+cursize);
        }
        vtkpolys->InsertNextCell(vtkpoly);
        vtkSmartPointer<vtkPolyData> polygonPolydata = vtkSmartPointer<vtkPolyData>::New();
        polygonPolydata->SetPoints(getCurVTKPts());
        polygonPolydata->SetPolys(vtkpolys);
        //add this to vtkAppendPolyData by face jointMaterial
        getAppendForTexture(getTexture(jointMatName))->AddInputData(polygonPolydata);
    }
}
void QTVtkBlockWindow::testBunny(vtkSmartPointer<vtkPolyData> polydata)
{
    if(!_textures.size())
    {
        cout<<"textures not set yet"<<endl;
    }
    vtkNew<vtkOpenGLPolyDataMapper> mapper;
    //use cubemap
    if(_textures.at(0)->GetCubeMap())
    {
        vtkNew<vtkECPolyDataNormals> norms;
        norms->SetInputData(polydata);
        mapper->SetInputConnection(norms->GetOutputPort());
        useCubeMapGlsl(mapper);
    }
    //not use cubemap
    if(!_textures.at(0)->GetCubeMap())
    {
        vtkSmartPointer<vtkTextureMapToECObject> textureMap = vtkSmartPointer<vtkTextureMapToECObject>::New();
        textureMap->SetInputData(polydata);
        mapper->SetInputConnection(textureMap->GetOutputPort());
    }

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->SetTexture(_textures.at(0));
    _assembly->AddPart(actor);
}
std::string QTVtkBlockWindow::ReadGlslFile(const char* path)
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
void QTVtkBlockWindow::useCubeMapGlsl(vtkOpenGLPolyDataMapper* mapper)
{
    std::string vs = ReadGlslFile("D:\\MyTestCode\\textureTest\\shader\\vertex.glsl");
    mapper->SetVertexShaderCode(vs.c_str());
    std::string fs = ReadGlslFile("D:\\MyTestCode\\textureTest\\shader\\fragment.glsl");
    mapper->SetFragmentShaderCode(fs.c_str());
}
void QTVtkBlockWindow::process(vtkPolyData* polydata)
{
    double center[3];
    polydata->GetCenter(center);
    vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
    double pt[3];
    pts->SetNumberOfPoints(polydata->GetNumberOfPoints());
    for (vtkIdType i = 0; i < polydata->GetNumberOfPoints(); ++i)
    {
        vtkMath::Subtract(polydata->GetPoint(i), center, pt);
        pts->SetPoint(i, pt);
    }
    polydata->SetPoints(pts);
    polydata->SetPolys(polydata->GetPolys());
}
void QTVtkBlockWindow::useReflectShader(vtkOpenGLPolyDataMapper* mapper)
{
    //reflect code
    // Add new code in default VTK vertex shader
    mapper->AddShaderReplacement(
        vtkShader::Vertex,
        "//VTK::PositionVC::Dec", // replace the normal block
        true, // before the standard replacements
        "//VTK::PositionVC::Dec\n" // we still want the default
        "varying vec3 TexCoords;\n",
        false // only do it once
    );
    mapper->AddShaderReplacement(
        vtkShader::Vertex,
        "//VTK::PositionVC::Impl", // replace the normal block
        true, // before the standard replacements
        "//VTK::PositionVC::Impl\n" // we still want the default
        "vec3 camPos = -MCVCMatrix[3].xyz * mat3(MCVCMatrix);\n"
        "TexCoords.xyz = reflect(vertexMC.xyz - camPos, normalize(normalMC));\n",
        false // only do it once
    );

    // Replace VTK fragment shader
    mapper->SetFragmentShaderCode(
        "//VTK::System::Dec\n"  // always start with this line
        "//VTK::Output::Dec\n"  // always have this line in your FS
        "varying vec3 TexCoords;\n"
        "uniform samplerCube texture_0;\n"
        "void main () {\n"
        "  gl_FragData[0] = texture(texture_0, TexCoords);\n"
        "}\n"
    );
}
void QTVtkBlockWindow::setTextures()
{
    _textures.clear();
    //read texture image 1
    QPixmap pm(":textures/face.jpg");
    QImage image = pm.toImage();
    vtkQImageToImageSource *trans = vtkQImageToImageSource::New();
    trans->SetQImage(&image);
    trans->Update();
    vtkTexture *atext = vtkTexture::New();
    atext->InterpolateOn();
    atext->SetInputConnection(trans->GetOutputPort());
    addTexture(atext,"jointmat0");

    //read texture image 2
    QPixmap pm2(":textures/test.jpg");
    QImage image2 = pm2.toImage();
    vtkQImageToImageSource *trans2 = vtkQImageToImageSource::New();
    trans2->SetQImage(&image2);
    trans2->Update();
    vtkTexture *atext2 = vtkTexture::New();
    atext2->SetInputConnection(trans2->GetOutputPort());
    atext2->InterpolateOn();
    atext2->RepeatOff();
    addTexture(atext2,"jointmat1");
    //_textures.push_back(atext2);
}
void QTVtkBlockWindow::setCubeMapTextures()
{
    _textures.clear();
    //read texture image 1
    QPixmap pm(":textures/face.jpg");
    QImage image = pm.toImage();
    vtkQImageToImageSource *trans = vtkQImageToImageSource::New();
    trans->SetQImage(&image);
    trans->Update();
    vtkTexture *atext = vtkTexture::New();
    atext->InterpolateOn();
    atext->CubeMapOn();
    atext->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_0,trans->GetOutputPort(0));
    atext->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_1,trans->GetOutputPort(0));
    atext->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_2,trans->GetOutputPort(0));
    atext->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_3,trans->GetOutputPort(0));
    atext->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_4,trans->GetOutputPort(0));
    atext->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_5,trans->GetOutputPort(0));
    addTexture(atext,"jointmat0");

    //read texture image 2
    QPixmap pm2(":textures/test.jpg");
    QImage image2 = pm2.toImage();
    vtkQImageToImageSource *trans2 = vtkQImageToImageSource::New();
    trans2->SetQImage(&image2);
    trans2->Update();
    vtkTexture *atext2 = vtkTexture::New();
    atext2->CubeMapOn();
    atext2->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_0, trans2->GetOutputPort(0));
    atext2->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_1, trans2->GetOutputPort(0));
    atext2->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_2, trans2->GetOutputPort(0));
    atext2->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_3, trans2->GetOutputPort(0));
    atext2->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_4, trans2->GetOutputPort(0));
    atext2->SetInputConnection(vtkProperty::VTK_TEXTURE_UNIT_5, trans2->GetOutputPort(0));
    atext2->InterpolateOn();
    atext2->RepeatOff();
    addTexture(atext2,"jointmat1");
}

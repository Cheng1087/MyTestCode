#pragma once
#include <QWidget>
#include "Base/BaseClass.h"
#include "ECMath/Point.h"
#include "Base/IDList.h"
#include "vector"
#include "map"
#include "string"
#include "vtkSmartPointer.h"
class QVTKOpenGLWidget;
class vtkRenderWindow;
class vtkRenderer;
class vtkAssembly;
class vtkActor;
class vtkPolyData;
class vtkAppendPolyData;
class vtkPoints;
class vtkIdList;
class vtkTexture;
class vtkOpenGLPolyDataMapper;
namespace ECD {
class Block3D;
}

namespace ECVG {

class QTVtkBlockWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QTVtkBlockWindow(QWidget *parent = nullptr);
    ~QTVtkBlockWindow();

    void render();
    void setWin(vtkRenderWindow* renWin, vtkRenderer* renderer);

    void setBlocks(std::vector<ECB::SmartPtr<ECD::Block3D> >& blocks);
    bool showBlocks();
    bool addTexture(vtkTexture* texture,std::string name);
    vtkTexture* getTexture(std::string name);
    void testBunny(vtkSmartPointer<vtkPolyData> polydata);
    void setTextures();
    void setCubeMapTextures();
private:
    void copyPoints(const ECMath::Points* pts, vtkPoints* vtkPts);
    void appendPoints(vtkPoints* pts1,ECMath::Points* pts2);
    ECB::IDList* appendIDList(ECB::IDList* ids,ECB::ID size);
    void showBlock(ECB::SmartPtr<ECD::Block3D> block);
    vtkSmartPointer<vtkPoints> getCurVTKPts();
    vtkSmartPointer<vtkActor> getActorForTexture(vtkSmartPointer<vtkTexture> texture);
    vtkSmartPointer<vtkAppendPolyData> getAppendForTexture(vtkSmartPointer<vtkTexture> texture);
    void initActors();
    std::string ReadGlslFile(const char* path);
    void useCubeMapGlsl(vtkOpenGLPolyDataMapper* mapper);
    void useReflectShader(vtkOpenGLPolyDataMapper* mapper);
    void process(vtkPolyData* polydata);
protected:
    QVTKOpenGLWidget *m_VTKWindow;
    std::vector<ECB::SmartPtr<ECD::Block3D> > _blocks;
    vtkSmartPointer<vtkPoints> _vtkPts;
    vtkSmartPointer<vtkAssembly> _assembly;
    //    std::vector<vtkSmartPointer<vtkActor> > _actors;
    std::vector<vtkSmartPointer<vtkTexture> > _textures;
    std::map<std::string, vtkSmartPointer<vtkTexture> > _map;
    std::map<vtkSmartPointer<vtkTexture>,vtkSmartPointer<vtkActor> > _mapTextureAndActor;
    std::map<vtkSmartPointer<vtkTexture>,vtkSmartPointer<vtkAppendPolyData> > _mapTextureAndAppend;

};
}

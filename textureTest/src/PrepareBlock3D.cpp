#include "PrepareBlock3D.h"
#include "Dataset/TopologyCells/Edge.h"
#include "Dataset/TopologyCells/Polygon.h"

#include "Dataset/TopologyCells/Cell2D.h"
#include "Dataset/TopologyCells/Cell3D.h"
#include "Dataset/TopologyCells/Cell3DBuilder.h"
#include "Dataset/TopologyCells/Polyhedron.h"
#include "Dataset/TopologyCells/PolyhedronBuildDirector.h"
#include "Dataset/TopologyCells/RelatedAlgorithms/UpdateCell3DIntegrationAlgorithm.h"
#include "Dataset/TopologyCells/RelatedAlgorithms/ReorderFacesForAngle3D_byEdgeOrder.h"
#include "Dataset/TopologyCells/RelatedAlgorithms/MatchOppositeEdges_Mapping.h"
using namespace ECB;
using namespace ECMath;
namespace ECV_TEST {
Points_ptr buildMyPoints1()
{
    Point a(3,1,0);
    Point b(3,0,0);
    Point c(0,0,0);
    Point d(0,1,0);
    Point diff(0,0,1);
    Point e = a + diff;
    Point f = d + diff;
    Point g = c + diff;
    Point h = b + diff;
    SmartPtr<Points> pts = Points::New();
    pts->push_back(a);
    pts->push_back(b);
    pts->push_back(c);
    pts->push_back(d);
    pts->push_back(e);
    pts->push_back(f);
    pts->push_back(g);
    pts->push_back(h);
    return pts.release();
}
Points_ptr buildMyPoints2()
{
    Point a(1,2,0);
    Point b(1,1,0);
    Point c(0,1,0);
    Point d(0,2,0);
    Point diff(0,0,1);
    Point e = a + diff;
    Point f = d + diff;
    Point g = c + diff;
    Point h = b + diff;
    SmartPtr<Points> pts = Points::New();
    pts->push_back(a);
    pts->push_back(b);
    pts->push_back(c);
    pts->push_back(d);
    pts->push_back(e);
    pts->push_back(f);
    pts->push_back(g);
    pts->push_back(h);
    return pts.release();
}
Points_ptr buildMyPoints3()
{
    Point a(3,2,0);
    Point b(3,1,0);
    Point c(2,1,0);
    Point d(2,2,0);
    Point diff(0,0,1);
    Point e = a + diff;
    Point f = d + diff;
    Point g = c + diff;
    Point h = b + diff;
    SmartPtr<Points> pts = Points::New();
    pts->push_back(a);
    pts->push_back(b);
    pts->push_back(c);
    pts->push_back(d);
    pts->push_back(e);
    pts->push_back(f);
    pts->push_back(g);
    pts->push_back(h);
    return pts.release();
}
Points_ptr buildMyPoints4()
{
    Point a(3,3,0);
    Point b(3,2,0);
    Point c(0,2,0);
    Point d(0,3,0);
    Point diff(0,0,1);
    Point e = a + diff;
    Point f = d + diff;
    Point g = c + diff;
    Point h = b + diff;
    SmartPtr<Points> pts = Points::New();
    pts->push_back(a);
    pts->push_back(b);
    pts->push_back(c);
    pts->push_back(d);
    pts->push_back(e);
    pts->push_back(f);
    pts->push_back(g);
    pts->push_back(h);
    return pts.release();
}
std::vector<IDList_ptr> buildMyIDList()
{
    std::vector<IDList_ptr> ids;
    SmartPtr<IDList> ID1=IDList::New();
    ID1->push_back(2);
    ID1->push_back(3);
    ID1->push_back(0);
    ID1->push_back(1);
    ids.push_back(ID1);

    SmartPtr<IDList> ID2=IDList::New();
    ID2->push_back(4);
    ID2->push_back(5);
    ID2->push_back(6);
    ID2->push_back(7);
    ids.push_back(ID2);

    SmartPtr<IDList> ID3=IDList::New();
    ID3->push_back(0);
    ID3->push_back(4);
    ID3->push_back(7);
    ID3->push_back(1);
    ids.push_back(ID3);

    SmartPtr<IDList> ID4=IDList::New();
    ID4->push_back(1);
    ID4->push_back(7);
    ID4->push_back(6);
    ID4->push_back(2);
    ids.push_back(ID4);

    SmartPtr<IDList> ID5=IDList::New();
    ID5->push_back(2);
    ID5->push_back(6);
    ID5->push_back(5);
    ID5->push_back(3);
    ids.push_back(ID5);

    SmartPtr<IDList> ID6=IDList::New();
    ID6->push_back(4);
    ID6->push_back(0);
    ID6->push_back(3);
    ID6->push_back(5);
    ids.push_back(ID6);

    return ids;
}

SmartPtr<Polyhedron> createP(Points_ptr pts)
{
    Polygon_ptrs polys;
//    SmartPtr<Points> pts = buildMyPoints1();
    std::vector<IDList_ptr> ids = buildMyIDList();
    for (ECB::ID i=0;i<ids.size();++i)
    {
        polys.push_back(createPolygon(pts,ids.at(i)));
    }
    SmartPtr<Polyhedron> polyhedron = Polyhedron::New();
    Angle3DBuilder_ptr angle3DBuilder = Angle3DBuilder::New();
    angle3DBuilder->setFaceReorderAlgorithm(
                ReorderFacesForAngle3D_byEdgeOrder::New());
    Cell3DBuilder_ptr cell3DBuilder = Cell3DBuilder::New();
    cell3DBuilder->setAngle3DBuilder(angle3DBuilder.get());
    cell3DBuilder->setMatchEdgesAlgorithm(
                MatchOppositeEdges_Mapping::New());
    cell3DBuilder->setUpdateCell3DIntegrationAlgorithm(UpdateCell3DIntegrationAlgorithm::New());
    PolyhedronBuildDirector_ptr phDirector = PolyhedronBuildDirector::New();
    phDirector->setCell3DBuilder(cell3DBuilder.get());
    phDirector->clearFaces();
    for(Polygon_ptrs::iterator it = polys.begin(); it!=polys.end(); it++)
        phDirector->addFace(*it);

    phDirector->setPolyhedron(Polyhedron::New());
    if(!phDirector->build())
        throwLogicException("Polyhedron","built failed.");
    polyhedron = phDirector->getPolyhedron();
    return polyhedron.release();
}
SmartPtr<BlockMaterial> mymaterial()
{
    BlockMaterial_ptr material = BlockMaterial::New();
    material->setMass(1.00);
    Vector6d stress;
    stress.setOnes(6);
    material->setStress(stress);
    material->setPoissonRatio(2.00);
    material->setElasticityModulus(3.00);
    Vector3d force;
    force.setOnes();
    material->setVolumeForce(force);
    material->setMaterialName("mymaterial");
    return material.release();
}
SmartPtr<JointMaterial> myjointmat1()
{
    JointMaterial_ptr jointmaterial = JointMaterial::New();
    jointmaterial->setTension(1.00);
    jointmaterial->setCohesion(1.00);
    jointmaterial->setFriction(1.00);
    jointmaterial->setMaterialName("jointmat0");
    return jointmaterial.release();
}
SmartPtr<JointMaterial> myjointmat2()
{
    JointMaterial_ptr jointmaterial = JointMaterial::New();
    jointmaterial->setTension(2.00);
    jointmaterial->setCohesion(2.00);
    jointmaterial->setFriction(2.00);
    jointmaterial->setMaterialName("jointmat1");
    return jointmaterial.release();
}
SmartPtr<Block3D> myblock3d1()
{
    Block3D_ptr block = Block3D::New();
    Cell3D_ptr cell3d = createP(buildMyPoints1());
    block->setCell3D(cell3d);
    block->setBlockMaterial(mymaterial());
    block->setJointMaterial(cell3d->getFace(0),myjointmat2());
    block->setJointMaterial(cell3d->getFace(1),myjointmat1());
    block->setJointMaterial(cell3d->getFace(2),myjointmat1());
    block->setJointMaterial(cell3d->getFace(3),myjointmat1());
    block->setJointMaterial(cell3d->getFace(4),myjointmat1());
    block->setJointMaterial(cell3d->getFace(5),myjointmat2());
//    block->setJointMaterial(block->getCell3D(),myjointmaterial1());
//    block->setRootCell(cell3d);
    return block.release();
}
SmartPtr<Block3D> myblock3d2()
{
    Block3D_ptr block = Block3D::New();
    Cell3D_ptr cell3d = createP(buildMyPoints2());
    block->setCell3D(cell3d);
    block->setBlockMaterial(mymaterial());
    block->setJointMaterial(cell3d->getFace(0),myjointmat2());
    block->setJointMaterial(cell3d->getFace(1),myjointmat1());
    block->setJointMaterial(cell3d->getFace(2),myjointmat2());
    block->setJointMaterial(cell3d->getFace(3),myjointmat2());
    block->setJointMaterial(cell3d->getFace(4),myjointmat1());
    block->setJointMaterial(cell3d->getFace(5),myjointmat2());
//    block->setJointMaterial(block->getCell3D(),myjointmaterial1());
//    block->setRootCell(cell3d);
    return block.release();
}
SmartPtr<Block3D> myblock3d3()
{
    Block3D_ptr block = Block3D::New();
    Cell3D_ptr cell3d = createP(buildMyPoints3());
    block->setCell3D(cell3d);
    block->setBlockMaterial(mymaterial());
    block->setJointMaterial(cell3d->getFace(0),myjointmat2());
    block->setJointMaterial(cell3d->getFace(1),myjointmat1());
    block->setJointMaterial(cell3d->getFace(2),myjointmat1());
    block->setJointMaterial(cell3d->getFace(3),myjointmat2());
    block->setJointMaterial(cell3d->getFace(4),myjointmat2());
    block->setJointMaterial(cell3d->getFace(5),myjointmat2());
//    block->setJointMaterial(block->getCell3D(),myjointmaterial1());
//    block->setRootCell(cell3d);
    return block.release();
}
SmartPtr<Block3D> myblock3d4()
{
    Block3D_ptr block = Block3D::New();
    Cell3D_ptr cell3d = createP(buildMyPoints4());
    block->setCell3D(cell3d);
    block->setBlockMaterial(mymaterial());
    block->setJointMaterial(cell3d->getFace(0),myjointmat2());
    block->setJointMaterial(cell3d->getFace(1),myjointmat1());
    block->setJointMaterial(cell3d->getFace(2),myjointmat1());
    block->setJointMaterial(cell3d->getFace(3),myjointmat2());
    block->setJointMaterial(cell3d->getFace(4),myjointmat1());
    block->setJointMaterial(cell3d->getFace(5),myjointmat1());
//    block->setJointMaterial(block->getCell3D(),myjointmaterial1());
//    block->setRootCell(cell3d);
    return block.release();
}
Block3D_ptrs getAllBlocks()
{
    Block3D_ptrs blocks;
    blocks.push_back(myblock3d1());
    blocks.push_back(myblock3d2());
    blocks.push_back(myblock3d3());
    blocks.push_back(myblock3d4());
    return blocks;
}

}

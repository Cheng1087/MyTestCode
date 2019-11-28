#pragma once
#include "ECMath/Point.h"
#include "Base/BaseClass.h"
#include "Base/IDList.h"
#include "Base/SmartPointer.h"
#include "Dataset/ComputingElements/Block.h"
#include "Dataset/TopologyCells/Polyhedron.h"
#include "Dataset/ComputingElements/BlockMaterial.h"
#include "Dataset/ComputingElements/JointMaterial.h"
using namespace ECB;
using namespace ECMath;
using namespace ECD;
namespace ECV_TEST
{
Points_ptr buildMyPoints1();
Points_ptr buildMyPoints2();
Points_ptr buildMyPoints3();
Points_ptr buildMyPoints4();
std::vector<IDList_ptr> buildMyIDList();
Polyhedron_ptr createP(Points_ptr pts);
BlockMaterial_ptr mymaterial();
JointMaterial_ptr myjointmat1();
JointMaterial_ptr myjointmat2();
Block3D_ptr myblock3d1();
Block3D_ptr myblock3d2();
Block3D_ptr myblock3d3();
Block3D_ptr myblock3d4();
Block3D_ptrs getAllBlocks();
}

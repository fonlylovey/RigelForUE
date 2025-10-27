#include "Components/RigelBillboardComponent.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "PrimitiveSceneProxy.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "DynamicMeshBuilder.h"       
#include "Engine/EngineTypes.h"       
#include "SceneManagement.h"          
#include "Math/Vector.h"
#include "RenderingThread.h"
#include "StaticMeshResources.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// 组件实现
URigelBillboardComponent::URigelBillboardComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAlwaysFaceCamera = true;
    bAutoScaleWithDistance = true;
    BaseSize = 100.0f;
    SetStaticMesh(CreateMesh());
}


void URigelBillboardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Pawn == nullptr)
    {
        Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    }

    FVector cameraLocation;
    FRotator cameraRotation;
    if (GetWorld()->HasBegunPlay())
    {
        cameraLocation = Pawn->GetActorLocation();
        cameraRotation = Pawn->GetActorRotation();
    }

    //朝向屏幕
    FVector location = GetComponentLocation();

    FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(location, cameraLocation);
    SetWorldRotation(FRotator(lookAt.Pitch, lookAt.Yaw, 0));
    //自动缩放

    double dis = FVector::Distance(location, cameraLocation);
    int scale = (int)dis * 0.01;
   // scale = FMath::Clamp(scale, 1, MaxScale);
    SetWorldScale3D(FVector(scale, scale, scale));
}

void URigelBillboardComponent::CreateGeometry(FStaticMeshRenderData* RenderData)
{
    RenderData->AllocateLODResources(1);
    FStaticMeshLODResources& LODResources = RenderData->LODResources[0];
    int vertexNum = 4;

    TArray<FVector3f> xyzList;
    xyzList.Add(FVector3f(-500 * BaseSize, 0, 0));
    xyzList.Add(FVector3f(500 * BaseSize, 0, 0));
    xyzList.Add(FVector3f(500 * BaseSize, 0, 1000 * BaseSize));
    xyzList.Add(FVector3f(-500 * BaseSize, 0, 1000 * BaseSize));

    TArray<FVector2f> uvList;
    uvList.Add(FVector2f(0, 1));
    uvList.Add(FVector2f(0, 0));
    uvList.Add(FVector2f(1, 0));
    uvList.Add(FVector2f(1, 1));

    // 设置顶点数据
    TArray<FStaticMeshBuildVertex> StaticMeshBuildVertices;
    StaticMeshBuildVertices.SetNum(vertexNum);
    for (int m = 0; m < vertexNum; m++) {
        StaticMeshBuildVertices[m].Position = xyzList[m];
        StaticMeshBuildVertices[m].Color = FColor(255, 0, 0);
        StaticMeshBuildVertices[m].UVs[0] = uvList[m];
        StaticMeshBuildVertices[m].TangentX = FVector3f(0, 1, 0);  //切线
        StaticMeshBuildVertices[m].TangentY = FVector3f(1, 0, 0);  //副切线
        StaticMeshBuildVertices[m].TangentZ = FVector3f(0, 0, 1);  //法向量
    }

    LODResources.bHasColorVertexData = false;

    //顶点buffer
    LODResources.VertexBuffers.PositionVertexBuffer.Init(StaticMeshBuildVertices);

    //法线，切线，贴图坐标buffer
    LODResources.VertexBuffers.StaticMeshVertexBuffer.Init(
        StaticMeshBuildVertices, 1);

    //设置索引数组
    TArray<uint32> indices;
    int numTriangles = 2;
    int indiceNum = numTriangles * 3;
    indices.SetNum(indiceNum);
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 3;
    indices[3] = 3;
    indices[4] = 1;
    indices[5] = 2;

    LODResources.IndexBuffer.SetIndices(indices,
        EIndexBufferStride::Type::AutoDetect);

    LODResources.bHasDepthOnlyIndices = false;
    LODResources.bHasReversedIndices = false;
    LODResources.bHasReversedDepthOnlyIndices = false;
    // LODResources.bHasAdjacencyInfo = false;

    FStaticMeshSectionArray& Sections =LODResources.Sections;
    {
        FStaticMeshSection& section = Sections.AddDefaulted_GetRef();

        section.bEnableCollision = false;
        section.MaterialIndex = 0;
        section.NumTriangles = 2;
        section.FirstIndex = 0;
        section.MinVertexIndex = 0;
        section.MaxVertexIndex = 5;
    }

    double boundArray[7] = { 0, 0, 0, 1000 * BaseSize, 1000 * BaseSize, 1000 * BaseSize, 1000 * BaseSize };

    //设置包围盒
    FBoxSphereBounds BoundingBoxAndSphere;
    BoundingBoxAndSphere.Origin =
        FVector(boundArray[0], boundArray[1], boundArray[2]);
    BoundingBoxAndSphere.BoxExtent =
        FVector(boundArray[3], boundArray[4], boundArray[5]);
    BoundingBoxAndSphere.SphereRadius = boundArray[6];
    RenderData->Bounds = BoundingBoxAndSphere;
}

void URigelBillboardComponent::CreateMaterial()
{
    if (GetMaterial(0) != nullptr && GetStaticMesh() != nullptr)
    {
        GetStaticMesh()->AddMaterial(GetMaterial(0));
    }
    
}

UStaticMesh* URigelBillboardComponent::CreateMesh()
{
    UStaticMesh* mesh = NewObject<UStaticMesh>(this, TEXT("Billboard Mesh"));
    mesh->NeverStream = true;
    //mesh->AddSourceModel();
    //mesh->CreateMeshDescription(0);
    TUniquePtr<FStaticMeshRenderData> RenderData =
        MakeUnique<FStaticMeshRenderData>();

    CreateGeometry(RenderData.Get());

    mesh->SetRenderData(MoveTemp(RenderData));
    mesh->InitResources();
    mesh->CalculateExtendedBounds();  //设置包围盒之后调用这个函数起效，否则会被视锥体剔除

    UMaterial* material = (UMaterial*)StaticLoadObject(
        UMaterial::StaticClass(), nullptr,
        TEXT("Material'/Engine/EngineMaterials/NaniteHiddenSectionMaterial.NaniteHiddenSectionMaterial'"));
    mesh->AddMaterial(material);
    return mesh;
}

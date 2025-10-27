#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RigelBillboardComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RIGEL_API URigelBillboardComponent : public UStaticMeshComponent
{
    GENERATED_BODY()

public:
    URigelBillboardComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** 是否始终朝向相机 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel Billboard")
    bool bAlwaysFaceCamera;

    /** 是否根据距离自动缩放 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel Billboard")
    bool bAutoScaleWithDistance;

    /** 基础大小（单位：厘米） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel Billboard")
    float BaseSize;

    void CreateMaterial();
protected:
    /** 更新变换以朝向相机 */
    void UpdateBillboardTransform();

    void CreateGeometry(FStaticMeshRenderData* RenderData);

private:
    UStaticMesh* CreateMesh();

    APawn* Pawn = nullptr;
};
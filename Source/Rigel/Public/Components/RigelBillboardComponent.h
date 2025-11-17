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

    /** �Ƿ�ʼ�ճ������ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel Billboard")
    bool bAlwaysFaceCamera;

    /** �Ƿ���ݾ����Զ����� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel Billboard")
    bool bAutoScaleWithDistance;

    /** ������С����λ�����ף� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rigel Billboard")
    float BaseSize;

    void CreateMaterial();
protected:
    /** ���±任�Գ������ */
    void UpdateBillboardTransform();

    void CreateGeometry(FStaticMeshRenderData* RenderData);

private:
    UStaticMesh* CreateMesh();

    APawn* Pawn = nullptr;
};
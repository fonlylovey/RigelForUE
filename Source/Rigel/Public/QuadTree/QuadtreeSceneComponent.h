#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Map.h"
#include "Math/IntPoint.h"
#include "TreeNode.h"
#include "QuadtreeSceneComponent.generated.h"

class APOIBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuadtreeCombineDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuadtreeSubdivideDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnPOIDelegate, const FPOIData&, POIData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeletePOIDelegate, const FPOIData&, POIData);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMapLevelChangeDelegate, int, OldLevel, int, Level);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F, const FVector&, Location);

UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RIGEL_API UQuadtreeSceneComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UQuadtreeSceneComponent();
    ~UQuadtreeSceneComponent();
    //
    UFUNCTION(BlueprintCallable)
    void CreateTreeNode(const FPOIData& POIData);

    UFUNCTION(BlueprintCallable)
    void ClearAllNode();

    UFUNCTION(BlueprintCallable)
    void Update(int level);

    void Combine();

    void Subdivide();

    void Traversal(FTreeNode* NodePath, bool isCreate = true);

    bool GetNodesByDepth(FTreeNode* NodePath, int32 TargetDepth, TArray<FPOIData>& OutNodeDatas);

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector Center = FVector(106.594627, 29.573882, 0);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int CellWidth = 1000;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Width = 100000.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Height = 100000.f;

    UPROPERTY(BlueprintAssignable)
    FSpawnPOIDelegate OnSpawnPOI;

    UPROPERTY(BlueprintAssignable)
    FDeletePOIDelegate OnDeletePOI;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int CurrentDepth = 1;

    void CalcDepth(int level);

protected:
    virtual void BeginPlay() override;

    void GetAllTreeNode(FTreeNode* InRootNode);
private:
    // 地理范围（经纬度）
    FVector2D MinGeo;
    FVector2D MaxGeo;
    
    // 根节点集合（每个Cell一个根节点）
    TMap<int, TArray<APOIBase*>> DepthNodeMap;

    // 四叉树最大深度（防止无限细分）
    int32 MaxTreeDepth = 5;

    FTreeNode* RootNode = nullptr;

    int CurrentZoomLevel = 0;

    TArray<FTreeNode*> AllNodes;
};
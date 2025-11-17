#pragma once

#include "CoreMinimal.h"
#include "Math/Box2D.h"
#include "Containers/Array.h"
#include "TreeNode.generated.h"

// POI数据结构（存储经纬度和属性）
USTRUCT(BlueprintType)
struct RIGEL_API FPOIData
{
    GENERATED_BODY()

    // POI唯一标识
    UPROPERTY(BlueprintReadWrite, Category = "POI")
    FString ID = TEXT("");

    // 
    UPROPERTY(BlueprintReadWrite, Category = "POI")
    FString DisplayName = TEXT("");

    // 经纬度坐标（X=经度，Y=纬度, Z= 高度）
    UPROPERTY(BlueprintReadWrite, Category = "POI")
    FVector GeoLocation = FVector::ZeroVector;

    //POI的类型
    UPROPERTY(BlueprintReadWrite, Category = "POI")
    int StyleType = 0;

    //事件类型
    UPROPERTY(BlueprintReadWrite, Category = "POI")
    FString EventType = TEXT("");

    UPROPERTY(BlueprintReadWrite, Category = "POI")
    FString JsonString = TEXT("");

    // 构造函数
    FPOIData() = default;
};

// 四叉树节点结构
USTRUCT()
struct RIGEL_API FTreeNode
{
    GENERATED_BODY()
    // 构造函数
    FTreeNode() = default;
    FTreeNode(const FPOIData& InData);
    FTreeNode(FVector InOrigin, float InWidth, float InHeight, int InDepth, int InMaxDepth);

    bool Insert(FTreeNode* GroupNode, const FPOIData& InData);

    FPOIData GetPOIData() { return POIData; };

    void AddChild(FTreeNode* child);

    FTreeNode* GetParent() {
        return Parent;
    }

    //自己是否显示，根节点一定是显示的
    bool IsChildrenDispaly() { return Display; }
    void SetChildrenDisplay(bool IsDisplay) { Display = IsDisplay; }

public:
    FString NodeID = TEXT("");
    TArray<FTreeNode*> Children;

private:
    
    // 父节点指针（用于层级回溯）
    FTreeNode* Parent = nullptr;
    bool Display = false;
    FVector Origin = FVector::ZeroVector;       // 节点区域的中心点
    float width = 10000.0;            // 节点区域的宽度
    float height = 10000.0;           // 节点区域的高度
    int32 Depth = 0;// 当前节点的深度（根节点为0）
    int MaxDepth = 10000.0;           // 四叉树的最大深度（达到后不再细分）
    
    TArray<FVector> points;  // 存储当前节点包含的点（叶子节点有效）
    FVector FirstNodePoint = FVector::ZeroVector;     // 当前节点第一个插入点的位置
    int LeafNodeCount = 10000.0;               // 该节点及其子树包含的叶子节点（点）总数
    FPOIData POIData;
};

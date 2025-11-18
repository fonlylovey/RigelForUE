#include "QuadTree/QuadtreeSceneComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Box.h"
#include "Camera/PlayerCameraManager.h"

UQuadtreeSceneComponent::UQuadtreeSceneComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

UQuadtreeSceneComponent::~UQuadtreeSceneComponent()
{
    if (RootNode != nullptr)
    {
        delete RootNode;
        RootNode = nullptr;
    }
}

void UQuadtreeSceneComponent::BeginPlay()
{
    Super::BeginPlay();
    if (RootNode == nullptr)
    {
        RootNode = new FTreeNode(Center, Width, Height, 0, MaxTreeDepth);
        RootNode->SetDisplay(true);
        RootNode->Children.Empty();
    }
}


void UQuadtreeSceneComponent::GetAllTreeNode(FTreeNode* InRootNode)
{
       if (InRootNode->Children.Num() > 0)
       {
           for (FTreeNode* child : InRootNode->Children)
           {
               GetAllTreeNode(child);
           }
       }
       else
       {
           AllNodes.Add(InRootNode);
       }
}


void UQuadtreeSceneComponent::CreateTreeNode(const FPOIData& POIData)
{
    RootNode->Insert(RootNode, POIData);
}

void UQuadtreeSceneComponent::ClearAllNode()
{
    for (FTreeNode* child : RootNode->Children)
    {
        GetAllTreeNode(child);
    }
    
    for (auto treeNode : AllNodes)
    {
        OnDeletePOI.Broadcast(treeNode->GetPOIData());
        delete treeNode;
    }
    AllNodes.Empty();
    RootNode->Children.Empty();
}

void UQuadtreeSceneComponent::Update(int level)
{
    if (level > CurrentZoomLevel && CurrentDepth <= MaxTreeDepth)
    {
        CalcDepth(level);
        Subdivide();
    }
    else if (level < CurrentZoomLevel && CurrentDepth >= 1)
    {
        CalcDepth(level);
        Combine();
    }
    CurrentZoomLevel = level;
}

void UQuadtreeSceneComponent::Combine()
{
    TArray<FPOIData> DataList;
    if (GetNodesByDepth(RootNode, CurrentDepth + 1, DataList))
    {
        for (FPOIData& poiData : DataList)
        {
            OnDeletePOI.Broadcast(poiData);
            FString strMsg = TEXT("删除：") + poiData.ID;
            GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, strMsg);
        }
    }
}

void UQuadtreeSceneComponent::Subdivide()
{
    // 队列存储「节点+当前深度」（避免重复调用 GetDepth()，优化性能）
    TQueue<TTuple<FTreeNode*, int32>> NodeQueue;
    NodeQueue.Enqueue(MakeTuple(RootNode, RootNode->GetDepth()));

    while (!NodeQueue.IsEmpty())
    {
        TTuple<FTreeNode*, int32> CurrentTuple;
        if (NodeQueue.Dequeue(CurrentTuple))
        {
            FTreeNode* CurrentNode = CurrentTuple.Key;
            int32 CurrentNodeDepth = CurrentTuple.Value;

            if (CurrentNode && CurrentNodeDepth <= CurrentDepth)
            {
                OnSpawnPOI.Broadcast(CurrentNode->GetPOIData());
                FString strMsg = TEXT("创建：") + CurrentNode->GetPOIData().ID;
                GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, strMsg);
            }
            else
            {
                continue;
            }

            // 子节点入队（仅当当前节点深度 < MaxDepth 时，才入队子节点）
            // 优化：如果当前节点已达最大深度，其子节点无需入队，减少队列开销
            if (CurrentNode && CurrentNodeDepth < CurrentDepth)
            {
                for (FTreeNode* Child : CurrentNode->Children)
                {
                    if (Child)
                    {
                        NodeQueue.Enqueue(MakeTuple(Child, Child->GetDepth()));
                    }
                }
            }
        }
    }
}



bool UQuadtreeSceneComponent::GetNodesByDepth(FTreeNode* NodePath, int32 TargetDepth, TArray<FPOIData>& OutNodeDatas)
{
    // 初始化输出数组（清空原有数据）
    OutNodeDatas.Empty();

    // 安全校验
    if (!NodePath)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetNodesByDepth: RootNode is null!"));
        return false;
    }
    if (TargetDepth < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetNodesByDepth: TargetDepth cannot be negative!"));
        return false;
    }

    // 广度优先遍历（仅遍历到目标深度，避免多余计算）
    TQueue<TTuple<FTreeNode*, int32>> NodeQueue;
    NodeQueue.Enqueue(MakeTuple(NodePath, NodePath->GetDepth()));

    while (!NodeQueue.IsEmpty())
    {
        TTuple<FTreeNode*, int32> CurrentTuple;
        if (NodeQueue.Dequeue(CurrentTuple))
        {
            FTreeNode* CurrentNode = CurrentTuple.Key;
            int32 TheDepth = CurrentTuple.Value;

            // 1. 超过目标深度：直接跳过（无需入队子节点）
            if (TheDepth > TargetDepth)
            {
                continue;
            }

            // 2. 等于目标深度：拷贝节点数据到输出数组（不含父/子引用）
            if (TheDepth == TargetDepth)
            {
                OutNodeDatas.Add(CurrentNode->GetPOIData());
                continue; //目标深度节点无需入队子节点
            }

            // 3. 小于目标深度：子节点入队，继续遍历
            if (CurrentNode && TheDepth < TargetDepth)
            {
                for (FTreeNode* Child : CurrentNode->Children)
                {
                    if (Child)
                    {
                        NodeQueue.Enqueue(MakeTuple(Child, Child->GetDepth()));
                    }
                }
            }
        }
    }
    return true;
}

void UQuadtreeSceneComponent::CalcDepth(int level)
{
    if (level < 10)
    {
        CurrentDepth = 1;
    }
    else if (level >= 10 && level < 11)
    {
        CurrentDepth = 2;
    }
    else if (level >= 11 && level < 12)
    {
        CurrentDepth = 3;
    }
    else if (level >= 12 && level < 13)
    {
        CurrentDepth = 4;
    }
    else if (level >= 13 && level < 18)
    {
        CurrentDepth = 5;
    }
}

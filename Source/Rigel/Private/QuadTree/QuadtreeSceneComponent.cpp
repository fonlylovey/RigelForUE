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
        RootNode->SetChildrenDisplay(false);
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
    if (level > CurrentZoomLevel)
    {
        CurrentDepth++;
        Subdivide();
    }
    else if (level < CurrentZoomLevel)
    {
        CurrentDepth--;
        Combine();
    }
    CurrentZoomLevel = level;
}

void UQuadtreeSceneComponent::Combine()
{
    Traversal(RootNode, false);
}

void UQuadtreeSceneComponent::Subdivide()
{
    Traversal(RootNode, true);
}

void UQuadtreeSceneComponent::Traversal(FTreeNode* NodePath, bool isCreate /*= true*/)
{
    if (isCreate)
    {
        //如果自己的子节点已经显示就递归子节点
        if (NodePath->IsChildrenDispaly())
        {
            for (FTreeNode* child : NodePath->Children)
            {
                Traversal(child, isCreate);
            }
        }
        else //如果没有显示，就显示子节点
        {
            for (FTreeNode* child : NodePath->Children)
            {
                //SpawnPOI(child->GetPOIData());
                OnSpawnPOI.Broadcast(child->GetPOIData());
                FString strMsg = TEXT("创建：") + child->GetPOIData().ID;
                GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, strMsg);
            }
            NodePath->SetChildrenDisplay(true);
            return;
        }
    }
    else
    {
        //如果自己的子节点已经显示就递归子节点
        if (NodePath->IsChildrenDispaly())
        {
            for (FTreeNode* child : NodePath->Children)
            {
                Traversal(child, isCreate);
            }
        }
        else //如果子节点没有显示，就删除自己
        {
            //DeletePOI(NodePath->GetPOIData());
            OnDeletePOI.Broadcast(NodePath->GetPOIData());
            FString strMsg = TEXT("删除：") + NodePath->GetPOIData().ID;
            GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, strMsg);
            NodePath->SetChildrenDisplay(false);
            return;
        }
    }
    
}

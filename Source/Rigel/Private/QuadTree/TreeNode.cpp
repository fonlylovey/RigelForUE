// Fill out your copyright notice in the Description page of Project Settings.


#include "QuadTree/TreeNode.h"

FTreeNode::FTreeNode(FVector InOrigin, float InWidth, float InHeight, int InDepth, int InMaxDepth)
{
    Origin = InOrigin;
    width = InWidth;
    height = InHeight;
    Depth = InDepth;
    MaxDepth = InMaxDepth;
}

FTreeNode::FTreeNode(const FPOIData& InData)
{
    NodeID = InData.ID;
    POIData = InData;
    Origin = POIData.GeoLocation;
}

bool FTreeNode::Insert(FTreeNode* GroupNode, const FPOIData& InData)
{
    // 1. 边界校验（避免崩溃和无效数据）
    if (!GroupNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("FTreeNode::Insert: GroupNode is null!"));
        return false;
    }
    if (InData.ID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("FTreeNode::Insert: InData ID is empty!"));
        return false;
    }

    // 2. 第一步：尝试插入到 GroupNode 的直接子节点（当前层级）
    if (GroupNode->Children.Num() < 4)
    {
        FTreeNode* NewNode = new FTreeNode(InData);
        NewNode->Depth = GroupNode->Depth + 1;
        NewNode->Parent = GroupNode;
        GroupNode->AddChild(NewNode);

        UE_LOG(LogTemp, Log, TEXT("Insert Node: ID=%s, Depth=%d, ParentID=%s"),
            *NewNode->POIData.ID,
            NewNode->Depth,
            *GroupNode->POIData.ID);
        return true;
    }

    // 3. 第二步：当前层级满了，递归遍历所有子节点，按广度优先顺序填充子节点的直接子节点
    // 用队列实现广度优先遍历，确保每一层级填满后再往下一层
    TQueue<FTreeNode*> NodeQueue;
    for (FTreeNode* Child : GroupNode->Children)
    {
        if (Child)
        {
            NodeQueue.Enqueue(Child);
        }
    }

    while (!NodeQueue.IsEmpty())
    {
        FTreeNode* CurrentChild = nullptr;
        if (NodeQueue.Dequeue(CurrentChild) && CurrentChild)
        {
            // 尝试插入到当前子节点的直接子节点（下一层级）
            if (CurrentChild->Children.Num() < 4)
            {
                FTreeNode* NewNode = new FTreeNode(InData);
                NewNode->Depth = CurrentChild->Depth + 1;
                NewNode->Parent = CurrentChild;
                CurrentChild->AddChild(NewNode);

                UE_LOG(LogTemp, Log, TEXT("Insert Node: ID=%s, Depth=%d, ParentID=%s"),
                    *NewNode->POIData.ID,
                    NewNode->Depth,
                    *CurrentChild->POIData.ID);
                return true;
            }

            // 当前子节点的直接子节点也满了，将其所有子节点入队，继续遍历下一层
            for (FTreeNode* GrandChild : CurrentChild->Children)
            {
                if (GrandChild)
                {
                    NodeQueue.Enqueue(GrandChild);
                }
            }
        }
    }

    // 4. 所有层级都满了，插入失败
    UE_LOG(LogTemp, Warning, TEXT("FTreeNode::Insert: All levels are full! Cannot insert ID=%s"), *InData.ID);
    return false;
}

void FTreeNode::AddChild(FTreeNode* child)
{
    child->Parent = this;
    this->Children.Add(child);
}

bool FTreeNode::HasChildren()
{
    return !Children.IsEmpty();
}

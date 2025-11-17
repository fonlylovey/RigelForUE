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
     // 小于4个直接子节点，再往下添加
    if (GroupNode->Children.Num() < 4)
    {
        FTreeNode* poiNode = new FTreeNode(InData);
        poiNode->Depth = GroupNode->Depth + 1;
        poiNode->Parent = GroupNode;
        GroupNode->AddChild(poiNode);
        return true;
    }

    // 第当前节点的直接子节点已满（4个），现在依次填满每个子节点的直接子节点
    // 遍历所有直接子节点，给每个子节点填到4个为止，不递归深层
    for (FTreeNode* child : GroupNode->Children)
    {
        // 只要子节点的直接子节点没满，就直接填这里（第二级）
        if (child->Children.Num() < 4)
        {
            FTreeNode* poiNode = new FTreeNode(InData);
            poiNode->Depth = child->Depth + 1;
            poiNode->Parent = child;
            child->AddChild(poiNode);
            return true;
        }
    }

    // 第三步：所有直接子节点的直接子节点也满了（第二级满），现在递归到第三级
    // 遍历每个直接子节点，让它们重复上面的逻辑（先填自己的直接子节点，再递归）
    for (FTreeNode* child : GroupNode->Children)
    {
        if (child->Insert(child, InData))
        {
            return true;
        }
    }

    // 所有层级都满了，插入失败
    return false;
}

void FTreeNode::AddChild(FTreeNode* child)
{
    child->Parent = this;
    this->Children.Add(child);
}

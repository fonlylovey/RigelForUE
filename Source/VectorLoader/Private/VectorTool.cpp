
#include "VectorTool.h"
#include "earcut.hpp"

namespace mapbox 
{
    namespace util
    {
        template <>
        struct nth<0, FVector3d> 
        {
            inline static auto get(const FVector3d &t) 
            {
                return t.X;
            };
        };

        template <>
        struct nth<1, FVector3d> 
        {
            inline static auto get(const FVector3d &t) 
            {
                return t.Y;
            };
        };

        template <>
        struct nth<2, FVector3d> 
        {
            inline static auto get(const FVector3d &t) 
            {
                return t.Z;
            };
        };
    } // namespace util
} // namespace mapbox

struct FRingContainer
{
    typedef FVector3d          value_type;
    typedef TArray<value_type> container_type;

    container_type Data;

    FRingContainer() = default;

    FRingContainer(const container_type& InPoints)
        : Data(InPoints)
    {}

    SIZE_T size() const
    {
        return Data.Num();
    }

    bool empty() const
    {
        return Data.IsEmpty();
    }

    const FVector3d& operator[](SIZE_T Index) const
    {
        return Data[Index];
    }
};

struct FPolygon
{
    typedef FRingContainer value_type;
    typedef TArray<value_type> container_type;

    container_type Data;

    FPolygon() = default;

    FPolygon(const container_type& InPolygon)
        : Data(InPolygon)
    {
    }

    SIZE_T size() const
    {
        return Data.Num();
    }

    bool empty() const
    {
        return Data.Num() == 0;
    }

    FRingContainer& operator[](SIZE_T i)
    {
        return Data[i];
    }

    const FRingContainer& operator[](SIZE_T i) const
    {
        return Data[i];
    }
};

TArray<int32> VectorTool::TriangulateLineString(const TArray<TArray<FVector3d>>& holes)
{
     TArray<int32> indices;
     FPolygon EarPoly;
     for (const TArray<FVector3d>& polyPoints : holes)
     {
         EarPoly.Data.Add(FRingContainer(polyPoints));
     }
     std::vector<int32> earIndices = mapbox::earcut<int32>(EarPoly);
     size_t indexSize = earIndices.size() * sizeof(int32);
     indices.SetNumUninitialized(earIndices.size());
     FMemory::Memcpy(indices.GetData(), earIndices.data(), indexSize);
     return MoveTemp(indices);
}

#include "group.h"
#include <kloss/algorithm.h>
#include <kloss/buffer.h>
#include <kloss/groupinstance.h>
#include <kloss/ptrarray.h>
#include <GL/gl.h>
#include <float.h>
#include <stdlib.h>

typedef struct Vertex Vertex;

struct Vertex
{
    Vec3 Normal;
    Vec3 Position;
};

struct Group
{
    PtrArray *Blocks;
    PtrArray *Instances;
    Buffer *Vertices;
    size_t RefCount;
};

Group *CreateGroup(void)
{
    Group *group = malloc(sizeof(Group));
    group->Blocks    = CreatePtrArray();
    group->Instances = CreatePtrArray();
    group->Vertices  = CreateBuffer();
    group->RefCount  = 1;
    return group;
}

Group *RetainGroup(Group *group)
{
    ++group->RefCount;
    return group;
}

void ReleaseGroup(Group *group)
{
    if (group && --group->RefCount == 0)
    {
        size_t count = PtrArrayCount(group->Blocks);
        for (size_t i = 0; i < count; ++i)
        {
            DestroyBlock(GetPtrArray(group->Blocks, i));
        }

        count = PtrArrayCount(group->Instances);
        for (size_t i = 0; i < count; ++i)
        {
            DestroyGroupInstance(GetPtrArray(group->Instances, i));
        }

        DestroyPtrArray(group->Blocks);
        DestroyPtrArray(group->Instances);
        DestroyBuffer(group->Vertices);

        free(group);
    }
}

static void AppendTriangles(Group *group, Buffer *buffer)
{
    Triangle *triangles = BufferData(buffer);
    size_t count = BufferSize(buffer) / sizeof(Triangle);

    for (size_t i = 0; i < count; ++i)
    {
        Vec3 normal;

        TriangleNormal(&normal, &triangles[i]);
        Vec3Normalize(&normal, &normal);

        Vertex vertices[3] =
        {
            {normal, triangles[i].A},
            {normal, triangles[i].B},
            {normal, triangles[i].C}
        };

        AppendToBuffer(group->Vertices, vertices, sizeof(vertices));
    }
}

void InsertBlocksInGroup(Group *group, Block **blocks, size_t count)
{
    Buffer *buffer = CreateBuffer();

    for (size_t i = 0; i < count; ++i)
    {
        Block *block = blocks[i];
        PushPtrArray(group->Blocks, block);
        GetBlockTriangles(block, buffer);
        AppendTriangles(group, buffer);
        ClearBuffer(buffer);
    }

    DestroyBuffer(buffer);
}

void DeleteBlocksFromGroup(Group *group, Block **blocks, size_t count)
{
    DetatchBlocksFromGroup(group, blocks, count);

    for (size_t i = 0; i < count; ++i)
    {
        DestroyBlock(blocks[i]);
    }
}

void DetatchBlocksFromGroup(Group *group, Block **blocks, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        RemovePtrArray(group->Blocks, blocks[i]);
    }

    UpdateGroupVertexArray(group);
}

void ForEachBlockInGroup(Group *group, void (function)(Block*,void*), void *data)
{
    size_t groupCount = PtrArrayCount(group->Blocks);

    for (size_t i = 0; i < groupCount; ++i)
    {
        function(GetPtrArray(group->Blocks, i), data);
    }
}

void InsertGroupInstance(Group *group, GroupInstance *instance)
{
    PushPtrArray(group->Instances, instance);
}

void ForEachGroupInstance(Group *group, void (function)(GroupInstance*,void*), void *data)
{
    size_t instanceCount = PtrArrayCount(group->Instances);

    for (size_t i = 0; i < instanceCount; ++i)
    {
        function(GetPtrArray(group->Instances, i), data);
    }
}

Pick PickGroupBlock(Group const *group, Ray const *ray)
{
    float nearest = FLT_MAX;
    Block *nearestBlock = NULL;
    Triangle nearestTriangle;

    Buffer *buffer = CreateBuffer();
    size_t blockCount = PtrArrayCount(group->Blocks);

    for (size_t i = 0; i < blockCount; ++i)
    {
        Block *block = GetPtrArray(group->Blocks, i);

        GetBlockTriangles(block, buffer);

        Triangle *triangles = BufferData(buffer);
        size_t triangleCount = BufferSize(buffer) / sizeof(Triangle);

        for (size_t j = 0; j < triangleCount; ++j)
        {
            float temp;
            if (RayIntersectTriangle(&temp, ray, &triangles[j]))
            {
                /*
                 * Use <= for distance comparsion here so that when there are
                 * multiple candidates for selection the last block added will
                 * be selected. It is useful after pasting that the new blocks
                 * can be dragged to a new location.
                 */
                if (temp <= nearest)
                {
                    nearest = temp;
                    nearestBlock = block;
                    nearestTriangle = triangles[j];
                }
            }
        }

        ClearBuffer(buffer);
    }

    DestroyBuffer(buffer);

    Vec3 intersection;
    Vec3AddScaled(&intersection, &ray->Origin, &ray->Direction, nearest);
    return (Pick){nearestBlock, nearestTriangle, intersection};
}

static bool Project(Vec3 *out, Vec3 const *in, Mat4 const *model, Mat4 const *projection, Viewport const *viewport)
{
    Mat4 transform;

    Mat4Transform(&transform, model, projection);
    Vec3Transform(out, in, &transform);

    float w = transform.X.W * in->X +
              transform.Y.W * in->Y +
              transform.Z.W * in->Z +
              transform.T.W;

    if (out->Z < -w || out->Z > w)
    {
        return false;
    }

    out->X /= w;
    out->Y /= w;
    out->Z /= w;

    out->X = out->X * 0.5f + 0.5f;
    out->Y = out->Y * 0.5f + 0.5f;
    out->X = out->X * viewport->width + viewport->x;
    out->Y = out->Y * viewport->height + viewport->y;

    return true;
}

struct VertexPickData
{
    Mat4 const *ModelView;
    Mat4 const *Projection;
    Viewport const *Viewport;
    Vec2 const *Mouse;
    float Distance;
    CornerRef CornerRef;
};

void CheckVertex(struct VertexPickData *data, Vec3 position, CornerRef const *ref, uint8_t flag)
{
    float const radius = 5.0f;
    Vec3 screenPos;

    if (Project(&screenPos, &position, data->ModelView, data->Projection, data->Viewport))
    {
        Vec2 screenPosXY = {screenPos.X, data->Viewport->height - screenPos.Y};
        float distance = Vec2Distance(data->Mouse, &screenPosXY);

        if (distance < radius && screenPos.Z < data->Distance)
        {
            data->Distance = screenPos.Z;
            data->CornerRef = (CornerRef){ref->Corner, flag};
        }
    }
}

bool PickGroupVertex(Group const *group, Mat4 const *model, Mat4 const *projection, Viewport const *viewport, Vec2 const *mouse, CornerRef *ref)
{
    struct VertexPickData data =
    {
        model,
        projection,
        viewport,
        mouse,
        FLT_MAX
    };

    size_t blockCount = PtrArrayCount(group->Blocks);

    for (size_t i = 0; i < blockCount; ++i)
    {
        Block *block = GetPtrArray(group->Blocks, i);
        CornerRef cornerRefs[4];
        MakeCornerRefs(cornerRefs, block);

        for (size_t i = 0; i < 4; ++i)
        {
            CornerRef *cornerRef = cornerRefs + i;
            CheckVertex(&data, CornerTop   (cornerRef->Corner), cornerRef, CORNER_REF_TOP);
            CheckVertex(&data, CornerBottom(cornerRef->Corner), cornerRef, CORNER_REF_BOTTOM);
        }
    }

    if (data.Distance != FLT_MAX)
    {
        *ref = data.CornerRef;
        return true;
    }
    else
    {
        return false;
    }
}

static void ExpandBoundingBoxByCorner(BoundingBox *bbox, Corner const *corner)
{
    bbox->Lower.X = Minf(bbox->Lower.X, corner->X);
    bbox->Upper.X = Maxf(bbox->Upper.X, corner->X);
    bbox->Lower.Y = Minf(bbox->Lower.Y, corner->Y);
    bbox->Upper.Y = Maxf(bbox->Upper.Y, corner->Y);
    bbox->Lower.Z = Minf(bbox->Lower.Z, corner->Bottom);
    bbox->Upper.Z = Maxf(bbox->Upper.Z, corner->Top);
}

BoundingBox GroupBoundingBox(Group const *group, Vec3 const *translation)
{
    BoundingBox bbox;
    InitBoundingBox(&bbox);

    size_t count = PtrArrayCount(group->Blocks);

    if (count > 0)
    {
        for (size_t i = 0; i < count; ++i)
        {
            Block const *block = GetPtrArray(group->Blocks, i);

            ExpandBoundingBoxByCorner(&bbox, block->Corners + 0);
            ExpandBoundingBoxByCorner(&bbox, block->Corners + 1);
            ExpandBoundingBoxByCorner(&bbox, block->Corners + 2);
            ExpandBoundingBoxByCorner(&bbox, block->Corners + 3);
        }

        // Don't translate if there are no blocks
        TranslateBoundingBox(&bbox, translation);
    }

    count = PtrArrayCount(group->Instances);
    Vec3 const zero = {0.0f, 0.0f, 0.0f};

    for (size_t i = 0; i < count; ++i)
    {
        GroupInstance *instance = GetPtrArray(group->Instances, i);
        BoundingBox childBBox = GroupInstanceBoundingBox(instance, &zero);
        ExpandBoundingBox(&bbox, &childBBox);
    }

    return bbox;
}

void UpdateGroupVertexArray(Group *group)
{
    ClearBuffer(group->Vertices);
    Buffer *buffer = CreateBuffer();

    size_t blockCount = PtrArrayCount(group->Blocks);

    for (size_t i = 0; i < blockCount; ++i)
    {
        Block *block = GetPtrArray(group->Blocks, i);
        GetBlockTriangles(block, buffer);
        AppendTriangles(group, buffer);
        ClearBuffer(buffer);
    }

    DestroyBuffer(buffer);
}

void DrawGroup(Group const *group)
{
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    Vec4 lightPos = {-1.0f, -2.0f, 3.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, &lightPos.X);

    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    DrawGroup_(group);

    glPopClientAttrib();
    glPopAttrib();
}

void DrawGroup_(Group const *group)
{
    Vertex *vertices = BufferData(group->Vertices);
    size_t vertexCount = BufferSize(group->Vertices) / sizeof(Vertex);

    glNormalPointer(GL_FLOAT, sizeof(Vertex), &vertices->Normal);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertices->Position);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    size_t instanceCount = PtrArrayCount(group->Instances);

    for (size_t i = 0; i < instanceCount; ++i)
    {
        DrawGroupInstance(GetPtrArray(group->Instances, i));
    }
}

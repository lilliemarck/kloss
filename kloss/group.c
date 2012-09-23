#include "group.h"
#include <kloss/algorithm.h>
#include <kloss/buffer.h>
#include <kloss/groupinstance.h>
#include <kloss/ptrarray.h>
#include <GL/gl.h>
#include <float.h>
#include <stdlib.h>

typedef struct vertex vertex;

struct vertex
{
    vec3 normal;
    vec3 position;
};

struct group
{
    ptrarray *blocks;
    ptrarray *instances;
    buffer *vertices;
    size_t refcount;
};

group *create_group(void)
{
    group *group = malloc(sizeof(group));
    group->blocks    = create_ptrarray();
    group->instances = create_ptrarray();
    group->vertices  = create_buffer();
    group->refcount  = 1;
    return group;
}

group *retain_group(group *group)
{
    ++group->refcount;
    return group;
}

void release_group(group *group)
{
    if (group && --group->refcount == 0)
    {
        size_t count = ptrarray_count(group->blocks);
        for (size_t i = 0; i < count; ++i)
        {
            destroy_block(get_ptrarray(group->blocks, i));
        }

        count = ptrarray_count(group->instances);
        for (size_t i = 0; i < count; ++i)
        {
            destroy_groupinstance(get_ptrarray(group->instances, i));
        }

        destroy_ptrarray(group->blocks);
        destroy_ptrarray(group->instances);
        destroy_buffer(group->vertices);

        free(group);
    }
}

static void append_triangles(group *group, buffer *buffer)
{
    triangle *triangles = buffer_data(buffer);
    size_t count = buffer_size(buffer) / sizeof(triangle);

    for (size_t i = 0; i < count; ++i)
    {
        vec3 normal;

        triangle_normal(&normal, &triangles[i]);
        vec3_normalize(&normal, &normal);

        vertex vertices[3] =
        {
            {normal, triangles[i].a},
            {normal, triangles[i].b},
            {normal, triangles[i].c}
        };

        append_buffer(group->vertices, vertices, sizeof(vertices));
    }
}

void insert_blocks(group *group, block **blocks, size_t count)
{
    buffer *buffer = create_buffer();

    for (size_t i = 0; i < count; ++i)
    {
        block *block = blocks[i];
        push_ptrarray(group->blocks, block);
        get_block_triangles(block, buffer);
        append_triangles(group, buffer);
        clear_buffer(buffer);
    }

    destroy_buffer(buffer);
}

void delete_blocks(group *group, block **blocks, size_t count)
{
    detatch_blocks(group, blocks, count);

    for (size_t i = 0; i < count; ++i)
    {
        destroy_block(blocks[i]);
    }
}

void detatch_blocks(group *group, block **blocks, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        remove_ptrarray(group->blocks, blocks[i]);
    }

    update_group_vertexarray(group);
}

void foreach_block(group *group, void (function)(block*,void*), void *data)
{
    size_t groupcount = ptrarray_count(group->blocks);

    for (size_t i = 0; i < groupcount; ++i)
    {
        function(get_ptrarray(group->blocks, i), data);
    }
}

void insert_groupinstance(group *group, groupinstance *instance)
{
    push_ptrarray(group->instances, instance);
}

void foreach_groupinstance(group *group, void (function)(groupinstance*,void*), void *data)
{
    size_t instancecount = ptrarray_count(group->instances);

    for (size_t i = 0; i < instancecount; ++i)
    {
        function(get_ptrarray(group->instances, i), data);
    }
}

pick pick_group_block(group const *group, ray const *ray)
{
    float nearest = FLT_MAX;
    block *nearestblock = NULL;
    triangle nearesttriangle;

    buffer *buffer = create_buffer();
    size_t blockcount = ptrarray_count(group->blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        block *block = get_ptrarray(group->blocks, i);

        get_block_triangles(block, buffer);

        triangle *tris = buffer_data(buffer);
        size_t tricount = buffer_size(buffer) / sizeof(triangle);

        for (size_t j = 0; j < tricount; ++j)
        {
            float temp;
            if (ray_intersect_triangle(&temp, ray, &tris[j]))
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
                    nearestblock = block;
                    nearesttriangle = tris[j];
                }
            }
        }

        clear_buffer(buffer);
    }

    destroy_buffer(buffer);

    vec3 intersection;
    vec3_add_scaled(&intersection, &ray->origin, &ray->direction, nearest);
    return (pick){nearestblock, nearesttriangle, intersection};
}

static bool project(vec3 *out, vec3 const *in, mat4 const *model, mat4 const *projection, viewport const *viewport)
{
    mat4 transform;

    mat4_transform(&transform, model, projection);
    vec3_transform(out, in, &transform);

    float w = transform.x.w * in->x +
              transform.y.w * in->y +
              transform.z.w * in->z +
              transform.t.w;

    if (out->z < -w || out->z > w)
    {
        return false;
    }

    out->x /= w;
    out->y /= w;
    out->z /= w;

    out->x = out->x * 0.5f + 0.5f;
    out->y = out->y * 0.5f + 0.5f;
    out->x = out->x * viewport->width + viewport->x;
    out->y = out->y * viewport->height + viewport->y;

    return true;
}

struct vertexpickdata
{
    mat4 const *modelview;
    mat4 const *projection;
    viewport const *viewport;
    vec2 const *mouse;
    float distance;
    cornerref cornerref;
};

void check_vertex(struct vertexpickdata *data, vec3 position, cornerref const *ref, uint8_t flag)
{
    float const radius = 5.0f;
    vec3 screenpos;

    if (project(&screenpos, &position, data->modelview, data->projection, data->viewport))
    {
        vec2 screenposxy = {screenpos.x, data->viewport->height - screenpos.y};
        float distance = vec2_distance(data->mouse, &screenposxy);

        if (distance < radius && screenpos.z < data->distance)
        {
            data->distance = screenpos.z;
            data->cornerref = (cornerref){ref->corner, flag};
        }
    }
}

bool pick_group_vertex(group const *group, mat4 const *model, mat4 const *projection, viewport const *viewport, vec2 const *mouse, cornerref *ref)
{
    struct vertexpickdata data =
    {
        model,
        projection,
        viewport,
        mouse,
        FLT_MAX
    };

    size_t blockcount = ptrarray_count(group->blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        block *block = get_ptrarray(group->blocks, i);
        cornerref cornerrefs[4];
        make_cornerrefs(cornerrefs, block);

        for (size_t i = 0; i < 4; ++i)
        {
            cornerref *cornerRef = cornerrefs + i;
            check_vertex(&data, cornertop   (cornerRef->corner), cornerRef, CORNERREF_TOP);
            check_vertex(&data, cornerbottom(cornerRef->corner), cornerRef, CORNERREF_BOTTOM);
        }
    }

    if (data.distance != FLT_MAX)
    {
        *ref = data.cornerref;
        return true;
    }
    else
    {
        return false;
    }
}

static void expand_bbox_with_corner(boundingbox *bbox, corner const *corner)
{
    bbox->lower.x = minf(bbox->lower.x, corner->x);
    bbox->upper.x = maxf(bbox->upper.x, corner->x);
    bbox->lower.y = minf(bbox->lower.y, corner->y);
    bbox->upper.y = maxf(bbox->upper.y, corner->y);
    bbox->lower.z = minf(bbox->lower.z, corner->bottom);
    bbox->upper.z = maxf(bbox->upper.z, corner->top);
}

boundingbox group_bounding_box(group const *group, vec3 const *translation)
{
    boundingbox bbox;
    init_boundingbox(&bbox);

    size_t count = ptrarray_count(group->blocks);

    if (count > 0)
    {
        for (size_t i = 0; i < count; ++i)
        {
            block const *block = get_ptrarray(group->blocks, i);

            expand_bbox_with_corner(&bbox, block->corners + 0);
            expand_bbox_with_corner(&bbox, block->corners + 1);
            expand_bbox_with_corner(&bbox, block->corners + 2);
            expand_bbox_with_corner(&bbox, block->corners + 3);
        }

        // Don't translate if there are no blocks
        translate_boundingbox(&bbox, translation);
    }

    count = ptrarray_count(group->instances);
    vec3 const zero = {0.0f, 0.0f, 0.0f};

    for (size_t i = 0; i < count; ++i)
    {
        groupinstance *instance = get_ptrarray(group->instances, i);
        boundingbox childBBox = groupinstance_boundingbox(instance, &zero);
        expand_boundingbox(&bbox, &childBBox);
    }

    return bbox;
}

void update_group_vertexarray(group *group)
{
    clear_buffer(group->vertices);
    buffer *buffer = create_buffer();
    size_t blockcount = ptrarray_count(group->blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        block *block = get_ptrarray(group->blocks, i);
        get_block_triangles(block, buffer);
        append_triangles(group, buffer);
        clear_buffer(buffer);
    }

    destroy_buffer(buffer);
}

void draw_group(group const *group)
{
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    vec4 lightpos = {-1.0f, -2.0f, 3.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, &lightpos.x);

    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    draw_group_(group);

    glPopClientAttrib();
    glPopAttrib();
}

void draw_group_(group const *group)
{
    vertex *vertices = buffer_data(group->vertices);
    size_t vertexcount = buffer_size(group->vertices) / sizeof(vertex);

    glNormalPointer(GL_FLOAT, sizeof(vertex), &vertices->normal);
    glVertexPointer(3, GL_FLOAT, sizeof(vertex), &vertices->position);
    glDrawArrays(GL_TRIANGLES, 0, vertexcount);

    size_t instancecount = ptrarray_count(group->instances);

    for (size_t i = 0; i < instancecount; ++i)
    {
        draw_groupinstance(get_ptrarray(group->instances, i));
    }
}

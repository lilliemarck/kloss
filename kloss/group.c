#include "group.h"
#include <kloss/algorithm.h>
#include <kloss/buffer.h>
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

struct groupdata
{
    ptrarray *blocks;
    ptrarray *groups;
    buffer *vertices;
    size_t refcount;
};

struct group
{
    struct vec3 position;
    struct groupdata *data;
};

static struct groupdata *create_groupdata(void)
{
    struct groupdata *groupdata = malloc(sizeof(struct groupdata));
    groupdata->blocks   = create_ptrarray();
    groupdata->groups   = create_ptrarray();
    groupdata->vertices = create_buffer();
    groupdata->refcount = 1;
    return groupdata;
}

static void release_groupdata(struct groupdata *groupdata)
{
    if (groupdata && --groupdata->refcount == 0)
    {
        size_t blockcount = ptrarray_count(groupdata->blocks);
        for (size_t i = 0; i < blockcount; ++i)
        {
            destroy_block(get_ptrarray(groupdata->blocks, i));
        }

        size_t groupcount = ptrarray_count(groupdata->groups);
        for (size_t i = 0; i < groupcount; ++i)
        {
            destroy_group(get_ptrarray(groupdata->groups, i));
        }

        destroy_ptrarray(groupdata->blocks);
        destroy_ptrarray(groupdata->groups);
        destroy_buffer(groupdata->vertices);

        free(groupdata);
    }
}

struct group *create_group(void)
{
    struct group *group = calloc(1, sizeof(struct group));
    group->data = create_groupdata();
    return group;
}

void destroy_group(struct group *group)
{
    if (group)
    {
        release_groupdata(group->data);
        free(group);
    }
}

static void append_triangles(struct groupdata *groupdata, buffer *buffer)
{
    triangle *triangles = buffer_data(buffer);
    size_t trianglecount = buffer_size(buffer) / sizeof(struct triangle);

    for (size_t i = 0; i < trianglecount; ++i)
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

        append_buffer(groupdata->vertices, vertices, sizeof(vertices));
    }
}

void insert_blocks(struct group *group, block **blocks, size_t count)
{
    buffer *buffer = create_buffer();

    for (size_t i = 0; i < count; ++i)
    {
        block *block = blocks[i];
        push_ptrarray(group->data->blocks, block);
        get_block_triangles(block, buffer);
        append_triangles(group->data, buffer);
        clear_buffer(buffer);
    }

    destroy_buffer(buffer);
}

void delete_blocks(struct group *group, block **blocks, size_t count)
{
    detatch_blocks(group, blocks, count);

    for (size_t i = 0; i < count; ++i)
    {
        destroy_block(blocks[i]);
    }
}

void detatch_blocks(struct group *group, block **blocks, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        remove_ptrarray(group->data->blocks, blocks[i]);
    }

    update_group_vertexarray(group);
}

static void foreach_block(struct groupdata *groupdata, void (function)(block*,void*), void *userdata)
{
    size_t groupcount = ptrarray_count(groupdata->blocks);

    for (size_t i = 0; i < groupcount; ++i)
    {
        function(get_ptrarray(groupdata->blocks, i), userdata);
    }
}

void insert_group(struct group *group, struct group *child)
{
    push_ptrarray(group->data->groups, child);
}

static void foreach_group(struct groupdata *groupdata, void (function)(struct group*,void*), void *userdata)
{
    size_t groupcount = ptrarray_count(groupdata->groups);

    for (size_t i = 0; i < groupcount; ++i)
    {
        function(get_ptrarray(groupdata->groups, i), userdata);
    }
}

pick pick_block(struct group const *group, ray const *ray)
{
    struct groupdata *groupdata = group->data;

    float nearest = FLT_MAX;
    block *nearestblock = NULL;
    triangle nearesttriangle;

    buffer *buffer = create_buffer();
    size_t blockcount = ptrarray_count(groupdata->blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        block *block = get_ptrarray(groupdata->blocks, i);

        get_block_triangles(block, buffer);

        triangle *tris = buffer_data(buffer);
        size_t tricount = buffer_size(buffer) / sizeof(struct triangle);

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

bool pick_vertex(struct group *group, mat4 const *model, mat4 const *projection, viewport const *viewport, vec2 const *mouse, cornerref *ref)
{
    struct groupdata *groupdata = group->data;

    struct vertexpickdata pickdata =
    {
        model,
        projection,
        viewport,
        mouse,
        FLT_MAX
    };

    size_t blockcount = ptrarray_count(groupdata->blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        block *block = get_ptrarray(groupdata->blocks, i);
        cornerref cornerrefs[4];
        make_cornerrefs(cornerrefs, block);

        for (size_t i = 0; i < 4; ++i)
        {
            cornerref *cornerRef = cornerrefs + i;
            check_vertex(&pickdata, cornertop   (cornerRef->corner), cornerRef, CORNERREF_TOP);
            check_vertex(&pickdata, cornerbottom(cornerRef->corner), cornerRef, CORNERREF_BOTTOM);
        }
    }

    if (pickdata.distance != FLT_MAX)
    {
        *ref = pickdata.cornerref;
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

void update_group_vertexarray(struct group *group)
{
    struct groupdata *groupdata = group->data;

    clear_buffer(groupdata->vertices);
    buffer *buffer = create_buffer();
    size_t blockcount = ptrarray_count(groupdata->blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        block *block = get_ptrarray(groupdata->blocks, i);
        get_block_triangles(block, buffer);
        append_triangles(groupdata, buffer);
        clear_buffer(buffer);
    }

    destroy_buffer(buffer);
}

static void draw_group_(struct group const *group)
{
    struct groupdata *groupdata = group->data;

    vertex *vertices = buffer_data(groupdata->vertices);
    size_t vertexcount = buffer_size(groupdata->vertices) / sizeof(struct vertex);

    glPushMatrix();
    glTranslatef(group->position.x, group->position.y, group->position.z);
    glNormalPointer(GL_FLOAT, sizeof(struct vertex), &vertices->normal);
    glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), &vertices->position);
    glDrawArrays(GL_TRIANGLES, 0, vertexcount);
    glPopMatrix();

    size_t groupcount = ptrarray_count(groupdata->groups);

    for (size_t i = 0; i < groupcount; ++i)
    {
        draw_group_(get_ptrarray(groupdata->groups, i));
    }
}

void draw_group(struct group const *group)
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

struct boundingbox group_boundingbox(struct group const *group)
{
    struct groupdata *groupdata = group->data;

    boundingbox bbox;
    init_boundingbox(&bbox);

    size_t blockcount = ptrarray_count(groupdata->blocks);

    if (blockcount > 0)
    {
        for (size_t i = 0; i < blockcount; ++i)
        {
            block const *block = get_ptrarray(groupdata->blocks, i);

            expand_bbox_with_corner(&bbox, block->corners + 0);
            expand_bbox_with_corner(&bbox, block->corners + 1);
            expand_bbox_with_corner(&bbox, block->corners + 2);
            expand_bbox_with_corner(&bbox, block->corners + 3);
        }

        // Don't translate if there are no blocks
        translate_boundingbox(&bbox, &group->position);
    }

    blockcount = ptrarray_count(groupdata->groups);

    for (size_t i = 0; i < blockcount; ++i)
    {
        struct group *group = get_ptrarray(groupdata->groups, i);
        boundingbox childbbox = group_boundingbox(group);
        expand_boundingbox(&bbox, &childbbox);
    }

    return bbox;
}

static void translate_block_(block *block, void *data)
{
    translate_block(block, data);
}

static void translate_group_(struct group *group, void *data)
{
    vec3_add(&group->position, &group->position, data);
}

void move_group_origin(struct group *group, vec3 const *position)
{
    vec3 translation;
    vec3_subtract(&translation, &group->position, position);

    foreach_block(group->data, translate_block_, &translation);
    foreach_group(group->data, translate_group_, &translation);

    group->position = *position;
    update_group_vertexarray(group);
}

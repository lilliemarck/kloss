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

static struct groupdata *retain_groupdata(struct groupdata *groupdata)
{
    ++groupdata->refcount;
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

struct group *copy_group(struct group *group)
{
    struct group *copy = calloc(1, sizeof(struct group));

    copy->position = group->position;
    copy->data     = retain_groupdata(group->data);

    return copy;
}

void destroy_group(struct group *group)
{
    if (group)
    {
        release_groupdata(group->data);
        free(group);
    }
}

vec3 get_group_position(struct group const *group)
{
    return group->position;
}

void set_group_position(struct group *group, vec3 pos)
{
    group->position = pos;
}

size_t child_group_count(struct group const *group)
{
    return ptrarray_count(group->data->groups);
}

static void append_triangles(struct groupdata *groupdata, buffer *buffer)
{
    triangle *triangles = buffer_data(buffer);
    size_t trianglecount = buffer_count(buffer, sizeof(struct triangle));

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

block **get_blocks(struct group *group)
{
    return ptrarray_data(group->data->blocks);
}

size_t block_count(struct group const *group)
{
    return ptrarray_count(group->data->blocks);
}

static void foreach_block(struct groupdata *groupdata, void (function)(block*,void*), void *userdata)
{
    size_t groupcount = ptrarray_count(groupdata->blocks);

    for (size_t i = 0; i < groupcount; ++i)
    {
        function(get_ptrarray(groupdata->blocks, i), userdata);
    }
}

void merge_group(struct group *group, struct group *child)
{
    struct groupdata *groupdata = group->data;
    struct groupdata *childdata = child->data;

    for (size_t i = 0; i < ptrarray_count(childdata->blocks); ++i)
    {
        struct block *copy = copy_block(get_ptrarray(childdata->blocks, i));
        translate_block(copy, &child->position);
        push_ptrarray(groupdata->blocks, copy);
    }

    for (size_t i = 0; i < ptrarray_count(childdata->groups); ++i)
    {
        struct group *copy = copy_group(get_ptrarray(childdata->groups, i));
        vec3_add(&copy->position, &copy->position, &child->position);
        insert_group(group, copy);
    }

    destroy_group(child);
    update_group_vertexarray(group);
}

void insert_group(struct group *group, struct group *child)
{
    push_ptrarray(group->data->groups, child);
}

void erase_group(struct group *group, struct group *child)
{
    remove_ptrarray(group->data->groups, child);
}

static void foreach_group(struct groupdata *groupdata, void (function)(struct group*,void*), void *userdata)
{
    size_t groupcount = ptrarray_count(groupdata->groups);

    for (size_t i = 0; i < groupcount; ++i)
    {
        function(get_ptrarray(groupdata->groups, i), userdata);
    }
}

struct nearest
{
    float distance;
    struct group *group;
    struct block *block;
    struct triangle triangle;
};

static void find_nearest_block(struct group *group, struct ray const *ray, struct nearest *nearest)
{
    struct groupdata *groupdata = group->data;

    struct ray groupspaceray;
    vec3_subtract(&groupspaceray.origin, &ray->origin, &group->position);
    groupspaceray.direction = ray->direction;

    buffer *buffer = create_buffer();
    ptrarray *blocks = groupdata->blocks;
    size_t blockcount = ptrarray_count(blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        block *block = get_ptrarray(blocks, i);

        get_block_triangles(block, buffer);

        triangle *tris = buffer_data(buffer);
        size_t tricount = buffer_count(buffer, sizeof(struct triangle));

        for (size_t j = 0; j < tricount; ++j)
        {
            float temp;
            if (ray_intersect_triangle(&temp, &groupspaceray, &tris[j]))
            {
                /*
                 * Use <= for distance comparsion here so that when there are
                 * multiple candidates for selection the last block added will
                 * be selected. It is useful after pasting that the new blocks
                 * can be dragged to a new location.
                 */
                if (temp <= nearest->distance)
                {
                    nearest->distance = temp;
                    nearest->group    = group;
                    nearest->block    = block;
                    nearest->triangle = tris[j];
                }
            }
        }

        clear_buffer(buffer);
    }

    destroy_buffer(buffer);

    struct ptrarray *groups = groupdata->groups;
    size_t groupcount = ptrarray_count(groups);

    for (size_t i = 0; i < groupcount; ++i)
    {
        struct group *childgroup = get_ptrarray(groups, i);
        find_nearest_block(childgroup, ray, nearest);
    }
}

pick pick_block(struct group *group, ray const *ray)
{
    struct nearest nearest = {FLT_MAX};

    find_nearest_block(group, ray, &nearest);

    vec3 intersection;
    vec3_add_scaled(&intersection, &ray->origin, &ray->direction, nearest.distance);
    return (pick){{nearest.block, nearest.group}, nearest.triangle, intersection};
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
    size_t vertexcount = buffer_count(groupdata->vertices, sizeof(struct vertex));

    glPushMatrix();
    glTranslatef(group->position.x, group->position.y, group->position.z);
    glNormalPointer(GL_FLOAT, sizeof(struct vertex), &vertices->normal);
    glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), &vertices->position);
    glDrawArrays(GL_TRIANGLES, 0, vertexcount);

    size_t groupcount = ptrarray_count(groupdata->groups);

    for (size_t i = 0; i < groupcount; ++i)
    {
        draw_group_(get_ptrarray(groupdata->groups, i));
    }

    glPopMatrix();
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

void foreach_block_in_hiearchy(
    struct group *group,
    struct vec3 const *parentpos,
    void (*proc)(struct blockref*, struct vec3 const *pos, void*),
    void *data)
{
    vec3 pos;
    vec3_add(&pos, parentpos, &group->position);

    struct groupdata *groupdata  = group->data;
    struct ptrarray  *blocks     = groupdata->blocks;
    size_t            blockcount = ptrarray_count(blocks);
    struct blockref   ref        = {NULL, group};

    for (size_t i = 0; i < blockcount; ++i)
    {
        ref.block = get_ptrarray(blocks, i);
        proc(&ref, &pos, data);
    }

    struct ptrarray *groups = groupdata->groups;
    size_t groupcount = ptrarray_count(groups);

    for (size_t i = 0; i < groupcount; ++i)
    {
        struct group *child = get_ptrarray(groups, i);
        foreach_block_in_hiearchy(child, &pos, proc, data);
    }
}

struct group *get_child_by_descendant(struct group const *group, struct group *descendant)
{
    struct groupdata *groupdata = group->data;
    struct ptrarray *groups = groupdata->groups;
    size_t groupcount = ptrarray_count(groups);

    for (size_t i = 0; i < groupcount; ++i)
    {
        struct group *child = get_ptrarray(groups, i);

        if (child == descendant)
        {
            return child;
        }

        if (get_child_by_descendant(child, descendant))
        {
            return child;
        }
    }

    return NULL;
}

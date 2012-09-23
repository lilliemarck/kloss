#pragma once

struct boundingbox;
struct group;
struct vec3;

typedef struct groupinstance groupinstance;

groupinstance *create_groupinstance(struct group *group);
void destroy_groupinstance(groupinstance *instance);
void draw_groupinstance(groupinstance const *instance);
struct boundingbox groupinstance_boundingbox(groupinstance const *instance, struct vec3 const* parentPos);
void move_groupinstance_origin(groupinstance *instance, struct vec3 const* position);

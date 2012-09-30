#include <check.h>
#include <kloss/group.h>
#include <kloss/ptrarray.h>

static struct group *group;

static void setup(void)
{
    group = create_group();
}

static void teardown(void)
{
    destroy_group(group);
    group = NULL;
}

START_TEST(add_child_group)
{
    struct group *child = create_group();
    insert_group(group, child);

    fail_unless(child_group_count(group) == 1, NULL);
    fail_unless(parent_group(child) == group);
}
END_TEST

START_TEST(destroy_child_group)
{
    struct group *child = create_group();
    insert_group(group, child);
    destroy_group(child);

    fail_unless(child_group_count(group) == 0, NULL);
}
END_TEST

START_TEST(merge_empty_group)
{
    struct group *child = create_group();
    insert_group(group, child);
    merge_group_into_parent(child);

    fail_unless(child_group_count(group) == 0, NULL);
}
END_TEST

START_TEST(merge_group_with_blocks)
{
    struct group *child = create_group();
    struct block *block = create_block();

    block->corners[0].x = 10;
    block->corners[3].bottom = 20;

    insert_blocks(child, &block, 1);
    insert_group(group, child);
    merge_group_into_parent(child);

    struct block **blocks = get_blocks(group);

    fail_unless(block_count(group) == 1, NULL);
    fail_unless(block->corners[0].x      == blocks[0]->corners[0].x, NULL);
    fail_unless(block->corners[0].bottom == blocks[0]->corners[0].bottom, NULL);
}
END_TEST

START_TEST(merge_translated_group)
{
    struct group *child = create_group();
    struct block *block = create_block();

    insert_blocks(child, &block, 1);
    insert_group(group, child);
    set_group_position(child, (vec3){1.0f, 2.0f, 3.0f});
    merge_group_into_parent(child);

    struct block **blocks = get_blocks(group);

    fail_unless(blocks[0]->corners[0].x      == 1.0f, NULL);
    fail_unless(blocks[0]->corners[0].y      == 2.0f, NULL);
    fail_unless(blocks[0]->corners[0].top    == 3.0f, NULL);
    fail_unless(blocks[0]->corners[0].bottom == 3.0f, NULL);
}
END_TEST

/*
 * Coordinates in the child group should remain unchanged after they are merged
 * into the parent.
 */
START_TEST(merge_group_to_translated_parent)
{
    set_group_position(group, (vec3){1.0f, 2.0f, 3.0f});

    struct group *child = create_group();
    struct block *block = create_block();

    insert_blocks(child, &block, 1);
    insert_group(group, child);
    merge_group_into_parent(child);

    struct block **blocks = get_blocks(group);

    fail_unless(blocks[0]->corners[0].x      == 0.0f, NULL);
    fail_unless(blocks[0]->corners[0].y      == 0.0f, NULL);
    fail_unless(blocks[0]->corners[0].top    == 0.0f, NULL);
    fail_unless(blocks[0]->corners[0].bottom == 0.0f, NULL);
}
END_TEST

Suite *group_suite(void)
{
    Suite *s = suite_create("Group");
    TCase *tc;

    tc = tcase_create("Nesting");
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, add_child_group);
    tcase_add_test(tc, destroy_child_group);
    suite_add_tcase(s, tc);

    tc = tcase_create("Merging");
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, merge_empty_group);
    tcase_add_test(tc, merge_group_with_blocks);
    tcase_add_test(tc, merge_translated_group);
    tcase_add_test(tc, merge_group_to_translated_parent);
    suite_add_tcase(s, tc);

    return s;
}

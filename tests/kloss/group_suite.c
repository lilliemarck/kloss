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

Suite *group_suite(void)
{
    Suite *s = suite_create("Group");

    TCase *tc = tcase_create("Nesting");
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, add_child_group);
    tcase_add_test(tc, destroy_child_group);
    suite_add_tcase(s, tc);

    return s;
}

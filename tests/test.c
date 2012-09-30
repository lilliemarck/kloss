#include <check.h>
#include <stdlib.h>

extern Suite *group_suite(void);

int main(void)
{
    SRunner *sr = srunner_create(NULL);
    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_add_suite(sr, group_suite());

    srunner_run_all (sr, CK_NORMAL);
    int failures = srunner_ntests_failed(sr);
    srunner_free (sr);
    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

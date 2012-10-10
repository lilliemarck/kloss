#include <gtest/gtest.h>
#include <memory>

extern "C" {
#include <kloss/group.h>
#include <kloss/ptrarray.h>
} // extern "C"

class group_test : public testing::Test
{
public:
    group_test() : group_(create_group())
    {
        std::cout << "construct\n";
    }

    ~group_test()
    {
        std::cout << "destructr\n";
        destroy_group(group_);
    }

    void SetUp() override
    {
        std::cout << "setup\n";
    }

    void TearDown() override
    {
        std::cout << "teardown\n";
    }

    group* group_;
};

TEST_F(group_test, add_child_group)
{
    struct group *child = create_group();
    insert_group(group_, child);

    EXPECT_EQ(1u, child_group_count(group_));
}

TEST_F(group_test, merge_empty_group)
{
    struct group *child = create_group();
    merge_group(group_, child);

    EXPECT_EQ(0u, child_group_count(group_));
}

TEST_F(group_test, merge_group_with_blocks)
{
    struct group *child = create_group();
    struct block *block = create_block();

    block->corners[0].x = 10;
    block->corners[3].bottom = 20;

    insert_blocks(child, &block, 1);
    merge_group(group_, child);

    struct block **blocks = get_blocks(group_);

    EXPECT_EQ(1u, block_count(group_));
    EXPECT_EQ(10.0f, blocks[0]->corners[0].x);
    EXPECT_EQ(20.0f, blocks[0]->corners[3].bottom);
}

TEST_F(group_test, merge_translated_group)
{
    struct group *child = create_group();
    struct block *block = create_block();

    insert_blocks(child, &block, 1);
    set_group_position(child, {1.0f, 2.0f, 3.0f});
    merge_group(group_, child);

    struct block **blocks = get_blocks(group_);

    EXPECT_EQ(1.0f, blocks[0]->corners[0].x);
    EXPECT_EQ(2.0f, blocks[0]->corners[0].y);
    EXPECT_EQ(3.0f, blocks[0]->corners[0].top);
    EXPECT_EQ(3.0f, blocks[0]->corners[0].bottom);
}

/*
 * Coordinates in the child group should remain unchanged after they are merged
 * into the parent.
 */
TEST_F(group_test, merge_group_to_translated_parent)
{
    set_group_position(group_, {1.0f, 2.0f, 3.0f});

    struct group *child = create_group();
    struct block *block = create_block();

    insert_blocks(child, &block, 1);
    merge_group(group_, child);

    struct block **blocks = get_blocks(group_);

    EXPECT_EQ(0.0f, blocks[0]->corners[0].x);
    EXPECT_EQ(0.0f, blocks[0]->corners[0].y);
    EXPECT_EQ(0.0f, blocks[0]->corners[0].top);
    EXPECT_EQ(0.0f, blocks[0]->corners[0].bottom);
}

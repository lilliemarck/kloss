#include <gtest/gtest.h>
#include <kloss/group.hpp>
#include <kloss/memory.hpp>
#include <memory>

class group_test : public testing::Test
{
public:
    group_test() : group_(make_unique<group>())
    {
    }

    group_ptr group_;
};

TEST_F(group_test, add_child_group)
{
    group_->insert(std::make_shared<group>());

    EXPECT_EQ(1u, group_->group_count());
}

TEST_F(group_test, merge_empty_group)
{
    group_ptr child = std::make_shared<group>();
    group_->merge(child);

    EXPECT_EQ(0u, group_->group_count());
}

TEST_F(group_test, merge_group_with_blocks)
{
    group_ptr child = std::make_shared<group>();
    block_ptr block = std::make_shared<struct block>();

    block->corners[0].x = 10;
    block->corners[3].bottom = 20;

    child->insert(&block, 1);
    group_->merge(child);

    block_ptr first_block = *group_->begin_blocks();

    EXPECT_EQ(1u, group_->block_count());
    EXPECT_EQ(10.0f, first_block->corners[0].x);
    EXPECT_EQ(20.0f, first_block->corners[3].bottom);
}

TEST_F(group_test, merge_translated_group)
{
    group_ptr child = std::make_shared<group>();
    block_ptr block = std::make_shared<struct block>();

    child->insert(&block, 1);
    child->set_position({1.0f, 2.0f, 3.0f});
    group_->merge(child);

    block_ptr first_block = *group_->begin_blocks();

    EXPECT_EQ(1.0f, first_block->corners[0].x);
    EXPECT_EQ(2.0f, first_block->corners[0].y);
    EXPECT_EQ(3.0f, first_block->corners[0].top);
    EXPECT_EQ(3.0f, first_block->corners[0].bottom);
}

/*
 * Coordinates in the child group should remain unchanged after they are merged
 * into the parent.
 */
TEST_F(group_test, merge_group_to_translated_parent)
{
    group_->set_position({1.0f, 2.0f, 3.0f});

    group_ptr child = std::make_shared<group>();
    block_ptr block = std::make_shared<struct block>();

    child->insert(&block, 1);
    group_->merge(child);

    block_ptr first_block = *group_->begin_blocks();

    EXPECT_EQ(0.0f, first_block->corners[0].x);
    EXPECT_EQ(0.0f, first_block->corners[0].y);
    EXPECT_EQ(0.0f, first_block->corners[0].top);
    EXPECT_EQ(0.0f, first_block->corners[0].bottom);
}

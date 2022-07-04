
#include "Queue.h"

#include <stdlib.h>
#include <gtest/gtest.h>

#define CHAR_SIZE 5

struct queue *q;

TEST(MyTest, basic)
{
    q = queue_new();

    ASSERT_EQ(queue_peek_head(NULL), nullptr);
    ASSERT_EQ(queue_push(nullptr, nullptr), -1);
    ASSERT_EQ(queue_get_length(q), 0);

    char *test_data = (char *)malloc(CHAR_SIZE);

    for (size_t i = 0; i < CHAR_SIZE - 1; i++)
    {
        test_data[i] = 'a';
    }
    test_data[CHAR_SIZE - 1] = '\0';

    queue_push(q, test_data);

    ASSERT_EQ(queue_get_length(q), 1);

    char *test_data2 = (char *)malloc(CHAR_SIZE);

    for (size_t i = 0; i < CHAR_SIZE - 1; i++)
    {
        test_data[i] = 'b';
    }
    test_data[CHAR_SIZE - 1] = '\0';

    queue_push(q, test_data2);

    ASSERT_EQ(queue_get_length(q), 2);

    queue_pop_free_head(q);

    ASSERT_EQ(queue_get_length(q), 1);

    queue_free_all(q);

    ASSERT_EQ(queue_get_length(q), 0);
}
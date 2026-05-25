#include <gtest/gtest.h>

#include <helpers/FixedDataContainer.h>

namespace {

using namespace testing;
using namespace mw::helpers;

constexpr int ITEMS_NUM = 2;

class FixedDataContainer_tests : public Test {
public:
    ~FixedDataContainer_tests() = default;
};

TEST_F(FixedDataContainer_tests, pushBack_one_elem) {
    FixedDataContainer<int> container{ITEMS_NUM};

    container.pushBack(int{1});
    EXPECT_EQ(container.getItems().size(), 1);
    EXPECT_EQ(container.getItem(0), 1);
    EXPECT_EQ(container.getItem(1), 0);
}

TEST_F(FixedDataContainer_tests, pushBack_out_of_scope) {
    FixedDataContainer<int> container{ITEMS_NUM};
    std::array<int, ITEMS_NUM + 2> datas = {1, 2, 3, 4};

    for (const auto& data : datas) {
        container.pushBack(data);
    }

    auto items = container.getItems();
    EXPECT_EQ(items.size(), ITEMS_NUM);
    EXPECT_EQ(items[0], 3);
    EXPECT_EQ(items[1], 4);
}

} // anonymous
#pragma once

#include <vector>

namespace mw { namespace helpers {

template<typename ITEM_T>
class FixedDataContainer {
public:
    FixedDataContainer(const std::size_t maxNumItems) noexcept;

    void pushBack(const ITEM_T& item);

    const std::vector<ITEM_T>& getItems() const;
    const ITEM_T& getItem(const std::size_t idx) const;

private:
    const std::size_t maxNumItems;
    const ITEM_T emptyItem;
    std::vector<ITEM_T> items;
};

template<typename ITEM_T>
FixedDataContainer<ITEM_T>::FixedDataContainer(const std::size_t maxNumItems) noexcept :
    maxNumItems{maxNumItems},
    emptyItem{}
{
    items.reserve(this->maxNumItems);
}

template<typename ITEM_T>
void FixedDataContainer<ITEM_T>::pushBack(const ITEM_T& item) {
    if (items.size() < maxNumItems) {
        items.push_back(item);
    } else {
        for (std::size_t i = 0; i < maxNumItems - 1; ++i) {
            items[i] = items[i + 1];
        }
        items[maxNumItems - 1] = item;
    }
}

template<typename ITEM_T>
const std::vector<ITEM_T>& FixedDataContainer<ITEM_T>::getItems() const {
    return items;
}

template<typename ITEM_T>
const ITEM_T& FixedDataContainer<ITEM_T>::getItem(const std::size_t idx) const {
    if (idx < items.size()) {
        return items[idx];
    }
    return emptyItem;
}

} } // mw::helpers
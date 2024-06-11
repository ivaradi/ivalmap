//----------------------------------------------------------------------------

#include <map>
#include <cstdio>

//----------------------------------------------------------------------------

/**
 * An interval map storing values mapped to ranges of the keys.
 *
 * Key and Value must be both copyable and assignable. Key must be comparable
 * with operator<, Value with operator==.
 */
template <typename Key, typename Value>
class IntervalMap
{
private:
    /**
     * The value for the keys before the first value in the map.
     */
    Value startValue;

    /**
     * The intervals as a map. The key is the first key of the interval.
     * The value is the value mapped to the key and all subsequent key values
     * until, and not including the next larger key.
     *
     * If the map is empty, all values are mapped to startValue.
     */
    std::map<Key, Value> intervals;

public:
    /**
     * Construct the interval map with the given value mapped to all keys.
     */
    IntervalMap(const Value& initialValue);

    /**
     * Set a value to the given key range. from is included in the
     * range, to is not.
     */
    void set(const Key& from, const Key& to, const Value& value);

    /**
     * Get the value for the given key.
     */
    const Value& get(const Key& key) const;
};

//------------------------------------------------------------------------------

template <typename Key, typename Value>
inline IntervalMap<Key, Value>::IntervalMap(const Value& initialValue) :
    startValue(initialValue)
{
}

//------------------------------------------------------------------------------

template <typename Key, typename Value>
inline void IntervalMap<Key, Value>::set(const Key& from, const Key& to, const Value& value)
{
    if (from < to && (!intervals.empty() || !(value==startValue))) {
        if (intervals.empty()) {
            auto it = intervals.emplace(to, startValue);
            intervals.emplace_hint(it.first, from, value);
        } else {
            auto it = intervals.lower_bound(from);

            if (it==intervals.end()) {
                auto previous = it; --previous;
                if (!(previous->second==value)) {
                    it = intervals.emplace_hint(it, to, previous->second);
                    intervals.emplace_hint(it, from, value);
                }
            } else {
                Value const* lastValue = &startValue;
                bool assignBegin = false;
                bool keepBegin = false;

                if (it!=intervals.begin()) {
                    auto previous = it; --previous;
                    if (previous->second==value) {
                        it = previous;
                        lastValue = &value;
                    } else if (from < it->first) {
                        it = intervals.emplace_hint(it, from, value);
                        lastValue = &previous->second;
                    } else {
                        lastValue = &it->second;
                        assignBegin = true;
                    }
                    keepBegin = true;
                } else if (!(value==startValue)) {
                    if (from < it->first) {
                        it = intervals.emplace_hint(it, from, value);
                    } else {
                        lastValue = &it->second;
                        assignBegin = true;
                    }
                    keepBegin = true;
                }

                auto begin = it;
                if (keepBegin) ++it;
                for(; it!=intervals.end() && it->first < to; ++it) {
                    lastValue = &it->second;
                }

                if (it==intervals.end() || to < it->first) {
                    if (!(*lastValue==value)) {
                        it = intervals.emplace_hint(it, to, *lastValue);
                    }
                } else if (!(it->first < to)) {
                    if (value==it->second) {
                        it++;
                    }
                }

                if (assignBegin) begin->second = value;
                if (keepBegin) ++begin;
                while(begin!=it) {
                    auto next = begin; ++next;
                    intervals.erase(begin);
                    begin = next;
                }
            }
        }
    }
}

//------------------------------------------------------------------------------

template <typename Key, typename Value>
inline const Value& IntervalMap<Key, Value>::get(const Key& key) const
{
    auto it = intervals.upper_bound(key);
    return (it==intervals.begin()) ? startValue : (--it)->second;
}

#ifndef CPP_LAB_12_FILTERITERATOR_H
#define CPP_LAB_12_FILTERITERATOR_H

#include <iterator>
#include <functional>
#include <iostream>

template<typename I, typename T>
class FilterIterator : std::iterator<
        typename std::iterator_traits<I>::iterator_category,
        T,
        typename std::iterator_traits<I>::difference_type,
        typename std::iterator_traits<I>::pointer,
        typename std::iterator_traits<I>::reference
> {

private:
    const bool jumpMode;
    const I start;
    const I end;
    I iter;

    const std::function<bool(T)> func;

    FilterIterator skip(int amount) {
        while (amount != 0) {
            if ((amount > 0 ? end : start) == iter) {
                if (!jumpMode) {
                    return *this;
                }
                // no next elements, jump to opposite edge
                iter = amount > 0 ? start : end;
            } else {
                if (amount > 0) {
                    ++iter;
                } else {
                    --iter;
                }
            }

            if (func(*iter)) {
                amount = amount > 0 ? amount - 1 : amount + 1;
            }
        }
        return *this;
    }

    void skipUntil(int side) {
        if (side != -1 && side != 1) {
            throw std::invalid_argument("side must be 0 or 1");
        }

        while (!func(*iter) && (side < 0 ? start : end) != iter) {
            iter += side;
        }
    }

    FilterIterator clone() const {
        FilterIterator fi(start, end, func);
        fi.iter = this->iter;
        return fi;
    }

public:
    FilterIterator(I iter, I end, const std::function<bool(T)> func)
            : FilterIterator(iter, end, true, func) {
    }

    FilterIterator(I iter, I end, bool jumpMode, const std::function<bool(T)> func)
            : jumpMode(jumpMode), start(iter), iter(iter), end(end), func(func) {
        this->skipUntil(1);
    }

    ~FilterIterator() = default;

    T operator*() const {
        return *iter;
    }

    FilterIterator &operator++() {
        this->skip(1);
        return *this;
    }

    FilterIterator &operator--() {
        this->skip(-1);
        return *this;
    }

    // NOLINTNEXTLINE
    FilterIterator operator++(int) {
        FilterIterator temp = clone();
        this->skip(1);
        return temp;
    }

    // NOLINTNEXTLINE
    FilterIterator operator--(int) {
        FilterIterator temp = clone();
        this->skip(-1);
        return temp;
    }

    FilterIterator operator+(const int &positions) const {
        return clone().skip(positions);
    }

    FilterIterator operator-(const int &positions) const {
        return clone().skip(-positions);
    }

    FilterIterator operator+=(const int &positions) {
        this->skip(positions);
        return *this;
    }

    FilterIterator operator-=(const int &positions) {
        this->skip(-positions);
        return *this;
    }

    bool operator==(FilterIterator fi) const {
        return fi.iter == this->iter;
    }

    bool operator!=(FilterIterator fi) const {
        return fi.iter != this->iter;
    }

    FilterIterator getEnd() const {
        FilterIterator fi = clone();
        fi.iter = fi.end;
        fi.skipUntil(-1);
        return fi;
    }
};

// Deduction guide
template<typename Iter>
FilterIterator(Iter, Iter) -> FilterIterator<Iter, typename std::iterator_traits<Iter>::value_type>;

#endif //CPP_LAB_12_FILTERITERATOR_H

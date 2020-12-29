#ifndef ZOAL_ALGORITHM_KMP_HPP
#define ZOAL_ALGORITHM_KMP_HPP

namespace zoal { namespace algorithm {
    class kmp;

    template<class T, class B>
    class kmp_pattern {
    public:
        kmp_pattern(T pattern, B *prefix_buffer, size_t length)
            : pattern_(pattern)
            , prefix_(prefix_buffer)
            , length_(length) {
            prepare_prefix();
        }

        kmp_pattern(const kmp_pattern<T, B> &p)
            : pattern_(p.pattern_)
            , prefix_(p.prefix_)
            , length_(p.length_) {
        }
    private:
        void prepare_prefix() {
            prefix_[0] = 0;
            auto iter = pattern_;
            for (size_t k = 0, i = 1; *iter; ++i, ++iter) {
                while ((k > 0) && (pattern_[i] != pattern_[k])) {
                    k = prefix_[k - 1];
                }

                if (pattern_[i] == pattern_[k]) {
                    ++k;
                }

                prefix_[i] = k;
            }
        }

        T pattern_;
        B *prefix_;
        size_t length_;

        friend class kmp;
    };

    template <class T, class B>
    inline kmp_pattern<T, B> make_kmp_pattern(T pattern, B *prefix_buffer, size_t length) {
        return kmp_pattern<T, B>(pattern, prefix_buffer, length);
    }

    class kmp {
    public:
        template<class T, class B, class Y>
        intptr_t search(const kmp_pattern<T, B> &p, const Y start, const Y end) {
            auto ptr = start;
            for (pattern_index_ = 0; ptr != end; ++ptr) {
                auto value = *ptr;
                while ((pattern_index_ > 0) && (p.pattern_[pattern_index_] != value)) {
                    pattern_index_ = p.prefix_[pattern_index_ - 1];
                }

                if (p.pattern_[pattern_index_] == value) {
                    ++pattern_index_;
                }

                if (pattern_index_ == p.length_) {
                    return ptr - start - p.length_ + 1;
                }
            }

            return -1;
        }

        void reset() {
            pattern_index_ = 0;
        }

    private:
        uintptr_t pattern_index_{0};
    };
}}

#endif

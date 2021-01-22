#include "gtest/gtest.h"
#include <zoal/io/output_stream.hpp>
#include <zoal/utils/vt100.hpp>

struct mem_transport_clear_functor : zoal::io::transport_functor<mem_transport_clear_functor> {
    template<class L>
    inline void apply(L &mb) {
        mb.ptr = mb.buffer;
        mb.end = mb.buffer + mb.size;
        *mb.ptr = 0;
    }
};

class mem_transport {
public:
    mem_transport(void *b, size_t size)
        : buffer(reinterpret_cast<uint8_t *>(b))
        , ptr(buffer)
        , size(size)
        , end(buffer + size) {}

    void send_byte(uint8_t value) {
        *ptr++ = value;
    }

    void send_data(const void *data, size_t size) {
        const auto *p = reinterpret_cast<const uint8_t *>(data);
        while (size > 0) {
            *ptr++ = *p++;
            size--;
        }
    }

    template<class F>
    inline void apply_functor(::zoal::io::transport_functor<F> &fn) {
        static_cast<F &>(fn).apply(*this);
    }

    uint8_t *buffer;
    uint8_t *ptr;
    size_t size;
    uint8_t *end;
};

TEST(output_stream, output_char) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << 'H';
    EXPECT_STREQ(buffer, "H");
}

TEST(output_stream, output_c_string) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << "Hello";
    EXPECT_STREQ(buffer, "Hello");
}

TEST(output_stream, output_uint) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << static_cast<uint8_t>(10) << " ";
    stream << static_cast<uint16_t>(20) << " ";
    stream << static_cast<uint32_t>(30) << " ";
    stream << static_cast<uint64_t>(40);
    EXPECT_STREQ(buffer, "10 20 30 40");
}

TEST(output_stream, output_uint_hex) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << zoal::io::hex;
    stream << static_cast<uint8_t>(10) << " ";
    stream << static_cast<uint16_t>(20) << " ";
    stream << static_cast<uint32_t>(30) << " ";
    stream << static_cast<uint64_t>(40);
    EXPECT_STREQ(buffer, "A 14 1E 28");
}

TEST(output_stream, output_int) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << static_cast<int8_t>(10) << " " << static_cast<int8_t>(-10) << " ";
    stream << static_cast<int16_t>(20) << " " << static_cast<int16_t>(-20) << " ";
    stream << static_cast<int32_t>(30) << " " << static_cast<int32_t>(-30) << " ";
    stream << static_cast<int64_t>(40) << " " << static_cast<int64_t>(-40);
    EXPECT_STREQ(buffer, "10 -10 20 -20 30 -30 40 -40");
}

TEST(output_stream, output_double) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << static_cast<double>(0.0) << " " << static_cast<double>(40.05) << " " << static_cast<double>(-40.25);
    EXPECT_STREQ(buffer, "0.00 40.05 -40.25");
}

TEST(output_stream, output_float) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << static_cast<float>(0.0) << " " << static_cast<float>(40.05) << " " << static_cast<float>(-40.25);
    EXPECT_STREQ(buffer, "0.00 40.05 -40.25");
}

TEST(output_stream, output_double_precision_0) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << zoal::io::setprecision(0);
    stream << static_cast<double>(0.0) << " " << static_cast<double>(40.05) << " " << static_cast<double>(-40.25);
    EXPECT_STREQ(buffer, "0 40 -40");
}

TEST(output_stream, output_ptr) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    void *ptr = reinterpret_cast<void *>(0xDEADBEEF);
    stream << ptr;
    EXPECT_STREQ(buffer, "0x00000000DEADBEEF");
}

TEST(output_stream, output_functor_right_value) {
    using namespace zoal::utils::vt100;
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << color(3, 9) << "Text" << sgr0() << cr_lf();
    EXPECT_STREQ(buffer, "\x1B[49;33mText\x1B[m\r\n");
}

TEST(output_stream, output_functor_ref) {
    using namespace zoal::utils::vt100;
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    auto a = color(3, 9);
    auto b = sgr0();
    auto c = cr_lf();
    stream << a << "Text" << b << c;
    EXPECT_STREQ(buffer, "\x1B[49;33mText\x1B[m\r\n");
}

TEST(output_stream, transport_functor) {
    using namespace zoal::utils::vt100;
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);

    stream << "Hello";
    EXPECT_STREQ(buffer, "Hello");

    stream << mem_transport_clear_functor();
    EXPECT_STREQ(buffer, "");

    stream << "Hello";
    EXPECT_STREQ(buffer, "Hello");
}

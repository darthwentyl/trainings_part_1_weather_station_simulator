#include <gtest/gtest.h>

#include <filesystem>

#include <helpers/FixedLinesFileWriter.h>

namespace {

using namespace testing;
using namespace mw::helpers;
using namespace mw::exceptions;

namespace fs = std::filesystem;

constexpr const char* TEST_FILE = "test.file";
constexpr const std::size_t MAX_NUMBER_OF_LINES = 2;
constexpr const std::size_t NUMBER_OF_SAMPLES = MAX_NUMBER_OF_LINES + 5;

class FixedLinesFileWriter_tests : public Test {
public:
    ~FixedLinesFileWriter_tests() = default;

protected:
    void SetUp() {
        for (size_t i = 0; i < NUMBER_OF_SAMPLES; ++i) {
            datas[i] = i;
        }
    }

    void TearDown() {
        if (fs::exists(TEST_FILE)) {
            fs::remove(TEST_FILE);
        }
    }

    std::array<std::size_t, NUMBER_OF_SAMPLES> datas;
};

TEST_F(FixedLinesFileWriter_tests, empty_file_name) {
    FixedLinesFileWriter writer{std::string{}, 0};

    try {
        writer.write<int>(123);
        EXPECT_FALSE(true);
    } catch (const fixed_lines_file_writer_error& e) {
        std::cout << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(FixedLinesFileWriter_tests, numbers_of_lines_zero) {
    FixedLinesFileWriter writer{std::string{TEST_FILE}, 0};

    try {
        writer.write<double>(321);
        EXPECT_FALSE(true);
    } catch (const fixed_lines_file_writer_error& e) {
        std::cout << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(FixedLinesFileWriter_tests, write_elem) {
    FixedLinesFileWriter writer{std::string{TEST_FILE}, MAX_NUMBER_OF_LINES};
    const unsigned elem = 1234;

    try {
        writer.write<unsigned>(elem);
        EXPECT_TRUE(fs::exists(TEST_FILE));
        std::ifstream ifs{TEST_FILE};
        EXPECT_TRUE(ifs);
        unsigned readElem = 0;
        ifs >> readElem;
        EXPECT_EQ(readElem, elem);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(FixedLinesFileWriter_tests, write_more_than_lines) {
    FixedLinesFileWriter writer{std::string{TEST_FILE}, MAX_NUMBER_OF_LINES};

    try {
        for (const auto& item : datas) {
            writer.write<std::size_t>(item);
        }

        std::ifstream ifs{TEST_FILE};
        std::size_t elem;
        std::size_t lines = 0;
        while (ifs >> elem) {
            ++lines;
        }
        EXPECT_EQ(lines, MAX_NUMBER_OF_LINES);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(FixedLinesFileWriter_tests, write_more_than_lines_and_valid_data) {
    FixedLinesFileWriter writer{std::string{TEST_FILE}, MAX_NUMBER_OF_LINES};

    try {
        for (const auto& item : datas) {
            writer.write<std::size_t>(item);
        }

        std::ifstream ifs{TEST_FILE};
        std::size_t elem1;
        std::size_t elem2;
        ifs >> elem1;
        ifs >> elem2;
        EXPECT_EQ(elem1, datas[NUMBER_OF_SAMPLES - MAX_NUMBER_OF_LINES]);
        EXPECT_EQ(elem2, datas[NUMBER_OF_SAMPLES - MAX_NUMBER_OF_LINES + 1]);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(FixedLinesFileWriter_tests, write_to_the_exist_measurement_file_less_than_max_line_numbers) {
    std::ofstream ofs{TEST_FILE};
    ofs << datas[0] << std::endl;

    FixedLinesFileWriter writer{std::string{TEST_FILE}, MAX_NUMBER_OF_LINES};

    try {
        for (const auto& item : datas) {
            writer.write<std::size_t>(item);
        }

        std::ifstream ifs{TEST_FILE};
        std::size_t elem1;
        std::size_t elem2;
        ifs >> elem1;
        ifs >> elem2;
        EXPECT_EQ(elem1, datas[NUMBER_OF_SAMPLES - MAX_NUMBER_OF_LINES]);
        EXPECT_EQ(elem2, datas[NUMBER_OF_SAMPLES - MAX_NUMBER_OF_LINES + 1]);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(FixedLinesFileWriter_tests, write_to_the_exist_measurement_file_greatest_than_max_line_numbers) {
    std::ofstream ofs{TEST_FILE};
    for (std::size_t i = 0; i < 4; ++i) {
        ofs << datas[i] << std::endl;
    }

    FixedLinesFileWriter writer{std::string{TEST_FILE}, MAX_NUMBER_OF_LINES};

    try {
        for (const auto& item : datas) {
            writer.write<std::size_t>(item);
        }

        std::ifstream ifs{TEST_FILE};
        std::size_t elem1;
        std::size_t elem2;
        ifs >> elem1;
        ifs >> elem2;
        EXPECT_EQ(elem1, datas[NUMBER_OF_SAMPLES - MAX_NUMBER_OF_LINES]);
        EXPECT_EQ(elem2, datas[NUMBER_OF_SAMPLES - MAX_NUMBER_OF_LINES + 1]);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

} // anonymous
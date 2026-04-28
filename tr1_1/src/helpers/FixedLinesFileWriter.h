#pragma once

#include <exceptions/fixed_lines_file_writer_error.h>

#include <logger/Log.h>

#include <string>
#include <filesystem>
#include <fstream>

namespace mw { namespace helpers {

class FixedLinesFileWriter {
public:
    FixedLinesFileWriter(const std::string& fileName, const std::size_t maxNumOfLines) noexcept;

    template<typename ELEM_T>
    void write(const ELEM_T& elem);

private:
    std::size_t countLines() noexcept;

    const std::string fileName;
    const std::size_t maxNumOfLines;
    std::size_t currNumOfLines;
};

template<typename ELEM_T>
void FixedLinesFileWriter::write(const ELEM_T& elem) {
    if (fileName.empty()) {
        throw exceptions::fixed_lines_file_writer_error{__FUNCTION__, __LINE__, "File name is empty"};
    }
    if (maxNumOfLines == 0) {
        throw exceptions::fixed_lines_file_writer_error{__FUNCTION__, __LINE__, "Numbers of lines is zero"};
    }

    if (currNumOfLines < maxNumOfLines) {
        std::ofstream ofs{fileName, std::ios::app};
        ofs << elem << std::endl;
        ++currNumOfLines;
    } else {
        const std::string tmpFile = fileName + ".tmp";

        std::ifstream ifs{fileName, std::ios::binary};
        std::string skipLine;
        while (currNumOfLines != maxNumOfLines) {
            std::getline(ifs, skipLine);
            --currNumOfLines;
        }
        std::getline(ifs, skipLine);

        std::ofstream ofs{tmpFile, std::ios::binary};
        ofs << ifs.rdbuf();
        ofs << elem << std::endl;

        ifs.close();
        ofs.close();

        std::filesystem::rename(tmpFile, fileName);
        std::filesystem::remove(tmpFile);
        currNumOfLines = maxNumOfLines;
    }
}

} } // mw::helpers
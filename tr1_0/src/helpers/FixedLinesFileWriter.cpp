#include <helpers/FixedLinesFileWriter.h>

#include <algorithm>
#include <iterator>

namespace mw { namespace helpers {

FixedLinesFileWriter::FixedLinesFileWriter(
    const std::string& fileName,
    const std::size_t maxNumOfLines) noexcept :
    fileName{fileName},
    maxNumOfLines{maxNumOfLines}
{
    currNumOfLines = countLines();
    DEBUG("currNumOfLines: " << currNumOfLines);
}

std::size_t FixedLinesFileWriter::countLines() noexcept {
    std::ifstream ifs{fileName, std::ios::binary};
    if (!ifs) {
        return 0;
    }

    return std::count(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>(),
        '\n'
    );
}


} } // mw::helpers
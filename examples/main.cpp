#include <cstdlib>
#include <iostream>

#include "include/result.hpp"

namespace examples {
[[nodiscard]] result one() noexcept;
[[nodiscard]] result two() noexcept;
}  // namespace examples

int main() {
    using namespace examples;

    if (one() == result::FAILURE || two() == result::FAILURE) {
        std::cout << "Examples failed to run." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Examples ran successfully." << std::endl;
    return EXIT_SUCCESS;
}
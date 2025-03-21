#include <last-outpost/maps_data.h>
#include <my-lib/matrix.h>
#include <string>

namespace Game
{

    Mylib::Matrix<int> parseMapFromString(const std::string &mapString, int rows, int cols)
    {
        Mylib::Matrix<int> matrix(rows, cols);

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                char tile = mapString[row * cols + col];
                if (tile == ' ')
                {
                    matrix[row, col] = 0;
                }
                else if (tile == 'P')
                {
                    matrix[row, col] = 1;
                }
                else if (tile == 'S')
                {
                    matrix[row, col] = 2;
                }
                else if (tile == 'T')
                {
                    matrix[row, col] = 3;
                }
                else
                {
                    matrix[row, col] = 0;
                }
            }
        }

        return matrix;
    }

    const std::string rawStringMap =
        "                              "
        "        P                     "
        "        P                     "
        "    S   P                     "
        "        P                     "
        "        P                     "
        "        P                     "
        "                              ";

    const Mylib::Matrix<int> MAP1 = parseMapFromString(rawStringMap, 8, 30);
}
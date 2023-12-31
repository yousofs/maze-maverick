#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <random>
#include <string>
#include <iomanip>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

const int START_COLOR = 32;
const int END_COLOR = 34;
const int PATH_COLOR = 33;
int path_length;
void clear_screen()
{
    cout << "\x1B[2J\x1B[H"; // ANSI escape codes to clear the screen
}

vector<vector<int>> create_grid(int x, int y, int a_l, int a_u, int b_l, int b_u, int path_length);

void save_grid(const vector<vector<int>> &grid, const string &filename, int cell_width, int path_length);

void display_grid(const vector<vector<int>> &grid, const vector<vector<bool>> &path);

void handle_commands(vector<vector<int>> &grid, vector<vector<bool>> &path, int &x, int &y, int &path_length);

void HardMood();

void displayMaps()
{
    string pathaddress = "./Maps"; // path to the Maps directory
    int index = 1;

    for (const auto &entry : fs::directory_iterator(pathaddress))
    {
        cout << index << ". " << entry.path().filename() << endl;
        index++;
    }
}
void displayMenu()
{
    cout << "----- Maze Maverick Menu -----" << endl;
    cout << "1. Create a New Map" << endl;
    cout << "2. Playground" << endl;
    cout << "3. Solve a Maze" << endl;
    cout << "4. History" << endl;
    cout << "5. Leaderboard" << endl;
    cout << "6. Exit" << endl;
    return;
}
void Select_Choice(int choice)
{
    if (choice == 1)
    {
        cout << "1.1 Easy" << endl;
        cout << "1.2 Hard" << endl;
        cout << "Enter your choice: ";
    }
    else if (choice == 2)
    {
        cout << "2.1 Choose from Existing Maps" << endl;
        cout << "2.2 Import a Custom Map" << endl;
        cout << "Enter your choice: ";
    }
    else if (choice == 3)
    {
        cout << "3.1 Choose from Existing Maps" << endl;
        cout << "3.2 Import a Custom Map" << endl;
        cout << "Enter your choice: ";
    }
    else if (choice == 4)
    {
        cout << "null4" << endl;
    }
    else if (choice == 5)
    {
        cout << "null5" << endl;
    }
    else if (choice == 6)
    {
        cout << "null6" << endl;
    }
    else
    {
        cout << "Invalid choice" << endl;
        Select_Choice(choice);
    }
}

void do_Choice(double &subchoice)
{
    if (subchoice == 1.2)
    {
        HardMood();
    }
    else if (subchoice == 2.1)
    {
        displayMaps();
        cout << "Enter the name of the map you want to play with: ";
        string mapName;
        cin >> mapName;

        // Load the map using the mapName
        ifstream file("./Maps/" + mapName); // Assuming the maps are in the "./Maps/" directory
        if (file.is_open())
        {
            string line;
            int read_path_length = -1;
            while (getline(file, line))
            {
                if (line.find("PathLength: ") == 0)
                {
                    read_path_length = stoi(line.substr(12));
                    break;
                }
            }

            if (read_path_length == -1)
            {
                cout << "Error: PathLength information not found in the map file.\n";
                return;
            }

            path_length = read_path_length;
            cout << path_length;
            // Read the grid from the file (similar to save_grid function)
            vector<vector<int>> grid;

            while (getline(file, line))
            {

                vector<int> row;
                istringstream iss(line);
                int cell_value;
                while (iss >> cell_value)
                {
                    row.push_back(cell_value);
                }
                grid.push_back(row);
            }

            int x_pos = 0;
            int y_pos = 0;
            vector<vector<bool>> path(grid.size(), vector<bool>(grid[0].size(), false));
            path[0][0] = true; // Mark the start position as part of the path

            display_grid(grid, path);
            handle_commands(grid, path, x_pos, y_pos, path_length);
        }
        else
        {
            cout << "Error: Unable to open the selected map.\n";
        }
    }
    else if (subchoice == 2.2)
    {
        cout << "Enter the path to the grid file: " << endl;
        string gridPath;
        cin >> gridPath;

        // Load the grid using the gridPath
        ifstream file(gridPath);
        if (file.is_open())
        {
            // Read the grid from the file and play
            vector<vector<int>> grid;
            // Read the grid from the file (similar to save_grid function)
            int cell_value;
            while (file >> cell_value)
            {
                grid.push_back({cell_value});
            }
            int x_pos = 0;
            int y_pos = 0;
            vector<vector<bool>> path(grid.size(), vector<bool>(grid[0].size(), false));
            path[0][0] = true; // Mark the start position as part of the path

            display_grid(grid, path);
            handle_commands(grid, path, x_pos, y_pos, path_length);
        }
        else
        {
            cout << "Error: Unable to open the specified grid file.\n";
        }
    }
}

int main()
{
    int rows, columns, pathLength, minCellValue, maxCellValue, minBlocks, maxBlocks;
    displayMenu();
    int choice;
    cout << "Enter your choice: " << endl;
    cin >> choice;
    Select_Choice(choice);
    double subchoice;
    cin >> subchoice;
    do_Choice(subchoice);
    return 0;
}

vector<vector<int>> create_grid(int x, int y, int a_l, int a_u, int b_l, int b_u, int path_length)
{
    vector<vector<int>> grid(x, vector<int>(y, 0));
    vector<vector<bool>> path(x, vector<bool>(y, false));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis_a(a_l, a_u);
    uniform_int_distribution<> dis_b(b_l, b_u);

    int current_x = 0;
    int current_y = 0;
    int path_sum = 0;

    for (int i = 0; i < path_length - 1; i++) // Ensure the last step reaches [x][y]
    {
        int direction = gen() % 4; // 0: up, 1: left, 2: down, 3: right

        switch (direction)
        {
        case 0: // Move up
            if (current_x > 0)
                current_x--;
            break;
        case 1: // Move left
            if (current_y > 0)
                current_y--;
            break;
        case 2: // Move down
            if (current_x < x - 1)
                current_x++;
            break;
        case 3: // Move right
            if (current_y < y - 1)
                current_y++;
            break;
        }
        int num;
        do
        {
            num = dis_a(gen);
        } while (num == 0);

        path_sum += num;

        grid[current_x][current_y] = num;
        path[current_x][current_y] = true;
    }

    // last step must leads to [x][y]
    int remaining_x_steps = x - 1 - current_x;
    int remaining_y_steps = y - 1 - current_y;

    for (int i = 0; i < remaining_x_steps; i++)
    {
        current_x++;
        int num = dis_a(gen);
        path_sum += num;
        grid[current_x][current_y] = num;
        path[current_x][current_y] = true;
    }

    for (int i = 0; i < remaining_y_steps; i++)
    {
        current_y++;
        int num = dis_a(gen);
        path_sum += num;
        grid[current_x][current_y] = num;
        path[current_x][current_y] = true;
    }

    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (!path[i][j])
            {
                grid[i][j] = dis_b(gen);
            }
        }
    }

    return grid;
}

void save_grid(const vector<vector<int>> &grid, const string &filename, int cell_width, int path_length)
{
    ofstream file(filename);
    if (file.is_open())
    {
        file << "PathLength: " << path_length << "\n"; // Save path_length at the beginning of the file

        for (const auto &row : grid)
        {
            for (const auto &val : row)
            {
                file << setw(cell_width) << val;
            }
            file << "\n";
        }
        file.close();
    }
    else
    {
        cout << "Unable to open file\n";
    }
}

void display_grid(const vector<vector<int>> &grid, const vector<vector<bool>> &path)
{
    int largest_num = 0;
    for (const auto &row : grid)
    {
        for (const auto &val : row)
        {
            largest_num = max(largest_num, val);
        }
    }
    int cell_width = to_string(largest_num).length() + 1;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (i == 0 && j == 0)
            {
                cout << "\x1B[" << START_COLOR << "m" << setw(cell_width) << grid[i][j] << "\x1B[0m"; // Print the start position in green
            }
            else if (i == grid.size() - 1 && j == grid[0].size() - 1)
            {
                cout << "\x1B[" << END_COLOR << "m" << setw(cell_width) << grid[i][j] << "\x1B[0m"; // Print the end position in blue
            }
            else if (path[i][j])
            {
                cout << "\x1B[" << PATH_COLOR << "m" << setw(cell_width) << grid[i][j] << "\x1B[0m"; // Print the cells in the path in yellow
            }
            else
            {
                cout << setw(cell_width) << grid[i][j];
            }
        }
        cout << "\n";
    }
}

void handle_commands(vector<vector<int>> &grid, vector<vector<bool>> &path, int &x, int &y, int &path_length)
{
    char command;
    int x_pos, y_pos;
    int moves = 0;

    cout << "Enter command (W:up, A:left, S:down, D:right) : ";
    while (true)
    {
        cin >> command;
        moves++;

        switch (command)
        {
        case 'W': // Move up
            if (x > 0 && grid[x - 1][y] != 0 && !path[x - 1][y])
            {
                x--; // Decrease the row index
            }
            break;
        case 'A': // Move left
            if (y > 0 && grid[x][y - 1] != 0 && !path[x][y - 1])
            {
                y--; // Decrease the column index
            }
            break;
        case 'S': // Move down
            if (x < grid.size() - 1 && grid[x + 1][y] != 0 && !path[x + 1][y])
            {
                x++; // Increase the row index
            }
            break;
        case 'D': // Move right
            if (y < grid[0].size() - 1 && grid[x][y + 1] != 0 && !path[x][y + 1])
            {
                y++;
            }
            break;
        case 'Q':
            return;
        case 'G':
            cout << "You gave up. Your final position is (" << x << ", " << y << ").\n";
            return;
        default:
            cout << "Invalid command. Please try again.\n";
            continue;
        }

        x_pos = x;
        y_pos = y;
        path[x][y] = true;

        if (moves >= path_length)
        {
            if (x == grid.size() - 1 && y == grid[0].size() - 1)
            {
                cout << "YOU WON!\n";
                int choice;
                double subchoice;

                // Ask if the player wants to play again
                char playAgain;
                cout << "Do you want to play again? (Y/N): ";
                cin >> playAgain;

                if (playAgain == 'Y' || playAgain == 'y')
                {
                    // Reset the game state
                    x = 0;
                    y = 0;
                    path = vector<vector<bool>>(grid.size(), vector<bool>(grid[0].size(), false));
                    path[0][0] = true;

                    // Show the menu again
                    do
                    {
                        displayMenu();
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice)
                        {
                        case 1:
                            Select_Choice(choice);
                            cin >> subchoice;
                            do_Choice(subchoice);
                            break;
                        case 2:
                            Select_Choice(choice);
                            cin >> subchoice;
                            do_Choice(subchoice);
                            break;
                        case 3:
                            Select_Choice(choice);
                            break;
                        case 4:
                            cout << "History option selected. (To be implemented)\n";
                            break;
                        case 5:
                            cout << "Leaderboard option selected. (To be implemented)\n";
                            break;
                        case 6:
                            cout << "Exiting the program. Goodbye!\n";
                            return;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                            break;
                        }
                    } while (choice != 6);

                    continue;
                }
                else
                {
                    cout << "GOOD GAME!\n";
                    return;
                }
            }
            if (x == grid.size() - 1 && y == grid[0].size() - 1)
            {
                cout << "YOU LOSE! Reached maximum allowed moves\n";
                int choice;
                double subchoice;

                // Ask if the player wants to play again
                char playAgain;
                cout << "Do you want to play again? (Y/N): ";
                cin >> playAgain;

                if (playAgain == 'Y' || playAgain == 'y')
                {
                    // Reset the game state
                    x = 0;
                    y = 0;
                    path = vector<vector<bool>>(grid.size(), vector<bool>(grid[0].size(), false));
                    path[0][0] = true;

                    // Show the menu again
                    do
                    {
                        displayMenu();
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice)
                        {
                        case 1:
                            Select_Choice(choice);
                            cin >> subchoice;
                            do_Choice(subchoice);
                            break;
                        case 2:
                            Select_Choice(choice);
                            cin >> subchoice;
                            do_Choice(subchoice);
                            break;
                        case 3:
                            Select_Choice(choice);
                            break;
                        case 4:
                            cout << "History option selected. (To be implemented)\n";
                            break;
                        case 5:
                            cout << "Leaderboard option selected. (To be implemented)\n";
                            break;
                        case 6:
                            cout << "Exiting the program. Goodbye!\n";
                            return;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                            break;
                        }
                    } while (choice != 6);

                    continue;
                }
                else
                {
                    cout << "GOOD GAME!\n";
                }
                return;
            }
        }
        display_grid(grid, path);
        clear_screen();
        display_grid(grid, path);
    }
}

void HardMood()
{
    int x, y, a_l, a_u, b_l, b_u, path_length;
    int min_plen, max_plen;
    cout << "Enter the number of rows: ";
    cin >> x;
    cout << "Enter the number of columns: ";
    cin >> y;
    min_plen = x + y - 2;
    max_plen = x * y - 2;
    cout << "Enter the path length: ";
    cin >> path_length;
    while (path_length < min_plen || path_length >= max_plen)
    {
        cout << "Invalid path length. Please enter a value between " << min_plen << " and " << max_plen << ": ";
        cin >> path_length;
    }
    cout << "Enter the lower and upper bounds for the path period [a_l, a_u]: ";
    cin >> a_l >> a_u;
    cout << "Enter the lower and upper bounds for the rest of the grid [b_l, b_u]: ";
    cin >> b_l >> b_u;
    vector<vector<int>>
        grid = create_grid(x, y, a_l, a_u, b_l, b_u, path_length);
    string filename;
    cout << "Say your grid name : ";
    cin >> filename;
    int x_pos = 0;
    int y_pos = 0;
    int largest_num = 0;
    for (const auto &row : grid)
    {
        for (const auto &val : row)
        {
            largest_num = max(largest_num, val);
        }
    }
    int cell_width = to_string(largest_num).length() + 1;

    save_grid(grid, filename, cell_width, path_length);
    cout << "Grid saved to " << filename << "\n";
    vector<vector<bool>> path(x, vector<bool>(y, false));
    path[0][0] = true; // Mark the start position as part of the path

    display_grid(grid, path);
    handle_commands(grid, path, x_pos, y_pos, path_length);
    return;
}

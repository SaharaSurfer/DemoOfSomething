#include "Interface.h"

void Interface::LoadLocation(const std::string& LocationName)
{
	std::ifstream file_in(LocationName);
    if (file_in.is_open())
    {
        std::string row;
        while (std::getline(file_in, row))
        {
            ASCII_ART_HEIGHT++;
            ascii_art.push_back(row);
        }
        file_in.close();
        CreateFrame();
    }
}

void Interface::CreateFrame()
{
    int offset_y = static_cast<int>(ASCII_ART_HEIGHT * 0.1);
    offset_y += offset_y % 2;
    FRAME_HEIGHT = ASCII_ART_HEIGHT + offset_y * 2;

    framed_art.push_back("+" + std::string(FRAME_WIDTH - 2, '-') + "+\n");
    for (int i = 1; i < FRAME_HEIGHT - 1; i++)
    {
        if (i >= offset_y and i < ASCII_ART_HEIGHT + offset_y)
        {
            int ln = static_cast<int>(ascii_art[i - offset_y].length());
            std::string frame_line = "|" + std::string((FRAME_WIDTH - ln) / 2, ' ') + ascii_art[i - offset_y];
            frame_line += std::string(FRAME_WIDTH - frame_line.length() - 1, ' ') + "|\n";
            framed_art.push_back(frame_line);
        }
        else
        {
            framed_art.push_back("|" + std::string(FRAME_WIDTH - 2, ' ') + "|\n");
        }
    }
    framed_art.push_back("+" + std::string(FRAME_WIDTH - 2, '-') + "+\n");
}

void Interface::RenderLocation()
{
    for (const auto& line: framed_art)
    {
        std::cout << line;
    }
}

std::string Interface::LoadText(const std::string& LocationName)
{
    std::ifstream file_in(LocationName);
    if (file_in.is_open())
    {
        std::string str(std::istreambuf_iterator<char>{file_in}, {});
        file_in.close();
        return str;
    }
    else { return ""; }
}

void Interface::RenderText(const std::string& Text)
{
    int pause = 0, pointer = 0, splitter = std::string::npos;
    for (int i = 0; i < Text.size(); i++)
    {
        //pause = Text[i] == '.' or Text[i] == '!' or Text[i] == '?'
        //   ? 150 : (Text[i] == ' ' ? 30 : 15);
        std::this_thread::sleep_for(std::chrono::milliseconds(pause));

        pointer++;
        if (pointer % 80 == 0)
        {
            splitter = Text.find(' ', i);
        }

        if (splitter != std::string::npos and i == splitter)
        {
            std::cout << "\n";
            pointer = 0;
        }
        else
        {
        std::cout << Text[i];
        }
        
        if (Text[i] == '\n' and Text[i + 1] == '\n')
        {
            splitter = std::string::npos;
            pointer = 0;
            system("pause");
        }
        else if (Text[i] == '\n')
        {
            splitter = std::string::npos;
            pointer = 0;
        }
    }
}

json Interface::LoadJSON(const std::string& LocationName)
{
    std::string str = LoadText(LocationName);
    if (!str.empty())
    {
        json table{ json::parse(str) };
        return table;
    }
    return json();
}

size_t Interface::CollectPlayerChoice(size_t upper_border)
{
    std::string player_choice;
    std::getline(std::cin, player_choice);

    int choice = 0;

    try {
        choice = std::stoi(player_choice);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid input. Please enter a number.\n";
        return CollectPlayerChoice(upper_border);
    }
    catch (const std::out_of_range& e) {
        std::cout << "Input is out of range. Please enter a valid number.\n";
        return CollectPlayerChoice(upper_border);
    }

    if (choice > 0 and choice <= upper_border) 
    {
        return static_cast<size_t>(choice - 1);
    }
    else
    {
        std::cout << "Please choose a number greater than 0 and less than " << upper_border << ".\n";
        return CollectPlayerChoice(upper_border);
    }
}

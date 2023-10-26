#include "Interface.h"

void Interface::LoadLocation(const std::string& LocationName)
{
	std::ifstream file_in(LocationName.c_str());
    std::string row;
    while(std::getline(file_in, row))
    {
        ASCII_ART_HEIGHT++;
        ascii_art.push_back(row);
    }
    file_in.close();
    
    CreateFrame();
}

void Interface::CreateFrame()
{
    int offset_y = (int)(ASCII_ART_HEIGHT * 0.1);
    offset_y += offset_y % 2;
    FRAME_HEIGHT = ASCII_ART_HEIGHT + offset_y * 2;
    
    framed_art.push_back("+");
    framed_art[0].append(FRAME_WIDTH - 2, '-');
    framed_art[0] += "+\n";

    for (int i = 1; i < FRAME_HEIGHT - 1; i++)
    {
        framed_art.push_back("|");
        if (i >= offset_y and i < ASCII_ART_HEIGHT + offset_y)
        {
            int ln = (int)ascii_art[i - offset_y].length();

            framed_art[i].insert(1, (FRAME_WIDTH - ln) / 2, ' ');
            framed_art[i] += ascii_art[i - offset_y];
            framed_art[i].append(FRAME_WIDTH - (int)framed_art[i].length() - 1, ' ');
        }
        else
        {
            framed_art[i].append(FRAME_WIDTH - 2, ' ');
        }
        framed_art[i] += "|\n";
    }
    framed_art.push_back("+");
    framed_art[FRAME_HEIGHT - 1].append(FRAME_WIDTH - 2, '-');
    framed_art[FRAME_HEIGHT - 1] += "+\n";
}

void Interface::RenderLocation()
{
    for (int i = 0; i < FRAME_HEIGHT; i++)
    {
        std::cout << framed_art[i];
    }
    // Будет расширение
}

std::string Interface::LoadText(const std::string& LocationName)
{
    std::ifstream file_in(LocationName.c_str());
    std::string str(std::istreambuf_iterator<char>{file_in}, {});
    return str;
}

void Interface::RenderText(const std::string& Text)
{
    int pause = 0;
    for (int i = 0; i < Text.size(); i++)
    {
        //pause = Text[i] == '.' or Text[i] == '!' or Text[i] == '?'
        //    ? 150 : (Text[i] == ' ' ? 70 : 20);
        std::cout << Text[i];
        if (Text[i] == '\n' and Text[i + 1] == '\n')
        {
            system("pause");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    }
}

json Interface::LoadJSON(const std::string& LocationName)
{
    std::string str = LoadText(LocationName);
    json table{ json::parse(str) };

    return table;
}
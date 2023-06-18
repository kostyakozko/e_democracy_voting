#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>
namespace fs = std::filesystem;
std::map<uint32_t, uint32_t> parse_voting(fs::path file)
{
    std::fstream voting_file(file, std::fstream::in);
    std::string line;
    std::string token;
    const char rows_delim = ' ';
    std::map<uint32_t, uint32_t> votes;
    while(std::getline(voting_file, token, rows_delim))
    {
        if(token.find("SEL=") != std::string::npos)
        {
            std::string vote_str=token.substr(4);
            std::stringstream vote_stream(vote_str);
            std::string vote_entry;
            while(std::getline(vote_stream, vote_entry, ','))
            {
                ++votes[std::stoi(vote_entry)];
            }
        }
    }
    return votes;
}

std::map <uint32_t, std::string> parse_names(fs::path)
{
    //TODO:implement this
    return {};
}

int generate_csv()
{
    //TODO:implement this
    return 0;
}
int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
       std::cout << "Usage: ./voting <voting_report> [<list of names>]" << std::endl;
       return -1;
    }

    std::map<uint32_t, uint32_t> votes = parse_voting(argv[1]);
    if (votes.empty())
    {
       std::cerr << "Wrong file with voting report" << std::endl;
       return -2;
    }

    std::map <uint32_t, std::string> names_map;
    if (argc == 3)
    {
        names_map = parse_names(argv[2]);
    }

    std::multimap<uint32_t, uint32_t, std::greater<uint32_t>> sorted_votes;
    for (auto& it : votes)
    {
        sorted_votes.insert({it.second, it.first});
    }
    for (auto& it : sorted_votes)
    {
        std::cout << it.first << " " << it.second << std::endl;
    }

    return 0;
}

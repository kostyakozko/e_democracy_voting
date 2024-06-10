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
    std::map<uint32_t, uint32_t> votes;

    while (std::getline(voting_file, line))
    {
        std::string::size_type pos = line.find("V=");
        if (pos != std::string::npos)
        {
            std::string vote_str = line.substr(pos + 2);  // +2 to skip "V="
            std::stringstream vote_stream(vote_str);
            std::string vote_entry;
            while (std::getline(vote_stream, vote_entry, ','))
            {
                ++votes[std::stoi(vote_entry)];
            }
        }
    }
    return votes;
}

std::map<uint32_t, std::string> parse_names(fs::path file)
{
    std::fstream names_file(file, std::fstream::in);
    std::string line;
    std::map<uint32_t, std::string> names;
    uint32_t count = 0;
    while (std::getline(names_file, line))
    {
        ++count;
        names[count] = line;
    }
    return names;
}

int generate_csv(const std::multimap<uint32_t, uint32_t, std::greater<uint32_t>> votes, const std::map<uint32_t, std::string> names)
{
    std::cout << "Name,Votes count" << std::endl;
    if (names.empty() || names.size() != votes.size())
    {
        for (const auto& it : votes)
        {
            std::cout << it.second << "," << it.first << std::endl;
        }
    }
    else
    {
        for (const auto& it : votes)
        {
            std::cout << names.at(it.second) << "," << it.first << std::endl;
        }
    }

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

    std::map<uint32_t, std::string> names_map;
    if (argc == 3)
    {
        names_map = parse_names(argv[2]);
        if (names_map.empty() || names_map.size() != votes.size())
        {
            std::cout << "Wrong file with names" << std::endl;
        }
    }

    std::multimap<uint32_t, uint32_t, std::greater<uint32_t>> sorted_votes;
    for (const auto& it : votes)
    {
        sorted_votes.insert({it.second, it.first});
    }

    generate_csv(sorted_votes, names_map);

    return 0;
}


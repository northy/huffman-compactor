#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <utility>
#include <map>
#include <queue>
#include <limits>
#include <compare>
#include <bitset>

#include <fire-hpp/fire.hpp>

struct Tree {
    std::optional<char> byte;
    uint64_t freq;
    std::unique_ptr<Tree> left, right;

    std::strong_ordering operator<=>(const Tree& o) const {
        return (freq<=>o.freq);
    }

    Tree(char b, uint64_t f): byte(b), freq(f) {}
    Tree(Tree* l, Tree* r): left(l), right(r), freq(l->freq+r->freq) {}
};

struct Cmp {
    bool operator()(const Tree* a, const Tree* b) const {
        return *a > *b;
    }
};

int fired_main(std::string file_path = fire::arg({"--file-path","-f"}), fire::optional<int> print = fire::arg({"--print","-p"})) {
    //open file
    std::ifstream file(file_path, std::fstream::binary);

    if (!file.good()) {
        std::cerr << "Couldn't open file!" << std::endl;
        return 1;
    }
    file >> std::noskipws;

    uint64_t total_bytes=0, total_bits_huffman=0;

    //read file + save byte frequency
    std::unordered_map<char,uint64_t> freq;

    std::cout << "ASCII: ";

    char buf;
    while (file >> buf) {
        std::cout << std::bitset<8>(buf);
        freq[buf]++;
        total_bytes++;
    }
    std::cout << " (" << total_bytes*8 << ')' << std::endl;

    //create tree leaves
    std::priority_queue<Tree*, std::vector<Tree*>, Cmp> leaves;

    for (const auto& f : freq) {
        leaves.push(new Tree(f.first, f.second));
    }

    //create tree root
    while (leaves.size()!=1) {
        Tree* a = leaves.top(); leaves.pop();
        Tree* b = leaves.top(); leaves.pop();
        leaves.push(new Tree(a, b));
    }

    //traverse the tree and create paths
    std::map<char,std::string> path;
    std::queue<std::pair<Tree*,std::string>> traverse;
    traverse.push(std::make_pair(leaves.top(),""));
    leaves.pop();

    while (traverse.size()>0) {
        auto [t,p] = traverse.front(); traverse.pop();

        if (t->left.get()==NULL) {
            path[*t->byte]=p;
            total_bits_huffman+=p.size()*freq[*t->byte];
            if (print.value_or(1)) std::cout << *t->byte << ": " << path[*t->byte] << '\n';
            continue;
        }

        traverse.push(std::make_pair(t->left.get(),p+'0'));
        traverse.push(std::make_pair(t->right.get(),p+'1'));
    }

    //print the paths
    std::cout << "Huffman: ";

    file.clear(); file.seekg(0);
    while (file >> buf) {
        std::cout << path[buf];
    }
    std::cout << " (" << total_bits_huffman << ')' << std::endl;

    file.close();
    return 0;
}

FIRE(fired_main, "Calculate file entropy")

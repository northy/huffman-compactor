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
#include <cmath>

#include <fire-hpp/fire.hpp>

uint64_t INF = ~(uint64_t(0));

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

uint64_t write_tree(std::vector<char>& position, Tree* node, uint64_t index) {
    if (node->left.get()==NULL) {
        position[index]=*node->byte;
        if (*node->byte==0) return index;
        return INF;
    }

    return std::min(write_tree(position, node->left.get(), index*2+1), write_tree(position, node->right.get(), index*2+2));
}

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

    if (print.value_or(0)) std::cout << "ASCII: ";

    char buf;
    while (file >> buf) {
        if (print.value_or(0)) std::cout << std::bitset<8>(buf);
        freq[buf]++;
        total_bytes++;
    }
    if (print.value_or(0)) std::cout << " (" << total_bytes*8 << ')' << std::endl;

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
    Tree* root = leaves.top(); leaves.pop();
    traverse.push(std::make_pair(root,""));

    int deepest=0;

    while (traverse.size()>0) {
        auto [t,p] = traverse.front(); traverse.pop();

        if (t->left.get()==NULL) {
            if (p.size()>deepest) deepest=p.size();

            path[*t->byte]=p;
            total_bits_huffman+=p.size()*freq[*t->byte];
            if (print.value_or(0)) std::cout << std::bitset<8>(*t->byte) << ": " << path[*t->byte] << '\n';
            continue;
        }

        traverse.push(std::make_pair(t->left.get(),p+'0'));
        traverse.push(std::make_pair(t->right.get(),p+'1'));
    }

    //create the buffer with 3 extra bits (trailing bits in the last byte)
    std::ofstream out_file("out.huff", std::fstream::binary);
    std::bitset<8> binary_buf((8-((total_bits_huffman+3))%8)<<5);
    char buf_pos=3;

    //print the paths + output to file
    if (print.value_or(0)) std::cout << "Huffman: ";

    file.clear(); file.seekg(0);
    while (file >> buf) {
        if (print.value_or(0)) std::cout << path[buf];
        for (const char c : path[buf]) {
            binary_buf[7-(buf_pos++)]=c-'0';
            if (buf_pos==8) {
                buf_pos=0;
                out_file << (unsigned char)binary_buf.to_ulong();
            }
        }
    }
    if (buf_pos!=0)
        out_file << (unsigned char)binary_buf.to_ulong();
    if (print.value_or(0)) std::cout << " (" << total_bits_huffman << ')' << std::endl;

    file.close();
    out_file.close();

    //write tree
    std::ofstream out_tree("out.tree", std::fstream::binary);
    uint64_t tree_size=pow(2,deepest+1)-1;
    std::vector<char> positions(tree_size,0);
    uint64_t zeroindex = write_tree(positions, root, 0);
    out_tree << zeroindex;
    //std::cout << std::bitset<64>(zeroindex) << '\n';
    for (char c : positions)
        out_tree << c;
    out_file.close();

    return 0;
}

FIRE(fired_main, "Calculate file entropy")

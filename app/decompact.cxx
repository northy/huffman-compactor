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
#include <set>
#include <vector>

#include <fire-hpp/fire.hpp>

struct Tree {
    char byte;
    std::unique_ptr<Tree> left, right;

    Tree(char b): byte(b) {}
};

int fired_main(std::string file_path = fire::arg({"--file-path","-f"}), std::string tree_path = fire::arg({"--tree","-t"}), fire::optional<int> print = fire::arg({"--print","-p"})) {
    //open file
    std::ifstream file(file_path, std::fstream::binary);

    if (!file.good()) {
        std::cerr << "Couldn't open file!" << std::endl;
        return 1;
    }
    file >> std::noskipws;

    //read tree
    std::ifstream tree_file(tree_path, std::fstream::binary);
    if (!file.good()) {
        std::cerr << "Couldn't open tree!" << std::endl;
        return 1;
    }
    tree_file >> std::noskipws;

    uint64_t zeroindex;
    tree_file >> zeroindex;

    std::vector<char> tree_positions;

    char buf;
    while (tree_file >> buf) {
        tree_positions.push_back(buf);
    }

    using q_item = std::tuple<Tree*,char,uint64_t>;
    std::queue<q_item> q;

    Tree* root = new Tree(tree_positions[0]);
    q.push(q_item(root,0,1));
    q.push(q_item(root,1,2));

    while (!q.empty()) {
        auto [p,side,index] = q.front(); q.pop();

        Tree* self = new Tree(tree_positions[index]);
        if (side) p->right.reset(self);
        else p->left.reset(self);
        
        if (tree_positions[index]==0 && index!=zeroindex && index*2+2<tree_positions.size()) {
            q.push(q_item(self,0,index*2+1));
            q.push(q_item(self,1,index*2+2));
        }
    }
    tree_file.close();

    //read file
    if (print.value_or(0)) std::cout << "Huffman message: ";

    unsigned char buf_pos=3;
    file >> buf;
    std::bitset<8> binary_buf(buf);
    file >> buf;

    const unsigned char trailing = binary_buf[7]*4+binary_buf[6]*2+binary_buf[5];
    Tree* traversing = root;
    std::string original;

    do {
        while (buf_pos!=8) {
            if (print.value_or(0)) std::cout << binary_buf[7-buf_pos];
            if (binary_buf[7-buf_pos])
                traversing = traversing->right.get();
            else
                traversing = traversing->left.get();
            
            if (traversing->left.get()==NULL) {
                original+=traversing->byte;
                traversing = root;
            }
            buf_pos++;
        }
        buf_pos=0;
        binary_buf = std::bitset<8>(buf);
    } while (file >> buf);

    while (buf_pos!=8-trailing) {
        if (print.value_or(0)) std::cout << binary_buf[7-buf_pos];
        if (binary_buf[7-buf_pos])
            traversing = traversing->right.get();
        else
            traversing = traversing->left.get();
        
        if (traversing->left.get()==NULL) {
            original+=traversing->byte;
            traversing = root;
        }
        buf_pos++;
    }
    if (print.value_or(0)) std::cout << std::endl;

    file.close();

    if (print.value_or(0)) std::cout << "Original message: " << original << std::flush;

    //write tree
    std::ofstream out_plain("out.plain", std::fstream::binary);
    out_plain << original;
    out_plain.close();

    return 0;
}

FIRE(fired_main, "Calculate file entropy")

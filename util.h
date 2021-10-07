//
// STARTER CODE: util.h
//
// Ahmed Benchehida, Mohammed Hashmehh. This program take in a file and encodeds
// it using a the Huffman Alogrithm where each letter is encoded using a
// distintt combionation of ones and zeros, the program is able to decode aswell
// this program can compress and decompress fies into a .huf file
//

#pragma once
#include "hashmap.h"
typedef hashmap hashmapF;
typedef unordered_map <int, string> hashmapE;
#include "priorityqueue.h"


struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;
    HuffmanNode* one;
};



//
// This method frees the memory allocated for the Huffman tree.
// this function takes in HuffmanNode and traverses through the tree
// going left and right and then deleted the node.
void freeTree(HuffmanNode* node) {
    if (node == nullptr) {
        return;
    }
    freeTree(node->zero);
    freeTree(node->one);
    delete node;
}

// This function build the frequency map.  If isFile is true, then it reads
// from filename.  If isFile is false, then it reads from a string filename.

// This function takes in the file and hashmap and the string filename.
// function builds map and grabs the letter and its asscii value as well as
// its frequency in the text file.
void buildFrequencyMap(string filename, bool isFile, hashmapF &map) {
    // TO DO:  Write this function.
    int counter = 1;
    int val;
    char c;
    if (isFile) {
        ifstream is(filename);
        while (is.get(c)) {
            if (map.containsKey(c)) {
                val = map.get(c);
                val++;
                map.put(c, val);
            } else {
                map.put(c, counter);
            }
        }
        map.put(256, 1);
    } else {
        for (int x = 0; x < filename.size(); x++) {
            if (map.containsKey(filename[x])) {
                val = map.get(filename[x]);
                val++;
                map.put(filename[x], val);
            } else {
                map.put(filename[x], counter);
            }
        }
        map.put(256, 1);
    }
}

//
// This function builds an encoding tree from the frequency map.
// functions takes in the hashmapF Map. BuildEncodingTree builds a tree
// where characters with least frequency are inserted in that order to buil
// a binary tree. Function returns root.
HuffmanNode* buildEncodingTree(hashmapF &map) {
    priorityqueue<HuffmanNode*> pq;
    int priority;
    for (int key : map.keys()) {
        HuffmanNode* new_node = new HuffmanNode();
        new_node->zero = nullptr;
        new_node->one = nullptr;
        new_node->count = map.get(key);
        new_node->character = key;
        priority = map.get(key);
        pq.enqueue(new_node, priority);
    }
    HuffmanNode* first;
    HuffmanNode* second;
    HuffmanNode* root;
    priorityqueue<HuffmanNode*> pq2;

    while (pq.Size() != 1) {
      HuffmanNode* new_node = new HuffmanNode();
      first = pq.dequeue();
      second = pq.dequeue();
      new_node->zero = first;
      new_node->one = second;
      new_node->count = first->count + second->count;
      new_node->character = 257;
      pq.enqueue(new_node, new_node->count);
      root = new_node;
    }
    return root;  // TO DO: update this return
}

// Recursive helper function for building the encoding map.
/* build encoding map takes in node, and the encodingMap and the string str
and HuffmanNode prev, function traverses through the tree and lables the path 
with 1 and 0 till it gets to the node. */
void _buildEncodingMap(HuffmanNode* node, hashmapE &encodingMap,
    string str, HuffmanNode* prev) {
    // TO DO:  Write this function.
    if (node == nullptr) {
        return;
    }
    if (node->zero != nullptr) {
         str +="0";
     }
    _buildEncodingMap(node->zero, encodingMap, str, node);

    if (node->character != 257) {
        encodingMap[node->character] = str;
    }
    if (node->zero != nullptr) {
        str.erase(str.size()-1);
    }
    if (node->one != nullptr) {
        str+="1";
    }
    _buildEncodingMap(node->one, encodingMap, str, node);
}

// This function builds the encoding map from an encoding tree.
// function takes in HuffmanNode tree, calls recurives helper function and
// returns the encoded map.
hashmapE buildEncodingMap(HuffmanNode* tree) {
    hashmapE encodingMap;
    string str = "";
    _buildEncodingMap(tree, encodingMap, str, tree);
    return encodingMap;
}

//
// This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.

// function takes in ifstream& input, hashmapE encodingMap, ofbitstream output,
// int size, bool makeFile. Function encodes the text file
string encode(ifstream& input, hashmapE &encodingMap, ofbitstream& output,
int &size, bool makeFile) {
    string encoded ="";
    char c;
    int num;
    // Takes each character from the input
    while (input.get(c)) {
        //adds the binary value to the encoded string
        encoded+= encodingMap[c];
    }
    // adds the EOF to at the end
    encoded+=encodingMap[256];
    size = encoded.size();
    // Traverses the string to writeBit every binary number
    for (int x = 0; x < encoded.size(); x++){
        // subtracts by 48, the ASCII value of zero 48-48
        // if the number is 1 then it will subtract 49-48
        output.writeBit(encoded[x] - 48);
    }
    // returns the encoded string
    return encoded;
}


//
// This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
string decode(ifbitstream &input, HuffmanNode* encodingTree, ofstream &output) {
    // TO DO:  Write this function.
    // initlize decoded string.
    string decode = "";
    int x;
    HuffmanNode* temp = encodingTree;
    while (true) {
        // read the huffman bit that we wrote in encoded.
        x = input.readBit();
        // check if bit it one.
        if (x == 1) {
            // traverse right.
            temp = temp->one;
        }
        // check if bit is zero
        if (x == 0) {
            // traverse left
            temp = temp->zero;
        }
        // check if reached end of file.
        if (temp->character != NOT_A_CHAR) {
            if (temp->character == PSEUDO_EOF) {
                break;
            }
            // concatenate letters together. 
           decode+=temp->character;
           temp = encodingTree;
        }
    }
    output<< decode;
    return decode;
}

// This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
string compress(string filename) {
    hashmapF frequency_map;
    int size = 0;
    string str ="";
    hashmapE encoded_map;
    HuffmanNode* root;

    // build frequency map.
    buildFrequencyMap(filename, true, frequency_map);
    // build encoding Tree
    root = buildEncodingTree(frequency_map);
    encoded_map = buildEncodingMap(root);

    // encode the file
    ofbitstream output(filename + ".huf");
    ifstream input(filename);
    stringstream ss;
    ss << frequency_map;
    output << frequency_map;
    str = encode(input, encoded_map, output, size, true);

    // destruct the tree nodes.
    freeTree(root);
    return str;
}


// This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note this function should reverse what the compress
// function did.

string decompress(string filename) {
    string decoded;

    // extract header from map.
    size_t pos = filename.find(".huf");
    if ((int)pos >= 0) {
        filename = filename.substr(0, pos);
    }

    pos = filename.find(".");
    string ext = filename.substr(pos, filename.length() - pos);
    filename = filename.substr(0, pos);
    ifbitstream input(filename + ext + ".huf");
    ofstream output(filename + "_unc" + ext);

    // build encoding tree
    hashmapF dump;
    input >> dump;
    HuffmanNode* root;
    root = buildEncodingTree(dump);
    // decode the file
    decoded = decode(input, root, output);

    // destruct the tree.
    freeTree(root);
    return decoded;
}

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <chrono>
#include <queue>

typedef unsigned long long int ll;
typedef unsigned char byte;

struct Node
{
    byte character;
    ll count;
    Node* left, * right;

    Node(ll count)
    {
        this->character = 0;
        this->count = count;
        this->left = this->right = nullptr;
    }

    Node(byte character, ll count)
    {
        this->character = character;
        this->count = count;
        this->left = this->right = nullptr;
    }
};

bool sortbysec(const Node* a, const Node* b)
{
    return (a->count > b->count);
}

class Huffman
{
private:
    std::vector<std::string> HuffmanValue;

    ll frequencyTable[256] = { 0 };
    Node* root = nullptr;
public:
    Huffman()
    {
        for (int i = 0; i < 256; i++)
        {
            HuffmanValue.push_back("");
        }
    }

    ~Huffman()
    {
        free_tree(root);
    }

    void free_tree(Node* node)
    {
        if (node)
        {
            free_tree(node->left);
            free_tree(node->right);
            delete node;
        }
    }

    ll get_file_size(const char* filename)
    {
        FILE* p_file = fopen((std::string(std::string("../data/") + filename)).c_str(), "rb");
        fseek(p_file, 0, SEEK_END);
        ll size = ftell(p_file);
        fclose(p_file);
        return size;
    }


    Node* combine(Node* a, Node* b)
    {
        Node* parent = new Node((a ? a->count : 0) + (b ? b->count : 0));
        parent->left = b;
        parent->right = a;
        return parent;
    }

    void parse_file(const char* filename, const ll Filesize)
    {
        FILE* ptr = fopen((std::string(std::string("../data/") + filename)).c_str(), "rb");
        if (ptr == NULL) {
            perror("Error: File not found:");
            exit(-1);
        }
        byte ch;
        ll size = 0, filesize = Filesize;

        while (size != filesize)
        {
            ch = fgetc(ptr);
            ++frequencyTable[ch];
            ++size;
        }

        fclose(ptr);
    }

    std::vector<byte> generate_header()
    {
        auto header = std::vector<byte>();
        header.push_back(0);
        byte UniqueCharacter = 255;

        for (int i = 0; i < 256; ++i) {
            if (HuffmanValue[i].size()) {
                header.push_back(i);
                header.push_back(HuffmanValue[i].length());
                auto bytes = std::vector<byte>();
                byte ch = 0, counter = 0;
                for (int j = HuffmanValue[i].length() - 1; j > -1; j--)
                {
                    if (counter == 8)
                    {
                        bytes.push_back(ch);
                        ch = 0;
                        counter = 0;
                    }

                    ch = ch | ((HuffmanValue[i][j] - '0') << counter);
                    counter++;
                }
                if (counter != 0)
                    bytes.push_back(ch);
                for (int j = bytes.size() - 1; j > -1; j--)
                    header.push_back(bytes[j]);
                ++UniqueCharacter;
            }
        }
        header[0] = UniqueCharacter;
        return header;
    }


    void store_huffman_value()
    {
        store_huffman_value(root, "");
    }

    void store_huffman_value(Node* node, const std::string& value)
    {
        if (!node)
            return;
        if (!node->left && !node->right)
        {
            HuffmanValue[node->character] = value.length() != 0 ? value : "0";
            return;
        }

        std::string left = value + '0';
        store_huffman_value(node->left, left);
        std::string right = value + '1';
        store_huffman_value(node->right, right);
    }


    void generate_huffman_tree()
    {
        std::priority_queue<Node*, std::vector<Node*>, bool (*)(const Node*, const Node*)> queue(sortbysec);
        for (int i = 0; i < 256; i++)
        {
            if (frequencyTable[i])
            {
                queue.push(new Node(i, frequencyTable[i]));
            }
        }

        while (queue.size() != 1)
        {
            Node* right = queue.top();
            queue.pop();

            Node* left = queue.top();
            queue.pop();

            Node* parent = combine(left, right);
            queue.push(parent);
        }

        root = queue.top();
    }

    void compress(const char* filename, const ll Filesize)
    {
        auto header = generate_header();
        int header_i = 0;
        const int h_length = header.size();
        FILE* iptr = fopen((std::string(std::string("../data/") + filename)).c_str(), "rb"),
            * optr = fopen((std::string(std::string("../data/c_") + filename)).c_str(), "wb");

        if (!iptr) {
            perror("Error: File not found: ");
            exit(-1);
        }

        fputc(0, optr);

        while (header_i < h_length) {
            fputc(header[header_i], optr);
            ++header_i;
        }

        unsigned char ch, fch = 0;
        char counter = 8;
        ll size = 0, i;
        while (size != Filesize)
        {
            ch = fgetc(iptr);
            i = 0;
            const std::string& huffmanStr = HuffmanValue[ch];
            while (huffmanStr[i] != '\0')
            {
                fch = fch | ((huffmanStr[i] - '0') << (counter - 1));
                counter--;
                if (counter == 0)
                {
                    fputc(fch, optr);
                    fch = 0;
                    counter = 8;
                }
                ++i;
            }
            ++size;
        }
        if (counter != 8)
        {
            fputc(fch, optr);
        }
        fseek(optr, 0, SEEK_SET);
        fputc(counter % 8, optr);

        fclose(iptr);
        fclose(optr);
    }

    void generate_huffman_tree(Node* node, const std::string& codes, const byte ch)
    {
        Node* traverse = node;
        int i = 0;
        while (codes[i] != '\0')
        {
            if (codes[i] == '0')
            {
                if (!traverse->left)
                {
                    traverse->left = new Node(0);
                }
                traverse = traverse->left;
            }
            else
            {
                if (!traverse->right)
                {
                    traverse->right = new Node(0);
                }
                traverse = traverse->right;
            }
            ++i;
        }
        traverse->character = ch;
    }

    std::pair<byte, ll> decode_header(FILE* iptr)
    {
        ll header_size = 0;
        root = new Node(0);
        byte ch, len;
        byte padding = fgetc(iptr);
        ++header_size;
        int charactercount = fgetc(iptr);
        ++header_size;
        std::string codes;
        ++charactercount;
        while (charactercount)
        {
            ch = fgetc(iptr);
            ++header_size;
            codes = "";
            len = fgetc(iptr);
            ++header_size;

            byte bytes_len = len % 8 ? len / 8 + 1 : len / 8;
            byte byte_padding = (8 - len % 8) % 8;
            auto bytes = std::vector<byte>();
            for (int i = 0; i < bytes_len; i++)
            {
                bytes.push_back(fgetc(iptr));
                ++header_size;
            }

            for (byte i = 0; i < bytes_len; i++)
            {
                for (byte j = 0; j < 8; j++)
                {
                    if (i == 0 && j < byte_padding)
                        continue;
                    byte value = bytes[i] & (1 << (7 - j));
                    if (value)
                    {
                        codes += '1';
                    }
                    else
                    {
                        codes += '0';
                    }
                }
            }

            if (codes.length() == 0) codes = "0";

            generate_huffman_tree(root, codes, ch);
            --charactercount;
        }

        return { padding, header_size };
    }

    void print_tree()
    {
        for (int i = 0; i < 256; i++)
        {
            if (HuffmanValue[i].length() != 0)
            {
                std::cout << i << "(" << (char) i <<  ") " << HuffmanValue[i] << "\n";
            }
        }
    }

    void decompress(const char* filename, const ll Filesize)
    {
        const std::string fl = filename;
        FILE* iptr = fopen(std::string("../data/" + fl).c_str(), "rb");
        FILE* optr = fopen(std::string("../data/d_" + fl).c_str(), "wb");

        if (iptr == NULL)
        {
            perror("Error: File not found");
            exit(-1);
        }

        auto [padding, headersize] = decode_header(iptr);
        store_huffman_value();
        print_tree();

        byte ch;
        char counter = 7;
        ll size = 0;
        const ll filesize = Filesize - headersize;
        Node* traverse = root;
        ch = fgetc(iptr);
        while (size != filesize)
        {
            while (counter >= 0)
            {
                traverse = ch & (1 << counter) ? traverse->right : traverse->left;
                --counter;
                if (!traverse->left && !traverse->right) {
                    fputc(traverse->character, optr);
                    if (size == filesize - 1 && padding == counter + 1) {
                        break;
                    }
                    traverse = root;
                }
            }
            ++size;
            counter = 7;
            ch = fgetc(iptr);
        }
        fclose(iptr);
        fclose(optr);
    }
};


int main()
{
    std::string option = "c";
    std::string filename = "text.txt";
    std::string path = "../data/";

    ll filesize, comp_filesize;
    if (option == "c") {
        Huffman huf = Huffman();
        const char* filename_c = filename.c_str();
        filesize = huf.get_file_size(filename_c);
        huf.parse_file(filename_c, filesize);
        huf.generate_huffman_tree();
        huf.store_huffman_value();
        huf.print_tree();

        huf.compress(filename_c, filesize);

        comp_filesize = huf.get_file_size(std::string(std::string("../data/c_") + filename).c_str());
        double coeff = 0.0;

        if (comp_filesize > filesize)
            coeff = - (double) (comp_filesize - filesize) / filesize * 100;
        else
            coeff = (double) (filesize - comp_filesize) / filesize * 100;

        std::cout << "Compress coefficient: " << coeff << " % \n";
    }
    else if (option == "d")
    {
        filename = "c_" + filename;
        const char* filename_c = filename.c_str();
        Huffman huf = Huffman();
        filesize = huf.get_file_size(filename_c);
        huf.decompress(filename_c, filesize);
    }

    return 0;
}
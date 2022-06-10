#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

// int_to_binary_string(5, 8) = "00000101"
string int_to_binary_string(int number, int total_length)
{
    string binary_string = "";

    while (number > 0)
    {
        bool digit = number % 2;
        binary_string += digit + '0';
        number /= 2;
    }

    // apply padding
    while (binary_string.size() < total_length)
    {
        binary_string += '0';
    }

    reverse(binary_string.begin(), binary_string.end());

    return binary_string;
}

// binary_string_to_int("101") = 5
int binary_string_to_int(string binary_string)
{
    int result = 0;
    for (int i = 0; i < binary_string.size(); i++)
    {
        result *= 2;
        result += binary_string[i] - '0';
    }
    return result;
}

// get unique characters in alphabetical order
string generate_dictionary(string text)
{
    bool tab[128] = {};
    for (int i = 0; i < text.size(); i++)
    {
        tab[text[i]] = true;
    }

    string unique = "";
    for (int i = 0; i < 128; i++)
    {
        if (tab[i]) unique += i;
    }
    return unique;
}

string compress_text(string text)
{
    string dict = generate_dictionary(text);

    int key_bit_size = max(1, (int) ceil(log2(dict.size())));

    // // print dictionary
    // for (int i = 0; i < dict.size(); i++)
    // {
    //     cout << dict[i] << " --> " << int_to_binary_string(i, key_bit_size) << endl;
    // }

    int compressed_bits_size = key_bit_size * text.size();
    int padding = (8 - ((compressed_bits_size + 3) % 8)) % 8;

    string final_binary_text = int_to_binary_string(padding, 3);

    // for each char from original text
    for (int i = 0; i < text.size(); i++)
    {
        // find value in dictionary
        for (int j = 0; j < dict.size(); j++)
        {
            if (text[i] == dict[j])
            {
                final_binary_text += int_to_binary_string(j, key_bit_size);
                break;
            }
        }
    }

    // apply padding
    for (int i = 0; i < padding; i++)
    {
        final_binary_text += '1';
    }

    string compressed = "";

    compressed += (char) dict.size();
    compressed += dict;

    for (int i = 0; i < final_binary_text.size() / 8; i++)
    {
        string byte = "";
        for (int j = 0; j < 8; j++)
        {
            byte += final_binary_text[i * 8 + j];
        }
        compressed += (char) binary_string_to_int(byte);
    }

    return compressed;
}

string decompress_text(string dict, string text_padded)
{
    unsigned char first_byte = text_padded[0];
    unsigned int padding = first_byte >> 5;

    string with_padding = "";
    for (int i = 0; i < text_padded.size(); i++)
    {
        with_padding += int_to_binary_string((unsigned char) text_padded[i], 8);
    }

    string binary_string = "";
    // skip 3 first characters (index 0, 1, 2) and padding at the end
    for (int i = 3; i < with_padding.size() - padding; i++)
    {
        binary_string += with_padding[i];
    }

    int key_bit_size = max(1, (int) ceil(log2(dict.size())));

    string decompressed_data = "";
    for (int i = 0; i < binary_string.size() / key_bit_size; i++)
    {
        string binary_cpr;
        for (int j = 0; j < key_bit_size; j++)
        {
            binary_cpr += binary_string[i * key_bit_size + j];
        }

        int index_in_dict = binary_string_to_int(binary_cpr);

        decompressed_data += dict[index_in_dict];
    }

    return decompressed_data;
}

void compress_file(string input_filename, string output_filename, bool benchmark)
{
    // https://stackoverflow.com/a/2602258
    ifstream input_file(input_filename);
    stringstream buffer;
    buffer << input_file.rdbuf();
    string file_content = buffer.str();
    input_file.close();

    string compressed = compress_text(file_content);

    ofstream output_file(output_filename);
    output_file << compressed;
    output_file.close();
}

void decompress_file(string input_filename, string output_filename, bool benchmark)
{
    // https://stackoverflow.com/a/2602258
    // https://stackoverflow.com/a/17584871
    ifstream input_file(input_filename, ios::binary);
    stringstream buffer;
    buffer << input_file.rdbuf();
    string file_content = buffer.str();
    input_file.close();

    // recreate dictionary from file
    int dict_size = file_content[0];
    string dict = "";
    for (int i = 0; i < dict_size; i++)
    {
        dict += file_content[1 + i];
    }

    // read compressed data with padding
    string compressed_data = "";
    for (int i = dict_size + 1; i < file_content.size(); i++)
    {
        compressed_data += file_content[i];
    }

    string decompressed = decompress_text(dict, compressed_data);

    ofstream output_file(output_filename);
    output_file << decompressed;
    output_file.close();
}

int main()
{
    cout << "re_TAR_d v1.0.0" << endl << endl;

    char benchmark = 0;
    char mode = 0; // true - compression, false - decompression

    while (benchmark != 'y' && benchmark != 'n')
    {
        cout << "Benchmark [Y/N]: ";
        cin >> benchmark;
        benchmark = tolower(benchmark);
    }
    cout << endl;

    while (mode != 'c' && mode != 'd')
    {
        cout << "Choose option:" << endl;
        cout << "- [C] Compression" << endl;
        cout << "- [D] Decompression" << endl;
        cin >> mode;
        mode = tolower(mode);
    }
    cout << endl;

    string input_filename;
    cout << "Input filename: ";
    cin >> input_filename;

    string output_filename;
    cout << "Output filename: ";
    cin >> output_filename;

    if (mode == 'c')
    {
        compress_file(input_filename, output_filename, benchmark == 'y');
    }
    if (mode == 'd')
    {
        decompress_file(input_filename, output_filename, benchmark == 'y');
    }

    return 0;
}




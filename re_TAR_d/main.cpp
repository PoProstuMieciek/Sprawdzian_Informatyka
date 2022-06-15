#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <assert.h>
using namespace std;
using namespace std::chrono;

// int_to_binary_string(5, 8) = "00000101"
string int_to_binary_string(unsigned int number, int total_length)
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
unsigned int binary_string_to_int(string binary_string)
{
    int result = 0;
    for (int i = 0; i < binary_string.size(); i++)
    {
        result *= 2;
        result += binary_string[i] - '0';
    }
    return result;
}

// read data from stream, save dictionary with all unique characters in alphabetical order, return size of stream
unsigned long long generate_dictionary_from_stream(ifstream &stream, string &dictionary)
{
    unsigned long long bytes_read = 0;
    bool byte_exists[256] = {};

    // unsigned char = byte, byte range: (0, 255)
    unsigned char current_byte;
    while (stream.get((char &) current_byte))
    {
        bytes_read++;
        byte_exists[current_byte] = true;
    }

    // loop over every existing byte
    dictionary = "";
    for (int i = 0; i < 256; i++)
    {
        if (byte_exists[i]) dictionary += i;
    }

    return bytes_read;
}

// read data from stream, return dictionary
string restore_dictionary_from_stream(ifstream &stream)
{
    unsigned char current_byte;

    stream.get((char &) current_byte);

    unsigned int dictionary_size = current_byte;

    // this is an ugly edge case, byte has range (0, 255), but dictionary can have size of 256, so 256 is saved as 0
    // however we cannot have a dictionary of size 0, that makes no sense
    if (dictionary_size == 0) dictionary_size = 256;

    string dictionary;

    for (int i = 0; i < dictionary_size; i++)
    {
        stream.get((char &) current_byte);

        dictionary += current_byte;
    }

    // just to catch any bugs
    assert(dictionary.size() == dictionary_size);

    return dictionary;
}

// gets dictionary, reads input_stream byte by byte, compresses data, outputs byte by byte to output_stream, returns size of compressed file in bytes
unsigned long long compress_stream(const string &dictionary, ifstream &input_stream, unsigned long long input_stream_size, ofstream &output_stream)
{
    unsigned long long compressed_bytes = 0;

    // ceil(log2(x)) gives number of bits required to save number in binary format
    int key_bit_size = max(1, (int) ceil(log2(dictionary.size())));

    // calculate total padding
    int compressed_bits_size = key_bit_size * input_stream_size;
    int padding = (8 - ((compressed_bits_size + 3) % 8)) % 8;

    // first byte of output is the size of dictionary, lets call it N
    output_stream << (unsigned char) dictionary.size();
    compressed_bytes++;

    // next N bytes are the dictionary
    output_stream << dictionary;
    compressed_bytes += dictionary.size();

    // next we have padded data until the end of file
    string buffer = int_to_binary_string(padding, 3);
    buffer.reserve(16); // we dont need larger buffer

    unsigned char current_byte;
    while (input_stream.get((char &) current_byte))
    {
        // search for index in dictionary
        for (int i = 0; i < dictionary.size(); i++)
        {
            if (current_byte == dictionary[i])
            {
                buffer += int_to_binary_string(i, key_bit_size);
                break;
            }
        }

        // save all bytes if we have any
        while (buffer.size() >= 8)
        {
            // get first 8 bits from buffer, and save to string
            string byte_string_from_buffer = buffer.substr(0, 8);

            // remove those first 8 bits
            buffer.erase(0, 8);

            // convert that string to integer
            unsigned char byte_from_buffer = binary_string_to_int(byte_string_from_buffer);

            // pipe to output
            output_stream << byte_from_buffer;

            compressed_bytes++;
        }
    }

    // apply padding at the end
    for (int i = 0; i < padding; i++)
    {
        buffer += '1';
    }

    // save all bytes if we have any, this is the same as above
    while (buffer.size() >= 8)
    {
        // get first 8 bits from buffer, and save to string
        string byte_string_from_buffer = buffer.substr(0, 8);

        // remove those first 8 bits
        buffer.erase(0, 8);

        // convert that string to integer
        unsigned char byte_from_buffer = binary_string_to_int(byte_string_from_buffer);

        // pipe to output
        output_stream << byte_from_buffer;

        compressed_bytes++;
    }

    // buffer MUST be empty here. if it is not, then we have a bug!
    assert(buffer.empty());

    return compressed_bytes;
}

void decompress_stream(const string &dictionary, ifstream &input_stream, unsigned long long input_stream_size, ofstream &output_stream)
{
    // ceil(log2(x)) gives number of bits required to save number in binary format
    int key_bit_size = max(1, (int) ceil(log2(dictionary.size())));

    // calculate total padding
    unsigned char current_byte;
    input_stream.get((char &) current_byte);
    int padding = current_byte >> 5;

    string buffer = int_to_binary_string(current_byte, 8);
    buffer.reserve(16); // we dont need larger buffer
    buffer.erase(0, 3); // remove padding from beginning

    unsigned long long bits_read = 8;
    while (input_stream.get((char &) current_byte))
    {
        bits_read += 8;
        buffer += int_to_binary_string(current_byte, 8);

        // save all bytes if we have any
        while (buffer.size() >= key_bit_size && bits_read <= input_stream_size * 8 - padding)
        {
            // let N = key_bit_size
            // get first N bits from buffer, and save to string
            string byte_string_from_buffer = buffer.substr(0, key_bit_size);

            // remove those first N bits
            buffer.erase(0, key_bit_size);

            // convert that string to integer
            unsigned char byte_from_buffer = binary_string_to_int(byte_string_from_buffer);

            // pipe char from dictionary to output
            output_stream << dictionary[byte_from_buffer];
        }
    }

    // N = key_bit_size
    // buffer MUST be of size N. if it is not, then we have a bug!
    assert(buffer.size() == key_bit_size);
}

void compress_file(const string &input_filename, const string &output_filename, bool benchmark)
{
    // https://stackoverflow.com/a/22387757
    auto start = high_resolution_clock::now();

    // https://stackoverflow.com/a/7681612
    // https://stackoverflow.com/a/22390788
    ifstream input_stream(input_filename, ios::in | ios::binary);
    ofstream output_stream(output_filename, ios::out | ios::binary);

    string dictionary;
    unsigned long long raw_size_bytes = generate_dictionary_from_stream(input_stream, dictionary);

    input_stream.clear();
    input_stream.seekg(0, ios::beg);

    unsigned long long compressed_size_bytes = compress_stream(dictionary, input_stream, raw_size_bytes, output_stream);

    input_stream.close();
    output_stream.close();

    auto stop = high_resolution_clock::now();

    if (benchmark)
    {
        double percent_compressed = (1 - ((double) compressed_size_bytes / (double) raw_size_bytes)) * 100;
        duration<double, std::milli> time_ms = stop - start;

        cout << endl << "BENCHMARK" << endl;
        cout << "Compression time: " << time_ms.count() << " ms" << endl;
        cout << "Original file size: " << raw_size_bytes << " B" << endl;
        cout << "Compressed file size: " << compressed_size_bytes << " B" << endl;
        cout << "Compression ratio: " << percent_compressed << " %" << endl;
    }
}

void decompress_file(const string &input_filename, const string &output_filename, bool benchmark)
{
    // https://stackoverflow.com/a/22387757
    auto start = high_resolution_clock::now();

    // https://stackoverflow.com/a/7681612
    // https://stackoverflow.com/a/22390788
    ifstream input_stream(input_filename, ios::in | ios::binary);
    ofstream output_stream(output_filename, ios::out | ios::binary);

    string dictionary = restore_dictionary_from_stream(input_stream);

    // calculate file size
    unsigned long long input_stream_size = 0;
    char unused;
    while (input_stream.get(unused))
    {
        input_stream_size++;
    }

    // back to place when we finished extracting the dictionary
    input_stream.clear();
    input_stream.seekg(dictionary.size() + 1);

    decompress_stream(dictionary, input_stream, input_stream_size, output_stream);

    input_stream.close();
    output_stream.close();

    auto stop = high_resolution_clock::now();

    if (benchmark)
    {
        duration<double, std::milli> time_ms = stop - start;

        cout << endl << "BENCHMARK" << endl;
        cout << "Decompression time: " << time_ms.count() << " ms" << endl;
    }
}

int main()
{
    cout << "re_TAR_d v1.0.0" << endl << endl;

    char benchmark = 0;
    char mode = 0; // c - compression, d - decompression

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

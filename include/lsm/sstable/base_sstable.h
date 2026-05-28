/**
  SSTable File Format
  ==========================

  [ Data Block 0 ] [ Data Block 1 ] .... [ Indexes Block ] [ Footer ]

  Data Block 
  ---------------
  [ Record Header | Key(string) | Value(string) ]

  RecordHeader 
    - size_t key_size
    - size_t value_size

  
  Indexes Block 
  ---------------
  [ Data Block 0 index ] [Data Block 1 index] .....

  Data Block Index
    - offset_t block_start
    - offset_t block_end

  
  Footer
  ---------------
    - offset_t indexes_bloc_start_offset
    - size_t indexes_bloc_size
*/


#pragma once 

#include <cstddef>
#include <cstdint>
#include <string>

class BaseSStable {
  public: 
    typedef uint64_t offset_t;
    typedef std::pair<offset_t, offset_t> index_t;

    struct RecordHeader { 
      size_t key_size;
      size_t value_size;

      RecordHeader() {}
      RecordHeader(size_t key_size, size_t value_size): key_size(key_size), value_size(value_size) {}
    };

    struct Footer { 
      offset_t indexes_bloc_start_offset;
      size_t indexes_bloc_size;

      Footer() {}
      Footer(offset_t indexes_bloc_start_offset, size_t indexes_bloc_size): 
        indexes_bloc_start_offset(indexes_bloc_start_offset), indexes_bloc_size(indexes_bloc_size) {}
    };

    BaseSStable(const std::string& file_name);

  protected:
    std::string file_path_;
};
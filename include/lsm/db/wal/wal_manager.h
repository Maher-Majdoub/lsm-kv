#pragma once 

#include "lsm/db/wal/wal_builder.h"
#include "lsm/db/wal/wal_iterator.h"

#include <filesystem>
#include <optional>
#include <string>

namespace lsm {
  class WALManager {
    public: 
      WALManager(const std::filesystem::path& work_dir);
      
      std::optional<WALIterator> recover();
      void add_entry(const std::string& key, const std::string& value);
      void rotate();
      
    private: 
      std::filesystem::path work_dir_;
      std::optional<WALBuilder> wal_builder_;
      std::optional<std::filesystem::path> curr_wal_path_;
      
      std::optional<std::string> read_current_file_();
      void update_current_file(const std::string& wal_name);

      constexpr static std::string CURRENT_FILE_NAME = "CURRENT"; 
  };
}
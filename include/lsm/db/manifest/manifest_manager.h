#pragma once 

#include "lsm/db/common/sstable_metadata.h"

#include <memory>
#include <fstream>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace lsm {
  class ManifestManager {
    public: 
      ManifestManager(const std::filesystem::path& work_dir);

      void add_sstable(std::shared_ptr<SSTableMetadata> metadata);

    public: 
      std::filesystem::path work_dir_;
      std::unique_ptr<std::ofstream> active_manifest_;
      std::vector<std::vector<SSTableMetadata>> sstables_;
      
      void load_manifest_();
      void recover_(const std::string& name);
      void create_(const std::string& name);
      void use_manifest_(std::ofstream& manifest, const std::string& name);
      std::optional<std::string> read_current_file_();
      void update_current_file_(const std::string& manifest_name);
      
      constexpr static std::string CURRENT_FILE_NAME = "CURRENT"; 
  };
}
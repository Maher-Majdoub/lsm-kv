#pragma once 

#include "lsm/db/common/sstable_metadata.h"
#include "lsm/db/sstable/sstable_manager.h"

#include <memory>
#include <fstream>
#include <filesystem>
#include <optional>
#include <string>

namespace lsm {
  class ManifestManager {
    public: 
      ManifestManager(const std::filesystem::path& work_dir, SSTableManager& sst_manager_);

      void add_sstable(std::shared_ptr<SSTableMetadata> metadata);
      void load();

    private: 
      std::filesystem::path work_dir_;
      std::unique_ptr<std::ofstream> active_manifest_;
      SSTableManager& sst_manager_;
      
      void recover_(const std::string& name);
      void create_(const std::string& name);
      void use_manifest_(std::ofstream& manifest, const std::string& name);
      std::optional<std::string> read_current_file_();
      void update_current_file_(const std::string& manifest_name);
      
      constexpr static std::string CURRENT_FILE_NAME = "CURRENT"; 
  };
}
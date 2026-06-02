#include "lsm/db/manifest/manifest_manager.h"
#include "lsm/db/manifest/manifest.h"
#include "lsm/db/manifest/manifest_entry.h"
#include "lsm/db/sstable/sstable_manager.h"
#include "lsm/utils/time.h"

#include <cstddef>
#include <fstream>
#include <filesystem>
#include <optional>
#include <memory>
#include <string>
#include <vector>

namespace lsm {
  ManifestManager::ManifestManager(const std::filesystem::path& work_dir, SSTableManager& sst_manager):
    work_dir_(work_dir), sst_manager_(sst_manager) {
    std::filesystem::create_directories(work_dir_);
  }

  void ManifestManager::load() {
    std::optional<std::string> existing = read_current_file_();

    std::string manifest_name;
    if (existing) {
      manifest_name = *existing;
      recover_(manifest_name);
    } else {
      manifest_name = "MANIFEST-" + current_timestamp();
      create_(manifest_name);
    }

    std::filesystem::path path = work_dir_ / manifest_name;
    std::ofstream manifest_file(path, std::ios::binary | std::ios::app);
    use_manifest_(manifest_file, manifest_name);
  }

  void ManifestManager::add_sstable(std::shared_ptr<SSTableMetadata> metadata) {
    // write [operation][level][file_path_size][file_path][min_key_size][min_key][max_key_size][max_key]
    std::string file_path = metadata->path.string();

    size_t file_path_size = file_path.size();
    size_t min_key_size = metadata->min_key.size();
    size_t max_key_size = metadata->max_key.size();

    manifest::Operation op = manifest::Operation::ADD;

    active_manifest_->write((char*) &op, sizeof(op));
    active_manifest_->write((char*) &metadata->level, sizeof(metadata->level));

    active_manifest_->write((char*) &file_path_size, sizeof(file_path_size));
    active_manifest_->write(file_path.data(),  file_path_size);

    active_manifest_->write((char*) &min_key_size, sizeof(min_key_size));
    active_manifest_->write(metadata->min_key.data(),  min_key_size);

    active_manifest_->write((char*) &max_key_size, sizeof(max_key_size));
    active_manifest_->write(metadata->max_key.data(),  max_key_size);

    active_manifest_->flush();

    sst_manager_.add(*metadata);
  }

  void ManifestManager::recover_(const std::string& name) {
    std::filesystem::path path = work_dir_ / name;

    Manifest manifest(path);
    std::vector<manifest::Entry> entries = manifest.parse();

    for (manifest::Entry& entry: entries) {
      switch (entry.operation) {
        case manifest::Operation::ADD: {
          sst_manager_.add(std::move(entry.metadata));
          break;
        }
        case manifest::Operation::REMOVE: {
          sst_manager_.remove(entry.metadata.level, entry.metadata.path);
          break;
        }
        default: throw std::runtime_error("UNKNOWN OPERATION");
      }
    }

    
  }

  void ManifestManager::create_(const std::string& name) {
    std::filesystem::path path = work_dir_ / name;
    std::ofstream manifest(path, std::ios::binary | std::ios::app);

    // TODO: Add Manifest Header
    
    use_manifest_(manifest, name);
  }

  std::optional<std::string> ManifestManager::read_current_file_() {
    std::filesystem::path file_path = work_dir_ / ManifestManager::CURRENT_FILE_NAME;

    if (!std::filesystem::exists(file_path)) return std::nullopt;

    std::ifstream current_file(file_path);
    std::string manifest_name;
    std::getline(current_file, manifest_name);

    if (manifest_name.empty()) return std::nullopt;
    return manifest_name;
  }

  void ManifestManager::update_current_file_(const std::string& manifest_name) {
    std::filesystem::path path = work_dir_ / ManifestManager::CURRENT_FILE_NAME;
    std::ofstream current_file(path);
    
    current_file << manifest_name << "\n";
    current_file.close();
  }

  void ManifestManager::use_manifest_(std::ofstream& manifest, const std::string& name) {
    active_manifest_ = std::make_unique<std::ofstream>(std::move(manifest));
    update_current_file_(name);
  }
}